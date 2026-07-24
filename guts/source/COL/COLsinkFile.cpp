//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLsinkFile
//-------------------------------------------------------

#include "COLsinkFile.h"
#include <stdio.h>

#include "COLfileImp.h"

COLsinkFile::COLsinkFile(int FileHandle) : m_FileHandle(FileHandle){}

int COLsinkFile::write(const char* pData, int Size){
   return COLwrite(m_FileHandle, pData, Size);
} 

void COLsinkFile::flush(){
   COLfsync(m_FileHandle);
} 