#include "StatProject.h"
#include "StatProjectLua.h"
#include "Connection.h"
#include "IO.h"
#include "Config.h"

extern "C"
{
	int __lua_main(lua_State* L);
}

// Lua should call this when loading the library
// loadlib("statproject.so", "__lua_main")
int __lua_main(lua_State* L)
{
	const char* dir = "/home/nicholas/Work/projects/StatProject/bin/debug/statproject";
	const char** dirargarray = &dir;

	StatProject::Initialize(1, dirargarray);
	StatProjectLua::Initialize(L);

	//auto TestPage = Connection::DownloadPage("https://api.typeform.com/v1/form/l29JZA?key=" + Config::GetConfigValues()["TypeformAPIKey"]);

	//lua_pushlstring(L, TestPage.c_str(), TestPage.length());

	return 1;
}