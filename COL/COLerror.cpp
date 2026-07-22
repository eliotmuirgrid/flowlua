//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLerror
// 
// Implementation
//-------------------------------------------------------

#include "COLerror.h"
#include "COLstring.h"
#include "COLtrace.h"
COL_TRACE_INIT;

#include <errno.h>

#ifdef _WIN32
#include <windows.h>
COLstring COLerrorMessage(int ErrorCode){
   char* MessageBuffer;
   DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM;
   ::FormatMessage(dwFormatFlags, NULL, 
      ErrorCode,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
      (char*) &MessageBuffer, 0, NULL);
   COLstring Result = MessageBuffer;
   LocalFree(MessageBuffer);
   COL_VAR2(ErrorCode, Result);
   return Result;
}
#else
COLstring COLerrorMessage(int ErrorCode){
   COL_FUNCTION(COLerrorMessage);
   COLstring Result;
   Result.setCapacity(255);
   strerror_r(ErrorCode, Result.data(), Result.capacity());
   Result.setSize(strlen(Result.data()));
   COL_VAR2(ErrorCode, Result);
   return Result;
}
#endif
