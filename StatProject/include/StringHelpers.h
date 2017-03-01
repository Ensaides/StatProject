#pragma once
#include "Dataset.h"
#include <string>
#include <codecvt>
#include <locale>
#include <algorithm>

namespace Data
{
	static void PrintStringT(StringT inString)
	{
		for (auto Char : inString)
		{
			std::wcout << (wchar_t)Char;
		}
		std::cout << std::endl;
	}

	// Converts from an ASCII/UTF8 string to StringT
	static StringT toStringT(std::string Rhs)
	{
		return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(Rhs);
	}


	// std::string
	// Splits a string into a vector of strings by the delimiter
	static std::vector<std::string> SplitStringByDelim(std::string inString, std::string inDelim)
	{
		std::vector<std::string> Out;
		size_t pos = 0;
		while ((pos = inString.find(inDelim)) != std::string::npos) 
		{
			auto Split = inString.substr(0, pos);
			if (Split.size())
			{
				Out.push_back(Split);
			}
			inString.erase(0, pos + inDelim.length());
		}

		if (inString.size())
		{
			Out.push_back(inString);
		}
		return Out;
	}


	// StringT
	// Splits a string into a vector of strings by the delimiter
	static std::vector<StringT> SplitStringTByDelim(StringT inString, StringT inDelim)
	{
		std::vector<StringT> Out;
		size_t pos = 0;
		while ((pos = inString.find(inDelim)) != StringT::npos)
		{
			auto Split = inString.substr(0, pos);
			if (Split.size())
			{
				Out.push_back(Split);
			}
			inString.erase(0, pos + inDelim.length());
		}

		if (inString.size())
		{
			Out.push_back(inString);
		}
		return Out;
	}

	/*
	// Split multiple strings by a single delim
	static std::vector<StringT> SplitStringTByDelim(std::vector<StringT> inStrings, StringT inDelim)
	{
		std::vector<StringT> Out;

		for (auto String : inStrings)
		{
			auto OutStrings = SplitStringTByDelim(String, inDelim);
			for (auto OutString : OutStrings)
			{
				Out.push_back(OutString);
			}
		}

		return Out;
	}

	// Split a string by multiple delimiters
	static std::vector<StringT> SplitStringTByDelim(StringT inString, std::vector<StringT> inDelim)
	{
		std::vector<StringT> Out;

		if (inDelim.size())
		{
			Out = SplitStringTByDelim(inString, inDelim.back());
			inDelim.pop_back();

			for (auto Delim : inDelim)
			{
				Out = SplitStringTByDelim(Out, Delim);
			}
		}

		return Out;
	}
	*/
	
	static uint32_t GetOccurrencesInString(std::string inString, std::string OccurrenceString, bool bFindLowercaseOccurrences = true)
	{
		using namespace std;
		if (bFindLowercaseOccurrences)
		{
			for (auto& Char : inString)
			{
				Char = tolower(Char);
			}
			
			for (auto& Char : OccurrenceString)
			{
				Char = tolower(Char);
			}
		}

		uint32_t occurrences = 0;
		std::string::size_type start = 0;

		while ((start = inString.find(OccurrenceString, start)) != string::npos) 
		{
			++occurrences;
			start += OccurrenceString.length();
		}

		return occurrences;
	}

	static uint32_t GetOccurrencesInString(std::u16string inString, std::u16string OccurrenceString, bool bFindLowercaseOccurrences = true)
	{
		using namespace std;
		if (bFindLowercaseOccurrences)
		{
			for (auto& Char : inString)
			{
				Char = tolower(Char);
			}

			for (auto& Char : OccurrenceString)
			{
				Char = tolower(Char);
			}
		}

		uint32_t occurrences = 0;
		std::u16string::size_type start = 0;

		while ((start = inString.find(OccurrenceString, start)) != string::npos) 
		{
			++occurrences;
			start += OccurrenceString.length(); // see the note
		}

		return occurrences;
	}


	struct StringOccurrence
	{
		uint32_t NumberOfOccurrences = 0;
		StringT String;
	};

	static std::vector<StringOccurrence> GetNumberOfWordOccurrences(std::vector<StringT> Words)
	{
		std::vector<StringOccurrence> Out;

		for (auto Word : Words)
		{
			StringOccurrence WordOccurrence;
			WordOccurrence.String = Word;

			for (auto CompWord : Words)
			{
				if (Word == CompWord)
				{
					WordOccurrence.NumberOfOccurrences++;
				}
			}

			for (auto iter = Words.begin(); iter != Words.end(); )
			{
				if (*iter == Word)
					iter = Words.erase(iter);
				else
					++iter;
			}

			Out.push_back(WordOccurrence);
		}

		return Out;
	}
}