#pragma once
#include <string>
#include <memory>
#include <list>
#include "Dataset.h"

namespace Data
{
	class Typeform
	{
		std::string TypeformID;
		std::string APIKey;
		std::string Name;

		std::unique_ptr<Data::Dataset> Dataset;

	public:
		Typeform(const std::string& inID, const std::string& inAPIKey, const std::string& inName, bool bDownloadData = true);

		Typeform(const Typeform& Other);

		void DownloadTypeform();

		Data::Dataset& GetData();

		std::string GetTypeformID() const;
		std::string GetAPIKey() const;
		std::string GetName() const;
	};

	class TypeformList : public std::list<Typeform>
	{
	public:
		Typeform& operator[](std::string Index);

		bool Exists(std::string Index);
	};
}
