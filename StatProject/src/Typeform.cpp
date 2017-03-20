#include "Typeform.h"
#include "IO.h"
#include "Connection.h"
#include "JSONParser.h"
#include "Dataset.h"
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
			if (Typeform.GetName() == Index)
			{
				return Typeform;
			}
		}

		throw std::runtime_error("Could not find index '" + Index + "' in Typeform list");
	}
}