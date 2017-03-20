#pragma once
#include <json/json.h>

namespace Parsers
{
	namespace TSV
	{
		// This will parse a text file with tab separated values and turn it into a json array
		Json::Value TSVToJson(std::string& TSV);
	}
}