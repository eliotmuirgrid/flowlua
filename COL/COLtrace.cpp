//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLtrace
// 
// Implementation
//-------------------------------------------------------

#include "COLtrace.h"

#ifdef COL_TRACE_OFF
// Do nothing is tracing is switched off
void COLtrace(const char* Pattern){}
void COLsetTraceFile(const char* FileName){}
#else

#include "COLglob.h"
#include "COLhex.h"
#include "COLtimestamp.h"
#include "COLsinkFile.h"
#include "COLthread.h"

#include <time.h>
#include <stdio.h>  // for printf

#include <inttypes.h>
#include <math.h>
#include <string.h>


#include <errno.h>
#include <fcntl.h>


//COLmutex s_LogMutex;

COLstream COLlog(new COLsinkFile(1), false);  // purposely leaked.

static thread_local int s_COLindentLevel=0;

void COLcallIncrease(){
   s_COLindentLevel++;
}

void COLcallDecrease(){
   s_COLindentLevel--;
}

static char   s_COLtimeBuffer[64];
static time_t s_COLlastTimeStamp=0;

void COLtimeStamp(const char* pModule, COLstream& Stream){
   COLtimestamp Time = COLcurrentTime();
   if (s_COLlastTimeStamp != Time.Seconds){  //Calculating the Julian date from unix epoch is CPU intensive, so only do it when you need to.
      s_COLlastTimeStamp = Time.Seconds;
      struct tm* tm = localtime(&s_COLlastTimeStamp);
      strftime(s_COLtimeBuffer, sizeof(s_COLtimeBuffer), "  %H:%M:%S", tm); 
   }
   COLlog << s_COLtimeBuffer;
   char Buffer[50];
   int Count = snprintf(Buffer, sizeof(Buffer), ".%06lli ", Time.Microseconds);  // pad milliseconds and thread id.
   Stream.sink()->write(Buffer, Count);
   Count = snprintf(Buffer, sizeof(Buffer), "%-9d ", COLthreadId());
   Stream.sink()->write(Buffer, Count);
   Count = snprintf(Buffer, sizeof(Buffer), "%-25s ", pModule); 
   Stream.sink()->write(Buffer, Count);
   COLwriteIndent(Stream.sink(), s_COLindentLevel);
}

COLmodule::COLmodule(const char* pFileName){
   strncpy(ModuleName, pFileName, sizeof(ModuleName));
}

static const char* s_TracePattern = "";

void COLtrace(const char* pPattern){
   COLout << "# Tracing files matching: " << pPattern << newline;
   s_TracePattern = strdup(pPattern);  // purposely leaked.
}

void COLheader(){
   COLlog << "  Timestamp       Thread ID File                       Trace Output" << newline;
}

/*void COLsetTraceFile(const char* FileName){
   int ErrorCode;
   int FileHandle = COLopen(FileName, O_BINARY | O_RDWR | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);  
   if (FileHandle == -1){
      COLout << "### Failed to open " << FileName << " for tracing." << newline;
      return;
   }
   COLlog.setSink(new COLsinkFile(FileHandle));  // Does it matter that we don't close the file handle?
   COLout << "### Tracing redirected into " << FileName << newline;
}*/

bool COLloggingEnabled(const char* ModuleName, int* pResult){
   if (COLglobMatch(ModuleName, s_TracePattern)){
      *pResult = 1;
      return true;
   } else {
      *pResult = -1;
      return false;
   }
}

COLraiiFunc::COLraiiFunc(const char* Name, const char* pModule, int Line, bool Trace) : m_pModule(pModule), m_pName(Name), m_Trace(Trace) {
   if (Trace){
      //COLlocker Lock(s_LogMutex);
      COLtimeStamp(pModule, COLlog); COLlog << ">" << Name << " Line:" << Line << newline;
      COLcallIncrease();
   }
}

COLraiiFunc::COLraiiFunc(const char* Name, const char* pModule, int Line, const void* pInstance, bool Trace) : m_pModule(pModule), m_pName(Name), m_Trace(Trace) {
   if (Trace){
   //   COLlocker Lock(s_LogMutex);
      COLtimeStamp(pModule, COLlog); COLlog << ">" << Name << " Line:" << Line << " this=" << pInstance << newline;
      COLcallIncrease();
   }
}

COLraiiFunc::~COLraiiFunc(){
   if (m_Trace){
      COLcallDecrease();
 //     COLlocker Lock(s_LogMutex);
      COLtimeStamp(m_pModule, COLlog); COLlog << "<" << m_pName << newline;
   }
}

#define G ". . . . . . . . . . . . . . . . . . . . . . . . . "
static const char* s_pIndent = G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G;

void COLwriteIndent(COLsink* pSink, int Level){
   pSink->write(s_pIndent, Level*2);
}


void COLhexTrace(int Size, const void* pBuffer, COLstream& Stream){
   Stream << "= (size=" << Size << ")" << newline;
   COLhex(Size, pBuffer, Stream.sink(), 60);
}
#endif // COL_TRACE_OFF
