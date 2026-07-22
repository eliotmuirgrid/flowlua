#ifndef _COL_HEX_H_
#define _COL_HEX_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLhex
// 
// Function to convert a block of data into a hex block.
//-------------------------------------------------------

class COLsink;

void COLhex(int Size, const void* pData, COLsink* pSink, int Indent);

#endif