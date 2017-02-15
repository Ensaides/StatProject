#pragma once

#include <string>
#include <functional>

namespace Connection
{
	// Takes in the page url, returns the raw data in the std::string return
	// This is a blocking function
	std::string DownloadPage(const std::string URL);

	// Takes in the page url and a callback function,
	// the callback function will be passed in the raw HTML once the page is done downloading.
	void DownloadPageThreaded(const std::string URL, std::function<void(std::string)> Callback);
}
