#pragma once
#include <json/json.h>

namespace TSVParser
{
	Json::Value TSVToJson(std::string& TSV);
}