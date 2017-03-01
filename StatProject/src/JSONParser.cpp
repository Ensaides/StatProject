#include "JSONParser.h"
#include "Dataset.h"
#include "IO.h"
#include <iostream>
#include <memory>
#include "StringHelpers.h"

static void GetQuestions(const Json::Value& Value, Data::Dataset& Out);
static void GetResponses(const Json::Value& Value, Data::Dataset& Out);

void JSONParser::PrintJson(const Json::Value& Value, uint16_t Level = 0)
{
	using namespace Json;

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
			PrintJson(Value[Name], Level + 1);
		}
	}
}

void JSONParser::GetDatasetFromTypeform(const Json::Value& Value, Data::Dataset& Out)
{
	using namespace Data;
	Out = Dataset();

	GetQuestions(Value, Out);
	GetResponses(Value, Out);
}

void GetQuestions(const Json::Value& Value, Data::Dataset& Out)
{
	using namespace Data;

	int i = 0;
	for (auto Elem : Value["questions"])
	{
		int64_t Index;
		for (auto InnerElem : Elem)
		{
			switch (i % 3)
			{
			// 0 = field_id
			case 0:
				Index = (int64_t)InnerElem.asInt64();
				break;

			// 1 = id
			case 1:
				if (!Out.FieldExists(Index))
				{
					auto SplitID = SplitStringByDelim(InnerElem.asString(), "_");

					if (SplitID.size())
					{
						if (SplitID[0] == "list" || SplitID[0] == "textfield" || SplitID[0] == "textarea")
						{
							Out.AddField(Field<StringT>("New Field", Index));
						}
						else if (SplitID[0] == "number")
						{
							Out.AddField(Field<IntegralT>("New Field", Index));
						}
					}
				}
				break;

			// 2 = question
			case 2:
				if (Out.FieldExists(Index) && InnerElem.isConvertibleTo(Json::ValueType::stringValue))
				{
					// Push back a new field for the question
					Out[Index].SetName(InnerElem.asString());
				}
				break;

			default:
				break;
			}

			i++;
		}
	}
}

void GetResponses(const Json::Value& Value, Data::Dataset& Out)
{
	using namespace std;
	using namespace Data;

	for (auto Elem : Value["responses"])
	{
		// For each answer in the responses
		for (string MemberName : Elem["answers"].getMemberNames())
		{
			auto& Answer = Elem["answers"][MemberName];
			// Split up the member name, get the ID from it
			vector<string> SplitNames = SplitStringByDelim(MemberName, "_");

			if (SplitNames.size() >= 2)
			{
				int64_t ID = stoll(SplitNames[1]);
				if (Out.FieldExists(ID))
				{
					// Switch on the type of the field
					switch (Out[ID].GetType())
					{

					// If the field type is string
					case FieldType::String:
					{
						auto StringField = Cast<StringT>(Out[ID]);
						if (StringField)
						{
							if (Answer.isConvertibleTo(Json::ValueType::stringValue))
							{
								StringT FieldData = toStringT(Answer.asString());
								StringField->GetData().push_back(FieldData);
							}
						}
					}
					break;

					// If the field type is integral
					case FieldType::Integral:
					{
						auto IntegralField = Cast<IntegralT>(Out[ID]);
						if (IntegralField)
						{
							if (Answer.isConvertibleTo(Json::ValueType::intValue))
							{
								IntegralField->GetData().push_back((IntegralT)Answer.asInt64());
							}
							else if (Answer.isConvertibleTo(Json::ValueType::stringValue))
							{
								// If the type is a string, convert it first
								IntegralField->GetData().push_back(std::stoll(Answer.asString()));
							}
						}
					}
					break;

					default:
						break;
					}
				}
			}
		}
	}
}