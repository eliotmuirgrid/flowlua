#pragma once
/*************************
 Copyright (C) Eliot Muir Forever
 
  BASarray
 
  Date: Wed  1 Jul 2026 06:17:53 EST
 **************************/

#include "BAS/BASbase.h"

typedef struct lua_State lua_State;

void LUApushArray(lua_State* L, const BASarray<BASstring>& In);
