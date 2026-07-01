#ifndef _BAS_FILE_H_
#define _BAS_FILE_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASfile
// 
// Basic cross platform file API for writing and reading from files.
// This is deliberately written in a C style.
//
// See BASerror.h for function to translate the error codes in a platform independent way.
//-------------------------------------------------------

class BASstring;

#include "BAStypes.h"

class BASstring;

// Will return zero if successful, otherwise int contains errno.
int BASwriteFile(const BASstring& Name, const BASstring& Content);
int BASreadFile(const BASstring& Name, BASstring* pContent);

enum BASfileOpenMode{BASFread, BASFwrite, BASFappend, BASFrewrite};
int BASfileOpen(const char* pFileName, BASfileOpenMode Mode, int* pErrorCode);

int BASfileWrite(int FileHandle, const char* pData, int Size, int* pErrorCode);
int BASfileWrite(int FileHandle, const BASstring& Data, int* pErrorCode);
int BASfileRead (int FileHandle, void* pBuffer, int SizeOfBuffer, int* pErrorCode);
int BASfileRead (int FileHandle, BASstring* pBuffer, int* pErrorCode);

BASint64 BASfileSize(int FileHandle, int* pErrorCode);
BASint64 BASfilePosition(int FileHandle, int* pErrorCode);  // Returns -1 if the function fails.

bool BASfileFlush(int FileHandle, int* pErrorCode);  
bool BASfileClose(int FileHandle, int* pErrorCode);

#endif
