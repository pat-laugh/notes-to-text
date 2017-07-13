//MIT License
//Copyright 2017 Patrick Laughrea
#pragma once

#include "utilsHtml.hpp"
#include "various/smartIterator.hpp"

namespace ntt
{
    //returns true if text is found, else false
    //brings the file pointer to the first character after the found text
    various::SmartIterator& getToText(various::SmartIterator& it, const std::string& text);
	
    //returns true if tag is found, else false
    //brings it to the first character after the tag
    various::SmartIterator& getToTag(various::SmartIterator& it, HtmlTag tag, bool endTag);
	
	various::SmartIterator& readToText(various::SmartIterator& it, const std::string& text, std::string& readText);
	various::SmartIterator& readToTag(various::SmartIterator& it, HtmlTag tag, bool endTag, std::string& readText);
}