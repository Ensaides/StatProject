#include "Dataset.h"
#include <iostream>

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

	void FieldBase::SetName(std::string inName)
	{
		Name = inName;
	}

	const int64_t FieldBase::GetID() const
	{
		return ID;
	}

	void FieldBase::SetID(int64_t inID)
	{
		ID = inID;
	}

	void FieldBase::Print()
	{
		using namespace std;
		cout << "Field name: " << GetName() << endl;

		switch (GetType())
		{

		case FieldType::String:
		{
			auto Field = Cast<StringT>(*this);
			if (Field)
			{
				for (auto Data : *Field)
				{
					cout << "\t";
					for (wchar_t Char : Data)
					{
						wcout << Char;
					}
					cout << endl;
				}
				cout << endl << endl;
			}
		}
		break;

		case FieldType::Integral:
		{
			auto Field = Cast<IntegralT>(*this);

			if (Field)
			{
				for (auto Data : *Field)
				{
					cout << "\t" << Data << endl;
				}
				cout << endl << endl;
			}
		}
		break;

		default:
			break;
		}
	}


	// Dataset members
	FieldBase& Dataset::GetField(std::string Name)
	{
		for (auto& Field : Fields)
		{
			if (Field->GetName() == Name)
			{
				return *Field;
			}
		}

		throw std::runtime_error("Could not find field '" + Name + "' in dataset");
	}

	bool Dataset::FieldExists(std::string Name)
	{
		for (auto& Field : Fields)
		{
			if (Field->GetName() == Name)
			{
				return true;
			}
		}
		return false;
	}

	bool Dataset::FieldExists(int64_t Index)
	{
		for (auto& Field : Fields)
		{
			if (Field->GetID() == Index)
			{
				return true;
			}
		}
		return false;
	}

	Dataset::~Dataset()
	{
		for (auto& Field : Fields)
		{
			delete Field;
		}
	}
}