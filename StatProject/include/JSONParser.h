#pragma once
#include <json/json.h>

// Haven't added anything to this yet

namespace Data
{
	class Dataset;
}

namespace JSONParser
{
	void PrintJson(const Json::Value& Value, uint16_t Level);

	void GetDatasetFromTypeform(const Json::Value& Value, Data::Dataset& Out);
}

