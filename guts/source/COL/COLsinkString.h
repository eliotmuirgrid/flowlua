#ifndef _COL_SINK_STRING_H_
#define _COL_SINK_STRING_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLsinkString
// 
// Sink which writes to a string - which is doesn't own.
//
// Why not implement this directly on COLstring?  Because it would
// mean that every COLstring will need a virtual vtable pointer
// which increases the size of every string by 8 bytes on a 64 bit machine.
//-------------------------------------------------------

#include "COLsink.h"
#include "COLstring.h"

class COLsinkString : public COLsink{
public:
   COLsinkString(COLstring* pString) : m_pString(pString) {}
   virtual ~COLsinkString() {}

   virtual int write(const char* pData, int Size) { m_pString->append(pData, Size); return Size; }
   virtual void flush() {}
private:
   COLstring* m_pString;
};

#endif
