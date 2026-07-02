/***************************************
 * Copyright (C) Eliot Muir Forever
 *
 * Set a lua path
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 ***************************************/

#include "MOD/MODpathSet.h"

#include "FIL/FILdir.h"

#include <stdlib.h>

#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

void MODpathSet(lua_State* L){
   BAS_FUNCTION(MODpathSet);

   BASstring Dir = FILdirCurrent() + "/../code/?.lua";
   Dir = FILpathSimplify(Dir);
   BAS_VAR(Dir);
   setenv("LUA_PATH", Dir.data(), Dir.size());
}
