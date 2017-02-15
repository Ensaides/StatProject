#include "Config.h"
#include "IO.h"
#include <fstream>
#include <iostream>

using namespace std;

static std::map<std::string, std::string> ConfigVars;

std::map<std::string, std::string>& Config::GetConfigValues()
{
	return ConfigVars;
}

void Config::LoadConfig()
{
	// Create the config file if it doesnt exist and open it
	fstream ConfigFile(IO::GetProgramDirectory() + "config/config.ini", fstream::in | fstream::out);
	std::string Line;
	
	// Parse the config file by line
	while (getline(ConfigFile, Line))
	{
		std::pair<std::string, std::string> NewConfigVar;
		bool bReadingComment = false;
		bool bReadingSecond = false;

		for (char c : Line)
		{
			switch (c)
			{
			// Hash means comment
			case '#':
				bReadingComment = true;
				break;

			case '=':
				bReadingSecond = true;
				break;

			// Don't interpret these
			case '\n':
			case '\t':
			case '\r':
			case ' ':
			case EOF:
			case 0:
				break;

			default:
				if (!bReadingComment)
				{
					bReadingSecond ? NewConfigVar.second += c : NewConfigVar.first += c;
				}
				break;
			}
		}

		if (NewConfigVar.first.size() && NewConfigVar.second.size())
		{
			// Add the new config to the config vars
			ConfigVars.insert(NewConfigVar);
		}
	}
}