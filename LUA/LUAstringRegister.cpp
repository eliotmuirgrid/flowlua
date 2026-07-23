/***************************************
 * Copyright (C) Eliot Muir Forever
 *
 * LUAstringRegister
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 ***************************************/

#include "LUA/LUAstringRegister.h"
#include "LUA/LUAstringSplit.h"
#include "LUA/LUAlua.h"

#include "COL/COLtrace.h"
COL_TRACE_INIT;

// We have to extend Lua 5.0 to do what 5.1 already has
void LUAstringRegister(lua_State* L){
   COL_FUNCTION(LUAstringRegister);
   lua_pushliteral(L, "");           // Push any string
   lua_newtable(L);                  // Create the metatable

   // metatable.__index = string
   lua_pushliteral(L, "__index");
   lua_getglobal(L, "string");
   lua_settable(L, -3);
   lua_setmetatable(L, -2);
   lua_pop(L, 1);               // Pop the string
}
