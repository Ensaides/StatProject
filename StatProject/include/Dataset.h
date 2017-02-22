#pragma once
#include <memory>
#include <type_traits>
#include <vector>
#include <string>

namespace Data
{
	typedef uint8_t FieldTypeT;

	// This will determine FieldType from a given type T
	template<typename T>
	FieldTypeT GetFieldType();

	// Override this with template specialization to define custom types.
	template<typename T>
	FieldTypeT GetFieldTypeOverride();

	// Interface class for fields.
	class FieldBase
	{
	protected:
		// The Name and Type are immutable
		std::string Name;
		// The Type member is included for downcasting. 
		FieldTypeT Type;

	public:
		const FieldTypeT GetType() const;
		const std::string GetName() const;
	};

	// Field class. Holds the name of the field, the type of the data, and a vector of type T
	template <typename T>
	class Field : FieldBase
	{
	protected:
		// The raw data
		// Notes: Should this be a vector for fast iteration or should it just be a pointer to a user decided type?
		std::vector<T> Data;

	public:
		// The default constructor. This will use GetFieldType<T>() to figure out what the Type should be
		Field(std::string inName);

		// In case the user wants to set a custom type
		Field(std::string inName, FieldTypeT inType);

		// In case the user wants to put in data in the constructor
		Field(std::string inName, std::vector<T>&& inData);

		// In case the user wants to put in data in the constructor and use a custom type
		Field(std::string inName, std::vector<T>&& inData, FieldTypeT inType);

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
	};


	// A vector of Fields
	class Dataset
	{
	private:
		std::vector<FieldBase> Fields;

	public:
		// Push back a NewField on to Fields
		void AddField(FieldBase NewField);

		// Get a Field by Name. It will throw an exception if it can't find the Field
		FieldBase& GetField(std::string Name);

		// Check if a Field exists. Will return true if it does exist, false otherwise
		bool FieldExists(std::string Name);

		// Operators and helpers
		FieldBase& operator[](std::string Name)
		{
			return GetField(Name);
		}

		auto begin()
		{
			return Fields.begin();
		}

		auto end()
		{
			return Fields.end();
		}
	};
}

namespace Data
{
	// Built in FieldType types
	namespace FieldType
	{
		static const uint8_t null = 0;
		static const uint8_t Integral = 1;
		static const uint8_t Decimal = 2;
		static const uint8_t String = 3;
		static const uint8_t Choice = 4;
		static const uint8_t ExclusiveChoice = 5;
		static const uint8_t CustomTypeBeginning = 16;
	};

	typedef int64_t			IntegralT;
	typedef double			DecimalT;
	typedef std::wstring	StringT;

	typedef uint32_t		ChoiceT;
	typedef uint64_t		ExclusiveChoiceT;


	// Use this struct to get the type from an enum
	template<uint8_t n>
	struct FieldTypeFromInt;

	// null = 0
	template<>
	struct FieldTypeFromInt<0>
	{
		typedef void Value;
	};

	// Integral = 1
	template<>
	struct FieldTypeFromInt<1>
	{
		typedef IntegralT Value;
	};

	// Decimal = 2
	template<>
	struct FieldTypeFromInt<2>
	{
		typedef DecimalT Value;
	};

	// String = 3
	template<>
	struct FieldTypeFromInt<3>
	{
		typedef StringT Value;
	};

	// Choice = 4
	template<>
	struct FieldTypeFromInt<4>
	{
		typedef ChoiceT Value;
	};

	// ExclusiveChoice = 5
	template<>
	struct FieldTypeFromInt<5>
	{
		typedef ExclusiveChoiceT Value;
	};

	// CustomTypeBeginning = 16
	template<>
	struct FieldTypeFromInt<16>
	{
		typedef void Value;
	};
}

// Most of the implementations are in this .inl file
#include "Dataset.inl"