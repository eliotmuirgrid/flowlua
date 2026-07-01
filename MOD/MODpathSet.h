#ifndef MOD_PATH_SET
#define MOD_PATH_SET
/*************************
 * Copyright (C) Eliot Muir Forever
 *
 * Set a lua path
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 **************************/

typedef struct lua_State lua_State;

void MODpathSet(lua_State* L, const char* ProgramPath);
#endif
