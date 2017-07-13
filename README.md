# Notes-to-text

This is an application to convert iPhone Notes documents to text.

The purpose is to allow an user to AirDrop a document and then use its text. The
challenge is that the documents use a kind of broken HTML to store their
content. This application seeks to fetch the information in a pure textual form.

I don't know the exact specifications of a Notes document. Therefore, the
application may output incorrect text. Many errors have been fixed (the
documents' format lack uniform consistency). As of now all the text I've
converted was correct.

## Installation

### Linux and Mac

The external library [various](https://github.com/pat-laugh/various-cpp)
is used in this project. You can make a symbolic link to it. After that, make
should create the executable.

## Usage

The executable can only be called with one parameter that is the name of a file.

The output is the content of the note as it would be if it were text only.

## Concept

### Library

The library `notesToText` does most of the work.

The text data of notes is stored in HTML. There is a body element. Then, most
lines are stored inside div elements. Sometimes the div element is not closed.
Sometimes the first line is not in a div element. The parser takes care of
handling the inconsistencies. It then returns a list containing all the lines.

Some text may contain HTML entities. Empty lines are denoted with `<br>`.
Everything is properly filtered to look like it should in text.

### Parsing HTML

A real parser is not used since that is unnecessary. Before and after the body
element, there is junk that is ignored. The attributes of tags are wholly
ignored. The whole thing is parsed as if there were only body and div tags and
the rest is text. Tags are not nested. It wouldn't make sense anyways since a
div tag represents a new line.

### Executable

The executable takes the list of lines returned from the parser and outputs
each item followed by a newline.

## Possible errors

Errors related to encoding have not yet been tested.
