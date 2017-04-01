#include "StatProjectLua.h"
#include "Typeform.h"
#include "IO.h"
#include "Connection.h"
#include "JSONParser.h"
#include "Dataset.h"
#include "Database.h"
#include <memory>

using namespace Json;
using namespace std;

namespace Data
{
	// Typeform Members
	Typeform::Typeform(const std::string& inID, const std::string& inAPIKey, const std::string& inName, bool bDownloadData)
	{
		TypeformID = inID;
		APIKey = inAPIKey;
		Name = inName;
		
		if (bDownloadData)
		{
			DownloadTypeform();
		}
		else
		{
			Dataset = nullptr;
		}
	}

	Typeform::Typeform(const Typeform& Other)
	{
		using namespace std;

		TypeformID = Other.TypeformID;
		APIKey = Other.APIKey;
		Name = Other.Name;
		Dataset = (Other.Dataset ? unique_ptr<Data::Dataset>(new Data::Dataset(*Other.Dataset)) : nullptr);
	}

	void Typeform::DownloadTypeform()
	{
		Dataset = move(unique_ptr<Data::Dataset>(new Data::Dataset()));
		
		// Download the json data
		auto TypeformRawJson = Connection::DownloadPage("https://api.typeform.com/v1/form/" + TypeformID + "?key=" + APIKey);
		
		// Parse the json data
		Reader NewReader;
		Value Root;
		NewReader.parse(TypeformRawJson, Root);

		// Get the data from the json
		Parsers::JSON::GetDatasetFromTypeform(Root, *Dataset.get());
	}

	Data::Dataset& Typeform::GetData()
	{
		if (Dataset.get())
		{
			return *Dataset.get();
		}
		else
		{
			throw std::runtime_error("Typeform::Dataset was null");
		}
	}

	std::string Typeform::GetTypeformID() const
	{
		return TypeformID;
	}

	std::string Typeform::GetAPIKey() const
	{
		return APIKey;
	}

	std::string Typeform::GetName() const
	{
		return Name;
	}

	// TypeformList Members
	Typeform& TypeformList::operator[](std::string Index)
	{
		for (auto& Typeform : *this)
		{
			if (Typeform.GetTypeformID() == Index)
			{
				return Typeform;
			}
		}

		throw std::runtime_error("Could not find index '" + Index + "' in Typeform list");
	}

	bool TypeformList::Exists(std::string Index)
	{
		for (auto& Typeform : *this)
		{
			if (Typeform.GetTypeformID() == Index)
			{
				return true;
			}
		}

		return false;
	}
}

#include <iostream>

lua_function(GetTypeformData)
{
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		auto Index = std::string(lua_tostring(L, 1));

		if (Database::GetTypeforms().Exists(Index))
		{
			Json::Value Root;

			auto& Typeform = Database::GetTypeforms()[Index];
			auto& Data = Typeform.GetData();

			Root["Fields"] = {};

			for (auto& Field : Data)
			{
				Root["Fields"][Field->GetName()] = {};
				Root["Fields"][Field->GetName()]["Type"] = Data::TypeNames[Field->GetType()];
			}

			lua_pushstring(L, Root.toStyledString().c_str());
			return 1;
		}
	}
	return 0;
}

lua_function(GetTypeformField)
{
	if (lua_type(L, 1) == lua_type(L, 2) == LUA_TSTRING)
	{
		auto TypeformID = std::string(lua_tostring(L, 1));
		auto FieldID = std::string(lua_tostring(L, 2));

		std::cout << "TypeformID: " << TypeformID << " FieldID: " << FieldID << std::endl;
	}

	return 0;
}