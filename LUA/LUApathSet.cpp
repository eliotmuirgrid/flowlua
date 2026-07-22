/***************************************
 * Copyright (C) Eliot Muir Forever
 *
 * Set a lua path
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 ***************************************/

#include "LUApathSet.h"

#include "FIL/FILdir.h"

#include <stdlib.h>

#include "COL/COLtrace.h"
COL_TRACE_INIT;

void LUApathSet(lua_State* L){
   COL_FUNCTION(LUApathSet);

   COLstring Dir = FILdirCurrent() + "/code/?.lua";
   COL_VAR(Dir);
   FILpathSimplify(&Dir);
   COL_VAR(Dir);
   setenv("LUA_PATH", Dir.data(), Dir.size());
}
