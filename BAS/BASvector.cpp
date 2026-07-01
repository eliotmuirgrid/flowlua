//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASvector
//
// Implementation
//-------------------------------------------------------

#include "BASvector.h"
#include "BASerror.h"
#include "BAStrace.h"
BAS_TRACE_INIT;

#include <assert.h>

void BAScheckBoundary(int i, int Size){
   //BAS_FUNCTION(BAScheckBoundary);
   assert(i < Size);
}

// Copy pasted from BASstring deliberately so we get tracing
unsigned int BAScalculateCapacity(unsigned int v){
   BAS_FUNCTION(BAScalculateCapacity);
   BAS_VAR(v);
   v--;
   v |= v >> 1;
   v |= v >> 2;
   v |= v >> 4;
   v |= v >> 8;
   v |= v >> 16;
   v++;
   BAS_VAR(v);
   return v;
}
