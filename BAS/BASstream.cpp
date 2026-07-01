//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASstream
// 
// Stream interface like iostream
//-------------------------------------------------------

#include "BASstream.h"
#include "BASsink.h"
#include "BASsinkFile.h"
#include "BAShex.h"

#include <string.h>
#include <stdio.h>

BASstream BASout(new BASsinkFile(1), true);
BASstream BASerr(new BASsinkFile(2), true); 

BASstream::BASstream(BASsink& Sink) : m_pSink(&Sink), m_IsOwner(false){

}

BASstream::BASstream(BASsink* pSink, bool IsOwner) : m_pSink(pSink), m_IsOwner(IsOwner) {

}

void BASstream::setSink(BASsink* pSink, bool Owner) {
   if (m_IsOwner){
      delete m_pSink;
   }
   m_IsOwner = Owner;
   m_pSink = pSink;
}


BASstream::~BASstream(){
   if (m_IsOwner){
      delete m_pSink;
   }
}

BASsink* BASstream::sink(){
   return m_pSink;
}

BASstream& operator<<(BASstream& Stream, const char* pString){
   Stream.sink()->write(pString, strlen(pString));
   return Stream;
}


BASstream& operator<<(BASstream& Stream, int Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%d", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

BASstream& operator<<(BASstream& Stream, bool Value){
   if (Value){
      Stream.sink()->write("true", 4);
   } else {
      Stream.sink()->write("false", 5);
   }
   return Stream;
}

BASstream& operator<<(BASstream& Stream, long Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%ld", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

BASstream& operator<<(BASstream& Stream, unsigned int Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%d", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

BASstream& operator<<(BASstream& Stream, unsigned long Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%lu", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

BASstream& operator<<(BASstream& Stream, double Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%f", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

BASstream& operator<<(BASstream& Stream, BASint64 Number){
   char Buffer[32];
   int Size = sprintf(Buffer, "%llu", Number);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}


BASstream& operator<<(BASstream& Stream, const void* pPointer){
   char Buffer[32];
   int Size = sprintf(Buffer, "%p", pPointer);
   Stream.sink()->write(Buffer, Size);
   return Stream;
}

BASstream& operator<<(BASstream& Stream, char Character){
   Stream.sink()->write(&Character, 1);
   return Stream;
}

BASstream& newline(BASstream& Stream) {
   Stream.sink()->write("\n", 1);
   return Stream;
}

BASstream& flush(BASstream& Stream) {
   Stream.sink()->flush();
   return Stream;
}

BASstream& operator<<(BASstream& Stream, BASstream&(*pFunc)(BASstream&)){
   return (*pFunc)(Stream);
}