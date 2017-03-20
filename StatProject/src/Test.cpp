#include "StatProjectLua.h"
#include "TSVParser.h"
#include "IO.h"
//#include "Database.h"
#include <sstream>
/*
lua_function(TestFunction)
{
	std::string TestString(IO::ReadFile("data/Meeting(1) S2017-report.tsv"));
	Json::Value Root = Parsers::TSV::TSVToJson(TestString);

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
	auto Meeting1Json = Parsers::TSV::TSVToJson(Meeting1);

	for (auto Record : Meeting1Json)
	{
		for (int i = 0; i < 7; i++)
		{
			try
			{
				switch (i) // 3-6
				{
				case 3:
					printf("\t%s\n", Record.getMemberNames()[i].c_str());
					break;
				case 4:
					printf("\t%s\n", Record.getMemberNames()[i].c_str());
					break;
				case 5:
					printf("\t%s\n", Record.getMemberNames()[i].c_str());
					break;
				case 6:
					printf("\t%s\n", Record.getMemberNames()[i].c_str());
					break;

				default:
					break;
				}
			}
			catch (std::exception& e)
			{

			}
		}

		printf("\n");
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
*/