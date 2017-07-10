//MIT License
//Copyright 2017 Patrick Laughrea
#pragma once

namespace ntt
{
	enum class HtmlTag {B, BODY, BR, DIV, HTML, I, P, SPAN, SUB, U, NONE, ERROR};
	
	template <bool endTag>
	std::string makeTagString(HtmlTag tag)
	{
		return (endTag ? "</" : "<") + getHtmlTagName(HtmlTag tag) + ">";
	}
	
	template <bool endTag>
	std::string makeTagString(std::string tagName)
	{
		return (endTag ? "</" : "<") + std::move(tag) + ">";
	}
	
	std::string getTagName(HtmlTag tag);
	
	HtmlTag getTag(const string& str);
}