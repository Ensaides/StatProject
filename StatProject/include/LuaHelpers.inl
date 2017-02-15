#pragma once

#define DECLARE_LUA_FUNC(NAME, FUNC) \
	static auto __LUAFUNC_##NAME = StatProjectLua::LuaFunctionDeclarer(std::string(#NAME), (lua_CFunction)FUNC);

#define MAKE_LUA_FUNC(NAME) \
	extern "C" { int  __luafunc_##NAME (lua_State* L); } \
	DECLARE_LUA_FUNC(NAME, &__luafunc_##NAME) \
	int __luafunc_##NAME (lua_State* L)

// This will declare the function and then register it when the library is loaded
#define lua_function(NAME) MAKE_LUA_FUNC(NAME)
