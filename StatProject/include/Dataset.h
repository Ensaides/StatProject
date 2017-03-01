#pragma once
#include <memory>
#include <type_traits>
#include <vector>

namespace Data
{
	typedef uint8_t FieldTypeT;

	// This will determine FieldType from a given type T
	template<typename T>
	static FieldTypeT GetFieldType();

	// Override this with template specialization to define custom types.
	template<typename T>
	static FieldTypeT GetFieldTypeOverride()
	{
		return 0;
	};

	// Interface class for fields.
	class FieldBase
	{
	protected:
		// The Name and Type are immutable
		std::string Name;
		// The Type member is included for downcasting. 
		FieldTypeT Type;
		// Used for Typeform Fields
		int64_t ID;

	public:
		const FieldTypeT GetType() const;

		const std::string GetName() const;
		void SetName(std::string inName);

		const int64_t GetID() const;
		void SetID(int64_t inID);

		void Print();

		virtual FieldBase* Clone() { return new FieldBase(*this); };
	};

	// Field class. Holds the name of the field, the type of the data, and a vector of type T
	template <typename T>
	class Field : public FieldBase
	{
	protected:
		// The raw data
		// Notes: Should this be a vector for fast iteration or should it just be a pointer to a user decided type?
		std::vector<T> Data;

	public:
		Field(const Field<T>& Other);

		// The default constructor. This will use GetFieldType<T>() to figure out what the Type should be
		Field(std::string inName);

		// In case the user wants to set a custom type
		Field(std::string inName, FieldTypeT inType);

		Field(std::string inName, int64_t inID);

	public:
		std::vector<T>& GetData();

		// Operators and helpers
		T& operator[](int32_t Index);

		auto begin()
		{
			return Data.begin();
		}

		auto end()
		{
			return Data.end();
		}

		void operator<<(const T& Rhs)
		{
			Data.push_back(Rhs);
		}

		virtual FieldBase* Clone() override
		{
			return new Field<T>(*this);
		}
	};


	// A vector of Fields
	class Dataset
	{
	private:
		std::vector<std::shared_ptr<FieldBase>> Fields;

	public:
		// Push back a NewField on to Fields
		template<class T>
		void AddField(T NewField);

		// Get a Field by Name. It will throw an exception if it can't find the Field
		FieldBase& GetField(std::string Name);

		// Check if a Field exists. Will return true if it does exist, false otherwise
		bool FieldExists(std::string Name);
		bool FieldExists(int64_t Index);

		// Operators and helpers
		FieldBase& operator[](std::string Name)
		{
			return GetField(Name);
		}

		FieldBase& operator[](int64_t Index)
		{
			for (auto& Field : Fields)
			{
				if (Field->GetID() == Index)
				{
					return *Field;
				}
			}

			throw std::runtime_error("Could not find field number '" + std::to_string(Index) + "' in dataset");
		}

		std::vector<std::shared_ptr<FieldBase>>::iterator begin()
		{
			return Fields.begin();
		}

		std::vector<std::shared_ptr<FieldBase>>::iterator end()
		{
			return Fields.end();
		}
	};
}

namespace Data
{
	// Use this struct to get the type from an enum
	template<uint8_t n>
	struct FieldTypeFromInt;

	template<typename T>
	FieldTypeT GetFieldType()
	{
		using namespace std;

		// Field type user override
		if (GetFieldTypeOverride<T>())
			return GetFieldTypeOverride<T>();

		return 0;
	};

	template<typename T>
	static inline Field<T>* Cast(FieldBase& Base)
	{
		return GetFieldType<T>() == Base.GetType() ? (Field<T>*) &Base : nullptr;
	}

	template<typename T>
	static inline Field<T>* Cast(FieldBase* Base)
	{
		return GetFieldType<T>() == Base->GetType() ? (Field<T>*) Base : nullptr;
	}

	template<typename T>
	static inline Field<T>* Cast(std::shared_ptr<FieldBase> Base)
	{
		return GetFieldType<T>() == Base->GetType() ? (Field<T>*) Base.get() : nullptr;
	}
}

// Most of the implementations are in this .inl file
#include "Dataset.inl"

// Built in types
DECLARE_FIELD_TYPE(void,			0,	 null)
DECLARE_FIELD_TYPE(int64_t,			1,	 Integral)
DECLARE_FIELD_TYPE(double,			2,	 Decimal)
DECLARE_FIELD_TYPE(std::u16string,	3,	 String)
DECLARE_FIELD_TYPE(uint32_t,		4,	 Choice)
DECLARE_FIELD_TYPE(uint64_t,		5,	 ExclusiveChoice)