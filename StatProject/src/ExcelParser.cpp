#include "ExcelParser.h"
#include <libxl.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace libxl;

static int getdir(string dir, vector<string>& files)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}

int ExcelParser::Test()
{
	string dir = string(".");
	vector<string> files = vector<string>();

	getdir(dir, files);

	for (unsigned int i = 0; i < files.size(); i++) {
		cout << files[i] << endl;
	}
	return 0;
}

static void Test()
{
	Book* book = xlCreateBook();

	if (book->load(""))
	{

	}
}
