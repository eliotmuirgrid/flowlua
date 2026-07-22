//-------------------------------------------------------
// Copyright (C) 2026 Eliot Muir.  All Rights Reserved
//
// COLarray
//
// Implementation
//-------------------------------------------------------

#include "COLarray.h"
#include "COLerror.h"
#include "COLtrace.h"
COL_TRACE_INIT;

#include <assert.h>

void COLcheckBoundary(int i, int Size){
   //COL_FUNCTION(COLcheckBoundary);
   assert(i < Size);
}

// Copy pasted from COLstring deliberately so we get tracing
unsigned int COLcalculateCapacity(unsigned int v){
   COL_FUNCTION(COLcalculateCapacity);
   COL_VAR(v);
   v--;
   v |= v >> 1;
   v |= v >> 2;
   v |= v >> 4;
   v |= v >> 8;
   v |= v >> 16;
   v++;
   COL_VAR(v);
   return v;
}
