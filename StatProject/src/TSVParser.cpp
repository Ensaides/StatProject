#include "Config.h"
#include "TSVParser.h"
#include "IO.h"
#include <sstream>
#include <iostream>

Json::Value TSVParser::TSVToJson(std::string& TSV)
{
	using namespace std;

	// Create the config file if it doesnt exist and open it
	stringstream TSVStream(TSV);
	std::string Line;

	Json::Value ReturnValue;
	//ReturnValue.append(Json::Value(Json::arrayValue));

	// Parse the config file by line
	int LineNum = 0;
	std::vector<std::string> FieldNames;
	while (getline(TSVStream, Line))
	{
		if (LineNum == 0 && Line[0] == '#')
		{
			std::string Field;
			for (char c : Line)
			{
				switch (c)
				{
				case '\t':
					if (Field.size())
					{
						FieldNames.push_back(Field);
					}
					Field = "";
					break;

				default:
					Field += c;
					break;
				}
			}
		}
		else
		{
			bool bParsingComment = false;
			int FieldNum = 0;

			string Field;

			for (char c : Line)
			{
				// If we are not parsing a comment, parse the array
				if (!bParsingComment)
				{
					switch (c)
					{
					case '#':
						bParsingComment = true;
						break;

					case '\t':
						if (Field.size())
						{
							ReturnValue[LineNum][FieldNames[FieldNum]] = Field;
						}
						Field = "";
						FieldNum++;
						break;

					default:
						Field += c;
						break;
					}
				}
			}
			LineNum++;
		}
	}

	return ReturnValue;
}