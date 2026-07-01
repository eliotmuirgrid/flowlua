//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BAShex
// 
// Implementation
//-------------------------------------------------------
#include "BAShex.h"
#include "BASsink.h"
#include "BASutil.h"

//#include "BAStrace.h"
//BAS_TRACE_INIT;

static void BASwriteChar(char Char, char** pBuffer){
   if ((Char > 30) && (Char < 127)){
      *((*pBuffer)++) = Char;
   } else {
      *((*pBuffer)++) = '.';
   }
}

static void BASwriteHex(char Char, char** pBuffer){
//   BAS_FUNCTION(BASwriteHex);
   int High = (Char & 0xF0) >> 4;
   int Low = Char & 0xF;
//   BAS_VAR2(High, Low);
   *((*pBuffer)++) =  High < 10 ? '0' + High : 'A' + High -10;
   *((*pBuffer)++) =  Low < 10 ? '0' + Low : 'A' + Low -10;
   *((*pBuffer)++) = ' ';
//   BAS_VAR(*pBuffer);
}

static void BASpadd(char** pBuffer, int Size){
   while (Size){
      Size--;
      *((*pBuffer)++) = ' ';
   }
}

#define G "                                                      "
static const char* s_pBAShexIndent = G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G;

// Not the most beautiful implementation
void BAShex(int Size, const void* pRawData, BASsink* pSink, int Indent){
//   BAS_FUNCTION(BAShex);
   const char* pData = (const char*)pRawData;
//   BAS_VAR2(Size, pData);
   char Buffer[34];
   for (int j=0; j < Size; j+=8){
      char* pBuffer = Buffer;
      int Max = BAS_MIN(j+8, Size);
      for (int i=j; i < Max; i++){
         BASwriteHex(pData[i], &pBuffer);
      }
      if (j+8 > Size){
         BASpadd(&pBuffer,(j+8 - Size)*3);
      }
//      BAS_VAR(pBuffer);
      int MaxHalf = BAS_MIN(j+4, Size);
      for (int i=j; i < MaxHalf; i++){
         BASwriteChar(pData[i], &pBuffer);
      }
      *(pBuffer++) = ' ';
      for (int i=j+4; i < Max; i++){
         BASwriteChar(pData[i], &pBuffer);
      }
      pSink->write(s_pBAShexIndent, Indent);
      if (j+8 > Size) {
         pSink->write(Buffer, 25 + Size-j);
      } else {
         pSink->write(Buffer, 33);
      }
      if (j+8 < Size){
         pSink->write("\n", 1);
      }
   }
}