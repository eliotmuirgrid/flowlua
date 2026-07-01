#ifndef _BAS_TRACE_H_
#define _BAS_TRACE_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BAStrace
// 
// Tracing framework
//
// To use it do:
// #include <BAS/BAStrace.h>
// BAS_TRACE_INIT;
//
// Then you can use:
//  BAS_TRC("This is tracing.");   // Traces to output
//  BAS_FUNCTION(myFunctionName);  // Gives entry into a function and out of it
//  BAS_METHOD(Foo::MyMethod);     // Gives entry into a method and shows the this pointer and out of it
//  BAS_VAR(AVariable);            // Gives name=value of a variable - use BAS_VAR2, BAS_VAR3 etc. to display 2, 3 variables etc.
//
// Tracing is enabled through the BASargParser which can be used to control the command line.  Or you can just
// call BASsetTracePattern directly with a glob expression that is used to match what files to display tracing from.
//-------------------------------------------------------

#include "BASstream.h"
#include <stdio.h>
#include "BASmutex.h"
#include "BASstring.h"
#include "BASsinkString.h"
#include <string.h>

// Call this to activate tracing if you need more control - using
// BASargParser will make it difficult to trace the BASstring class etc.
// Since that object uses BASstring and BASavlTree etc.
void BAStrace(const char* Pattern);
void BASsetTraceFile(const char* FileName);

//#define BAS_TRACE_OFF  // uncomment this to turn tracing off.
#ifdef BAS_TRACE_OFF
#define BAS_TRC(A)
#define BAS_VAR(A)
#define BAS_VAR2(A,B)
#define BAS_VAR3(A,B,C)
#define BAS_VAR4(A,B,C,D)
#define BAS_FUNCTION(NAME)
#define BAS_METHOD(NAME)
#define BAS_HEX(LABEL, BUFFER, SIZE)
#define BAS_TRACE_INIT
#else

extern BASstream BASlog;

void BAStimeStamp(const char* pModule, BASstream& Stream);
void BASmilliSeconds();
extern BASmutex s_LogMutex;

class BASmodule{
public:
    BASmodule(const char* pFileName);
    char ModuleName[25];
};

#ifdef _WIN32
   #define __FILENAME__ (strrchr("\\" __FILE__, '\\') + 1)
#else
   #define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)
#endif

#define BAS_TRACE_INIT static BASmodule sModule(__FILENAME__);

bool BASloggingEnabled(const char* ModuleName, int* pResult);

class BASlogStream : public BASstream{
public:
   BASstring m_String;
   BASlogStream() : BASstream(new BASsinkString(&m_String), true) {}
   ~BASlogStream() {}
private:
};

#define BAS_TRC(X)\
   do {\
      static int BASdoLog;\
      if (BASdoLog > 0 || (BASdoLog == 0 && BASloggingEnabled(sModule.ModuleName, &BASdoLog)) ){\
         BASlogStream LogStream;\
         LogStream << X << newline;\
         BASlocker Lock(s_LogMutex);\
         BAStimeStamp(sModule.ModuleName, BASlog);\
         BASlog << LogStream.m_String;\
      }\
   } while(0)

void BAShexTrace(int Size, const void* pBuffer, BASstream& Stream);

#define BAS_HEX(LABEL, BUFFER, SIZE)\
   do {\
      static int BASdoLog;\
      if (BASdoLog > 0 || (BASdoLog == 0 && BASloggingEnabled(sModule.ModuleName, &BASdoLog)) ){\
         BASlogStream LogStream;\
         LogStream << LABEL; BAShexTrace(SIZE, BUFFER, LogStream); LogStream << newline;\
         BASlocker Lock(s_LogMutex);\
         BAStimeStamp(sModule.ModuleName, BASlog);\
         BASlog << LogStream.m_String;\
      }\
   } while(0)




#define BAS_VAR(A)        BAS_TRC(#A << " = " << A);
#define BAS_VAR2(A,B)     BAS_TRC(#A << " = " << A << ", " << #B << " = " << B);
#define BAS_VAR3(A,B,C)   BAS_TRC(#A << " = " << A << ", " << #B << " = " << B << ", " << #C << " = " << C);
#define BAS_VAR4(A,B,C,D) BAS_TRC(#A << " = " << A << ", " << #B << " = " << B << ", " << #C << " = " << C << ", " << #D << " = " << D);


class BASraiiFunc{
public:
    BASraiiFunc(const char* Name, const char* pModule, int Line, bool Trace);
    BASraiiFunc(const char* Name, const char* pModule, int Line, const void* pThis, bool Trace);
    
    ~BASraiiFunc();
private:
   bool m_Trace;
   const char* m_pName;
   const char* m_pModule; 
};


#define BAS_FUNCTION(NAME) static int BASfOn; BASraiiFunc ggFFF(#NAME, sModule.ModuleName, __LINE__,(BASfOn > 0) || BASloggingEnabled(sModule.ModuleName, &BASfOn))

#define BAS_METHOD(NAME) static int BASfOn; BASraiiFunc ggFFF(#NAME, sModule.ModuleName, __LINE__, this, (BASfOn > 0) || BASloggingEnabled(sModule.ModuleName, &BASfOn))

void BASwriteIndent(BASsink* pSink, int Level);
#endif

#endif
