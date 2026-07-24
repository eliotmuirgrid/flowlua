#ifndef _COL_LINKED_LIST_H_
#define _COL_LINKED_LIST_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLlinkedList
// 
// Single linked list.
//-------------------------------------------------------

class COLlink{
public:
   COLlink() : pPrevious(0), pNext(0) {}
   COLlink* pPrevious;
   COLlink* pNext;
};

class COLlinkedListBase{
public:
   COLlinkedListBase();
   virtual ~COLlinkedListBase();
   
   void addItem(COLlink* pLink);
   void removeItem(COLlink* pLink);

   COLlink* head() const { return m_pHead; }
   COLlink* tail() const { return m_pTail; }

   int size() { return m_Size; }

   void clear();

private:
   virtual void destroyLink(COLlink* pLink)=0;
   COLlink* m_pHead;
   COLlink* m_pTail;
   int m_Size;
};

class COLlinkedListIteratorBase{
public:
   COLlinkedListIteratorBase(COLlink* pLink) : m_pLink(pLink) {}
   void operator++();
   bool operator==(const COLlinkedListIteratorBase& Rhs) const { return m_pLink == Rhs.m_pLink; }
   bool operator!=(const COLlinkedListIteratorBase& Rhs) const { return m_pLink != Rhs.m_pLink; }
protected:
    COLlink* m_pLink;
};


template <class TValue>
class COLlinkT : public COLlink{
public:
   COLlinkT(TValue iValue) : COLlink(), Value(iValue) {}
   virtual ~COLlinkT() {}
   TValue Value;
};

template <class TValue>
class COLlinkedListIterator : public COLlinkedListIteratorBase{
public:
   COLlinkedListIterator(COLlink* pLink) : COLlinkedListIteratorBase(pLink) {}
   TValue& value()             { return ((COLlinkT<TValue>*)m_pLink)->Value; }
   const TValue& value() const { return ((COLlinkT<TValue>*)m_pLink)->Value; }
};
   
template <class TValue>
class COLlinkedList : public COLlinkedListBase{
public:
   COLlinkedList() {}
   virtual ~COLlinkedList() { clear(); }

   void add(const TValue& Value) { addItem(new COLlinkT<TValue>(Value)); }

   TValue& front()             {  return ((COLlinkT<TValue>*)head())->Value; } 
   const TValue& front() const {  return ((COLlinkT<TValue>*)head())->Value; }
   TValue& back()              {  return ((COLlinkT<TValue>*)tail())->Value; } 
   const TValue& back() const  {  return ((COLlinkT<TValue>*)tail())->Value; } 

   COLlinkedListIterator<TValue> begin()              { return COLlinkedListIterator<TValue>(head()); }
   COLlinkedListIterator<TValue> end()                { return COLlinkedListIterator<TValue>(0); }
   const COLlinkedListIterator<TValue> cbegin() const { return COLlinkedListIterator<TValue>(head()); }
   const COLlinkedListIterator<TValue> cend()   const { return COLlinkedListIterator<TValue>(0); }

private:
   virtual void destroyLink(COLlink* pLink){
      delete (COLlinkT<TValue>*)pLink;
   }
};

#endif
