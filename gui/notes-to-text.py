#!/usr/bin/env python3
# GUI automation. For Mac only.

import os, sys

FILE_EXT_IN, FILE_EXT_OUT = '.notesairdropdocument', '.txt'

USAGE = '''%s source-dir dest-dir

This uses GUI automation to open note files (using the Mac `open` command) from
source-dir and copy their text. It then puts the copied text in a file in
dest-dir with the same name as the original file, but with the extension "%s".

The script will guide you on how to set it up so it copies properly.

Running the script takes more than a second per note file, and since it takes
control of the mouse and keyboard during that time, it's probably best to run
it while you don't need the computer.''' % (sys.argv[0], FILE_EXT_OUT)

try:
	source_dir = sys.argv[1]
	if source_dir == 'help':
		print(USAGE)
		sys.exit()
	dest_dir = sys.argv[2]
except IndexError:
	sys.exit(USAGE)

# import stuff after checking args because pyautogui is so slow to import
import pyautogui, pyperclip, subprocess, time

if not os.path.isdir(dest_dir):
	os.mkdir(dest_dir)

print('Open the notes app and move the mouse to the section containing editable text.')
print('The notes app does not need focus now, but it needs to be clickable there.')
print('The script needs this to select the text.')

if input('Type "y" when done. ') != 'y':
	sys.exit()

x_text, y_text = pyautogui.position()
	
print('Move the mouse to the menu at the left to click on the note\'s title.')
print('The notes app does not need focus now, but it needs to be clickable there.')
print('The script needs this to delete the note.')

if input('Type "y" when done. The script will begin right after. ') != 'y':
	sys.exit()

x_del, y_del = pyautogui.position()

pyautogui.moveTo(x_text, y_text)
pyautogui.click()
num_note_files_copied = 0
for file_name in os.listdir(source_dir):
	index_ext = file_name.rfind(FILE_EXT_IN)
	if index_ext != len(file_name) - len(FILE_EXT_IN):
		continue
	subprocess.run(['open', os.path.join(source_dir, file_name)])
	time.sleep(1)
	pyautogui.moveTo(x_text, y_text)
	pyautogui.click()
	pyautogui.hotkey('command', 'a')
	pyautogui.hotkey('command', 'c')
	content = pyperclip.paste()
	new_file_name = os.path.join(dest_dir, file_name[:index_ext] + FILE_EXT_OUT)
	with open(new_file_name, 'w') as f:
		f.write(content)
	print('File %s copied to %s' % (file_name, new_file_name))
	pyautogui.moveTo(x_del, y_del)
	pyautogui.click()
	pyautogui.press('delete')
	pyautogui.press('enter')
	num_note_files_copied += 1

pyautogui.hotkey('command', 'tab')
print('--------------------\n%s note files copied' % num_note_files_copied)
