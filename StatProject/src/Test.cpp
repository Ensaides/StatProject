#include "StatProjectLua.h"
#include "TSVParser.h"
#include "IO.h"
#include "Database.h"
#include <sstream>

lua_function(TestFunction)
{
	std::string TestString(IO::ReadFile("data/Meeting(1) S2017-report.tsv"));
	Json::Value Root = TSVParser::TSVToJson(TestString);

	TestString = "";
	for (auto Elem : Root)
	{
		TestString += Elem.toStyledString();
		TestString += '\n';
	}

	lua_pushstring(L, TestString.c_str());

	return 1;
}

static void LoadMeeting1()
{
	// Load the tsv from meeting[1] into a string
	std::string Meeting1(IO::ReadFile("data/Meeting(1) S2017-report.tsv"));
	Json::Value Root = TSVParser::TSVToJson(Meeting1);

	Meeting1 = "";
	for (auto Elem : Root)
	{
		Meeting1 += Elem.toStyledString();
		Meeting1 += '\n';
	}
}

lua_function(LoadData)
{
	LoadMeeting1();

	//std::stringstream DataStatistics; // Report the amount of data loaded in this string

	//DataStatistics << "Loaded " << Root.size() << " records from meeting 1\n";

	//lua_pushstring(L, DataStatistics.str().c_str());
	//lua_pushstring(L, Meeting1.c_str());
	lua_pushstring(L, "Hello!");

	return 1;
}