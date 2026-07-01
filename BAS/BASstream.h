#ifndef _BAS_STREAM_H_
#define _BAS_STREAM_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASstream
// 
// Stream inteface like iostream
//-------------------------------------------------------

class BASsink;

#include "BAStypes.h"

class BASstream{
public:
   BASstream(BASsink& Sink);  // does not take ownership
   BASstream(BASsink* pSink, bool IsOwner);
   ~BASstream();

   BASsink* sink();

   void setSink(BASsink* pSink, bool Owner=true);
   
private:
   bool m_IsOwner;
   BASsink* m_pSink; 
};

BASstream& operator<<(BASstream& Stream, const char* pString);
BASstream& operator<<(BASstream& Stream, int Number);
BASstream& operator<<(BASstream& Stream, bool Value);
BASstream& operator<<(BASstream& Stream, long Value);
BASstream& operator<<(BASstream& Stream, const void* pObject);
BASstream& operator<<(BASstream& Stream, char Character);
BASstream& operator<<(BASstream& Stream, unsigned int Number);
BASstream& operator<<(BASstream& Stream, unsigned long Number);
BASstream& operator<<(BASstream& Stream, BASint64 Number);
BASstream& operator<<(BASstream& Stream, double Number);


BASstream& operator<<(BASstream& Stream, BASstream&(*pFunc)(BASstream&));

BASstream& flush(BASstream& Stream);
BASstream& newline(BASstream& Stream);

extern BASstream BASout; 
extern BASstream BASerr;

#endif
