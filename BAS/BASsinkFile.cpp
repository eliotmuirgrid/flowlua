//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASsinkFile
//-------------------------------------------------------

#include "BASsinkFile.h"
#include <stdio.h>

#include "BASfileImp.h"

BASsinkFile::BASsinkFile(int FileHandle) : m_FileHandle(FileHandle){}

int BASsinkFile::write(const char* pData, int Size){
   return BASwrite(m_FileHandle, pData, Size);
} 

void BASsinkFile::flush(){
   BASfsync(m_FileHandle);
} 