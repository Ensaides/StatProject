#include "Database.h"
#include "IO.h"
#include "Config.h"
#include <sstream>
#include <tuple>
#include <utility>

#define OTL_ODBC // Compile OTL 4.0/ODBC
#define OTL_ODBC_UNIX
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_UNICODE // Compile OTL with Unicode

#define OTL_CPP_14_ON
#include <otlv4.h>

static otl_connect db;

// Handy dandy lil function for printing errors
static inline void PrintErrors(otl_exception& p)
{
	printf("OTL Error Code:\t\t %d\n", p.code);
	printf("OTL Error Message:\t%s\n", p.msg);
}

// Execute a query on the database object
// If bCatchException = true, then the func will catch any OTL exceptions and print errors
// If bCatchException = false, it will rethrow the exceptions
void Database::Query(std::string Query, bool bCatchException = true)
{
	try
	{
		std::string SQLStatement;
		auto Stream = std::istringstream(Query);
		while (std::getline(Stream, SQLStatement, ';'))
		{
			otl_cursor::direct_exec
			(
				db,
				SQLStatement.c_str(),
				otl_exception::enabled
			);
		}

	}
	catch (otl_exception& p)
	{
		if (bCatchException)
		{
			PrintErrors(p);
		}
		else
		{
			throw;
		}
	}
}

void Database::Connect()
{
	// initialize the database API environment
	otl_connect::otl_initialize();

	try
	{
		// Login to the database
		db.rlogon(std::string(
			  Config::GetConfigValues()["DatabaseUsername"] + "/"
			+ Config::GetConfigValues()["DatabasePassword"] + "@"
			+ Config::GetConfigValues()["ConnectorName"]
		).c_str());

		printf("Connected to database.\n");
	}
	catch (otl_exception& p)
	{
		PrintErrors(p);
		return;
	}

	// This is the startup script for the database. It will set up the schema.
	// This will not be called if we aren't logged in to the database
	Query(IO::ReadFile("src/sql/database.sql"));
}