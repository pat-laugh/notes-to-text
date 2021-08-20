**NOTE**: This automatically removes files starting with `._`, which is garbage
Mac may introduce when archiving/compressing/zipping files. Be careful if you
name files starting with `._`!

This script combines two ad hoc algorithms that determine the content of the
Notes files. The first part of the script gets the files to parse within the
current working directory; it automatically removes files starting with `._`. It
only cares about files ending with `.notesairdropdocument` and should output
files with the same name, but with the extension changed to `.txt`. Directories
are ignored.

The Notes files appear to be consisted of 3 sections: garbage, pure content,
then more garbage. Note that if you use
[WYSIWYG](https://en.wikipedia.org/wiki/WYSIWYG) markup in your files, HTML
markup might appear in your text, like getting the source of a webpage versus
copying the presented text.

The first algorithm (`f04`) tries to determine when the content of the Notes
file ends (wthen the useless garbage at then end starts). The second algorithm
(`f06`) tries to determine when the content starts. This gives the start and end
indices to get the content from the whole file.

The script will output errors and warning if it cannot accurately determine a
Note's content. You should manually check the content of each file where this
happens. The simplest course of action is probably to open the file with a
source text editor (not the Notes app), and then copy the content.