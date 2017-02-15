#include "JSONParser.h"

void JSONParser::PrintJson(Json::Value& Value, uint16_t Level)
{
	using namespace Json;

	for (int i = 0; i < Level; i++)
		printf("\t");
	printf("Value Type: %d\n", Value.type());

	switch (Value.type())
	{
	case ValueType::stringValue:
		for (int i = 0; i < Level; i++)
			printf("\t");
		printf("%s\n", Value.asCString());
		break;

	case ValueType::intValue:
		for (int i = 0; i < Level; i++)
			printf("\t");
		printf("%d\n", Value.asInt());
		break;

	case ValueType::uintValue:
		for (int i = 0; i < Level; i++)
			printf("\t");
		printf("%u\n", Value.asUInt());
		break;

	case ValueType::realValue:
		for (int i = 0; i < Level; i++)
			printf("\t");
		printf("%f\n", Value.asFloat());
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

			printf("%s\n", Name.c_str());
		}

		for (auto Elem : Value)
		{
			PrintJson(Elem, Level + 1);
		}
	}
}