//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BAStrace
// 
// Implementation
//-------------------------------------------------------

#include "BAStrace.h"

#ifdef BAS_TRACE_OFF
// Do nothing is tracing is switched off
void BAStrace(const char* Pattern){}
void BASsetTraceFile(const char* FileName){}
#else

#include "BASsinkFile.h"
#include "BASglob.h"
#include "BAShex.h"
#include "BAStimestamp.h"

#include "BASsinkFile.h"

#include <time.h>
#include <stdio.h>  // for printf

#include <inttypes.h>
#include <math.h>
#include <string.h>


#include <errno.h>
#include <fcntl.h>

#ifdef _WIN32
  #include <windows.h>
  #include <sys/stat.h>
  #include <io.h>
#endif

#include "BASfileImp.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef _WIN32
BASint64 BASthreadId(){ return (BASint64)GetCurrentThreadId(); }
#else
#include <pthread.h>
BASint64 BASthreadId(){ return (BASint64)pthread_self(); }
#endif


BASmutex s_LogMutex;

BASstream BASlog(new BASsinkFile(1), false);  // purposely leaked.

static thread_local int s_BASindentLevel=0;

static char   s_BAStimeBuffer[64];
static time_t s_BASlastTimeStamp=0;

void BAStimeStamp(const char* pModule, BASstream& Stream){
   BAStimestamp Time = BAScurrentTime();
   if (s_BASlastTimeStamp != Time.Seconds){  //Calculating the Julian date from unix epoch is CPU intensive, so only do it when you need to.
      s_BASlastTimeStamp = Time.Seconds;
      struct tm* tm = localtime(&s_BASlastTimeStamp);
      strftime(s_BAStimeBuffer, sizeof(s_BAStimeBuffer), "  %H:%M:%S", tm); 
   }
   BASlog << s_BAStimeBuffer;
   char Buffer[50];
   int Count = sprintf(Buffer,".%06lli %0#11llx ", Time.Microseconds, BASthreadId());  // pad milliseconds and thread id.
   Stream.sink()->write(Buffer, Count);
   Stream << pModule << " ";  // TODO should output size.
   BASwriteIndent(Stream.sink(), s_BASindentLevel);
}

BASmodule::BASmodule(const char* pFileName){
   int Length = strlen(pFileName)-4;
   strncpy(ModuleName, pFileName, Length);
   char* pEnd = ModuleName + sizeof(ModuleName)-2;
   for (char* i = ModuleName + Length; i != pEnd; i++){
      *i = ' ';
   }
   ModuleName[sizeof(ModuleName)-1] = 0;
}

static const char* s_TracePattern = "";

void BAStrace(const char* pPattern){
   BASout << "### Tracing files matching: " << pPattern << newline;
   s_TracePattern = strdup(pPattern);  // purposely leaked.
   BASlog << "  Timestamp       Thread ID   File" << newline;
}

void BASsetTraceFile(const char* FileName){
   int ErrorCode;
   int FileHandle = BASopen(FileName, O_BINARY | O_RDWR | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);  
   if (FileHandle == -1){
      BASout << "### Failed to open " << FileName << " for tracing." << newline;
      return;
   }
   BASlog.setSink(new BASsinkFile(FileHandle));  // Does it matter that we don't close the file handle?
   BASout << "### Tracing redirected into " << FileName << newline;
}

bool BASloggingEnabled(const char* ModuleName, int* pResult){
   if (BASglobMatch(ModuleName, s_TracePattern)){
      *pResult = 1;
      return true;
   } else {
      *pResult = -1;
      return false;
   }
}

BASraiiFunc::BASraiiFunc(const char* Name, const char* pModule, int Line, bool Trace) : m_pModule(pModule), m_pName(Name), m_Trace(Trace) {
   if (Trace){
      BASlocker Lock(s_LogMutex);
      BAStimeStamp(pModule, BASlog); BASlog << ">" << Name << " Line:" << Line << newline;
      s_BASindentLevel++;
   }
}

BASraiiFunc::BASraiiFunc(const char* Name, const char* pModule, int Line, const void* pInstance, bool Trace) : m_pModule(pModule), m_pName(Name), m_Trace(Trace) {
   if (Trace){
      BASlocker Lock(s_LogMutex);
      BAStimeStamp(pModule, BASlog); BASlog << ">" << Name << " Line:" << Line << " this=" << pInstance << newline;
      s_BASindentLevel++;
   }
}

BASraiiFunc::~BASraiiFunc(){
   if (m_Trace){
      s_BASindentLevel--;
      BASlocker Lock(s_LogMutex);
      BAStimeStamp(m_pModule, BASlog); BASlog << "<" << m_pName << newline;
   }
}

#define G ". . . . . . . . . . . . . . . . . . . . . . . . . "
static const char* s_pIndent = G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G;

void BASwriteIndent(BASsink* pSink, int Level){
   pSink->write(s_pIndent, Level*2);
}


void BAShexTrace(int Size, const void* pBuffer, BASstream& Stream){
   Stream << "= (size=" << Size << ")" << newline;
   BAShex(Size, pBuffer, Stream.sink(), 60);
}
#endif // BAS_TRACE_OFF
