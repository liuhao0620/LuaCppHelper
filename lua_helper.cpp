//Copyright (c) 2017-2018 Beijing StormBringer Entertainment, Inc. All Rights Reserved.

#include "lua_helper.h"

namespace LuaCppHelper
{

	const LuaValue LuaHelper::NilValue;

	void LuaHelper::CheckBoolean(lua_State * L, int index, bool& val)
	{
		luaL_argcheck(L, lua_isboolean(L, index), index, "Need a Boolean");
		val = lua_toboolean(L, index) != 0;
	}

	void LuaHelper::CheckString(lua_State * L, int index, std::string& val)
	{
		size_t len = 0;
		const char * buf = luaL_checklstring(L, index, &len);
		val.assign(buf, len);
	}

	void LuaHelper::CheckLuaTable(lua_State * L, int index, LuaTable & val)
	{
		if (index < 0)
		{
			index = lua_gettop(L) + index + 1;
		}
		luaL_argcheck(L, lua_istable(L, index), index, "Need a Table");
		LuaValueDict dict;
		LuaValueArray array;
		lua_pushnil(L);
		while (lua_next(L, index) != 0)
		{
			LuaValue table_value;
			CheckLuaValue(L, -1, table_value);

			// key must a string or a integer
			if (lua_isstring(L, -2))
			{
				const char * table_key = lua_tostring(L, -2);
				dict.insert(std::make_pair(std::string(table_key), table_value));
			}
			else if (lua_isinteger(L, -2))
			{
				long long table_key = lua_tointeger(L, -2);
				array.insert(std::make_pair(table_key, table_value));
			}
			else
			{
				luaL_argerror(L, index, "Unsupported LuaValue key Type");
			}

			lua_pop(L, 1);
		}
		val = std::make_pair(dict, array);
	}

	void LuaHelper::CheckLuaObject(lua_State * L, int index, LuaObject & val)
	{
		std::string object_typename = val.second;
		if (lua_type(L, index) == LUA_TUSERDATA)
		{
			if (object_typename.empty())
			{
				val.first = *(void **)lua_touserdata(L, index);
			}
			else
			{
				val.first = *(void **)luaL_checkudata(L, index, object_typename.c_str());
			}
		}
		else if (lua_type(L, index) == LUA_TLIGHTUSERDATA)
		{
			val.first = (void *)lua_topointer(L, index);
			std::string err_msg;
			if (!object_typename.empty())
			{
				if (lua_getmetatable(L, index))
				{
					luaL_getmetatable(L, object_typename.c_str());
					if (!lua_rawequal(L, -1, -2))
					{
						val.first = nullptr;
						err_msg = "Need a ";
						err_msg += object_typename;
					}
					lua_pop(L, 2);
				}
				else
				{
					// doesn't have metatable
					val.first = nullptr;
					err_msg = "Need a ";
					err_msg += object_typename;
				}
			}
			if (!err_msg.empty())
			{
				luaL_argerror(L, index, err_msg.c_str());
			}
		}
		else
		{
			luaL_argerror(L, index, "Need Userdata or LightUserdata");
		}
	}

	void LuaHelper::CheckLuaFunction(lua_State * L, int index, LuaFunctionHelper & val)
	{
		if (!lua_isfunction(L, index))
		{
			luaL_argerror(L, index, "Expected a function");
		}
		luaL_unref(L, LUA_REGISTRYINDEX, val._func);
		lua_pushvalue(L, index);
		val._func = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	void LuaHelper::CheckLuaValue(lua_State * L, int index, LuaValue& val)
	{
		switch (lua_type(L, index))
		{
		case LUA_TNIL:
		{
			val = LuaValue::NilValue();
		}
		break;
		case LUA_TBOOLEAN:
		{
			val = LuaValue::BooleanValue(lua_toboolean(L, index));
		}
		break;
		case LUA_TNUMBER:
		{
			if (lua_isinteger(L, index))
			{
				val = LuaValue::IntValue(lua_tointeger(L, index));
			}
			else
			{
				val = LuaValue::NumberValue(lua_tonumber(L, index));
			}
		}
		break;
		case LUA_TSTRING:
		{
			size_t len = 0;
			const char * buf = lua_tolstring(L, index, &len);
			val = LuaValue::StringValue(std::string(buf, len));
		}
		break;
		case LUA_TTABLE:
		{
			LuaTable table;
			CheckLuaTable(L, index, table);
			val = LuaValue::TableValue(table);
		}
		break;
		case LUA_TLIGHTUSERDATA:
		{
			void * object_value = (void *)lua_topointer(L, index);
			std::string object_typename;
			if (luaL_getmetafield(L, index, "__name") != LUA_TNIL)
			{
				if (lua_isstring(L, -1))
				{
					object_typename = lua_tostring(L, -1);
				}
				lua_pop(L, 1);
			}
			val = LuaValue::ObjectValue(object_value, object_typename);
		}
		break;
		case LUA_TUSERDATA:
		{
			void * object_value = *(void **)lua_touserdata(L, index);
			std::string object_typename;
			if (luaL_getmetafield(L, index, "__name") != LUA_TNIL)
			{
				if (lua_isstring(L, -1))
				{
					object_typename = lua_tostring(L, -1);
				}
				lua_pop(L, 1);
			}
			val = LuaValue::ObjectValue(object_value, object_typename);
		}
		break;
		case LUA_TFUNCTION:
		{
			LuaFunctionHelper func;
			CheckLuaFunction(L, index, func);
			val = LuaValue::FunctionValue(func._func);
		}
		break;
		default:
		{
			luaL_argerror(L, index, "Unsupported LuaValueType");
		}
		break;
		}
	}

	void LuaHelper::PushNil(lua_State * L)
	{
		lua_pushnil(L);
	}

	void LuaHelper::PushBoolean(lua_State * L, bool value)
	{
		lua_pushboolean(L, value);
	}

	void LuaHelper::PushString(lua_State * L, const char * value)
	{
		lua_pushstring(L, value);
	}

	void LuaHelper::PushString(lua_State * L, const std::string & value)
	{
		lua_pushlstring(L, value.c_str(), value.size());
	}

	void LuaHelper::PushLuaTable(lua_State * L, const LuaTable & value)
	{
		lua_newtable(L);
		LuaValueDict dict = value.first;
		for (LuaValueDictIterator it = dict.begin(); it != dict.end(); ++it)
		{
			lua_pushstring(L, it->first.c_str());
			PushLuaValue(L, it->second);
			lua_rawset(L, -3);
		}
		LuaValueArray array = value.second;
		for (LuaValueArrayIterator it = array.begin(); it != array.end(); ++it)
		{
			lua_pushinteger(L, it->first);
			PushLuaValue(L, it->second);
			lua_rawset(L, -3);
		}
	}

	void LuaHelper::PushLuaObject(lua_State * L, const LuaObject & value)
	{
		// if metatable named object_typename has "__gc" field, push object_value as userdata
		// otherwise put object_value as lightuserdata
		void* object_value = value.first;
		std::string object_typename = value.second;
		do {
			if (object_typename.empty())
			{
				break;
			}
			luaL_getmetatable(L, object_typename.c_str());		/* L: mt */
			if (lua_isnil(L, -1))								/* L: mt */
			{
				lua_pop(L, 1);									/* L: */
				break;
			}
			lua_getfield(L, -1, "__gc");						/* L: mt, gc */
			if (lua_isnil(L, -1))								/* L: mt, gc */
			{
				lua_pop(L, 2);									/* L: */
				break;
			}
			lua_pop(L, 2);										/* L: */
			// push object_value as userdata
			*(void **)lua_newuserdata(L, sizeof(void *)) = object_value;	/* L: ud */
			luaL_getmetatable(L, object_typename.c_str());					/* L: ud, mt */
			lua_setmetatable(L, -2);										/* L: ud */
			return;
		} while (false);

		lua_pushlightuserdata(L, object_value);					/* L: lud */
		if (object_typename.empty())
		{
			return;
		}
		luaL_getmetatable(L, object_typename.c_str());			/* L: lud, mt */
		if (lua_isnil(L, -1))									/* L: lud, mt */
		{
			lua_pop(L, 1);										/* L: lud */
			return;
		}
		lua_setmetatable(L, -2);								/* L: lud */
	}

	void LuaHelper::PushLuaFunction(lua_State * L, const LuaFunctionHelper & value)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, value._func);
	}

	void LuaHelper::PushLuaValue(lua_State * L, const LuaValue & value)
	{
		const LuaValueType type = value.getType();
		if (type == LuaValueTypeNil)
		{
			PushNil(L);
		}
		else if (type == LuaValueTypeInt)
		{
			PushInteger(L, value.IntValue());
		}
		else if (type == LuaValueTypeFloat)
		{
			PushNumber(L, value.NumberValue());
		}
		else if (type == LuaValueTypeBoolean)
		{
			PushBoolean(L, value.BooleanValue());
		}
		else if (type == LuaValueTypeString)
		{
			PushString(L, value.StringValue());
		}
		else if (type == LuaValueTypeTable)
		{
			PushLuaTable(L, value.TableValue());
		}
		else if (type == LuaValueTypeObject)
		{
			PushLuaObject(L, value.ObjectValue());
		}
		else if (type == LuaValueTypeFunction)
		{
			PushLuaFunction(L, LuaFunctionHelper(value.FunctionValue()));
		}
	}

	void LuaHelper::PushLuaValueDict(lua_State * L, const LuaValueDict & dict)
	{
		lua_newtable(L);
		for (LuaValueDictIterator it = dict.begin(); it != dict.end(); ++it)
		{
			lua_pushstring(L, it->first.c_str());
			PushLuaValue(L, it->second);
			lua_rawset(L, -3);
		}
	}

	void LuaHelper::PushLuaValueArray(lua_State * L, const LuaValueArray & array)
	{
		lua_newtable(L);
		for (LuaValueArrayIterator it = array.begin(); it != array.end(); ++it)
		{
			lua_pushinteger(L, it->first);
			PushLuaValue(L, it->second);
			lua_rawset(L, -3);
		}
	}

	int LuaHelper::Traceback(lua_State * L)
	{
		if (!lua_isstring(L, 1))  /* 'message' not a string? */
			return 1;  /* keep it intact */
		lua_pushglobaltable(L);
		lua_getfield(L, -1, "debug");
		lua_remove(L, -2);
		if (!lua_istable(L, -1)) {
			lua_pop(L, 1);
			return 1;
		}
		lua_getfield(L, -1, "traceback");
		if (!lua_isfunction(L, -1)) {
			lua_pop(L, 2);
			return 1;
		}
		lua_pushvalue(L, 1);  /* pass error message */
		lua_pushinteger(L, 2);  /* skip this function and traceback */
		lua_call(L, 2, 1);  /* call debug.traceback */
		return 1;
	}

	void LuaHelper::CallFunction(lua_State * L, const LuaFunction func, int argc)
	{
		if (func == LUA_NOREF)
		{
			lua_pop(L, argc);
			return;
		}

		lua_pushcfunction(L, LuaHelper::Traceback);
		int errfunc = lua_gettop(L);
		// And insert it before the args if there are any.
		if (argc != 0)
		{
			lua_insert(L, -1 - argc);
			errfunc -= argc;
		}
		// Get the callback
		lua_rawgeti(L, LUA_REGISTRYINDEX, func);
		// And insert it before the args if there are any.
		if (argc)
		{
			lua_insert(L, -1 - argc);
		}

		if (lua_pcall(L, argc, 0, errfunc) != LUA_OK)
		{
			LCH_LOG("[LUA ERROR]: %s", lua_tostring(L, -1));        /* L: traceback error */
			lua_pop(L, 1);
		}

		// Remove the traceback function
		lua_pop(L, 1);
	}

	void LuaHelper::RemoveFunction(lua_State * L, const LuaFunction func)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, func);
	}

	void LuaHelper::CheckImpl(lua_State * L, int index, float & val, bool cannil)
	{
		if (cannil && lua_isnoneornil(L, index))
		{
			return;
		}
		CheckNumber<float>(L, index, val);
	}

	void LuaHelper::CheckImpl(lua_State * L, int index, double & val, bool cannil)
	{
		if (cannil && lua_isnoneornil(L, index))
		{
			return;
		}
		CheckNumber<double>(L, index, val);
	}

	void LuaHelper::CheckImpl(lua_State * L, int index, bool & val, bool cannil)
	{
		if (cannil && lua_isnoneornil(L, index))
		{
			return;
		}
		CheckBoolean(L, index, val);
	}

	void LuaHelper::CheckImpl(lua_State * L, int index, std::string & val, bool cannil)
	{
		if (cannil && lua_isnoneornil(L, index))
		{
			return;
		}
		CheckString(L, index, val);
	}

	void LuaHelper::CheckImpl(lua_State * L, int index, LuaTable & val, bool cannil)
	{
		if (cannil && lua_isnoneornil(L, index))
		{
			return;
		}
		CheckLuaTable(L, index, val);
	}

	void LuaHelper::CheckImpl(lua_State * L, int index, LuaObject & val, bool cannil)
	{
		if (cannil && lua_isnoneornil(L, index))
		{
			return;
		}
		CheckLuaObject(L, index, val);
	}

	void LuaHelper::CheckImpl(lua_State * L, int index, LuaFunctionHelper & val, bool cannil)
	{
		if (cannil && lua_isnoneornil(L, index))
		{
			return;
		}
		CheckLuaFunction(L, index, val);
	}

	void LuaHelper::CheckImpl(lua_State * L, int index, LuaValue & val, bool cannil)
	{
		if (cannil && lua_isnoneornil(L, index))
		{
			return;
		}
		CheckLuaValue(L, index, val);
	}

	void LuaHelper::ResultImpl(lua_State * L, const double & value)
	{
		PushNumber(L, value);
	}

	void LuaHelper::ResultImpl(lua_State * L, const bool & value)
	{
		PushBoolean(L, value);
	}

	void LuaHelper::ResultImpl(lua_State * L, const char * value)
	{
		PushString(L, value);
	}

	void LuaHelper::ResultImpl(lua_State * L, const std::string & value)
	{
		PushString(L, value);
	}

	void LuaHelper::ResultImpl(lua_State * L, const LuaTable & value)
	{
		PushLuaTable(L, value);
	}

	void LuaHelper::ResultImpl(lua_State * L, const LuaObject & value)
	{
		PushLuaObject(L, value);
	}

	void LuaHelper::ResultImpl(lua_State * L, const LuaFunctionHelper & value)
	{
		PushLuaFunction(L, value);
	}

	void LuaHelper::ResultImpl(lua_State * L, const LuaValue & value)
	{
		PushLuaValue(L, value);
	}

	void LuaHelper::ResultImpl(lua_State * L, const LuaValueDict & dict)
	{
		PushLuaValueDict(L, dict);
	}

	void LuaHelper::ResultImpl(lua_State * L, const LuaValueArray & array)
	{
		PushLuaValueArray(L, array);
	}

	int LuaHelper::result(lua_State * L)
	{
		return 0;
	}

}
