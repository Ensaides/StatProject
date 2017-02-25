#include "Connection.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cURLpp.hpp>

static size_t writer(char* data, size_t size, size_t nmemb, std::string* writerData);
static std::vector<std::thread> DownloadThreads;

std::string Connection::DownloadPage(const std::string URL)
{
	std::string OutBuffer;

	// Init CURL, curl_easy_init() also calls curl_global_init()
	CURL* curl;
	CURLcode res;
	curl = curl_easy_init();

	// If the curl init was successful
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &OutBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	// If the result != 0, it's an error code
	if (res)
	{
		fprintf(stderr, "CURL Error: %s\n", curl_easy_strerror(res));
	}

	return OutBuffer;
}

void Connection::DownloadPageThreaded(const std::string URL, std::function<void(std::string)> Callback)
{
	DownloadThreads.push_back(std::thread([URL, Callback]() -> void
	{
		Callback(DownloadPage(URL));
	}));
}

// Writer func, gets called by libcurl while downloading.
// It will append the data to the writerData string
size_t writer(char* data, size_t size, size_t nmemb, std::string* writerData)
{
	if (writerData == NULL)
	{
		return 0;
	}

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

#ifdef STATPROJECT_MODULE
#include "StatProjectLua.h"

lua_function(DownloadPage)
{
	std::string URL(lua_tostring(L, 1));

	lua_pushstring(L, Connection::DownloadPage(URL).c_str());
	return 1;
}

#endif