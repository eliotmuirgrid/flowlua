#pragma once
//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// BASarg
// 
// Command line argument parser routines
//-------------------------------------------------------

class BASstring;
template<class T> class BASarraySafe;

void BASarrayCopy(int argc, const char** pValues, BASarraySafe<BASstring>* pOut); 


