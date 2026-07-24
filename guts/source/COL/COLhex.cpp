//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLhex
// 
// Implementation
//-------------------------------------------------------
#include "COLhex.h"
#include "COLsink.h"
#include "COLutil.h"

//#include "COLtrace.h"
//COL_TRACE_INIT;

static void COLwriteChar(char Char, char** pBuffer){
   if ((Char > 30) && (Char < 127)){
      *((*pBuffer)++) = Char;
   } else {
      *((*pBuffer)++) = '.';
   }
}

static void COLwriteHex(char Char, char** pBuffer){
//   COL_FUNCTION(COLwriteHex);
   int High = (Char & 0xF0) >> 4;
   int Low = Char & 0xF;
//   COL_VAR2(High, Low);
   *((*pBuffer)++) =  High < 10 ? '0' + High : 'A' + High -10;
   *((*pBuffer)++) =  Low < 10 ? '0' + Low : 'A' + Low -10;
   *((*pBuffer)++) = ' ';
//   COL_VAR(*pBuffer);
}

static void COLpadd(char** pBuffer, int Size){
   while (Size){
      Size--;
      *((*pBuffer)++) = ' ';
   }
}

#define G "                                                      "
static const char* s_pCOLhexIndent = G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G;

// Not the most beautiful implementation
void COLhex(int Size, const void* pRawData, COLsink* pSink, int Indent){
//   COL_FUNCTION(COLhex);
   const char* pData = (const char*)pRawData;
//   COL_VAR2(Size, pData);
   char Buffer[34];
   for (int j=0; j < Size; j+=8){
      char* pBuffer = Buffer;
      int Max = COL_MIN(j+8, Size);
      for (int i=j; i < Max; i++){
         COLwriteHex(pData[i], &pBuffer);
      }
      if (j+8 > Size){
         COLpadd(&pBuffer,(j+8 - Size)*3);
      }
//      COL_VAR(pBuffer);
      int MaxHalf = COL_MIN(j+4, Size);
      for (int i=j; i < MaxHalf; i++){
         COLwriteChar(pData[i], &pBuffer);
      }
      *(pBuffer++) = ' ';
      for (int i=j+4; i < Max; i++){
         COLwriteChar(pData[i], &pBuffer);
      }
      pSink->write(s_pCOLhexIndent, Indent);
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