#define LUA_LIB
#include "lua_helper.h"
#include <iostream>

namespace 
{
	using namespace LuaCppHelper;
	int LchPrint(lua_State * L)
	{
		std::string msg;
		LuaHelper::Check(L, msg);
		std::cout << msg << std::endl;
		return LuaHelper::result(L);
	}
}

extern "C"
{
	LUALIB_API int luaopen_lch_example(lua_State * L)
	{
		static const luaL_Reg lch_example_functions[] =
		{
			{ "print", LchPrint },
			{ NULL, NULL }
		};
		luaL_newlib(L, lch_example_functions);
		return 1;
	}
}
