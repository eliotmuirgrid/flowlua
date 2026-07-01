/***************************************
 * Copyright (C) Eliot Muir Forever
 *
 * Set a lua path
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 ***************************************/

#include "MOD/MODpathSet.h"

void MODpathSet(lua_State* L, const char* ProgramDir){
   char Path[1024];
   //snprintf(Path, sizeof(Path),
   //      "%s/scripts/?.lua;%s/lib/?.lua",
   //      ProgramDirectory,
   //      ProgramDirectory);

   //lua_getglobal(L, "package");
   //lua_pushstring(L, Path);
   //lua_setfield(L, -2, "path");
   //lua_pop(L, 1);
}
