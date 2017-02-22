#pragma once
// Implementation file

// Why this is not part of the std, I do not know
namespace std
{
	template<template<typename...> class T, typename U>
	struct is_specialization_of : false_type { };

	template<template<typename...> class T, typename... Ts>
	struct is_specialization_of<T, T<Ts...>> : true_type { };
}

namespace Data
{
	template<typename T>
	FieldTypeT GetFieldType()
	{
		using namespace std;

		// Field type user override
		if (GetFieldTypeOverride<T>() != FieldType::null)
			return GetFieldTypeOverride<T>();

		if (is_same<T, ChoiceT>::value)
		{
			return FieldType::Choice;
		}
		else if (is_same<T, ExclusiveChoiceT>::value)
		{
			return FieldType::ExclusiveChoice;
		}
		else if (is_specialization_of<std::basic_string, T>::value)
		{
			return FieldType::String;
		}
		else if (is_integral<T>::value)
		{
			return FieldType::Integral;
		}
		else if (is_floating_point<T>::value)
		{
			return FieldType::Decimal;
		}

		return FieldType::null;
	};

	template<>
	FieldTypeT GetFieldTypeOverride<void>()
	{
		return FieldType::null;
	};


	// FieldBase members
	const FieldTypeT FieldBase::GetType() const
	{
		return Type;
	}

	const std::string FieldBase::GetName() const
	{
		return Name;
	}

	// Field members
	template<typename T>
	Field<T>::Field(std::string inName)
	{
		Name = inName;
		Data = std::vector<T>();
		Type = GetFieldType<T>();
	}

	template<typename T>
	Field<T>::Field(std::string inName, FieldTypeT inType) : Field<T>::Field(inName)
	{
		Type = inType;
	}

	template<typename T>
	Field<T>::Field(std::string inName, std::vector<T>&& inData) : Field<T>::Field(inName)
	{
		Data = std::move(inData);
	}

	template<typename T>
	Field<T>::Field(std::string inName, std::vector<T>&& inData, FieldTypeT inType) : Field<T>::Field(inName, inData)
	{
		Type = inType;
	}

	template<typename T>
	std::vector<T>& Field<T>::GetData()
	{
		return Data;
	}

	template<typename T>
	T& Field<T>::operator[](int32_t Index)
	{
		return Data[Index];
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

// Handy preprocessor macro to define a new Data::FieldType type
#define DECLARE_FIELD_TYPE(TYPE, NUMBER, NAME)																		\
	static_assert(std::is_integral<decltype(NUMBER)>::value, "NUMBER must be an integral!");						\
	static_assert(NUMBER >= 0,								 "NUMBER must be non-negative!");						\
	namespace Data																									\
	{																												\
		namespace FieldType																							\
		{																											\
			static const uint8_t NAME = NUMBER;																		\
		}																											\
																													\
		template<>																									\
		struct FieldTypeFromInt<NUMBER>																				\
		{																											\
			typedef TYPE Value;																						\
		};																											\
																													\
		template<>																									\
		FieldTypeT GetFieldTypeOverride<TYPE>()																		\
		{																											\
			return NUMBER;																							\
		}																											\
	}																								
																									