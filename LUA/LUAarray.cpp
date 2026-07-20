/*************************
 Copyright (C) Eliot Muir Forever
 
  BASarray
 
  Date: Wed  1 Jul 2026 06:17:53 EST
 **************************/

#include "LUAlua.h"

#include <BAS/BASarray.h>
#include <BAS/BAStrace.h>
BAS_TRACE_INIT

void LUApushArray(lua_State* L, const BASarray<BASstring>& In){
   BAS_FUNCTION(LUApushArray);
   lua_newtable(L);

   for (int i = 0; i < In.size(); i++) {
      lua_pushstring(L, In[i].data());
      lua_rawseti(L, -2, i + 1);
   }  
}
