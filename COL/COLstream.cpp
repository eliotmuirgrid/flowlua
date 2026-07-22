//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLstream
// 
// Stream interface like iostream
//-------------------------------------------------------

#include "COLstream.h"
#include "COLsink.h"
#include "COLsinkFile.h"
#include "COLhex.h"

#include <string.h>
#include <stdio.h>

COLstream COLout(new COLsinkFile(1), true);
COLstream COLerr(new COLsinkFile(2), true); 

COLstream::COLstream(COLsink& Sink) : m_pSink(&Sink), m_IsOwner(false){

}

COLstream::COLstream(COLsink* pSink, bool IsOwner) : m_pSink(pSink), m_IsOwner(IsOwner) {

}

void COLstream::setSink(COLsink* pSink, bool Owner) {
   if (m_IsOwner){
      delete m_pSink;
   }
   m_IsOwner = Owner;
   m_pSink = pSink;
}


COLstream::~COLstream(){
   if (m_IsOwner){
      delete m_pSink;
   }
}

COLsink* COLstream::sink(){
   return m_pSink;
}

COLstream& operator<<(COLstream& Stream, const char* pString){
   Stream.sink()->write(pString, strlen(pString));
   return Stream;
}


COLstream& operator<<(COLstream& Stream, int Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%d", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

COLstream& operator<<(COLstream& Stream, bool Value){
   if (Value){
      Stream.sink()->write("true", 4);
   } else {
      Stream.sink()->write("false", 5);
   }
   return Stream;
}

COLstream& operator<<(COLstream& Stream, long Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%ld", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

COLstream& operator<<(COLstream& Stream, unsigned int Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%d", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

COLstream& operator<<(COLstream& Stream, unsigned long Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%lu", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

COLstream& operator<<(COLstream& Stream, double Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%f", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

COLstream& operator<<(COLstream& Stream, COLint64 Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%llu", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}


COLstream& operator<<(COLstream& Stream, const void* pPointer){
   char Buffer[32];
   int Size = sprintf(Buffer, "%p", pPointer);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

COLstream& operator<<(COLstream& Stream, char Character){
   Stream.sink()->write(&Character, 1);
   return Stream;
}

COLstream& newline(COLstream& Stream) {
   Stream.sink()->write("\n", 1);
   return Stream;
}

COLstream& flush(COLstream& Stream) {
   Stream.sink()->flush();
   return Stream;
}

COLstream& operator<<(COLstream& Stream, COLstream&(*pFunc)(COLstream&)){
   return (*pFunc)(Stream);
}
