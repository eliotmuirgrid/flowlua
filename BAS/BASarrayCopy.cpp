//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// Implementation 
//-------------------------------------------------------
#include "BASarrayCopy.h"
#include "BASarray.h"

#include <string.h>

#include "BAStrace.h" 
BAS_TRACE_INIT

void BASarrayCopy(int argc, const char** pValues, BASarray<BASstring>* pOut){
   BAS_FUNCTION(BASarrayCopy);
   BASarray<BASstring>& Out = *pOut;
   Out.resize(argc);
   for (int i=0; i < argc; i++){
      Out[i] = pValues[i];
   }
   BAS_VAR(Out);
}
