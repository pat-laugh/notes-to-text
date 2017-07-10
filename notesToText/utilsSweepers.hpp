//MIT License
//Copyright 2017 Patrick Laughrea
#pragma once

#include "utilsHtml.hpp"
#include "various/smartIterator.hpp"

namespace ntt
{
	bool readToText(various::SmartIterator& it, const std::string& text, std::string& readText);
	bool readToTag(various::SmartIterator& it, HtmlTag tag, bool endTag, std::string& readText);
}