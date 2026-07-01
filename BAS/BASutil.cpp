//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASutil
//
// Implementation
//-------------------------------------------------------

#include "BASutil.h"
#include "BAStrace.h"
BAS_TRACE_INIT;

#ifdef _WIN32 
   #include <windows.h>
#else
    #include <time.h>
#endif  

void BASsleep(int Milliseconds){
   BAS_FUNCTION(BASsleep);
#ifdef _WIN32
   Sleep(Milliseconds);
#else
   timespec I;
   I.tv_sec = Milliseconds / 1000;
   I.tv_nsec = (Milliseconds % 1000) * 1000000;
   nanosleep(&I, NULL);
#endif
}

