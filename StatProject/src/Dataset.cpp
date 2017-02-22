#include "Dataset.h"

namespace Data
{
	// Field members
	const FieldTypeT FieldBase::GetType() const
	{
		return Type;
	}

	const std::string FieldBase::GetName() const
	{
		return Name;
	}


	// Dataset members
	void Dataset::AddField(FieldBase NewField)
	{
		Fields.push_back(NewField);
	}

	FieldBase& Dataset::GetField(std::string Name)
	{
		for (auto& Field : Fields)
		{
			if (Field.GetName() == Name)
			{
				return Field;
			}
		}

		throw std::runtime_error("Could not find field '" + Name + "' in dataset");
	}

	bool Dataset::FieldExists(std::string Name)
	{
		for (auto& Field : Fields)
		{
			if (Field.GetName() == Name)
			{
				return true;
			}
		}
		return false;
	}
}