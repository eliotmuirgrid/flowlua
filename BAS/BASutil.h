#ifndef _BAS_UTIL_H__
#define _BAS_UTIL_H__
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASutil
//
// MAX/MIN macros etc.
//-------------------------------------------------------

#define BAS_MIN(a,b) (((a)<(b))?(a):(b))
#define BAS_MAX(a,b) (((a)>(b))?(a):(b))

void BASsleep(int Milliseconds);

#endif
