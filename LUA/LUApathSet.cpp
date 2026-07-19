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

#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

void LUApathSet(lua_State* L){
   BAS_FUNCTION(LUApathSet);

   BASstring Dir = FILdirCurrent() + "/code/?.lua";
   BAS_VAR(Dir);
   FILpathSimplify(&Dir);
   BAS_VAR(Dir);
   setenv("LUA_PATH", Dir.data(), Dir.size());
}
