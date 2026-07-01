#ifndef _BAS_VECTOR_H_
#define _BAS_VECTOR_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASvector
// 
// A resizable array.  Could be further optimized with in place operators, move constructors etc.
//-------------------------------------------------------

#include "BASstream.h"

void BAScheckBoundary(int i, int m_Size);
unsigned int BAScalculateCapacity(unsigned int v); 

template<typename VType> class BASvector {
public:
   BASvector() : m_Size(0), m_Capacity(0) { m_pItems = 0; }
   BASvector(int DesiredCapacity) : m_Size(0){
      m_Capacity = BAScalculateCapacity(DesiredCapacity);
      m_pItems = new VType[m_Capacity];
   }
   ~BASvector(){ delete []m_pItems; }
   int size() const { return m_Size; }
   void clear(){ m_Size=0; }
   void zero(){
      m_Size=0;
      delete[]m_pItems;
      m_Capacity=0;
   }
   VType& push(const VType& Value){
      checkCapacity(m_Size+1);
      m_pItems[m_Size] = Value;
      m_Size++;
      return m_pItems[m_Size];
   }
   void resize(int NewSize) { checkCapacity(NewSize+1); m_Size=NewSize; }
   VType& operator[]      (int i)       { BAScheckBoundary(i, m_Size); return m_pItems[i]; }
   const VType& operator[](int i) const { BAScheckBoundary(i, m_Size); return m_pItems[i]; }
   void checkCapacity(int DesiredCapacity){
      if (DesiredCapacity <= m_Capacity){ return; }
      int NewCapacity = BAScalculateCapacity(DesiredCapacity);
      VType* pNewSpace = new VType[NewCapacity];
      VType* pEnd = pNewSpace+m_Size;
      for (VType* i=pNewSpace,*j=m_pItems; i!= pEnd; ++i, ++j){ *i = *j;}
      delete []m_pItems;
      m_Capacity = NewCapacity;
      m_pItems = pNewSpace;   
   } 
   const VType* cbegin() const { return m_pItems; }
   const VType* cend()   const { return m_pItems + m_Size; }
   VType* begin() { return m_pItems; }
   VType* end()   { return m_pItems + m_Size; }
   void printOn(BASstream& Stream) const {
      Stream << "Vector  Size=" << m_Size <<", Capacity=" << m_Capacity;
      int Count=0;
      for (auto i = cbegin(); i != cend(); ++i, ++Count){
         Stream << newline << " " << Count << " " << *i;
      }
   }
private:
   VType*  m_pItems;
   int     m_Size;
   int     m_Capacity;
};

template<typename VType>
BASstream operator<<(BASstream& Stream, const BASvector<VType>& Vector){
   Vector.printOn(Stream);
   return Stream;
}
#endif