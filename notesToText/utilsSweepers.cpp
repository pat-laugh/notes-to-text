//MIT License
//Copyright 2017 Patrick Laughrea
#include "utilsSweepers.hpp"

#include "utilsHtml.hpp"

#include <deque>

using namespace ntt;
using namespace std;
using namespace various;

static string::size_type matchBuffer(deque<char>& buffer, const string& text, string& readText);
static void dumpBuffer(deque<char>& buffer, string& readText);
static char pullFrontBuffer(deque<char>& buffer);

static SmartIterator& skipToChar(SmartIterator& it, char c)
{
	for (; it && *it != c; ++it)
		;
	return it;
}

static SmartIterator& readToChar(SmartIterator& it, char c, string& readText)
{
	for (; it && *it != c; ++it)
		readText += *it;
	return it;
}

static SmartIterator& getToGreaterThan(SmartIterator& it)
{
	for (; it && *it != '>'; ++it)
		if (*it == '"' || *it == '\'')
		{
			char c = *it;
			skipToChar(++it, c);
		}
	return it;
}

SmartIterator& ntt::getToTag(SmartIterator& it, HtmlTag tag, bool endTag)
{
	string tagString;
    if (endTag)
        return getToText(it, (tagString = makeTagString<true>(tag)));
    
    //read first to part of tag, like <div, then all before >
    tagString = makeTagString<false>(tag);
	tagString = tagString.substr(0, tagString.length() - 1);
	if (!getToText(it, tagString))
		return it;
    return ++getToGreaterThan(it);
}


SmartIterator& ntt::readToTag(SmartIterator& it, HtmlTag tag, bool endTag, string& readText)
{
	string tagString;
    if (endTag)
        return readToText(it, (tagString = makeTagString<true>(tag)), readText);
    
    //read first to part of tag, like <div, then all before >
    tagString = makeTagString<false>(tag);
	tagString = tagString.substr(0, tagString.length() - 1);
	if (!readToText(it, tagString, readText))
		return it;
    return ++getToGreaterThan(it);
}

SmartIterator& ntt::getToText(SmartIterator& it, const string& text)
{
    string dummy;
    return readToText(it, text, dummy);
}

SmartIterator& ntt::readToText(SmartIterator& it, const string& text, string& readText)
{
    readText.clear();
    if (text.empty())
        return it;
    
	deque<char> buffer;
    while (true)
    {
		auto i = matchBuffer(buffer, text, readText);
        
        if (i == text.length())
            return it;
		
		//if buffer is empty, it means no character has been matched yet, so match first
		if (buffer.empty())
		{
			if (!readToChar(it, text.front(), readText))
				return it;
			
			buffer.push_back(text.front());
			++it;
			++i;
		}
		
		//match remaining text
        for (; ; ++i)
        {
	        if (i == text.length())
	            return it;
			
            if (!it)
			{
				dumpBuffer(buffer, readText);
                return it;
			}
            
			char c = *it;
            buffer.push_back(c);
			++it;
            
            if (c != text[i])
                break;
        }
        
		readText += pullFrontBuffer(buffer);
    }
}

//how many sequential chars from buffer match those from text starting at index 0
static string::size_type matchBuffer(deque<char>& buffer, const string& text, string& readText)
{
	while (!buffer.empty())
	{
		if (buffer.front() != text.front())
		{
			readText += pullFrontBuffer(buffer);
			continue;
		}
		
		for (string::size_type i = 1; ; ++i)
			if (i == buffer.size() || i == text.length())
				return i;
			else if (buffer[i] != text[i])
			{
				readText += pullFrontBuffer(buffer);
				break;
			}
	}
	return 0;
}

static void dumpBuffer(deque<char>& buffer, string& readText)
{
	while (!buffer.empty())
		readText += pullFrontBuffer(buffer);
}

static char pullFrontBuffer(deque<char>& buffer)
{
	char c = buffer.front();
	buffer.pop_front();
	return c;
}

static string::size_type skipToChar(const string& str, char c, string::size_type i)
{
	for (; i < str.length() && str[i] != c; ++i)
		;
	return i;
}

static string::size_type getToGreaterThan(const string& str, string::size_type i)
{
	for (; i < str.length() && str[i] != '>'; ++i)
		if (str[i] == '"' || str[i] == '\'')
		{
			char c = str[i++];
			i = skipToChar(str, c, i);
		}
	return i;
}

void ntt::trimHtmlTags(string& str)
{
    for (string::size_type i = 0; i < str.length();)
        if (str[i] == '<')
            str.erase(i, getToGreaterThan(str, i + 1) + 1 - i);
        else
            ++i;
}