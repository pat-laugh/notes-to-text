//MIT License
//Copyright 2017 Patrick Laughrea
#include "utilsHtml.hpp"

#include <cassert>

using namespace ntt;
using namespace std;

string  ntt::getTagName(HtmlTag tag)
{
	switch (tag)
	{
	case HtmlTag::B: return "b";
	case HtmlTag::BODY: return "body";
	case HtmlTag::BR: return "br";
	case HtmlTag::DIV: return "div";
	case HtmlTag::HTML: return "html";
	case HtmlTag::I: return "i";
	case HtmlTag::P: return "p";
	case HtmlTag::SPAN: return "span";
	case HtmlTag::SUB: return "sub";
	case HtmlTag::U: return "u";
	}
	assert(false);
}

HtmlTag ntt::getTag(const string& str)
{
	if (str.empty())
		return HtmlTag::ERROR;
	switch (str[0])
	{
	case 'b':
		if (str == "b") return HtmlTag::B;
		else if (str == "body") return HtmlTag::BODY;
		else if (str == "br") return HtmlTag::BR;
		break;
	case 'd': if (str == "div") return HtmlTag::DIV; break;
	case 'h': if (str == "html") return HtmlTag::HTML; break;
	case 'i': if (str == "i") return HtmlTag::I; break;
	case 'p': if (str == "p") return HtmlTag::P; break;
	case 's':
		if (str == "span") return HtmlTag::SPAN;
		else if (str == "sub") return HtmlTag::SUB;
		break;
	case 'u': if (str == "u") return HtmlTag::U; break;
	default:
		break;
	}
	return HtmlTag::ERROR;
}