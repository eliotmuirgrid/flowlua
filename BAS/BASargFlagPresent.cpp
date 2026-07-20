//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// Implementation
//-------------------------------------------------------

#include "BASargFlagPresent.h"
#include "BASstring.h"
#include "BASarray.h"
#include "BAStrace.h"
BAS_TRACE_INIT


bool BASargFlagPresent(const BASstring& Name, BASstring* pValue, BASarray<BASstring>* pArgs){
   BAS_FUNCTION(BASargFlagPresent);
   BASarray<BASstring>& Args = *pArgs;
   BAS_VAR2(Name, Args);
   BASstring Search = "--" + Name;
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

bool BASargFindFlag(const BASstring& Name, BASarray<BASstring>* pArgs){
   BAS_FUNCTION(BASargFindFlag);
   BASarray<BASstring>& Args = *pArgs;
   BAS_VAR2(Name, Args);
   BASstring Search = "--" + Name;
   for (int i=1; i < Args.size(); i++){
      if (Search == Args[i]){
         Args.remove(i);
	 return true;
      }
   }
   return false;
}
