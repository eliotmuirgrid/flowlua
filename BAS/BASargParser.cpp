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

// We hard code tracing into the BASargParser
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
}

BASargParser::BASargParser(){
   initTracingFlags(this);
}

BASargParser::~BASargParser(){
}

void BASargParser::addArgFlag(const BASstring& Name, const BASstring& Arg, const BASstring& Description){
   m_Flags.add(Name, LineInfo(Description, Arg));
}

void BASargParser::addFlag(const BASstring& Name, const BASstring& Description){
   m_Flags.add(Name, LineInfo(Description));
}

void BASargParser::showUsage(BASstream& Stream) const{
   Stream << "Usage:" << newline
          << " " << m_Bin;
   if (m_Flags.size() ==0){
      Stream << newline;
      return;
   }
   for(auto i=m_Flags.cbegin(); i != m_Flags.cend(); ++i){
      Stream << " --" << i.key();
      if (i.value().HasArgument){
         Stream << " <" << i.value().Arg << ">";
      }
   }
   Stream << newline << "Flags:" << newline;
   for(auto i=m_Flags.cbegin(); i != m_Flags.cend(); ++i){
       Stream << "   --" << i.key();
       if (i.value().HasArgument){
          Stream << " <" << i.value().Arg << ">";
       }
       Stream << " " << i.value().Description << newline;
   }
   Stream << newline;
}

bool BASargParser::parse(int argc, const char** argv){
   m_Bin = argv[0];
   int i = 1;
   while (i < argc){
      if (strlen(argv[i]) >= 3){
         if (argv[i][0] == '-' && argv[i][1] == '-'){
            BASstring Flag(((char*)argv[i])+2);
            if (m_Flags.has(Flag)){
               m_Flags.value(Flag).Present = true;
               if (m_Flags.value(Flag).HasArgument){
                  i++;
                  if (i < argc){
                     m_Flags.value(Flag).ArgValue = argv[i];
                  } else {
                     return false;
                  }
               }
            } else {
               return false;
            }
         }
      }
      i++;
   }
   activateTracing(this);

   return true;
}

BASstream& operator<<(BASstream& Stream, const BASargParser& Parser){
   Parser.showUsage(Stream);
   return Stream;
}