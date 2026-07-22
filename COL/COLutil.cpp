//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLutil
//
// Implementation
//-------------------------------------------------------

#include "COLutil.h"
#include "COLtrace.h"
COL_TRACE_INIT;

#ifdef _WIN32 
   #include <windows.h>
#else
    #include <time.h>
#endif  

void COLsleep(int Milliseconds){
   COL_FUNCTION(COLsleep);
#ifdef _WIN32
   Sleep(Milliseconds);
#else
   timespec I;
   I.tv_sec = Milliseconds / 1000;
   I.tv_nsec = (Milliseconds % 1000) * 1000000;
   nanosleep(&I, NULL);
#endif
}

