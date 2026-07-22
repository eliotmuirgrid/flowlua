//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BAStimestamp
// 
// Implementation
//-------------------------------------------------------
#include "BAStimestamp.h"

#include <time.h>

BAStimestamp BAScurrentTime() {
   struct timespec Time;
   clock_gettime(CLOCK_REALTIME, &Time);

   BAStimestamp Timestamp;
   Timestamp.Seconds      = Time.tv_sec;
   Timestamp.Microseconds = Time.tv_nsec / 1000;
   return Timestamp;
}
