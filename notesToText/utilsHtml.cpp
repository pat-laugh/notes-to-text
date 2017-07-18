//MIT License
//Copyright 2017 Patrick Laughrea
#include "utilsHtml.hpp"

#include <cassert>

using namespace ntt;
using namespace std;

string ntt::getTagName(HtmlTag tag)
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