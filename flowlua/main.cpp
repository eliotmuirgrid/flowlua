#include "LUA/LUAlua.h"

#include "LUAC/lua.h"
#include "LUAC/lauxlib.h"
#include "LUAC/lualib.h"

#include "LUA/LUAarray.h"
#include "LUA/LUApathSet.h"
#include "LUA/LUAutil.h"
#include "LUA/LUAtrace.h"
#include "LUA/LUAdir.h"

#include "APPinstall.h"

#include "COL/COLargFlagPresent.h"
#include "COL/COLstring.h"
#include "COL/COLstream.h"
#include "COL/COLarray.h"
#include "COL/COLarrayCopy.h"
#include "COL/COLtrace.h"
COL_TRACE_INIT;

void APPrun(lua_State* L, const COLarray<COLstring>& Args){
   COL_FUNCTION(APPrun);
   LUAloadLib(L);
   LUAloadDir(L);
   LUApathSet(L);
   if (luaL_loadfile(L, "main.lua") || lua_pcall(L, 0, 0, 0)) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      lua_close(L);
      return;
   }
   lua_getglobal(L, "main");
   LUApushArray(L, Args);
   if (lua_pcall(L, 1, 0, 0)){
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      lua_close(L);
      return;
   }
   lua_close(L);
}

int main (int argc, const char** argv) {
  COL_FUNCTION(main);
  COLstring Match;
  COLarray<COLstring> Args;
  COLarrayCopy(argc, argv, &Args);

  bool Tracing = false;
  if (COLargFlagPresent("ctrace", &Match, &Args)){ COLtrace(Match.data()); Tracing=true; }
  if (COLargFindFlag   ("install", &Args))       { APPinstall(); return 0; }
  
  lua_State* L = lua_open();
  if (COLargFlagPresent("ltrace", &Match, &Args)){ LUAtrace(L, Match.data()); Tracing = true; }
  COL_VAR(Tracing);
  if (Tracing) { COLheader(); }

  APPrun(L, Args);

  return 0;  // 0 means success.  Nothing is success apparently.
}

