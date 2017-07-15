//MIT License
//Copyright 2017 Patrick Laughrea
#pragma once

#include <string>

namespace ntt
{
	enum class HtmlTag {B, BODY, BR, DIV, HTML, I, P, SPAN, SUB, U};
	
	std::string getTagName(HtmlTag tag);
	
	template <bool endTag>
	std::string makeTagString(HtmlTag tag)
	{
		return (endTag ? "</" : "<") + getTagName(tag) + ">";
	}
}