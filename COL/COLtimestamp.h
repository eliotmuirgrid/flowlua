#ifndef _COL_TIMESTAMP_H_
#define _COL_TIMESTAMP_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLtimestamp
// 
// Core timestamp
//-------------------------------------------------------

#include "COLtypes.h"
#include <time.h>

class COLtimestamp{
public:
   time_t Seconds;
   COLint64 Microseconds;
};

// Think for a POD this is neglible overhead.
COLtimestamp COLcurrentTime();

#endif
