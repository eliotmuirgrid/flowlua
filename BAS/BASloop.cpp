//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASloop
// 
// Implementation
//-------------------------------------------------------
#include "BASloop.h"
#include "BASerror.h"
#include "BAStrace.h"
BAS_TRACE_INIT;

#ifdef __APPLE__

#include <time.h>
#include <sys/event.h>
#include <unistd.h>
#include <errno.h>

BASloop::BASloop(){
   BAS_METHOD(BASloop::BASloop);
   
}

void BASloop::start(){
   BAS_METHOD(BASloop::start);
   int KHandle = kqueue();
   BAS_VAR(KHandle);
   struct timespec Timeout;
   Timeout.tv_sec = 1;
   BAS_TRC("Call kevent");
   int Count = kevent(KHandle, NULL, 0, NULL, 0, &Timeout);
   if (Count == -1){
      int LastError = errno;
      BAS_VAR(BASerrorMessage(LastError));
   }
   
   BAS_VAR(Count);
   close(KHandle);
}

void BASloop::shutdown(){
   BAS_METHOD(BASloop::shutdown);
}

#endif
