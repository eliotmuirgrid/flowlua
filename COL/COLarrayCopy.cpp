//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// Implementation 
//-------------------------------------------------------
#include "COLarrayCopy.h"
#include "COLarray.h"

#include <string.h>

#include "COLtrace.h" 
COL_TRACE_INIT

void COLarrayCopy(int argc, const char** pValues, COLarray<COLstring>* pOut){
   COL_FUNCTION(COLarrayCopy);
   COLarray<COLstring>& Out = *pOut;
   Out.resize(argc);
   for (int i=0; i < argc; i++){
      Out[i] = pValues[i];
   }
   COL_VAR(Out);
}
