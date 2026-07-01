//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BAStimestamp
// 
// Implementation
//-------------------------------------------------------
#include "BAStimestamp.h"

#include "BASstream.h"

#ifdef _WIN32
   #include <windows.h>
   BAStimestamp BAScurrentTime(){
      SYSTEMTIME st;
      GetSystemTime(&st);
      FILETIME ftime;
      SystemTimeToFileTime(&st, &ftime);
      //GetSystemTimeAsFileTime(&ftime);
      const BASint64 DELTA_EPOCH_IN_USEC = 11644473600000000 /*LL*/;
      BASint64 v = ftime.dwHighDateTime;
      v = v << 32;
      v |= ftime.dwLowDateTime;
      v /= 10;   // convert from 100 nanosecond periods to microseconds
      v -= DELTA_EPOCH_IN_USEC;  // convert to Unix epoch
      BAStimestamp Timestamp;
      Timestamp.Seconds =      v / 1000000;
      Timestamp.Microseconds = v % 1000000;
      return Timestamp;
   }
#else
   #include <time.h>
   BAStimestamp BAScurrentTime(){
      struct timespec spec;
      clock_gettime(CLOCK_REALTIME, &spec);
      BAStimestamp Timestamp;
      Timestamp.Seconds = spec.tv_sec;
      Timestamp.Microseconds = spec.tv_nsec / 1000;
      return Timestamp;
   }
#endif