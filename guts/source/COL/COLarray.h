#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLarray
// 
// Use this instead of raw arrays.  This stops you from
// writing insecure code and screwing with your users.
//
// You shouldn't screw with your users or members of your
// close family unless they are your spouse.  All these
// things should be pretty obvious.  And yet not obvious
// for a significant percentage of the population.
//
// Let's change that!
//-------------------------------------------------------

#include "COLstream.h"

void COLcheckBoundary(int i, int m_Size);
unsigned int COLcalculateCapacity(unsigned int v); 

template<typename VType> class COLarray {
public:
   COLarray() : m_Size(0), m_Capacity(0) { m_pItems = 0; }
   COLarray(int DesiredCapacity) : m_Size(0){
      m_Capacity = COLcalculateCapacity(DesiredCapacity);
      m_pItems = new VType[m_Capacity];
   }
   ~COLarray(){ delete []m_pItems; }
   int size() const { return m_Size; }
   void clear(){ m_Size=0; }
   void remove(int Index) {
      COLcheckBoundary(Index, m_Size);
      for (int i = Index; i < m_Size - 1; ++i) {
         m_pItems[i] = m_pItems[i + 1];
      }
      --m_Size;
      m_pItems[m_Size].~VType();
   }
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
   void pop() {
      --m_Size;
      m_pItems[m_Size].~VType();
   }
   void resize(int NewSize) { checkCapacity(NewSize+1); m_Size=NewSize; }
   VType& operator[]      (int i)       { COLcheckBoundary(i, m_Size); return m_pItems[i]; }
   const VType& operator[](int i) const { COLcheckBoundary(i, m_Size); return m_pItems[i]; }
   void checkCapacity(int DesiredCapacity){
      if (DesiredCapacity <= m_Capacity){ return; }
      int NewCapacity = COLcalculateCapacity(DesiredCapacity);
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
   void printOn(COLstream& Stream) const {
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
COLstream& operator<<(COLstream& Stream, const COLarray<VType>& Vector){
   Vector.printOn(Stream);
   return Stream;
}

