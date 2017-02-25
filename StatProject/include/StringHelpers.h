#pragma once
#include "Dataset.h"
#include <string>
#include <codecvt>
#include <locale>

namespace Data
{
	// Converts from an ASCII/UTF8 string to StringT
	StringT toStringT(std::string Rhs)
	{
		return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(Rhs);
	}

	std::vector<std::string> SplitStringByDelim(std::string inString, std::string inDelim)
	{
		std::vector<std::string> Out;
		size_t pos = 0;
		while ((pos = inString.find(inDelim)) != std::string::npos) {
			Out.push_back(inString.substr(0, pos));
			inString.erase(0, pos + inDelim.length());
		}
		Out.push_back(inString);
		return Out;
	}
}