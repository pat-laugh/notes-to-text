#!/usr/bin/env python3
# /t013/c11
import os, re, string, sys

FILE_EXT = '.notesairdropdocument'
ELLIPSIS = '…'.encode('utf-8')

debug = False

def clear_notes_files(dir_name='.'):
	DOT_BAR = '._'
	f_names, f_names_dot_bar = set(), set()
	for f_name in os.listdir(dir_name):
		f_name = os.path.join(dir_name, f_name)
		if os.path.isdir(f_name):
			print("WARN: directory ignored: %s" % f_name, file=sys.stderr)
			continue
		if not os.path.isfile(f_name):
			print("WARN: file ignored: not file: %s" % f_name, file=sys.stderr)
			continue
		if f_name.rfind(FILE_EXT) != (len(f_name) - len(FILE_EXT)):
			print("WARN: file ignored: unexpected file extension: %s" % f_name, file=sys.stderr)
			continue
		try:
			if f_name.find(DOT_BAR) == 0:
				name = f_name[len(DOT_BAR):]
				if name in f_names:
					os.remove(DOT_BAR + name)
				else:
					f_names_dot_bar.add(name)
			else:
				f_names.add(f_name)
				if f_name in f_names_dot_bar:
					os.remove(DOT_BAR + name)
		except FileNotFoundError:
			pass
	return f_names

def index_end(file_content, f_name):
	ascii_printable = string.printable.encode('utf-8')
	maybe_wrong, probaly_correct = set(), set()
	i_start = 0
	pattern_met = -1
	s = file_content
	while True:
		try:
			i = s.index(b'*', i_start)
			i_start = i + 1
			if i + 1 == len(s):
				print('WEIRD: %s' % (f_name))
				break
			val = s[i+1]
			if val == 0 or val == 10 or val == 13 or val >= 32:
				if val == 10 and pattern_met != -1:
					continue
				if pattern_met == -1:
					if val in ascii_printable:
						continue
					probaly_correct.add(f_name)
					continue
				print('!!! maybe wrong: %s: *val == %s; index: %s; s[i_start:i_start+120]: %s' % (f_name, val, i_start, s[i_start:i_start+120]))
				print('??? maybe wrong: %s: *val == %s; index: %s; s[i:i+120]: %s' % (f_name, val, i, s[i:i+120]))
				if f_name in probaly_correct:
					probaly_correct.remove(f_name)
				maybe_wrong.add(f_name)
				pattern_met = -2
				continue
			if pattern_met == -1:
				pattern_met = i
			if i + 2 + val <= len(s):
				s = s[i + 2 + val:]
				i_start = 0
			else:
				print('ERR non-pattern: %s: *val == %s; index: %s; len == %s' % (f_name, val, i, len(s)))
				break
		except ValueError:
			if pattern_met >= 0:  
				return pattern_met
	return None

def is_control(b):
	return 0 <= b < 32 or b == 127


def index_start(file_content, f_name):
	non_patterns, maybe_wrong, probaly_correct = set(), set(), set()
	s = file_content
	i = 0
	nl_met = 0
	while i < len(s):
		if is_control(s[i]):
			if s[i] == b'\n':
				nl_met += 1
			i += 1
		elif i + 1 < len(s) and is_control(s[i + 1]):
			i += 2
		else:
			break
	else:
		print('WEIRD: %s' % f_name)
		return None
	if nl_met > 4 or i > 20:
		maybe_wrong.add(f_name)
	i_title = i
	i_nl = s.find(b'\n', i_title)
	if i_nl == -1:
		i_nl = s.find(b'*', i_title)
		if i_nl == -1:
			non_patterns.add(f_name)
			return None
	try:
		title = s[i_title:i_nl].decode('utf-8')
	except:
		print('ERR got:"%s" exp:"%s" s[:60]:%s' % (s[i_title:i_nl], f_name, s[:60]))
		non_patterns.add(f_name)
		return None
	i = f_name.find(FILE_EXT)
	if i == -1:
		print('WEIRD: %s' % f_name)
		return None
	f_title = f_name[:i].encode('utf-8').decode('utf-8')
	# -2 is because of things like same_name_2
	if title != f_title and (title + f_title[-2:]) != f_title:
		title = title[1:]
		i_title += 1
	if title != f_title and (title + f_title[-2:]) != f_title:
		f_title = f_title.replace('-', '/')
	if title != f_title and (title + f_title[-2:]) != f_title:
		print('!!! got:"%s" exp:"%s" s[:60]:%s' % (title, f_title, s[:60]))
		maybe_wrong.add(f_name)
	else:
		return i_title
	return None

def ios_14_special_chars(s):
	s = s.replace(b'\xe2\x80\xa6', ELLIPSIS)
	s = s.replace(b'\xe2\x80\x99', b'\'')
	return s

def clean_content(content, f_name):
	s = content
	i_end = index_end(s, f_name)
	if i_end is None:
		return
	i_start = index_start(s, f_name)
	if i_start is None:
		return
	return ios_14_special_chars(s[i_start:i_end])

def set_debug(val):
	debug = val

def main(dir_in, dir_out):
	for f_name in clear_notes_files(dir_in):
		i = f_name.find(FILE_EXT)
		assert(i == len(f_name) - len(FILE_EXT))

		with open(f_name, 'rb') as f:
			s = f.read()
		content = clean_content(s, f_name)
		if content is None:
			continue
		f_name = f_name.replace(FILE_EXT, '.txt')
		with open(os.path.join(dir_out, f_name), 'wb') as f:
			f.write(content)


if __name__ == '__main__':
	USAGE = '''script out-dir

This gets all %s files in the current directory
and outputs a text version for each in out-dir.''' % FILE_EXT
	if len(sys.argv) != 2:
		sys.exit(USAGE)
	dir_out = sys.argv[1]
	if dir_out == 'help':
		print(USAGE)
		sys.exit()
	if not os.path.isdir(dir_out):
		print('ERR: "%s" is not a directory' % dir_out, file=sys.stderr)
		sys.exit(USAGE)
	main('.', dir_out)
