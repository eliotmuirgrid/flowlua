/*************************
 * Copyright (C) Eliot Muir Forever
 *
 * LUAtrace
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 **************************/

#include <BAS/BASstring.h>
#include <LUA/LUAlua.h>
#include <LUA/LUAtrace.h>

#include "BAS/BAStrace.h"
BAS_TRACE_INIT;


void LUAdebugHook(lua_State* L, lua_Debug* ar){
   BAS_FUNCTION(LUAdebugHook);
   if (!lua_getinfo(L, "nSl", ar)){
      return;
   }
   const char* name = ar->name ? ar->name : "<anonymous>";
   const char* kind = ar->what ? ar->what : "?";
   const char* source = ar->short_src[0] ? ar->short_src : "?";

   BAStimeStamp(sModule.ModuleName, BASlog);
   BASlog << ">" << name << " type=" << kind << " source=" << source << " line=" << ar->currentline << newline;
}



void LUAtrace(lua_State* L, const BASstring& String){
   BAS_FUNCTION(LUAtrace);
   lua_sethook(L, LUAdebugHook, LUA_MASKCALL, 0);
}
