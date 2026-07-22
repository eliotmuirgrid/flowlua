/*************************
 * Copyright (C) Eliot Muir Forever
 *
 * LUAtrace
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 **************************/

#include <COL/COLstring.h>
#include <LUA/LUAlua.h>
#include <LUA/LUAtrace.h>
#include <FIL/FILpathNameNoExt.h>

#include "COL/COLtrace.h"
COL_TRACE_INIT;

void LUAdebugHook(lua_State* L, lua_Debug* ar){
   COL_FUNCTION(LUAdebugHook);
   if (!lua_getinfo(L, "nSl", ar)){
      return;
   }
   COLstring name = ar->name ? ar->name : "<anonymous>";
   const char* kind = ar->what ? ar->what : "?";
   COLstring source = ar->short_src[0] ? ar->short_src : "?";

   COL_VAR(source);
   if (source != "[C]"){  
      source = FILpathNameNoExt(source);
   } else {
      source = "C++";
   }
   COLtimeStamp(source.data(), COLlog);
   COLlog << ">" << name << " type=" << kind << " line=" << ar->currentline << newline;
}

void LUAtrace(lua_State* L, const COLstring& Match){
   COL_FUNCTION(LUAtrace);
   COLlog << "# Tracing Lua files matching: " << Match << newline;
   lua_sethook(L, LUAdebugHook, LUA_MASKCALL, 0);
}
