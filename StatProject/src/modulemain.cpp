#include "StatProject.h"
#include "StatProjectLua.h"
#include "Connection.h"
#include "IO.h"
#include "JSONParser.h"
#include "Config.h"
#include "Dataset.h"
#include <iostream>
#include <fstream>
#include "StringHelpers.h"

extern "C"
{
	int __lua_main(lua_State* L);
}

using namespace std;
using namespace Data;

static void OnDownload(string Data);
static vector<StringT> SplitStringsByDelims(vector<StringT> Strings, vector<StringT> Delims);

// Lua should call this when loading the library
// loadlib("statproject.so", "__lua_main")
int __lua_main(lua_State* L)
{
	const char* dir = "/home/nicholas/Work/projects/StatProject/bin/debug/statproject";
	const char** dirargarray = &dir;

	StatProject::Initialize(1, dirargarray);
	StatProjectLua::Initialize(L);

	// Download the crap
	Connection::DownloadPageThreaded("https://api.typeform.com/v1/form/G1Zwk8?key=" + Config::GetConfigValues()["TypeformMacAPIKey"],
		std::function<void(string)>(&OnDownload));

	//OnDownload(String);

	return 1;
}

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
			cout << "ID2: " << Field->GetID() << endl;
			vector<StringT> WordDelims = { u",", u"/", u" ", u";" };

			vector<StringT> SplitString = SplitStringsByDelims(StringField->GetData(), WordDelims);

			for (auto NewString : SplitString)
			{
				for (wchar_t Char : NewString)
				{
					wcout << Char;
				}
				cout << endl;
			}
			cout << endl;

			cout << endl << endl;
		}
	}

	for (auto Field : NewDataset)
	{
		cout << "SHIT ID: " << Field->GetID() << endl;
	}
	cout << endl;
}

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