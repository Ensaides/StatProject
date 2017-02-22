#include "ExcelParser.h"
#include "IO.h"
#include <libxl.h>

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace libxl;

void ExcelParser::Init()
{
	string dir = string(IO::GetProgramDirectory() + "data/upload");
	vector<string> files = vector<string>();

	IO::GetFilesInDirectory(dir, files);

	for (auto file : files) 
	{
		IO::Print(file);
	}
}

static void Test()
{
	Book* book = xlCreateBook();

	if (book->load(""))
	{

	}
}
