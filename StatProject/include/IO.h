#pragma once
#include <string>

namespace IO
{
	void SetProgramDirectory(std::string ProgramDirectory);

	std::string GetProgramDirectory();

	// Read a file in to the std::string return
	// The directory is relative to the project directory
	std::string ReadFile(std::string FilePath);
}
