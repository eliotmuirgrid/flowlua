#include "LUA/LUAlua.h"

#include "LUAC/lua.h"
#include "LUAC/lauxlib.h"
#include "LUAC/lualib.h"

#include "LUA/LUApathSet.h"
#include "LUA/LUAutil.h"

#include "BAS/BASargFlagPresent.h"
#include "BAS/BASstring.h"
#include "BAS/BASstream.h"
#include "BAS/BASarray.h"
#include "BAS/BASarrayCopy.h"
#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

void APPhello(const BASarray<BASstring>& Args){
   lua_State *L = lua_open();
   LUAloadLib(L);
   LUApathSet(L);
   if (luaL_loadfile(L, "main.lua") || lua_pcall(L, 0, 0, 0)) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      lua_close(L);
      return;
   }
   lua_getglobal(L, "main");
   if (lua_pcall(L, 0, 0, 0)){
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

  if (BASargFlagPresent("trace", &Match, &Args)){
     BAStrace(Match.data());
  }
  APPhello(Args);

  return 0;  // 0 means success.  Nothing is success apparently.
}

