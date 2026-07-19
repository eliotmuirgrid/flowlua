#pragma once
//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// BASargFlagPresent
// 
// Command line argument parser routines
//-------------------------------------------------------

class BASstring;
template <class T> class BASarraySafe;

// If present it will populate the value in pValue
bool BASargFlagPresent(const BASstring& Name, BASstring* pValue, BASarraySafe<BASstring>* pArgs);

