#include "StatProject.h"
#include "Connection.h"
#include "HTMLParser.h"
#include "JSONParser.h"
#include "Config.h"

int main(int argc, char **argv)
{
	StatProject::Initialize(argc, argv);

	Connection::DownloadPageThreaded("https://api.typeform.com/v1/form/l29JZA?key=" + Config::GetConfigValues()["TypeformAPIKey"],
		[](std::string Data) -> void
	{
		using namespace Json;

		auto NewReader = Reader();
		Value Root;

		NewReader.parse(Data, Root);

		//JSONParser::PrintJson(Root, 0);
	});

	Connection::DownloadPageThreaded("https://api.typeform.com/v1/form/G1Zwk8?key=" + Config::GetConfigValues()["TypeformMacAPIKey"],
		[](std::string Data) -> void
	{

	});

	printf("Press any button to exit...\n");
	getc(stdin);
}