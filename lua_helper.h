//Copyright (c) 2017-2018 Beijing StormBringer Entertainment, Inc. All Rights Reserved.

#pragma once

#include "lua_value.h"
extern "C"
{
#include "lauxlib.h"
}

#define LCH_LOG(...)

namespace LuaCppHelper
{

	struct LuaFunctionHelper
	{
		LuaFunctionHelper(LuaFunction func = LUA_NOREF) : _func(func) {}
		LuaFunction _func;
	};

	/**
	* LuaHelper is used to read paramters from lua_State or write results to lua_State
	*/
	class LuaHelper
	{
	public:
		template <typename INTTYPE>
		static void CheckInteger(lua_State* L, int index, INTTYPE& val)
		{
			static_assert(std::is_same<INTTYPE, char>::value
				|| std::is_same<INTTYPE, unsigned char>::value
				|| std::is_same<INTTYPE, short>::value
				|| std::is_same<INTTYPE, unsigned short>::value
				|| std::is_same<INTTYPE, int>::value
				|| std::is_same<INTTYPE, unsigned int>::value
				|| std::is_same<INTTYPE, long>::value
				|| std::is_same<INTTYPE, unsigned long>::value
				|| std::is_same<INTTYPE, long long>::value
				|| std::is_same<INTTYPE, unsigned long long>::value,
				"Unsupported type");
			val = (INTTYPE)luaL_checkinteger(L, index);
		}
		template <typename NUMBERTYPE>
		static void CheckNumber(lua_State* L, int index, NUMBERTYPE& val)
		{
			static_assert(std::is_same<NUMBERTYPE, float>::value
				|| std::is_same<NUMBERTYPE, double>::value,
				"Unsupported type");
			val = (NUMBERTYPE)luaL_checknumber(L, index);
		}
		static void CheckBoolean(lua_State* L, int index, bool& val);
		static void CheckString(lua_State* L, int index, std::string& val);
		static void CheckLuaTable(lua_State* L, int index, LuaTable& val);
		static void CheckLuaObject(lua_State* L, int index, LuaObject& val);
		static void CheckLuaFunction(lua_State* L, int index, LuaFunctionHelper& val);
		static void CheckLuaValue(lua_State* L, int index, LuaValue& val);

		template <typename INTTYPE>
		static void PushInteger(lua_State* L, INTTYPE value)
		{
			static_assert(std::is_same<INTTYPE, char>::value
				|| std::is_same<INTTYPE, unsigned char>::value
				|| std::is_same<INTTYPE, short>::value
				|| std::is_same<INTTYPE, unsigned short>::value
				|| std::is_same<INTTYPE, int>::value
				|| std::is_same<INTTYPE, unsigned int>::value
				|| std::is_same<INTTYPE, long>::value
				|| std::is_same<INTTYPE, unsigned long>::value
				|| std::is_same<INTTYPE, long long>::value
				|| std::is_same<INTTYPE, unsigned long long>::value,
				"Unsupported type");
			lua_pushinteger(L, value);
		}
		template <typename NUMBERTYPE>
		static void PushNumber(lua_State* L, NUMBERTYPE value)
		{
			static_assert(std::is_same<NUMBERTYPE, float>::value
				|| std::is_same<NUMBERTYPE, double>::value,
				"Unsupported type");
			lua_pushnumber(L, value);
		}
		static void PushNil(lua_State* L);
		static void PushBoolean(lua_State* L, bool value);
		static void PushString(lua_State* L, const char * value);
		static void PushString(lua_State* L, const std::string& value);
		static void PushLuaTable(lua_State* L, const LuaTable& value);
		static void PushLuaObject(lua_State* L, const LuaObject& value);
		static void PushLuaFunction(lua_State* L, const LuaFunctionHelper& value);
		static void PushLuaValue(lua_State* L, const LuaValue& value);
		static void PushLuaValueDict(lua_State* L, const LuaValueDict& dict);
		static void PushLuaValueArray(lua_State* L, const LuaValueArray& array);

		static int Traceback(lua_State* L);
		static void CallFunction(lua_State* L, const LuaFunction func, int argc);
		static void RemoveFunction(lua_State* L, const LuaFunction func);

	private:
		template <typename T>
		static void CheckImpl(lua_State* L, int index, T& val, bool cannil)
		{
			if (cannil && lua_isnoneornil(L, index))
			{
				return;
			}
			CheckInteger<T>(L, index, val);
		}
		static void CheckImpl(lua_State* L, int index, float& val, bool cannil);
		static void CheckImpl(lua_State* L, int index, double& val, bool cannil);
		static void CheckImpl(lua_State* L, int index, bool& val, bool cannil);
		static void CheckImpl(lua_State* L, int index, std::string& val, bool cannil);
		static void CheckImpl(lua_State* L, int index, LuaTable& val, bool cannil);
		static void CheckImpl(lua_State* L, int index, LuaObject& val, bool cannil);
		static void CheckImpl(lua_State* L, int index, LuaFunctionHelper& val, bool cannil);
		static void CheckImpl(lua_State* L, int index, LuaValue& val, bool cannil);
		template <bool CANNIL, int MINARGC, int INDEX, typename T>
		static void CheckImpl(lua_State* L, T&& val)
		{
			CheckImpl(L, INDEX, std::forward<T>(val), (CANNIL && (MINARGC < INDEX)));
		}
		template <bool CANNIL, int MINARGC, int INDEX, typename T, typename ...ARGS>
		static void CheckImpl(lua_State* L, T&& val, ARGS&& ...args)
		{
			CheckImpl(L, INDEX, std::forward<T>(val), (CANNIL && (MINARGC < INDEX)));
			CheckImpl<CANNIL, MINARGC, INDEX + 1>(L, std::forward<ARGS>(args)...);
		}

		template <typename T>
		static void ResultImpl(lua_State* L, const T& value)
		{
			PushInteger<T>(L, value);
		}
		static void ResultImpl(lua_State* L, const float& value)
		{
			PushNumber(L, value);
		}
		static void ResultImpl(lua_State* L, const double& value);
		static void ResultImpl(lua_State* L, const bool& value);
		static void ResultImpl(lua_State* L, const char * value);
		static void ResultImpl(lua_State* L, const std::string& value);
		static void ResultImpl(lua_State* L, const LuaTable& value);
		static void ResultImpl(lua_State* L, const LuaObject& value);
		static void ResultImpl(lua_State* L, const LuaFunctionHelper& value);
		static void ResultImpl(lua_State* L, const LuaValue& value);
		static void ResultImpl(lua_State* L, const LuaValueDict& dict);
		static void ResultImpl(lua_State* L, const LuaValueArray& array);

	public:
		template <bool CANNIL, int MINARGC, typename ...ARGS>
		static void Check(lua_State* L, ARGS&& ...args)
		{
			CheckImpl<CANNIL, MINARGC, 1>(L, std::forward<ARGS>(args)...);
		}
		template <typename ...ARGS>
		static void Check(lua_State* L, ARGS&& ...args)
		{
			CheckImpl<false, 0, 1>(L, std::forward<ARGS>(args)...);
		}

		static int result(lua_State* L);
		template <typename T>
		static int Result(lua_State* L, T&& value)
		{
			ResultImpl(L, value);
			return 1;
		}
		template <typename T, typename ...ARGS>
		static int Result(lua_State* L, T&& value, ARGS&& ...args)
		{
			ResultImpl(L, value);
			return result(L, std::forward<ARGS>(args)...) + 1;
		}

	public:
		static const LuaValue NilValue;
	};

}
