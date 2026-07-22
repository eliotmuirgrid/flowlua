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

#include "BAS/BASargFlagPresent.h"
#include "BAS/BASstring.h"
#include "BAS/BASstream.h"
#include "BAS/BASarray.h"
#include "BAS/BASarrayCopy.h"
#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

void APPrun(lua_State* L, const BASarray<BASstring>& Args){
   BAS_FUNCTION(APPrun);
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
  BAS_FUNCTION(main);
  BASstring Match;
  BASarray<BASstring> Args;
  BASarrayCopy(argc, argv, &Args);

  bool Tracing = false;
  if (BASargFlagPresent("ctrace", &Match, &Args)){ BAStrace(Match.data()); Tracing=true; }
  if (BASargFindFlag   ("install", &Args))       { APPinstall(); return 0; }
  
  lua_State* L = lua_open();
  if (BASargFlagPresent("ltrace", &Match, &Args)){ LUAtrace(L, Match.data()); Tracing = true; }
  BAS_VAR(Tracing);
  if (Tracing) { BASheader(); }

  APPrun(L, Args);

  return 0;  // 0 means success.  Nothing is success apparently.
}

