#pragma once
//-------------------------------------------------------
// Copyright (C) Forever.  All Rights Reserved
//
// COLstring
// 
// Core string class.  Not reference counted.
//-------------------------------------------------------

class COLstream;

class COLstring{
public:
   COLstring();
   COLstring(const char* pString);
   COLstring(const char* pString, int Size);
   COLstring(const COLstring& Orig);
   ~COLstring();

   COLstring substr(int Start, int Size) const;
   COLstring substr(int Start) const;

   bool empty() const { return m_Size ==0; }

   int size() const { return m_Size; }
   void setSize(int NewSize) { m_Size = NewSize; }  // no checking - be careful

   int capacity() const { return m_Capacity; }
   void setCapacity(int NewCapacity);

   char* data()             { return m_Capacity < 16 ? m_pData.ShortBuffer : m_pData.pHeap; }
   const char* data() const { return m_Capacity < 16 ? m_pData.ShortBuffer : m_pData.pHeap; }

   COLstring& append(const char* pData, int Size);
   COLstring& append(const char* pData);
   COLstring& operator+=(const char Char);
   COLstring& operator+=(const char* pData);
   COLstring& operator+=(const COLstring& Orig); 

   COLstring& operator=(const COLstring& Orig);
   COLstring& operator=(const char* pData);

   bool operator!=(const char* pData) const;

   bool operator==(const COLstring& Rhs) const;
   bool operator!=(const COLstring& Rhs) const;

   int compare(const COLstring& Lhs) const;

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

int COLgrowCapacity(int* pCapacity, int DesiredCapacity);

unsigned int COLupperPowerOfTwo(unsigned int v);

COLstring operator+(const COLstring& Lhs, const char*     pRhs);
COLstring operator+(const COLstring& Lhs, const COLstring& Rhs);

bool COLisThisAlpha(char Character);

COLstream& operator<<(COLstream& Stream, const COLstring& String);

