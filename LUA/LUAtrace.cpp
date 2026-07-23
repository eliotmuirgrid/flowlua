/*************************
 * Copyright (C) Eliot Muir Forever
 *
 * LUAtrace
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 **************************/

#include <COL/COLstring.h>
#include <COL/COLglob.h>
#include <LUA/LUAlua.h>
#include <LUA/LUAtrace.h>
#include <FIL/FILpathNameNoExt.h>

#include "COL/COLtrace.h"
COL_TRACE_INIT;

static COLstring s_LUAmatch;

void LUAdebugHook(lua_State* L, lua_Debug* ar){
   COL_FUNCTION(LUAdebugHook);
   if (!lua_getinfo(L, "nSl", ar)){ return; }
   COLstring name = ar->name ? ar->name : "<anonymous>";
   COLstring source = ar->short_src[0] ? ar->short_src : "?";

   COL_VAR(source);
   if (source != "[C]"){
      source = FILpathNameNoExt(source);
      if (!COLglobMatch(source.data(), s_LUAmatch.data())){
         return;
      }
      source += ".lua";
   } else {
      source = "C++";
   }
   
   switch (ar->event){
   case LUA_HOOKCALL:                    COLtimeStamp(source.data(), COLlog);COLlog << ">" << name << newline; COLcallIncrease(); break;
   case LUA_HOOKRET : COLcallDecrease(); COLtimeStamp(source.data(), COLlog);COLlog << "<" << name << newline;                    break;
   }
}

void LUAtrace(lua_State* L, const COLstring& Match){
   s_LUAmatch = Match;
   lua_sethook(L, LUAdebugHook, LUA_MASKCALL | LUA_MASKRET, 0);
}
