#pragma once
#include <string>

namespace IO
{
	void SetProgramDirectory(std::string ProgramDirectory);

	std::string GetProgramDirectory();

	// Read a file in to the std::string return
	// The directory is relative to the project directory
	std::string ReadFile(std::string FilePath);

	// Print with a few options
	void Print(const std::string& Text, bool bPrintDate = true, bool bLogToFile = true);

	// Print formatted. Unfortunately you can't decide if you want to print the date or log to file
	void Printf(const char* Format, ...);
}
