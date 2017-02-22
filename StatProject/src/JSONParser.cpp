#include "JSONParser.h"
#include "Dataset.h"

void JSONParser::PrintJson(Json::Value& Value, uint16_t Level)
{
	using namespace Json;

	//for (int i = 0; i < Level; i++)
	//	printf("\t");
	//printf("Value Type: %d\n", Value.type());

	switch (Value.type())
	{
	case ValueType::stringValue:
		for (int i = 0; i < Level; i++)
			printf("\t");
		printf("String: %s\n", Value.asCString());
		break;

	case ValueType::intValue:
		for (int i = 0; i < Level; i++)
			printf("\t");
		printf("Int: %d\n", Value.asInt());
		break;

	case ValueType::uintValue:
		for (int i = 0; i < Level; i++)
			printf("\t");
		printf("UInt: %u\n", Value.asUInt());
		break;

	case ValueType::realValue:
		for (int i = 0; i < Level; i++)
			printf("\t");
		printf("Float: %f\n", Value.asFloat());
		break;

	default:
		break;
	}

	if (Value.type() == ValueType::arrayValue)
	{
		for (auto Elem : Value)
		{
			PrintJson(Elem, Level + 1);
		}
	}
	else if (Value.type() == ValueType::objectValue)
	{
		for (auto Name : Value.getMemberNames())
		{
			for (int i = 0; i < Level; i++)
				printf("\t");

			printf("Member: %s\n", Name.c_str());
		}

		for (auto Elem : Value)
		{
			PrintJson(Elem, Level + 1);
		}
	}
}

void JSONParser::GetDatasetFromTypeform(Json::Value& Value, Data::Dataset& Out)
{
	using namespace Data;
	Out = Dataset();
}
