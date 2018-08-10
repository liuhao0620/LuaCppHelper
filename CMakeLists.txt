cmake_minimum_required (VERSION 3.11.2)
project(LuaCppHelper)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)

# liblua
set(LUA_DIR ${CMAKE_CURRENT_LIST_DIR}/lua)
set(LIB_LUA_CORE_SRC
    ${LUA_DIR}/lapi.c
    ${LUA_DIR}/lauxlib.c
    ${LUA_DIR}/lbaselib.c
    ${LUA_DIR}/lbitlib.c
    ${LUA_DIR}/lcode.c
    ${LUA_DIR}/lcorolib.c
    ${LUA_DIR}/lctype.c
    ${LUA_DIR}/ldblib.c
    ${LUA_DIR}/ldebug.c
    ${LUA_DIR}/ldo.c
    ${LUA_DIR}/ldump.c
    ${LUA_DIR}/lfunc.c
    ${LUA_DIR}/lgc.c
    ${LUA_DIR}/linit.c
    ${LUA_DIR}/liolib.c
    ${LUA_DIR}/llex.c
    ${LUA_DIR}/lmathlib.c
    ${LUA_DIR}/lmem.c
    ${LUA_DIR}/loadlib.c
    ${LUA_DIR}/lobject.c
    ${LUA_DIR}/lopcodes.c
    ${LUA_DIR}/loslib.c
    ${LUA_DIR}/lparser.c
    ${LUA_DIR}/lstate.c
    ${LUA_DIR}/lstring.c
    ${LUA_DIR}/lstrlib.c
    ${LUA_DIR}/ltable.c
    ${LUA_DIR}/ltablib.c
    ${LUA_DIR}/ltm.c
    ${LUA_DIR}/lundump.c
    ${LUA_DIR}/lutf8lib.c
    ${LUA_DIR}/lvm.c
    ${LUA_DIR}/lzio.c)
add_library(liblua SHARED ${LIB_LUA_CORE_SRC})
set(LUA_LIB liblua)
set(LUA_INC_DIR ${LUA_DIR})
add_definitions(-DLUA_BUILD_AS_DLL)

# lua
set(LUA_SRC ${LUA_DIR}/lua.c)
add_executable(lua ${LUA_SRC})
target_include_directories(lua PUBLIC ${LUA_INC_DIR})
target_link_libraries(lua ${LUA_LIB})

# lch_example
set(LCH_EXAMPLE_SRC ${CMAKE_CURRENT_LIST_DIR}/lua_value.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lua_helper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lch_example.cpp)
add_library(lch_example SHARED ${LCH_EXAMPLE_SRC})
target_include_directories(lch_example PUBLIC ${LUA_INC_DIR})
target_link_libraries(lch_example ${LUA_LIB})
