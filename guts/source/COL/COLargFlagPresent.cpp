//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// Implementation
//-------------------------------------------------------

#include "COLargFlagPresent.h"
#include "COLstring.h"
#include "COLarray.h"
#include "COLtrace.h"
COL_TRACE_INIT


bool COLargFlagPresent(const COLstring& Name, COLstring* pValue, COLarray<COLstring>* pArgs){
   COL_FUNCTION(COLargFlagPresent);
   COLarray<COLstring>& Args = *pArgs;
   COL_VAR2(Name, Args);
   COLstring Search = "--" + Name;
   for (int i=1; i < Args.size()-1; i++){
      if (Search == Args[i]){
         *pValue = Args[i+1];
	 Args.remove(i);
	 Args.remove(i);
	 return true;
      }
   }
   return false;
}

bool COLargFindFlag(const COLstring& Name, COLarray<COLstring>* pArgs){
   COL_FUNCTION(COLargFindFlag);
   COLarray<COLstring>& Args = *pArgs;
   COL_VAR2(Name, Args);
   COLstring Search = "--" + Name;
   for (int i=1; i < Args.size(); i++){
      if (Search == Args[i]){
         Args.remove(i);
	 return true;
      }
   }
   return false;
}
