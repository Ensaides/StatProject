#pragma once
#include <string>
#include <lua.hpp>

namespace StatProjectLua
{
	void Initialize(lua_State* L);

	struct LuaFunctionDeclarer
	{
		LuaFunctionDeclarer(std::string Name, lua_CFunction Func);
	};
}

#include "LuaHelpers.inl"