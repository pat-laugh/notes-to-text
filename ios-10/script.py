#!/usr/bin/env python3
# /t013/c11
import os, sys

FILE_EXT = '.notesairdropdocument'

def get_notes_files(dir_name='.'):
	DOT_BAR = '._'
	files, files_dot_bar = set(), set()
	for f_name in os.listdir(dir_name):
		if os.path.isdir(f_name):
			print("WARN: directory ignored: %s" % f_name, file=sys.stderr)
			continue
		if not os.path.isfile(f_name):
			print("ERR: not file: %s" % f_name, file=sys.stderr)
			continue
		if f_name.rfind(FILE_EXT) != (len(f_name) - len(FILE_EXT)):
			print("ERR: unexpected file extension: %s" % f_name, file=sys.stderr)
			continue
		try:
			if f_name.find(DOT_BAR) == 0:
				name = f_name[len(DOT_BAR):]
				if name in files:
					os.remove(DOT_BAR + name)
				else:
					files_dot_bar.add(name)
			else:
				files.add(f_name)
				if f_name in files_dot_bar:
					os.remove(DOT_BAR + name)
		except FileNotFoundError:
			pass
	return files

import os, string, re

def f04(files):
	ascii_printable = string.printable.encode('utf-8')
	non_patterns, maybe_wrong, probaly_correct = set(), set(), set()
	contents = {}
	for f_name in files:
		with open(f_name, 'rb') as f:
			s = f.read()
		i_start = 0
		pattern_met = -1
		while True:
			try:
				i = s.index(b'*', i_start)
				i_start = i + 1
				if i + 1 == len(s):
					print('WEIRD: %s' % (f_name))
					break
				val = s[i+1]
				if (val == 0 or val > 9) and val != 24:
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
					non_patterns.add(f_name)
					break
			except ValueError:
				if pattern_met >= 0:  
					contents[f_name] = pattern_met
				break
	return contents
import os, string, re

def is_control(b):
	return 0 <= b < 32 or b == 127


def f06(files):
	non_patterns, maybe_wrong, probaly_correct = set(), set(), set()
	contents = {}
	for f_name in files:
		with open(f_name, 'rb') as f:
			s = f.read()
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
			continue
		if nl_met > 4 or i > 20:
			maybe_wrong.add(f_name)
		i_title = i
		i_nl = s.find(b'\n', i_title)
		if i_nl == -1:
			i_nl = s.find(b'*', i_title)
			if i_nl == -1:
				non_patterns.add(f_name)
				continue
		try:
			title = s[i_title:i_nl].decode('utf-8')
		except:
			print('ERR got:"%s" exp:"%s" s[:60]:%s' % (s[i_title:i_nl], f_name, s[:60]))
			non_patterns.add(f_name)
			continue
		i = f_name.find(FILE_EXT)
		if i == -1:
			print('WEIRD: %s' % f_name)
			continue
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
			contents[f_name] = i_title
	return contents
import os, string, re, sys

def main(out_dir):
	files = get_notes_files()
	files_ends = f04(files) # file_name, end_index
	files_starts = f06(files_ends) # file_name, start_index
	for f_name in files_starts:
		i_start = files_starts[f_name]
		i_end = files_ends[f_name]
		with open(f_name, 'rb') as f:
			s = f.read()
		i = f_name.find(FILE_EXT)
		assert(i == len(f_name) - len(FILE_EXT))
		f_name = f_name.replace(FILE_EXT, '.txt')
		with open(os.path.join(out_dir, f_name), 'wb') as f:
			f.write(s[i_start:i_end])


if __name__ == '__main__':
	USAGE = '''script out-dir

This gets all %s files in the current directory
and outputs a text version for each in out-dir.''' % FILE_EXT
	if len(sys.argv) != 2:
		sys.exit(USAGE)
	out_dir = sys.argv[1]
	if out_dir == 'help':
		print(USAGE)
		sys.exit()
	if not os.path.isdir(out_dir):
		print('ERR: "%s" is not a directory' % out_dir, file=sys.stderr)
		sys.exit(USAGE)
	main(out_dir)
