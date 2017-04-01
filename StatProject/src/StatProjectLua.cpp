#include "StatProjectLua.h"
#include "StringHelpers.h"
#include "IO.h"
#include <vector>
#include <map>

using namespace StatProjectLua;
using namespace std;

typedef pair<string, lua_CFunction> TLuaFunc;
typedef vector<TLuaFunc> TLuaFuncs;

static void PushLuaFunction(lua_State* L, TLuaFunc LuaFunc);
static TLuaFuncs& GetLuaFuncs();
static map<string, string> GetLuaArgs(lua_State* L);
static TLuaFuncs::size_type GetLuaFuncsTableSize(TLuaFuncs LuaFuncs, std::vector<std::string> Table = {});

void StatProjectLua::Initialize(lua_State* L)
{
	auto LuaArgs = GetLuaArgs(L);

	try
	{
		if (LuaArgs["colorize"] == "off")
		{
			IO::SetColorizedPrint(false);
		}
	}
	catch (exception& e)
	{
	}

	IO::Printf("Registering %zu functions...", GetLuaFuncs().size());

	if (GetLuaFuncs().size())
	{
		lua_createtable(L, 0, GetLuaFuncsTableSize(GetLuaFuncs()));
		for (auto LuaFunc : GetLuaFuncs())
		{
			PushLuaFunction(L, LuaFunc);
		}
	}
}

static TLuaFuncs& GetLuaFuncs()
{
	static TLuaFuncs LuaFuncs;

	return LuaFuncs;
}

static TLuaFuncs::size_type GetLuaFuncsTableSize(TLuaFuncs LuaFuncs, std::vector<std::string> Table)
{
	using namespace std;
	TLuaFuncs::size_type TableSize = 0;
	vector<string> TableNames;

	for (auto LuaFunc : LuaFuncs)
	{
		auto Namespace = Data::SplitStringByDelim(LuaFunc.first, "_");

		// If the namespace has more or an equal amount of nesting, see if the namespace names are the same as the table
		bool bNamespaceIsPartOfTable = (Namespace.size() >= Table.size());

		// Check if each nested name is the same as the table name
		if (bNamespaceIsPartOfTable)
		{
			for (int i = 0; i < Table.size(); i++)
			{
				if (Table[i] != Namespace[i])
				{
					bNamespaceIsPartOfTable = false;
				}
			}
		}

		// Now we can finally say that the namespace is part of the table
		if (bNamespaceIsPartOfTable)
		{
			if (Table.size())
			{
				// Chop off the first part of the namespace
				vector<string> NewNamespace;
				for (int i = Table.size(); i < Namespace.size(); i++)
				{
					NewNamespace.push_back(Namespace[i]);
				}
				Namespace = NewNamespace;
			}

			if (Namespace.size())
			{
				// If we find the namespace in the list of tables, then the table has already been accounted for in the TableSize
				bool bTableAlreadyAccounted = false;
				for (auto TableName : TableNames)
				{
					if (TableName == Namespace[0])
					{
						bTableAlreadyAccounted = true;
						break;
					}
				}

				// Push back the new table name and increment the TableSize
				if (!bTableAlreadyAccounted)
				{
					TableNames.push_back(Namespace[0]);
					TableSize++;
				}
			}
			else
			{
				TableSize++;
			}
		}
	}

	return TableSize;
}

LuaFunctionDeclarer::LuaFunctionDeclarer(std::string Name, lua_CFunction Func)
{
	GetLuaFuncs().push_back(TLuaFunc(Name, (lua_CFunction)Func));
}

void PushLuaFunction(lua_State* L, TLuaFunc LuaFunc)
{
	// If we found a '_' in the name, that means that this function is in a table
	if (LuaFunc.first.find('_') != std::string::npos)
	{
		
	}
	else
	{
		lua_pushstring(L, LuaFunc.first.c_str());
		lua_pushcfunction(L, (LuaFunc.second));
		lua_settable(L, -3);
	}
}

static map<string, string> GetLuaArgs(lua_State* L)
{
	map<string, string> ReturnVal;

	/* table is in the stack at index 't' */
	lua_pushnil(L);  /* first key */
	while (lua_next(L, 1) != 0)
	{
		pair<string, string> NewPair;

		NewPair.first = string(lua_tostring(L, -2));

		switch (lua_type(L, -1))
		{
		case LUA_TBOOLEAN:
		{
			NewPair.second = (lua_toboolean(L, -1) ? "true" : "false");
		}
		break;

		case LUA_TSTRING:
		{
			NewPair.second = string(lua_tostring(L, -1));
		}
		break;

		default:
			break;
		}

		/* removes 'value'; keeps 'key' for next iteration */
		lua_pop(L, 1);

		ReturnVal.insert(NewPair);
	}

	return ReturnVal;
}