//MIT License
//Copyright 2017 Patrick Laughrea
#include "parser.hpp"

#include "utilsHtml.hpp"
#include "utilsSweepers.hpp"

using namespace ntt;
using namespace std;
using namespace various;

static const char* TEMP_NL = "&nl;";

static void splitString(string& left, string& right, string::size_type index, string::size_type length = 1);

Parser::Parser(SmartIterator it) : it(move(it)) {}
Parser::~Parser() {}

static void replaceAll(string& s, const string& from, const string& to)
{
	for (string::size_type i = 0; (i = s.find(from, i)) != string::npos; i += to.length())
		s.replace(i, from.length(), to);
}

static string filterHtml(string&& s)
{
	trimHtmlTags(s);
	vector<string> from = {"&amp;", "&nbsp;", "&lt;", "&gt;", "&quot;", "&apos;", TEMP_NL};
	vector<string> to = {"&", " ", "<", ">", "\"", "\'", ""};
	for (vector<string>::size_type i = 0; i < from.size(); ++i)
		replaceAll(s, from[i], to[i]);
	return s;
}

vector<string> Parser::parse()
{
	vector<string> lines;
	readStart();
	lines.push_back(filterHtml(move(left)));
	while (getLine())
		lines.push_back(filterHtml(move(left)));
	return lines;
}

void Parser::readStart()
{
	//in note file, text immediately follows body, so get it
	//and put text in next tag in leftover for use later
	getToTag(it, HtmlTag::BODY, false);
	readToTag(it, HtmlTag::DIV, false, left);
	readToTag(it, HtmlTag::DIV, true, left.empty() ? left : leftover);
}

bool Parser::getLine()
{
    readNextTag();
    return !left.empty() || it;
}

static void checkStringSplit(string& left, string& leftover)
{
	string tag = makeTagString<false>(HtmlTag::DIV);
	auto index = left.find(tag);
	if (index != string::npos && index != 0)
		splitString(left, leftover, index, tag.length());
}

static void checkLastText(string& left)
{
	//there could still be text after the last tag, before the ending body tag
	string tag = makeTagString<true>(HtmlTag::BODY);
	auto index = left.find(tag);
	if (index != string::npos)
		left.erase(index);
	else
		left.clear();
}

void Parser::readNextTag()
{
    if (!leftover.empty())
    {
        left = move(leftover);
        leftover.clear(); //to be sure it's now empty
		checkStringSplit(left, leftover);
    }
	else if (readToTag(it, HtmlTag::DIV, false, left))
	{
		replaceAll(left, "<br>", TEMP_NL);
		left = filterHtml(move(left));
		if (!left.empty())
			readToTag(it, HtmlTag::DIV, true, leftover);
		else if (readToTag(it, HtmlTag::DIV, true, left))
			checkStringSplit(left, leftover);
		else
			checkLastText(left);
	}
	else
		checkLastText(left);
}

//takes the left string and puts all the part after the index into the right string
//the part in the middle (the index + length) is removed
static void splitString(string& left, string& right, string::size_type index, string::size_type length)
{
    if (index + length < left.length())
        right = left.substr(index + length);
    else
        right.clear();
    
    if (index < left.length())
        left = left.erase(index);
}