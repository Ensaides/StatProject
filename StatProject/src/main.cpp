#include "StatProject.h"
#include "Connection.h"
#include "HTMLParser.h"
#include "JSONParser.h"
#include "Config.h"

int main(int argc, char **argv)
{
	StatProject::Initialize(argc, argv);

	printf("Press any button to exit...\n");
	getc(stdin);
}