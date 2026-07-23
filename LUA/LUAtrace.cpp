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

void LUAprintValue(lua_State* L, int i){
   int type = lua_type(L, i);
   switch (type) {
      case LUA_TNIL:     COLlog << "nil";                                    break;
      case LUA_TBOOLEAN: COLlog << (lua_toboolean(L, i) ? "true" : "false"); break;
      case LUA_TNUMBER:  COLlog << lua_tonumber(L, i);                       break;
      case LUA_TSTRING:  COLlog << '"' << lua_tostring(L, i) << '"';         break;
      default:           COLlog << '<' << lua_typename(L, type) << '>';      break;
   }
}

void LUAshowArgs(lua_State* L, lua_Debug* ar){
   COLlog << "(";
   for (int i = 1;; ++i) {
      const char* localName = lua_getlocal(L, ar, i);
      if (!localName) { break; }
      if (i > 1) { COLlog << ", "; }

      COLlog << localName << "=";
      LUAprintValue(L, -1);
      lua_pop(L, 1);
   }
   COLlog << ")";
}

void LUAdebugHook(lua_State* L, lua_Debug* ar){
   COL_FUNCTION(LUAdebugHook);
   if (!lua_getinfo(L, "nSl", ar)){ return; }
   COLstring name = ar->name ? ar->name : "<anonymous>";
   COLstring source = ar->short_src[0] ? ar->short_src : "?";

   COL_VAR(source);
   if (source != "[C]"){
      source = FILpathNameNoExt(source);
      if (!COLglobMatch(source.data(), s_LUAmatch.data())){ return; }
      source += ".lua";
   } else {
      if (!COLglobMatch(name.data(), s_LUAmatch.data())) { return; }
      source = "C++";
   }
   
   switch (ar->event){
   case LUA_HOOKCALL:                            
      COLtimeStamp(source.data(), COLlog);COLlog << ">" << name; LUAshowArgs(L, ar); COLlog << newline; COLcallIncrease(); break;
   case LUA_HOOKRET : 
      COLcallDecrease(); COLtimeStamp(source.data(), COLlog);COLlog << "<" << name << newline;                             break;
   }
}

void LUAtrace(lua_State* L, const COLstring& Match){
   s_LUAmatch = Match;
   lua_sethook(L, LUAdebugHook, LUA_MASKCALL | LUA_MASKRET, 0);
}
