#pragma once
//-------------------------------------------------------
// Copyright (C) Forever.  All Rights Reserved
//
// BASstring
// 
// Core string class.  Not reference counted.
//-------------------------------------------------------

class BASstream;

class BASstring{
public:
   BASstring();
   BASstring(const char* pString);
   BASstring(const char* pString, int Size);
   BASstring(const BASstring& Orig);
   ~BASstring();

   BASstring substr(int Start, int Size) const;
   BASstring substr(int Start) const;

   bool empty() const { return m_Size ==0; }

   int size() const { return m_Size; }
   void setSize(int NewSize) { m_Size = NewSize; }  // no checking - be careful

   int capacity() const { return m_Capacity; }
   void setCapacity(int NewCapacity);

   char* data()             { return m_Capacity < 16 ? m_pData.ShortBuffer : m_pData.pHeap; }
   const char* data() const { return m_Capacity < 16 ? m_pData.ShortBuffer : m_pData.pHeap; }

   BASstring& append(const char* pData, int Size);
   BASstring& append(const char* pData);
   BASstring& operator+=(const char Char);
   BASstring& operator+=(const char* pData);
   BASstring& operator+=(const BASstring& Orig); 

   BASstring& operator=(const BASstring& Orig);
   BASstring& operator=(const char* pData);

   bool operator!=(const char* pData) const;

   bool operator==(const BASstring& Rhs) const;
   bool operator!=(const BASstring& Rhs) const;

   int compare(const BASstring& Lhs) const;

   int reverseFind(char Char) const;

   const char& operator[](int i) const{ return data()[i]; }	   
   char& operator[](int i)            { return data()[i]; }	   
private:
   void init() {
      m_Size = 0;
      m_Capacity = sizeof(m_pData.ShortBuffer)-1;
      m_pData.ShortBuffer[0] = 0;
   }

   int m_Size;
   int m_Capacity;

   union BU{
      char ShortBuffer[16];
      char* pHeap;
   };

   BU m_pData;
};

int BASgrowCapacity(int* pCapacity, int DesiredCapacity);

unsigned int BASupperPowerOfTwo(unsigned int v);

BASstring operator+(const BASstring& Lhs, const char*     pRhs);
BASstring operator+(const BASstring& Lhs, const BASstring& Rhs);

bool BASisThisAlpha(char Character);

BASstream& operator<<(BASstream& Stream, const BASstring& String);

