#include "StatProjectLua.h"
#include "StringHelpers.h"
#include "JSONParser.h"
#include "Database.h"
#include "IO.h"

using namespace std;

lua_function(GetExcelFiles) 
{
	string Dir = string(IO::GetProgramDirectory() + "data/upload");
	vector<string> Files;
	vector<string> ExcelFiles;

	IO::GetFilesInDirectory(Dir, Files);

	for (auto File : Files)
	{
		if (Data::EndsWith(File, ".xlsx"))
		{
			ExcelFiles.push_back(File);
		}
	}

	using namespace Json;
	Value Root;

	for (auto ExcelFile : ExcelFiles)
	{
		Root.append(ExcelFile);
	}

	lua_pushstring(L, Root.toStyledString().c_str());

	return 1;
}

lua_function(GetTypeforms)
{
	using namespace Json;
	Value Root;

	for (auto& Typeform : Database::GetTypeforms())
	{
		Value NewTypeform;
		NewTypeform["TypeformID"]	= Typeform.GetTypeformID();
		NewTypeform["Name"]			= Typeform.GetName();

		Root.append(NewTypeform);
	}

	lua_pushstring(L, Root.toStyledString().c_str());

	return 1;
}