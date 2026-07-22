//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLglob
// 
// Implementation
//-------------------------------------------------------

#include "COLglob.h"
#include "COLtrace.h"
//#include "COLstring.h"
//COL_TRACE_INIT;

#include <string.h>

bool COLmatchUntilChar(const char** ppTarget, char Char){
  // COL_FUNCTION(COLmatchUntilChar);
  // COL_VAR2(*ppTarget, Char);
   while (*(*ppTarget) != Char){
      (*ppTarget)++;
   }
   bool Result = *(*ppTarget) == Char;

  // COL_VAR2(Result, *ppTarget);
   return Result;
}

bool COLglob(const char* pTarget, const char* pPattern){
   //COL_FUNCTION(COLglob);
   //COL_VAR2(pTarget, pPattern);
   const char* i= pPattern;
   const char* j= pTarget;
   while (*i != 0){
    //  COL_VAR(*i);
      if (*i == '*'){
         i++;
         if (*i == 0){
      //      COL_TRC("Expression matches");
            return true;
         }
         if (!COLmatchUntilChar(&j, *i)){
            return false;
         } 
      } else {
         if (*i != *j){
        //    COL_TRC("Expression does not match.");
            return false;
         }
      }
      ++j;
      ++i;
   }
//   COL_TRC("Expression matches.");
   return true;
}

bool COLglobMatch(const char* pTarget, const char* pPattern){
  // COL_FUNCTION(COLglobMatch);
   bool Match = false;
   //COL_VAR(pPattern);
   char* Buffer = new char[strlen(pPattern)+1];
   strcpy(Buffer, pPattern);
	const char* delim = " ";
	char* pToken = strtok(Buffer, delim);
	while(pToken != NULL){
    //  COL_VAR(pToken);
      if (*pToken == '+'){
         pToken++;
         if (COLglob(pTarget, pToken)){
      //      COL_TRC("Positive Match");
            Match = true;
         }
      } else if (*pToken == '-'){
         pToken++;
         if (COLglob(pTarget, pToken)){
      //      COL_TRC("Negative match removed");
            Match = false;
         }
      } else {
         if (COLglob(pTarget, pToken)){
      //      COL_TRC("Positive match");
            Match = true;
         }
      }   
		pToken = strtok(NULL, delim);
	}
   delete []Buffer;
//   COL_VAR(Match);
   return Match;
}
