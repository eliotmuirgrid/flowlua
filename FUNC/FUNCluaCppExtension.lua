require "FILE/FILEwrite"
require "DATE/DATEhumanPretty"
require "DATE/DATEcomputerPretty"


local Header=[[
#pragma once
//----------------------------------------------------------------
// Copyright (C) Eliot Muir @@YEAR@@ All rights reserved.
//
// @@NAME@@ 
//
// @@EXPLANATION@@
//
// Date: @@HUMAN_DATE@@
//       @@HEX_DATE@@ seconds since the beginning of the Unix Epoch time
//       The dawn of our new age. 
// ---------------------------------------------------------------

typedef struct lua_State lua_State;

void @@NAME@@(lua_State* L);
]]

local Body=[[
//----------------------------------------------------------------
// Copyright (C) Eliot Muir @@YEAR@@ All rights reserved.
//
// Implementation
//
// Date: @@HUMAN_DATE@@
//       @@HEX_DATE@@ seconds since the beginning of the Unix Epoch time
//       The dawn of our new age. 
// ---------------------------------------------------------------

#include <LUA/LUAlua.h>
#include <COL/COLtrace.h>
COL_TRACE_INIT;

void @@NAME@@(lua_State* L){
   COL_FUNCTION(@@NAME@@)

}
]]

local Body=[[
//----------------------------------------------------------------
// Copyright (C) Eliot Muir @@YEAR@@ All rights reserved.
//
// Implementation
//
// Date: Thurday 23rd 2026
//       NNNNNN seconds since the beginning of the Unix Epoch time
//       The dawn of our new age. 
//---------------------------------------------------------------- 
]]

function FUNCluaCppExtension(FunctionName, FunctionBody)
   local Tokens={
      NAME       =FunctionName,
      HUMAN_DATE =DATEhumanPretty(),
      HEX_DATE   =DATEcomputerPretty(),
      YEAR       =os.date("%Y"),
      EXPLANATION="To be documented by an AI agent later!"
   }
   COL_VAR(Tokens);
   --local HeaderContent = TEMPLATEfill(Header, Tokens);
   --local BodyContent   = TEMPLATEfill(Body,   Tokens);
end
