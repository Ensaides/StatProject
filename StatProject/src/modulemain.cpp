#include "StatProject.h"
#include "StatProjectLua.h"
#include "Connection.h"
#include "StringHelpers.h"
#include "IO.h"
#include "JSONParser.h"
#include "Config.h"
#include "Typeform.h"
#include "Dataset.h"
#include <iostream>
#include <fstream>
#include "StringHelpers.h"
#include "Database.h"

extern "C"
{
	int __lua_main(lua_State* L);
}

using namespace std;
using namespace Data;

//static void OnDownload(string Data);
static vector<StringT> SplitStringsByDelims(vector<StringT> Strings, vector<StringT> Delims);
static vector<pair<string, string>> GetLuaArgs(lua_State* L);

// Lua should call this when loading the library
// loadlib("statproject.so", "__lua_main")
int __lua_main(lua_State* L)
{
	const char* dir = "/home/nicholas/Work/projects/StatProject/bin/debug/statproject";
	const char** dirargarray = &dir;

	StatProject::Initialize(1, dirargarray);
	StatProjectLua::Initialize(L);

	Database::SaveData();

	return 1;
}

/*
void OnDownload(string Data)
{
	using namespace Json;

	auto NewReader = Reader();

	Value Root;

	NewReader.parse(Data, Root);
	Data::Dataset NewDataset;

	JSONParser::GetDatasetFromTypeform(Root, NewDataset);

	for (auto Field : NewDataset)
	{
		//cout << "ID: " << Field->GetID() << endl;
	}
	//cout << endl;
	
	for (auto Field : NewDataset)
	{
		if (auto StringField = Cast<StringT>(Field))
		{
			vector<StringT> WordDelims = { u",", u"/", u" ", u";", u"\n", u"\r" };

			vector<StringT> SplitString = SplitStringsByDelims(StringField->GetData(), WordDelims);

			auto Occurrences = GetNumberOfWordOccurrences(StringField->GetData());

			struct CompareStruct
			{
				bool operator() (StringOccurrence& Lhs, StringOccurrence& Rhs) { return Lhs > Rhs; };
			} MyCompareStruct;

			sort(Occurrences.begin(), Occurrences.end(), MyCompareStruct);

			for (auto Occurrence : Occurrences)
			{
				cout << "Number : " << Occurrence.NumberOfOccurrences;

				cout << " Word : '";
				for (wchar_t Char : Occurrence.String)
				{
					wcout << Char;
				}
				cout << "'" << endl;
			}
			cout << endl;

			cout << endl << endl;
		}
	}
}
*/

vector<StringT> SplitStringsByDelims(vector<StringT> Strings, vector<StringT> Delims)
{
	vector<StringT> Out;

	for (auto String : Strings)
	{
		auto NewStrings = SplitStringTByDelim(String, Delims.back());

		for (auto NewString : NewStrings)
		{
			Out.push_back(NewString);
		}
	}
	Delims.pop_back();

	if (Delims.empty())
	{
		return Out;
	}
	else
	{
		return SplitStringsByDelims(Out, Delims);
	}
}