#ifndef _BAS_SINK_FILE_
#define _BAS_SINK_FILE_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASsinkFile
// 
// Sink which writes to a file descriptor.  It doesn't
// over the concern of opening or closing the file descriptor.
//-------------------------------------------------------

#include "BASsink.h"

class BASsinkFile : public BASsink{
public:
   BASsinkFile(int FileHandle);  
   virtual ~BASsinkFile(){}  

   virtual int write(const char* pData, int Size); 
   virtual void flush();
private:
   int m_FileHandle;
};

#endif
