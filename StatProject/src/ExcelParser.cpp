#include "ExcelParser.h"
#include "IO.h"
#include <libxl.h>

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace libxl;

namespace Parsers
{
	void Excel::Init()
	{
		string dir = string(IO::GetProgramDirectory() + "data/upload");
		vector<string> files = vector<string>();

		IO::GetFilesInDirectory(dir, files);

		IO::Printf("Loaded %d excel files...", files.size());
	}
}

static void Test()
{
	Book* book = xlCreateBook();

	if (book->load(""))
	{

	}
}
