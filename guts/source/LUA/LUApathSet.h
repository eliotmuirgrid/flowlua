#pragma once
/*************************
 * Copyright (C) Eliot Muir Forever
 *
 * Set a lua path
 *
 * Date: Wed  1 Jul 2026 06:17:53 EST
 **************************/

typedef struct lua_State lua_State;
class COLstring;

void LUApathSet(lua_State* L, const COLstring& Exe);
