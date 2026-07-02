#pragma once
//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// BASarg
// 
// Command line argument parser routines
//-------------------------------------------------------

class BASstring;

// If present it will populate the value in pValue
bool BASargFlagPresent(const BASstring& Name, BASstring* pValue, int argc, char* argv[]);

