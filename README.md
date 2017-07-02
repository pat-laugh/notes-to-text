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