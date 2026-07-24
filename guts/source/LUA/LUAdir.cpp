/***************************************
 * Copyright (C) Eliot Muir Forever
 *
 * Set a lua path
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 ***************************************/

#include "LUA/LUAdir.h"
#include "LUA/LUAlua.h"

#include <dirent.h>

#include "COL/COLtrace.h"
COL_TRACE_INIT;


static int l_dir(lua_State *L) {
   const char *path = luaL_checkstring(L, 1);
   DIR *directory = opendir(path);

   if (!directory) {
      return luaL_error(L, "cannot open directory: %s", path);
   }

   lua_newtable(L);

   int index = 1;
   struct dirent *entry;

   while ((entry = readdir(directory)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 ||
          strcmp(entry->d_name, "..") == 0) {
          continue;
      }
      lua_pushnumber(L, index++);
      lua_pushstring(L, entry->d_name);
      lua_settable(L, -3);
   }

   closedir(directory);
   return 1;
}

void LUAloadDir(lua_State* L){
   COL_FUNCTION(LUAloadDir);
   lua_register(L, "dir", l_dir);
}

