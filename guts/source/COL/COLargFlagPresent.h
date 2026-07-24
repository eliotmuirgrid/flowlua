#pragma once
//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// COLargFlagPresent
// 
// Command line argument parser routines
//-------------------------------------------------------

#include "COL/COLbase.h"

// If present it will populate the value in pValue
bool COLargFlagPresent(const COLstring& Name, COLstring* pValue, COLarray<COLstring>* pArgs);
bool COLargFindFlag(const COLstring& Name, COLarray<COLstring>* pArgs);

