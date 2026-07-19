//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// Implementation
//-------------------------------------------------------

#include "BASargFlagPresent.h"
#include "BASstring.h"
#include "BASarraySafe.h"
#include "BAStrace.h"
BAS_TRACE_INIT


bool BASargFlagPresent(const BASstring& Name, BASstring* pValue, BASarraySafe<BASstring>* pArgs){
   BAS_FUNCTION(BASargFlagPresent);
   BASarraySafe<BASstring>& Args = *pArgs;
   BAS_VAR2(Name, Args);
   BASstring Search = "--" + Name;
   for (int i=1; i < Args.size()-1; i++){
      if (Search == Args[i]){
         *pValue = Args[i+1];
	 //Args.remove(i);
	 //Args.remove(i);
	 return true;
      }
   }
   return false;
}

