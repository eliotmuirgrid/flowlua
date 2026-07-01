#ifndef _BAS_TIMESTAMP_H_
#define _BAS_TIMESTAMP_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BAStimestamp
// 
// Core timestamp
//-------------------------------------------------------

#include "BAStypes.h"
#include <time.h>

class BAStimestamp{
public:
   time_t Seconds;
   BASint64 Microseconds;
};

// Think for a POD this is neglible overhead.
BAStimestamp BAScurrentTime();

#endif
