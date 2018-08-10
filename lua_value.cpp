//Copyright (c) 2017-2018 Beijing StormBringer Entertainment, Inc. All Rights Reserved.

#include "lua_value.h"

namespace LuaCppHelper
{

	const LuaValue LuaValue::NilValue()
	{
		LuaValue value;
		value._type = LuaValueTypeNil;
		return value;
	}

	const LuaValue LuaValue::IntValue(const long long intValue)
	{
		LuaValue value;
		value._type = LuaValueTypeInt;
		value._field.intValue = intValue;
		return value;
	}

	const LuaValue LuaValue::NumberValue(const double numberValue)
	{
		LuaValue value;
		value._type = LuaValueTypeFloat;
		value._field.numberValue = numberValue;
		return value;
	}

	const LuaValue LuaValue::BooleanValue(const bool booleanValue)
	{
		LuaValue value;
		value._type = LuaValueTypeBoolean;
		value._field.booleanValue = booleanValue;
		return value;
	}

	const LuaValue LuaValue::StringValue(const char* stringValue)
	{
		LuaValue value;
		value._type = LuaValueTypeString;
		value._field.stringValue = new std::string(stringValue ? stringValue : "");
		return value;
	}

	const LuaValue LuaValue::StringValue(const std::string& stringValue)
	{
		LuaValue value;
		value._type = LuaValueTypeString;
		value._field.stringValue = new std::string(stringValue);
		return value;
	}

	const LuaValue LuaValue::TableValue(const LuaTable & tableValue)
	{
		LuaValue value;
		value._type = LuaValueTypeTable;
		value._field.tableValue = new LuaTable(tableValue);
		return value;
	}

	const LuaValue LuaValue::TableValue(const LuaValueDict & dictValue, const LuaValueArray & arrayValue)
	{
		LuaValue value;
		value._type = LuaValueTypeTable;
		value._field.tableValue = new LuaTable();
		value._field.tableValue->first = dictValue;
		value._field.tableValue->second = arrayValue;
		return value;
	}

	const LuaValue LuaValue::DictValue(const LuaValueDict& dictValue)
	{
		LuaValue value;
		value._type = LuaValueTypeTable;
		value._field.tableValue->first = dictValue;
		return value;
	}

	const LuaValue LuaValue::ArrayValue(const LuaValueArray& arrayValue)
	{
		LuaValue value;
		value._type = LuaValueTypeTable;
		value._field.tableValue->second = arrayValue;
		return value;
	}

	const LuaValue LuaValue::ObjectValue(void * object_value, const char * object_typename)
	{
		LuaValue value;
		value._type = LuaValueTypeObject;
		value._field.objectValue = new LuaObject();
		value._field.objectValue->first = object_value;
		value._field.objectValue->second = object_typename == nullptr ? "" : std::string(object_typename);
		return value;
	}

	const LuaValue LuaValue::ObjectValue(void * object_value, const std::string & object_typename)
	{
		return LuaValue::ObjectValue(object_value, object_typename.c_str());
	}

	const LuaValue LuaValue::FunctionValue(const LuaFunction functionValue)
	{
		LuaValue value;
		value._type = LuaValueTypeFunction;
		value._field.functionValue = functionValue;
		return value;
	}

	LuaValue::LuaValue(const LuaValue& rhs)
	{
		Copy(rhs);
	}

	LuaValue& LuaValue::operator=(const LuaValue& rhs)
	{
		if (this != &rhs) Copy(rhs);
		return *this;
	}

	LuaValue::~LuaValue(void)
	{
		if (_type == LuaValueTypeString)
		{
			delete _field.stringValue;
			_field.stringValue = nullptr;
		}
		else if (_type == LuaValueTypeTable)
		{
			delete _field.tableValue;
			_field.tableValue = nullptr;
		}
		else if (_type == LuaValueTypeObject)
		{
			delete _field.objectValue;
			_field.objectValue = nullptr;
		}
	}

	void LuaValue::Copy(const LuaValue& rhs)
	{
		memcpy(&_field, &rhs._field, sizeof(_field));
		_type = rhs._type;
		if (_type == LuaValueTypeString)
		{
			_field.stringValue = new std::string(*rhs._field.stringValue);
		}
		else if (_type == LuaValueTypeTable)
		{
			_field.tableValue = new LuaTable(*rhs._field.tableValue);
		}
		else if (_type == LuaValueTypeObject)
		{
			_field.objectValue = new LuaObject(*rhs._field.objectValue);
		}
	}

}
