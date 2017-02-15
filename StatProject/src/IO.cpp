#include "IO.h"
#include <fstream>
#include <sstream>
#include <exception>

static std::string Directory;
static bool bSetDirectory = false;

void IO::SetProgramDirectory(std::string ProgramDirectory)
{
	// Remove the program name, debug, bin folder from the string
	for (int i = 0; i < 3; i++)
	{
		while (ProgramDirectory.size() && ProgramDirectory[ProgramDirectory.size() - 1] != '/')
		{
			ProgramDirectory.pop_back();
		}

		if (i < 2 && ProgramDirectory[ProgramDirectory.size() - 1] == '/')
		{
			ProgramDirectory.pop_back();
		}
	}

	Directory = ProgramDirectory;

	bSetDirectory = true;
}

std::string IO::GetProgramDirectory()
{
	if (!bSetDirectory)
	{
		throw std::runtime_error("Failed to call SetProgramDirectory() before using IO functions");
	}

	return Directory;
}

std::string IO::ReadFile(std::string FilePath)
{
	if (!bSetDirectory)
	{
		throw std::runtime_error("Failed to call SetProgramDirectory() before using IO functions");
	}

	std::ifstream t(Directory + FilePath);
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}