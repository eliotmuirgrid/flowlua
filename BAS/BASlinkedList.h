#ifndef _BAS_LINKED_LIST_H_
#define _BAS_LINKED_LIST_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASlinkedList
// 
// Single linked list.
//-------------------------------------------------------

class BASlink{
public:
   BASlink() : pPrevious(0), pNext(0) {}
   BASlink* pPrevious;
   BASlink* pNext;
};

class BASlinkedListBase{
public:
   BASlinkedListBase();
   virtual ~BASlinkedListBase();
   
   void addItem(BASlink* pLink);
   void removeItem(BASlink* pLink);

   BASlink* head() const { return m_pHead; }
   BASlink* tail() const { return m_pTail; }

   int size() { return m_Size; }

   void clear();

private:
   virtual void destroyLink(BASlink* pLink)=0;
   BASlink* m_pHead;
   BASlink* m_pTail;
   int m_Size;
};

class BASlinkedListIteratorBase{
public:
   BASlinkedListIteratorBase(BASlink* pLink) : m_pLink(pLink) {}
   void operator++();
   bool operator==(const BASlinkedListIteratorBase& Rhs) const { return m_pLink == Rhs.m_pLink; }
   bool operator!=(const BASlinkedListIteratorBase& Rhs) const { return m_pLink != Rhs.m_pLink; }
protected:
    BASlink* m_pLink;
};


template <class TValue>
class BASlinkT : public BASlink{
public:
   BASlinkT(TValue iValue) : BASlink(), Value(iValue) {}
   virtual ~BASlinkT() {}
   TValue Value;
};

template <class TValue>
class BASlinkedListIterator : public BASlinkedListIteratorBase{
public:
   BASlinkedListIterator(BASlink* pLink) : BASlinkedListIteratorBase(pLink) {}
   TValue& value()             { return ((BASlinkT<TValue>*)m_pLink)->Value; }
   const TValue& value() const { return ((BASlinkT<TValue>*)m_pLink)->Value; }
};
   
template <class TValue>
class BASlinkedList : public BASlinkedListBase{
public:
   BASlinkedList() {}
   virtual ~BASlinkedList() { clear(); }

   void add(const TValue& Value) { addItem(new BASlinkT<TValue>(Value)); }

   TValue& front()             {  return ((BASlinkT<TValue>*)head())->Value; } 
   const TValue& front() const {  return ((BASlinkT<TValue>*)head())->Value; }
   TValue& back()              {  return ((BASlinkT<TValue>*)tail())->Value; } 
   const TValue& back() const  {  return ((BASlinkT<TValue>*)tail())->Value; } 

   BASlinkedListIterator<TValue> begin()              { return BASlinkedListIterator<TValue>(head()); }
   BASlinkedListIterator<TValue> end()                { return BASlinkedListIterator<TValue>(0); }
   const BASlinkedListIterator<TValue> cbegin() const { return BASlinkedListIterator<TValue>(head()); }
   const BASlinkedListIterator<TValue> cend()   const { return BASlinkedListIterator<TValue>(0); }

private:
   virtual void destroyLink(BASlink* pLink){
      delete (BASlinkT<TValue>*)pLink;
   }
};

#endif
