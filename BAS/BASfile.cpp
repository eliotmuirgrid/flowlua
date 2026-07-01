//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASfile
// 
// Implementation
//-------------------------------------------------------

#include "BASfile.h"
#include "BASfileImp.h"
#include "BASstring.h"
#include "BAStrace.h"
BAS_TRACE_INIT;

#include <errno.h>
#include <fcntl.h>

#ifdef _WIN32
  #include <windows.h>
  #include <sys/stat.h>
  #include <io.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

int BASwriteFile(const BASstring& Name, const BASstring& Content){
   BAS_FUNCTION(BASwriteFile);
   int ErrorCode=0;
   int F = BASfileOpen(Name.data(), BASFrewrite, &ErrorCode);
   if (F == -1) { return ErrorCode; }
   int AmountWritten = BASfileWrite(F, Content, &ErrorCode);
   BAS_VAR(AmountWritten);
   BASfileClose(F, &ErrorCode);
   return ErrorCode;
}

int BASreadFile(const BASstring& Name, BASstring* pContent){
   BAS_FUNCTION(BASreadFile);
   BAS_VAR(Name);
   int ErrorCode=0;
   int F = BASfileOpen(Name.data(), BASFread, &ErrorCode);
   if (F == -1) { return ErrorCode; }
   BASfileRead(F, pContent, &ErrorCode);
   BASfileClose(F, &ErrorCode);
   BAS_VAR(ErrorCode);
   BAS_HEX("Content", pContent->data(), pContent->size());
   return ErrorCode;
}

int BASfileOpen(const char* FileName, BASfileOpenMode Mode, int* pErrorCode){
   BAS_FUNCTION(BASfileOpen);
   BAS_VAR2(FileName, Mode);
   int FileHandle = -1;
   switch (Mode){
   case BASFread:    FileHandle=BASopen(FileName, O_BINARY | O_RDONLY);                                        break;
   case BASFwrite:   FileHandle=BASopen(FileName, O_BINARY | O_RDWR                     , S_IRUSR | S_IWUSR); break;
   case BASFappend:  FileHandle=BASopen(FileName, O_BINARY | O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); break;
   case BASFrewrite: FileHandle=BASopen(FileName, O_BINARY | O_RDWR | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR); break;
   }
   if (FileHandle == -1) { *pErrorCode = errno; BAS_VAR(*pErrorCode); }
   BAS_VAR(FileHandle);
   return FileHandle;  
}

int BASfileWrite(int FileHandle, const char* pData, int Size, int* pErrorCode){
   BAS_FUNCTION(BASfileWrite);
   int AmountWritten = BASwrite(FileHandle, pData, Size);
   if (AmountWritten == -1){ *pErrorCode = errno; }
   BAS_VAR(AmountWritten);
   return AmountWritten;
}

int BASfileWrite(int FileHandle, const BASstring& Data, int* pErrorCode){
   BAS_FUNCTION(BASfileWrite - BASstring);
   return BASfileWrite(FileHandle, Data.data(), Data.size(), pErrorCode);
}

int BASfileRead (int FileHandle, void* pBuffer, int SizeOfBuffer, int* pErrorCode){
   BAS_FUNCTION(BASfileRead);
   int AmountRead = ::read(FileHandle, pBuffer, SizeOfBuffer);
   if (AmountRead == -1){ *pErrorCode = errno; }
   BAS_VAR(AmountRead);
   return AmountRead;
}

int BASfileRead(int FileHandle, BASstring* pBuffer, int* pErrorCode){
   BAS_FUNCTION(BASfileRead - BASstring);
   int FileSize = BASfileSize(FileHandle, pErrorCode);
   if (FileSize == -1){
      return -1;
   }
   pBuffer->setCapacity(FileSize);
   pBuffer->setSize(pBuffer->capacity());
   int AmountRead = BASfileRead(FileHandle, pBuffer->data(), pBuffer->capacity(), pErrorCode);
   return AmountRead;
}

bool BASfileFlush(int FileHandle, int* pErrorCode){
   BAS_FUNCTION(BASfileFlush);
   int Result = BASfsync(FileHandle);
   if (-1 == Result){
      *pErrorCode = errno;
      return false;
   }
   return true;
}

bool BASfileClose(int FileHandle, int* pErrorCode){
   BAS_FUNCTION(BASfileClose);
   if (FileHandle != -1){
      int Result = ::close(FileHandle);
      if (-1 == Result){
         *pErrorCode = errno;
         return false;
      } else {
         return true;
      }
   }
   return false;
}

BASint64 BASfileSize(int FileHandle, int* pErrorCode) {
   BAS_FUNCTION(BASfileSize);
   BASint64 Position = BASseek(FileHandle, 0, SEEK_CUR);
   BASint64 Size = BASseek(FileHandle, 0, SEEK_END);
   int Result = BASseek(FileHandle, Position, SEEK_SET);
   if (Result == -1){
      *pErrorCode = errno;
   }
   BAS_VAR(Size);
   return Size;
}

BASint64 BASfilePosition(int FileHandle, int* pErrorCode){
   BAS_FUNCTION(BASfilePosition);
   BASint64 Position = BASseek(FileHandle, 0, SEEK_CUR);
   if (Position == -1){
      *pErrorCode = errno;
   }
   return Position;
} 