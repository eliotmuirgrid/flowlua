#ifndef _COL_UTIL_H__
#define _COL_UTIL_H__
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLutil
//
// MAX/MIN macros etc.
//-------------------------------------------------------

#define COL_MIN(a,b) (((a)<(b))?(a):(b))
#define COL_MAX(a,b) (((a)>(b))?(a):(b))

void COLsleep(int Milliseconds);

#endif
