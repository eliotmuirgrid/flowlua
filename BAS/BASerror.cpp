//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASerror
// 
// Implementation
//-------------------------------------------------------

#include "BASerror.h"
#include "BASstring.h"
#include "BAStrace.h"
BAS_TRACE_INIT;

#include <errno.h>

#ifdef _WIN32
#include <windows.h>
BASstring BASerrorMessage(int ErrorCode){
   char* MessageBuffer;
   DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM;
   ::FormatMessage(dwFormatFlags, NULL, 
      ErrorCode,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
      (char*) &MessageBuffer, 0, NULL);
   BASstring Result = MessageBuffer;
   LocalFree(MessageBuffer);
   BAS_VAR2(ErrorCode, Result);
   return Result;
}
#else
BASstring BASerrorMessage(int ErrorCode){
   BAS_FUNCTION(BASerrorMessage);
   BASstring Result;
   Result.setCapacity(255);
   strerror_r(ErrorCode, Result.data(), Result.capacity());
   Result.setSize(strlen(Result.data()));
   BAS_VAR2(ErrorCode, Result);
   return Result;
}
#endif
