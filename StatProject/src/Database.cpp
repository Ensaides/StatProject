#include "Database.h"
#include "IO.h"
#include "Config.h"
#include <sstream>
#include <tuple>
#include <utility>
#include "Typeform.h"

#define OTL_ODBC // Compile OTL 4.0/ODBC
#define OTL_ODBC_UNIX
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_UNICODE // Compile OTL with Unicode

#if defined(__GNUC__)

namespace std {
	typedef unsigned short unicode_char;
	typedef basic_string<unicode_char> unicode_string;
}

#define OTL_UNICODE_CHAR_TYPE std::unicode_char
#define OTL_UNICODE_STRING_TYPE std::unicode_string

#else

#define OTL_UNICODE_CHAR_TYPE wchar_t
#define OTL_UNICODE_STRING_TYPE wstring
#endif

// Hell yeah, tuples
#define OTL_STREAM_WITH_STD_TUPLE_ON

#define OTL_CPP_17_ON
#include <otlv4.h>

static otl_connect db;
static Data::TypeformList Typeforms;

static void DownloadTypeforms();

// Handy dandy lil function for printing errors
static inline void PrintErrors(const otl_exception& p)
{
	IO::Printf("OTL Error Code:\t\t %d", p.code);
	IO::Printf("OTL Error Message:\t%s", p.msg);
}

void Database::Query(std::string Query, bool bCatchException = true)
{
	try
	{
		std::string SQLStatement;
		auto Stream = std::istringstream(Query);
		while (std::getline(Stream, SQLStatement, ';'))
		{
			otl_cursor::direct_exec(db, SQLStatement.c_str(), otl_exception::enabled);
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
	// Initialize the database API environment
	otl_connect::otl_initialize();

	try
	{
		// Login to the database
		db.rlogon(std::string(
			  Config::GetConfigValues()["DatabaseUsername"] + "/"
			+ Config::GetConfigValues()["DatabasePassword"] + "@"
			+ Config::GetConfigValues()["ConnectorName"]
		).c_str());

		IO::Printf("Connected to database '%s'.", Config::GetConfigValues()["ConnectorName"].c_str());
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

void Database::LoadData()
{
	DownloadTypeforms();

	IO::Printf("Loaded %d Typeforms...", Typeforms.size());
}

void DownloadTypeforms()
{
	using namespace std;

	try
	{
		otl_stream SelectStream;
		SelectStream.open(256, "SELECT * FROM `pantherhackers`.`typeforms`", db);

		if (!SelectStream.eof())
		{
			// For each row, get the data and make a new typeform
			for (auto& Row : SelectStream)
			{
				struct
				{
					OTL_UNICODE_STRING_TYPE TypeformID;
					OTL_UNICODE_STRING_TYPE APIKey;
					OTL_UNICODE_STRING_TYPE Name;
				} TypeformData;

				Row >> TypeformData.TypeformID;
				Row >> TypeformData.APIKey;
				Row >> TypeformData.Name;

				Data::Typeform NewTypeform(string(TypeformData.TypeformID.begin(),	TypeformData.TypeformID.end()),
										   string(TypeformData.APIKey.begin(),		TypeformData.APIKey.end()),
										   string(TypeformData.Name.begin(),		TypeformData.Name.end()),
										   true);

				Database::GetTypeforms().push_back(NewTypeform);
			}
		}
	}
	catch (otl_exception& p)
	{
		PrintErrors(p);
		return;
	}
}

void Database::SaveData()
{
	for (auto& Typeform : Typeforms)
	{
		// Fix this so that people cant do injections
		Query("REPLACE INTO `pantherhackers`.`typeforms` (`TypeformID`, `APIKey`, `Name`) VALUES ('" + Typeform.GetTypeformID() + "', '" + Typeform.GetAPIKey() + "', '" + Typeform.GetName() + "');");
	}
}

Data::TypeformList& Database::GetTypeforms()
{
	return Typeforms;
}