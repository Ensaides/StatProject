#pragma once
#include <map>
#include <string>

namespace Config
{
	void LoadConfig();

	std::map<std::string, std::string>& GetConfigValues();
}