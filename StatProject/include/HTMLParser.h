#pragma once

#include <vector>
#include <string>

namespace HTMLParser
{
	// A single HTML element
	struct HTMLElem
	{
		std::string Name;
		std::vector<std::string> Attributes;
		std::string Text;

		//  This is probs slow for traversal, look at a better tree structure later
		//  If this is -1, then theres no parent
		int32_t ParentElemIndex;
	};

	//  A struct containing all of the html elements in a page
	struct HTMLPage
	{
		std::vector<HTMLElem> Elements;
	};

	// Takes in HTML, returns a vector of html elements
	HTMLPage ParseHTML(const std::string &html);
};
