/*************************
 Copyright (C) Eliot Muir Forever
 
  COLarray
 
  Date: Wed  1 Jul 2026 06:17:53 EST
 **************************/

#include "LUAlua.h"

#include <COL/COLarray.h>
#include <COL/COLtrace.h>
COL_TRACE_INIT

void LUApushArray(lua_State* L, const COLarray<COLstring>& In){
   COL_FUNCTION(LUApushArray);
   lua_newtable(L);

   for (int i = 0; i < In.size(); i++) {
      lua_pushstring(L, In[i].data());
      lua_rawseti(L, -2, i + 1);
   }  
}
