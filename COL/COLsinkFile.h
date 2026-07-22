#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLsinkFile
// 
// Sink which writes to a file descriptor.  It doesn't
// over the concern of opening or closing the file descriptor.
// 0 STDIN_FILENO	stdin	Standard input
// 1 STDOUT_FILENO	stdout	Standard output
// 2 STDERR_FILENO	stderr	Standard error
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

