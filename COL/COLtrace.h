#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLtrace
// 
// Tracing framework
//
// To use it do:
// #include <COL/COLtrace.h>
// COL_TRACE_INIT;
//
// Then you can use:
//  COL_TRC("This is tracing.");   // Traces to output
//  COL_FUNCTION(myFunctionName);  // Gives entry into a function and out of it
//  COL_METHOD(Foo::MyMethod);     // Gives entry into a method and shows the this pointer and out of it
//  COL_VAR(AVariable);            // Gives name=value of a variable - use COL_VAR2, COL_VAR3 etc. to display 2, 3 variables etc.
//
// Tracing is enabled through the COLargParser which can be used to control the command line.  Or you can just
// call COLsetTracePattern directly with a glob expression that is used to match what files to display tracing from.
//-------------------------------------------------------

#include "COLstream.h"
#include <stdio.h>
#include "COLstring.h"
#include "COLsinkString.h"
#include <string.h>

// Call this to activate tracing - see COLarg too
void COLtrace(const char* Pattern);
void COLsetTraceFile(const char* FileName);
void COLheader();

//#define COL_TRACE_OFF  // uncomment this to turn tracing off.
#ifdef COL_TRACE_OFF
#define COL_TRC(A)
#define COL_VAR(A)
#define COL_VAR2(A,B)
#define COL_VAR3(A,B,C)
#define COL_VAR4(A,B,C,D)
#define COL_FUNCTION(NAME)
#define COL_METHOD(NAME)
#define COL_HEX(LABEL, BUFFER, SIZE)
#define COL_TRACE_INIT
#else

extern COLstream COLlog;

void COLtimeStamp(const char* pModule, COLstream& Stream);
void COLmilliSeconds();
//extern COLmutex s_LogMutex;

class COLmodule{
public:
   COLmodule(const char* pFileName);
   char ModuleName[20];
};

#ifdef _WIN32
   #define __FILENAME__ (strrchr("\\" __FILE__, '\\') + 1)
#else
   #define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)
#endif

#define COL_TRACE_INIT static COLmodule sModule(__FILENAME__);

bool COLloggingEnabled(const char* ModuleName, int* pResult);

class COLlogStream : public COLstream{
public:
   COLstring m_String;
   COLlogStream() : COLstream(new COLsinkString(&m_String), true) {}
   ~COLlogStream() {}
private:
};

#define COL_TRC(X)\
   do {\
      static int COLdoLog;\
      if (COLdoLog > 0 || (COLdoLog == 0 && COLloggingEnabled(sModule.ModuleName, &COLdoLog)) ){\
	 COLtimeStamp(sModule.ModuleName, COLlog);\
         COLlog << X << newline;\
      }\
   } while(0)

void COLhexTrace(int Size, const void* pBuffer, COLstream& Stream);

#define COL_HEX(LABEL, BUFFER, SIZE)\
   do {\
      static int COLdoLog;\
      if (COLdoLog > 0 || (COLdoLog == 0 && COLloggingEnabled(sModule.ModuleName, &COLdoLog)) ){\
         COLlogStream LogStream;\
         LogStream << LABEL; COLhexTrace(SIZE, BUFFER, LogStream); LogStream << newline;\
         COLtimeStamp(sModule.ModuleName, COLlog);\
         COLlog << LogStream.m_String;\
      }\
   } while(0)

#define COL_VAR(A)        COL_TRC(#A << " = " << A);
#define COL_VAR2(A,B)     COL_TRC(#A << " = " << A << ", " << #B << " = " << B);
#define COL_VAR3(A,B,C)   COL_TRC(#A << " = " << A << ", " << #B << " = " << B << ", " << #C << " = " << C);
#define COL_VAR4(A,B,C,D) COL_TRC(#A << " = " << A << ", " << #B << " = " << B << ", " << #C << " = " << C << ", " << #D << " = " << D);

class COLraiiFunc{
public:
    COLraiiFunc(const char* Name, const char* pModule, int Line, bool Trace);
    COLraiiFunc(const char* Name, const char* pModule, int Line, const void* pThis, bool Trace);
    
    ~COLraiiFunc();
private:
   bool m_Trace;
   const char* m_pName;
   const char* m_pModule; 
};


#define COL_FUNCTION(NAME) static int COLfOn; COLraiiFunc ggFFF(#NAME, sModule.ModuleName, __LINE__,(COLfOn > 0) || COLloggingEnabled(sModule.ModuleName, &COLfOn))

#define COL_METHOD(NAME) static int COLfOn; COLraiiFunc ggFFF(#NAME, sModule.ModuleName, __LINE__, this, (COLfOn > 0) || COLloggingEnabled(sModule.ModuleName, &COLfOn))

void COLwriteIndent(COLsink* pSink, int Level);
#endif

