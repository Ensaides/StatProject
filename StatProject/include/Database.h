#pragma once
#include <string>

namespace Database
{
	// Set up the database connection and execute startup scripts to set up the database
	// if it hasn't been set up already
	void Connect();

	void Query(std::string Query, bool bCatchException);
}
