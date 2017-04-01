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
	// Field members
	template<typename T>
	Field<T>::Field(const Field<T>& Other)
	{
		Name = Other.Name;
		Data = Other.Data;
		Type = Other.Type;
		ID = Other.ID;
	}

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
	Field<T>::Field(std::string inName, int64_t inID) : Field<T>::Field(inName)
	{
		ID = inID;
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
	template<typename T>
	void Dataset::AddField(T NewField)
	{
		static_assert(std::is_specialization_of<Field, T>::value, "NewField must be of type field!");
		Fields.push_back(std::make_shared<T>(NewField));
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
			static const Data::DeclareTypeName NAME##Stuct = Data::DeclareTypeName(NUMBER, std::string(#NAME));		\
		}																											\
																													\
		template<>																									\
		struct FieldTypeFromInt<NUMBER>																				\
		{																											\
			typedef TYPE Value;																						\
		};																											\
																													\
																													\
		using NAME##T = TYPE;																						\
																													\
		template<>																									\
		FieldTypeT GetFieldTypeOverride<TYPE>()																		\
		{																											\
			return NUMBER;																							\
		};																											\
	}																								
																									