#pragma once
#include <string>
#include <lua.hpp>

namespace StatProjectLua
{
	void Initialize(lua_State* L);
}

#include "LuaHelpers.inl"