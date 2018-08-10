//Copyright (c) 2017-2018 Beijing StormBringer Entertainment, Inc. All Rights Reserved.

#pragma once

#include <list>
#include <map>
#include <string>
extern "C" {
#include "lua.h"
}

namespace LuaCppHelper
{

	typedef int LuaFunction;

	class LuaValue;

	typedef std::map<std::string, LuaValue>			LuaValueDict;
	typedef LuaValueDict::const_iterator			LuaValueDictIterator;
	typedef std::map<long long, LuaValue>			LuaValueArray;
	typedef LuaValueArray::const_iterator			LuaValueArrayIterator;
	typedef std::pair<LuaValueDict, LuaValueArray>	LuaTable;
	typedef std::pair<void *, std::string>			LuaObject;

	/// @cond
	typedef enum {
		LuaValueTypeNil,
		LuaValueTypeInt,
		LuaValueTypeFloat,
		LuaValueTypeBoolean,
		LuaValueTypeString,
		LuaValueTypeTable,
		LuaValueTypeObject,
		LuaValueTypeFunction
	} LuaValueType;
	/// @endcond

	/// @cond
	typedef union {
		long long           intValue;
		double              numberValue;
		bool                booleanValue;
		std::string*        stringValue;
		LuaTable*			tableValue;
		LuaObject*			objectValue;
		LuaFunction			functionValue;
	} LuaValueField;
	/// @endcond

	/**
	* Wrap different general types of data into a same specific type named LuaValue.
	* The general types supported as follows:int,float,bool,std::string,const char*,LuaValueDict,LuaValueArray,LuaObject.
	*
	* @lua NA
	*/
	class LuaValue
	{
	public:
		/**
		* Construct a LuaValue object by nothing.
		*
		* @return a LuaValue object.
		*/
		static const LuaValue NilValue();

		/**
		* Construct a LuaValue object by a long long value.
		*
		* @param intValue a int value.
		* @return a LuaValue object.
		*/
		static const LuaValue IntValue(const long long intValue);

		/**
		* Construct a LuaValue object by a double value.
		*
		* @param floatValue a float value.
		* @return a LuaValue object.
		*/
		static const LuaValue NumberValue(const double floatValue);

		/**
		* Construct a LuaValue object by a boolean value.
		*
		* @param booleanValue a bool value.
		* @return a LuaValue object.
		*/
		static const LuaValue BooleanValue(const bool booleanValue);

		/**
		* Construct a LuaValue object by a string pointer.
		*
		* @param stringValue a string pointer.
		* @return a LuaValue object.
		*/
		static const LuaValue StringValue(const char* stringValue);

		/**
		* Construct a LuaValue object by a std::string object.
		*
		* @param stringValue a std::string object.
		* @return a LuaValue object.
		*/
		static const LuaValue StringValue(const std::string& stringValue);

		/**
		* Construct a LuaValue object by a LuaValueDict value and a LuaValueArray value.
		*
		* @param tableValue a LuaTable object.
		* @return a LuaValue object.
		*/
		static const LuaValue TableValue(const LuaTable& tableValue);

		/**
		* Construct a LuaValue object by a LuaValueDict value and a LuaValueArray value.
		*
		* @param dictValue a LuaValueDict object.
		* @param arrayValue a LuaValueArray object.
		* @return a LuaValue object.
		*/
		static const LuaValue TableValue(const LuaValueDict& dictValue, const LuaValueArray& arrayValue);

		/**
		* Construct a LuaValue object by a LuaValueDict value.
		*
		* @param dictValue a LuaValueDict object.
		* @return a LuaValue object.
		*/
		static const LuaValue DictValue(const LuaValueDict& dictValue);

		/**
		* Construct a LuaValue object by a LuaValueArray value.
		*
		* @param arrayValue a LuaValueArray object.
		* @return a LuaValue object.
		*/
		static const LuaValue ArrayValue(const LuaValueArray& arrayValue);

		/**
		* Construct a LuaValue object by a pointer.
		*
		* @param object_value a void * pointer.
		* @param object_typename a string pointer point to the typename of object.
		* @return a LuaValue object.
		*/
		static const LuaValue ObjectValue(void* object_value, const char* object_typename);

		/**
		* Construct a LuaValue object by a pointer.
		*
		* @param object_value a void * pointer.
		* @param object_typename a std::string object represent the typename of object.
		* @return a LuaValue object.
		*/
		static const LuaValue ObjectValue(void* object_value, const std::string& object_typename);

		/**
		* Construct a LuaValue object by a LuaFunction value.
		*
		* @param functionValue a LuaFunction value.
		* @return a LuaValue object.
		*/
		static const LuaValue FunctionValue(const LuaFunction functionValue);


		/**
		* Default constructor of LuaValue.
		* Set the default value for _type(LuaValueTypeInt) and _ccobjectType(nullptr),and init the _field.
		*/
		LuaValue(void)
			: _type(LuaValueTypeNil)
		{
			memset(&_field, 0, sizeof(_field));
		}

		/**
		* Copy constructor of Data.
		*/
		LuaValue(const LuaValue& rhs);

		/**
		* Override of operator= .
		*/
		LuaValue& operator=(const LuaValue& rhs);

		/**
		* Destructor.
		*/
		~LuaValue(void);

		/**
		* Get the type of LuaValue object.
		*
		* @return the type of LuaValue object.
		*/
		LuaValueType getType() const {
			return _type;
		}

		///**
		//* Get the typename of the Ref object.
		//*
		//* @return the reference of _ccobjectType.
		//*/
		//const std::string& getObjectTypename(void) const {
		//	return *_ccobjectType;
		//}

		/**
		* Get the int value of LuaValue object.
		*
		* @return the int value.
		*/
		long long IntValue(void) const {
			return _field.intValue;
		}

		/**
		* Get the float value of LuaValue object.
		*
		* @return the float value.
		*/
		double NumberValue(void) const {
			return _field.numberValue;
		}

		/**
		* Get the boolean value of LuaValue object.
		*
		* @return the boolean value.
		*/
		bool BooleanValue(void) const {
			return _field.booleanValue;
		}

		/**
		* Get the boolean value of LuaValue object.
		*
		* @return the reference about string value.
		*/
		const std::string& StringValue(void) const {
			return *_field.stringValue;
		}

		/**
		* Get the LuaTable value of LuaValue object.
		*
		* @return the LuaTable value.
		*/
		const LuaTable& TableValue(void) const {
			return *_field.tableValue;
		}

		/**
		* Get the LuaObject value of LuaValue object.
		*
		* @return the LuaTable value.
		*/
		const LuaObject& ObjectValue(void) const {
			return *_field.objectValue;
		}

		/**
		* Get the LuaValueDict value of LuaValue object.
		*
		* @return the LuaValueDict value.
		*/
		const LuaValueDict& DictValue(void) const {
			return _field.tableValue->first;
		}

		/**
		* Get the LuaValueArray value of LuaValue object.
		*
		* @return the LuaValueArray value.
		*/
		const LuaValueArray& ArrayValue(void) const {
			return _field.tableValue->second;
		}

		/**
		* Get the LuaFunction value of LuaValue object.
		*
		* @return the LuaFunction value.
		*/
		const LuaFunction& FunctionValue(void) const {
			return _field.functionValue;
		}

		/**
		* Get the Ref object of LuaValue object.
		*
		* @return the pointer point to a Ref object.
		*/
		//Ref* ccobjectValue(void) const {
		//	return _field.ccobjectValue;
		//}

	private:
		LuaValueField _field;
		LuaValueType  _type;
		//	std::string*    _ccobjectType;

		void Copy(const LuaValue& rhs);
	};

}
