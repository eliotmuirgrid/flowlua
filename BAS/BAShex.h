#ifndef _BAS_HEX_H_
#define _BAS_HEX_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BAShex
// 
// Function to convert a block of data into a hex block.
//-------------------------------------------------------

class BASsink;

void BAShex(int Size, const void* pData, BASsink* pSink, int Indent);

#endif