//MIT License
//Copyright 2017 Patrick Laughrea
#include "parser.hpp"

#include "utilsHtml.hpp"

using namespace ntt;
using namespace std;
using namespace various;

void splitString(string& left, string& right, int index, int length = 1);

Parser::Parser(SmartIterator it) : it(move(it)) {}
Parser::~Parser() {}

vector<string> Parser::parse()
{
	vector<string> lines;
	readStart();
	lines.push_back(move(left));
	while (getLine())
		lines.push_back(move(left));
	return lines;
}

void Parser::readStart()
{
	//in note file, text immediately follows body, so get it
	//and put text in next tag in leftover for use later
	readToTag(in, HtmlTag::DIV, false, left);
	readToTag(in, HtmlTag::DIV, true, leftover);
}

bool FileManager::getLine()
{
    readNextTag();
    return !left.empty() || it;
}

void Parser::readNextTag()
{
    if (leftover.length() > 0)
    {
        left = move(leftover);
        leftover.clear(); //to be sure it's now empty
    }
    else
    {
        if (!utils::readNextTag(in, tagToGet, left))
        {
            //there could still be text after the last tag, before the ending body tag
            string tag = utils::makeTag(HtmlTag::BODY, true);
            auto index = left.find(tag);
            if (index != string::npos)
                left.erase(index);
            else
                left.clear();
        }
        else
        {
            string tag = utils::makeTag(tagToGet, false);
            auto index = left.find(tag);
            if (index != string::npos)
                splitString(left, leftover, index, tag.length());
        }
    }
}

//takes the left string and puts all the part after the index into the right string
//the part in the middle (the index + length) is removed
void splitString(string& left, string& right, int index, int length)
{
    if (index + length < left.length())
        right = left.substr(index + length);
    else
        right.clear();
    
    if (index < left.length())
        left = left.erase(index);
}