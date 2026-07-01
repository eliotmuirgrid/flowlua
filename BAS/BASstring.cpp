//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASstring
// 
// Implementation
//-------------------------------------------------------
#include "BASstring.h"
#include "BASstream.h"
#include "BASsink.h"

#include <string.h>

BASstring::BASstring(){
   init();
}

BASstring::BASstring(const char* pString){
   init();
   append(pString, strlen(pString));
}

BASstring::BASstring(const char* pString, int Size){
   init();
   append(pString, Size);
}

BASstring::BASstring(const BASstring& Orig){
   init();
   append(Orig.data(), Orig.size());
}

BASstring& BASstring::operator=(const char* pString){
   m_Size = 0;
   append(pString, strlen(pString));
   return *this;
}

// TODO - could optimize more for small strings.
BASstring& BASstring::operator=(const BASstring& Orig){
   m_Size = 0;
   append(Orig.data(), Orig.size());
   return *this;
}

BASstring::~BASstring(){
   if (m_Capacity >= sizeof(m_pData.ShortBuffer)){
      delete []m_pData.pHeap;
   } 
}

BASstring& BASstring::append(const char* pData){
   return append(pData, strlen(pData));
}

BASstring& BASstring::append(const char* pData, int AddSize){
   if (m_Size + AddSize > m_Capacity){
      setCapacity(BASupperPowerOfTwo(m_Size+AddSize+1));
   }
   strncpy((data() + m_Size), pData, AddSize);
   m_Size += AddSize;
   data()[m_Size] = 0;  // NULL terminate
   return *this;   
}

// Short --> Short
// Short --> Long
// Long --> Short
// Long --> Long
void BASstring::setCapacity(int NewCapacity){
   if (m_Capacity < sizeof(m_pData.ShortBuffer)){
      if (NewCapacity < sizeof(m_pData.ShortBuffer)){
         m_Capacity = NewCapacity;
      } else {
         char* pNewHeap = new char[NewCapacity];
         strncpy(pNewHeap, data(), m_Size+1);
         m_Capacity = NewCapacity;
         m_pData.pHeap = pNewHeap;
      }
      return;
   }
   // Coming from Long
   if (NewCapacity < sizeof(m_pData.ShortBuffer)){
      char* pOldHeap = m_pData.pHeap;
      m_Capacity = NewCapacity;
      strncpy(m_pData.ShortBuffer, pOldHeap, m_Size+1);
      delete pOldHeap;
   } else {
      char* pOldHeap = m_pData.pHeap;
      char* pNewHeap = new char[NewCapacity];
      strncpy(pNewHeap, pOldHeap, m_Size+1);
      m_Capacity = NewCapacity;
      m_pData.pHeap = pNewHeap;
      delete pOldHeap;
   }
}

BASstring& BASstring::operator+=(const char* pData){
   return append(pData, strlen(pData));
}

BASstring& BASstring::operator+=(const BASstring& Orig){
   return append(Orig.data(), Orig.size());
}

int BASstring::compare(const BASstring& Lhs) const { 
   return strcmp(data(), Lhs.data());
}

bool BASstring::operator!=(const char* pData) const{
   return strcmp(data(), pData) != 0;
}

bool BASstring::operator==(const BASstring& Rhs) const{
   if (size() != Rhs.size()){
      return false;
   }
   return memcmp(data(), Rhs.data(), size()) == 0; 
}

bool BASstring::operator!=(const BASstring& Rhs) const{
   return !operator==(Rhs);
}

BASstring operator+(const BASstring& Lhs, const char*   pRhs  ) { BASstring X(Lhs); X+= pRhs; return X;}
BASstring operator+(const BASstring& Lhs, const BASstring& Rhs) { BASstring X(Lhs); X+= Rhs; return X;}

BASstream& operator<<(BASstream& Stream, const BASstring& String){
   Stream.sink()->write(String.data(), String.size());
   return Stream;
}

unsigned int BASupperPowerOfTwo(unsigned int v){
   v--;
   v |= v >> 1;
   v |= v >> 2;
   v |= v >> 4;
   v |= v >> 8;
   v |= v >> 16;
   v++;
   return v;
}
