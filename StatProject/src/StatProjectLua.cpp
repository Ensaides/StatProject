#include "StatProjectLua.h"
#include <vector>

using namespace StatProjectLua;

typedef std::pair<std::string, lua_CFunction> TLuaFunc;
typedef std::vector<TLuaFunc> TLuaFuncs;

static TLuaFuncs& GetLuaFuncs()
{
	static TLuaFuncs LuaFuncs;

	return LuaFuncs;
};

LuaFunctionDeclarer::LuaFunctionDeclarer(std::string Name, lua_CFunction Func)
{
	GetLuaFuncs().push_back(TLuaFunc(Name, (lua_CFunction)Func));
}

void StatProjectLua::Initialize(lua_State* L)
{
	printf("Registering %zu functions...\n", GetLuaFuncs().size());

	if (GetLuaFuncs().size())
	{
		// Push all of the lua funcs into a table
		lua_createtable(L, 0, GetLuaFuncs().size());
		for (auto LuaFunc : GetLuaFuncs())
		{
			lua_pushstring(L, LuaFunc.first.c_str());
			lua_pushcfunction(L, (LuaFunc.second));
			lua_settable(L, -3);
		}
	}
}