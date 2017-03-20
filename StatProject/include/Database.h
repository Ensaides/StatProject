#pragma once
#include <string>
#include <list>
#include "Typeform.h"

namespace Database
{
	// Set up the database connection and execute startup scripts to set up the database
	// if it hasn't been set up already
	void Connect();

	// Execute a query on the database object
	// If bCatchException = true, then the func will catch any OTL exceptions and print errors
	// If bCatchException = false, it will rethrow the exceptions
	void Query(std::string Query, bool bCatchException);

	void LoadData();

	void SaveData();

	Data::TypeformList& GetTypeforms();
}
