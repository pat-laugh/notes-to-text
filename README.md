# Notes-to-text / iPhone Notes Copier

This is a suite of applications to convert iPhone Notes documents to text. There
are 3 different applications:
-	ios-9: Tested with iOS 9 files. Written in C++, using a Mac.
-	gui: Controls the keyboard and mouse (as if you were doing the work). You'd
	expect this to be perfectly accurate, but the Notes app is buggy so there
	are some edge cases (like HTML markup appearing in the iPhone, but
	not in the Mac). Written in Python, using a Mac.
-	ios-10: Tested with iOS 10+ files. Written in Python, using Linux.

See the README file in each for more information. Since I don't know the exact
specifications of Notes documents, none of these solutions are guaranteed to
work with all files, but they'll probably work with most of your files
(especially if written in plain English with no special markup).

## Use cases

If your notes are from iOS 9, use the `ios-9` app. (Earlier than that, I don't
have any solution.)

Otherwise, use the `ios-10` app. The `gui` app is mostly there because I'd built
it before the `ios-10` app, but it could be useful if `ios-10` can't parse many
of your notes.
