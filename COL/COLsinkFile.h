#ifndef _COL_SINK_FILE_
#define _COL_SINK_FILE_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLsinkFile
// 
// Sink which writes to a file descriptor.  It doesn't
// over the concern of opening or closing the file descriptor.
//-------------------------------------------------------

#include "COLsink.h"

class COLsinkFile : public COLsink{
public:
   COLsinkFile(int FileHandle);  
   virtual ~COLsinkFile(){}  

   virtual int write(const char* pData, int Size); 
   virtual void flush();
private:
   int m_FileHandle;
};

#endif
