#include "StatProject.h"
#include "StatProjectLua.h"
#include "Connection.h"
#include "IO.h"
#include "JSONParser.h"
#include "Config.h"
#include "Dataset.h"

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

	// Download the crap
	Connection::DownloadPageThreaded("https://api.typeform.com/v1/form/G1Zwk8?key=" + Config::GetConfigValues()["TypeformMacAPIKey"],
		[](std::string Data) -> void
	{
		using namespace Json;

		auto NewReader = Reader();

		Value Root;

		NewReader.parse(Data, Root);

		JSONParser::PrintJson(Root, 0);
	});

	using namespace Data;
	auto NewDataset = Dataset();
	NewDataset.AddField(Field<uint32_t>("New Field"));

	return 1;
}