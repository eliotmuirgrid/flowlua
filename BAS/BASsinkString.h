#ifndef _BAS_SINK_STRING_H_
#define _BAS_SINK_STRING_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASsinkString
// 
// Sink which writes to a string - which is doesn't own.
//
// Why not implement this directly on BASstring?  Because it would
// mean that every BASstring will need a virtual vtable pointer
// which increases the size of every string by 8 bytes on a 64 bit machine.
//-------------------------------------------------------

#include "BASsink.h"
#include "BASstring.h"

class BASsinkString : public BASsink{
public:
   BASsinkString(BASstring* pString) : m_pString(pString) {}
   virtual ~BASsinkString() {}

   virtual int write(const char* pData, int Size) { m_pString->append(pData, Size); return Size; }
   virtual void flush() {}
private:
   BASstring* m_pString;
};

#endif
