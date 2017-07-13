//MIT License
//Copyright 2017 Patrick Laughrea
#pragma once

#include <string>

namespace ntt
{
	enum class HtmlTag {B, BODY, BR, DIV, HTML, I, P, SPAN, SUB, U};
	
	std::string getTagName(HtmlTag tag);
	
	HtmlTag getTag(const std::string& str);
	
	template <bool endTag>
	std::string makeTagString(HtmlTag tag)
	{
		return (endTag ? "</" : "<") + getTagName(tag) + ">";
	}
	
	template <bool endTag>
	std::string makeTagString(std::string tagName)
	{
		return (endTag ? "</" : "<") + std::move(tagName) + ">";
	}
}