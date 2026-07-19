#pragma once
//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// BASargFlagPresent
// 
// Command line argument parser routines
//-------------------------------------------------------

class BASstring;
template <class T> class BASarray;

// If present it will populate the value in pValue
bool BASargFlagPresent(const BASstring& Name, BASstring* pValue, BASarray<BASstring>* pArgs);

