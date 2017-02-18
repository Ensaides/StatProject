#include "IO.h"
#include <fstream>
#include <sstream>
#include <exception>
#include <time.h>
#include <cstdarg>

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

void IO::Print(const std::string& Text, bool bPrintDate, bool bLogToFile)
{
	if (bPrintDate)
	{
		time_t rawtime;
		struct tm* timeinfo;
		char date[80];
		char time[80];

		::time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(date, 80, "%x", timeinfo);
		strftime(time, 80, "%X", timeinfo);

		// These weird expressions colorize the output
		printf("\033[38;5;253m%s\033[0m \033[38;5;247m%s\033[0m %s\n", date, time, Text.c_str());
	}
	else
	{
		printf("%s\n", Text.c_str());
	}
}

void IO::Printf(const char* Format, ...)
{
	// Forward the args into snprintf
	va_list args;
	va_start(args, Format);
	char buffer[1024];
	vsnprintf(buffer, 1024, Format, args);
	va_end(args);

	Print(std::string(buffer));
}