#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLstream
// 
// Stream inteface like iostream
//-------------------------------------------------------

class COLsink;

#include "COLtypes.h"

class COLstream{
public:
   COLstream(COLsink& Sink);  // does not take ownership
   COLstream(COLsink* pSink, bool IsOwner);
   ~COLstream();

   COLsink* sink();

   void setSink(COLsink* pSink, bool Owner=true);
   
private:
   bool m_IsOwner;
   COLsink* m_pSink; 
};

COLstream& operator<<(COLstream& Stream, const char* pString);
COLstream& operator<<(COLstream& Stream, int Number);
COLstream& operator<<(COLstream& Stream, bool Value);
COLstream& operator<<(COLstream& Stream, long Value);
COLstream& operator<<(COLstream& Stream, const void* pObject);
COLstream& operator<<(COLstream& Stream, char Character);
COLstream& operator<<(COLstream& Stream, unsigned int Number);
COLstream& operator<<(COLstream& Stream, unsigned long Number);
COLstream& operator<<(COLstream& Stream, COLint64 Number);
COLstream& operator<<(COLstream& Stream, double Number);


COLstream& operator<<(COLstream& Stream, COLstream&(*pFunc)(COLstream&));

COLstream& flush(COLstream& Stream);
COLstream& newline(COLstream& Stream);

extern COLstream COLout; 
extern COLstream COLerr;

