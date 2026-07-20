#pragma once
//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// BASargFlagPresent
// 
// Command line argument parser routines
//-------------------------------------------------------

#include "BAS/BASbase.h"

// If present it will populate the value in pValue
bool BASargFlagPresent(const BASstring& Name, BASstring* pValue, BASarray<BASstring>* pArgs);
bool BASargFindFlag(const BASstring& Name, BASarray<BASstring>* pArgs);

