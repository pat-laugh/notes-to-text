//MIT License
//Copyright 2017 Patrick Laughrea
#include "utilsSweepers.hpp"

#include <deque>

using namespace ntt;
using namespace std;

string::size_type matchBuffer(deque<char>& buffer, const string& text, string& readText);
void dumpBuffer(deque<char>& buffer, string& readText);
char pullFrontBuffer(deque<char>& buffer);
bool readToChar(various::SmartIterator& it, char toGet, string& readText);

bool ntt::readToText(various::SmartIterator& it, const string& text, string& readText)
{
    readText.clear();
    if (text.empty())
        return true;
    
	deque<char> buffer;
    while (true)
    {
		auto i = matchBuffer(buffer, text, readText);
        
        if (i == text.length())
            return true;
		
		//if buffer is empty, it means no character has been matched yet, so match first
		if (buffer.empty())
		{
			if (!readToChar(file, text.front(), readText))
				return false;
			
			buffer.push_back(text.front());
			i++;
		}
		
		//match remaining text
        for (; i < text.length(); i++)
        {
        	int c = file.get();
            if (file.eof())
			{
				dumpBuffer(buffer, readText);
                return false;
			}
            
            buffer.push_back((char)c);
            
            if (c != text[i])
                break;
        }
        
        if (i == text.length())
            return true;
        
		readText += pullFrontBuffer(buffer);
    }
}

string::size_type matchBuffer(deque<char>& buffer, const string& text, string& readText)
{
	while (!buffer.empty())
	{
		if (buffer.front() != text.front())
		{
			readText += pullFrontBuffer(buffer);
			continue;
		}
		
		for (string::size_type i = 1; i < text.length(); i++)
			if (i == buffer.size())
				return i;
			else if (buffer[i] != text[i])
			{
				readText += pullFrontBuffer(buffer);
				break;
			}
	}
	return 0;
}

void dumpBuffer(deque<char>& buffer, string& readText)
{
	while (!buffer.empty())
		readText += pullFrontBuffer(buffer);
}

char pullFrontBuffer(deque<char>& buffer)
{
	char c = buffer.front();
	buffer.pop_front();
	return c;
}

bool readToChar(ifstream& file, char toGet, string& readText)
{
	int c;
	while ((c = file.get()) != toGet && !file.eof())
		readText += (char)c;
	
	return (file.eof()) ? false : true;
}

bool ntt::readToTag(various::SmartIterator& it, HtmlTag tag, bool endTag, string& readText)
{
    if (endTag)
        return readToText(file, makeTag(tag, endTag), readText);
    
    //read first to part of tag, like <div, then all before >
    string stringTag = makeTag(tag, endTag);
    if (!readToText(file, stringTag.substr(0, stringTag.length() - 1), readText))
        return false;
    
    string insideTag;
    do
    {
        string temp;
        bool found = readToText(file, stringTag.substr(stringTag.length() - 1), temp);
        insideTag += temp;
        if (!found)
        {
            readText += insideTag;
            return false;
        }
    }
    while (getIndexTagEnd(insideTag, 0) != insideTag.length()); //means end of tag was within quotes, so restart

    return true;
}