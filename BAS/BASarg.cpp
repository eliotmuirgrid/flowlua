//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASargParser
// 
// Implementation
//-------------------------------------------------------

#include "BASargParser.h"

#include <string.h>

#include "BAStrace.h"  // Cannot trace BASargParser since we use it in tracing.

/* // We hard code tracing into the BASargParser
static void initTracingFlags(BASargParser* pParser){
#ifndef BAS_TRACE_OFF
   pParser->addArgFlag("trace", "glob", "Turn on debug tracing of C++ code using glob match expression for files. i.e. --trace \"* -BASstring\" (match everything, exclude BASstring).");
   pParser->addArgFlag("out", "filename", "Redirect tracing output to a file.");
#endif
}

static void activateTracing(BASargParser* pParser){
#ifndef BAS_TRACE_OFF
   if (pParser->present("out"))  { BASsetTraceFile(pParser->flagArg("out").data());     }
   if (pParser->present("trace")){ BAStrace(pParser->flagArg("trace").data());}
#endif
}*/

bool BASargFlagPresent(const BASstring& Name, BASstring* pValue, int argc, char* argv[]){
   BASstring Search = "--" + Name;
   for (int i=1; i < argc; i++){
      if (Search == argv[i]){
         if (argc > i + 1){ 	
            *pValue = argv[i+1];
	    return true;
	 } else {
            *pValue = "No argument supplied for " + Name;
            return false;
	 }
      }
   }
   return false;
}

