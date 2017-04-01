#include "StatProjectLua.h"
#include "StringHelpers.h"
/*
#include "JSONParser.h"
#include "Database.h"
*/
#include "IO.h"

lua_function(GetPathAndOptionsFromURL)
{
	// If the first arg is a string
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		std::string URL(lua_tostring(L, 1));

		auto Files = Data::SplitStringByDelim(URL, "/");
		std::string Path;
		std::vector<std::string> Options;

		bool bReturnOptions;

		for (int i = 0; i < Files.size(); i++)
		{
			if (Files[i] != ".." && Files[i].find('.') != std::string::npos)
			{
				// Extract the path
				for (int j = 0; j <= i; j++)
				{
					Path += Files[j];

					if (j < i)
					{
						Path += '/';
					}
				}

				// Extract the options
				if ((i + 1) < Files.size())
				{
					bReturnOptions = true;
					for (int j = i + 1; j < Files.size(); j++)
					{
						Options.push_back(Files[j]);
					}
				}

				break;
			}
		}

		// Push the path
		lua_pushstring(L, Path.c_str());

		// Then push the options as a table
		if (bReturnOptions)
		{
			lua_createtable(L, 0, Options.size());

			int i = 1;
			for (auto Option : Options)
			{
				lua_pushnumber(L, i);
				lua_pushstring(L, Option.c_str());
				lua_settable(L, -3);

				i++;
			}
		}

		return bReturnOptions + 1;
	}
}

