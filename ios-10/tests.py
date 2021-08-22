import os
import difflib

from script import index_end, index_start, ios_14_special_chars, clean_content
from script import set_debug, main, FILE_EXT

def test_content(id, f_name, ok_i1, ok_i2, ok_content):
	f_name = f_name + FILE_EXT
	with open('./test-data/%s' % f_name, 'rb') as f:
		s = f.read()
		i1 = index_start(s, f_name)
		failed = False
		if i1 != ok_i1:
			print('test', id, 'failed:', 'bad index start')
			print('got', i1, 'expected', ok_i1)
			failed = True
		i2 = index_end(s, f_name)
		if i2 != ok_i2:
			print('test', id, 'failed:', 'bad index end')
			print('got', i2, 'expected', ok_i2)
			failed = True
		if failed:
			return
		if clean_content(s, f_name) != ok_content:
			print('test', id, 'failed:', 'content differ')

def test_file_out(id, f_name, ok_content):
	EXT = '.txt'
	try:
		with open(f_name + EXT, 'rb') as f:
			s = f.read()
			if s != ok_content:
				print('test', id, 'failed:', 'file', f_name + EXT, 'content wrong')
				print('obtained vs expected:')
				diff = difflib.ndiff(s.decode('utf-8').splitlines(keepends=True),
				ok_content.decode('utf-8').splitlines(keepends=True))
				print(''.join(diff), end='')
	except FileNotFoundError:
		print('test', id, 'failed:', 'file', f_name + EXT, 'not found')

def run_tests():
	set_debug(True)
	ELLIPSIS = '…'.encode('utf-8')

	f_names = [
		'Plan for next-2',
		'iOS-10-test-1',
		'iOS-10-test-2',
		'iOS-14-test-1',
		'iOS-14-test-2',
	]
	contents = [
		b'Plan for next\n\nDo that, and...\n\nWork on this.',
		b'iOS-10-test-1\n\nNeed to get rid of items!\n\nit could be...',
		b'iOS-10-test-2\nline',
		b'iOS-14-test-1\n\nyou don\'t want to work in that context.',
		b'iOS-14-test-2\n\nMessage on board:\n\nI haven\'t used all' + ELLIPSIS,
	]

	test_content(1, f_names[0], 15, 60, contents[0])
	test_content(2, f_names[1], 15, 71, contents[1])
	test_content(3, f_names[2], 10, 28, contents[2])
	test_content(4, f_names[3], 15, 71, contents[3])
	test_content(5, f_names[4], 15, 72, contents[4])

	DIR_OUT = 'test-out'
	try:
		os.mkdir(DIR_OUT)
	except FileExistsError:
		pass
	main('test-data', DIR_OUT)

	test_file_out(6, os.path.join(DIR_OUT, f_names[0]), contents[0])
	test_file_out(7, os.path.join(DIR_OUT, f_names[1]), contents[1])
	test_file_out(8, os.path.join(DIR_OUT, f_names[2]), contents[2])
	test_file_out(9, os.path.join(DIR_OUT, f_names[3]), contents[3])
	test_file_out(10, os.path.join(DIR_OUT, f_names[4]), contents[4])

if __name__ == '__main__':
	run_tests()
