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
#include <FIL/FILpathNameNoExt.h>

#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

void LUAdebugHook(lua_State* L, lua_Debug* ar){
   BAS_FUNCTION(LUAdebugHook);
   if (!lua_getinfo(L, "nSl", ar)){
      return;
   }
   BASstring name = ar->name ? ar->name : "<anonymous>";
   const char* kind = ar->what ? ar->what : "?";
   BASstring source = ar->short_src[0] ? ar->short_src : "?";

   BAS_VAR(source);
   if (source != "[C]"){  
      source = FILpathNameNoExt(source);
   } else {
      source = "C++";
   }
   BAStimeStamp(source.data(), BASlog);
   BASlog << ">" << name << " type=" << kind << " line=" << ar->currentline << newline;
}

void LUAtrace(lua_State* L, const BASstring& Match){
   BAS_FUNCTION(LUAtrace);
   BASlog << "# Tracing Lua files matching: " << Match << newline;
   lua_sethook(L, LUAdebugHook, LUA_MASKCALL, 0);
}
