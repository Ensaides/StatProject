#include "StatProject.h"
#include "IO.h"
#include "Config.h"
#include "ExcelParser.h"
#include "Database.h"

void StatProject::Initialize(int argc, const char **argv)
{
	// Don't touch this order
	if (argc)
	{
		IO::SetProgramDirectory(std::string(argv[0]));
	}

	Config::LoadConfig();
	Database::Connect();
	Database::LoadData();
	Parsers::Excel::Init();
}