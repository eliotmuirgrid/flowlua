//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// Implementation 
//-------------------------------------------------------
#include "BASarrayCopy.h"
#include "BASarraySafe.h"

#include <string.h>

#include "BAStrace.h" 
BAS_TRACE_INIT

void BASarrayCopy(int argc, const char** pValues, BASarraySafe<BASstring>* pOut){
   BAS_FUNCTION(BASarrayCopy);
   BASarraySafe<BASstring>& Out = *pOut;
   Out.resize(argc);
   for (int i=0; i < argc; i++){
      Out[i] = pValues[i];
   }
   BAS_VAR(Out);
}
