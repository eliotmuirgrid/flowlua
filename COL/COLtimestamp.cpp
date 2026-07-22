//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLtimestamp
// 
// Implementation
//-------------------------------------------------------
#include "COLtimestamp.h"

#include <time.h>

COLtimestamp COLcurrentTime() {
   struct timespec Time;
   clock_gettime(CLOCK_REALTIME, &Time);

   COLtimestamp Timestamp;
   Timestamp.Seconds      = Time.tv_sec;
   Timestamp.Microseconds = Time.tv_nsec / 1000;
   return Timestamp;
}
