#pragma once
#include <json/json.h>

// TSV = Tab Separated Values

namespace TSVParser
{
	// This will parse a text file with tab separated values and turn it into a json array
	Json::Value TSVToJson(std::string& TSV);
}