//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASlinkedList
// 
// Implementation
//-------------------------------------------------------
#include "BASlinkedList.h"

#include "BAStrace.h"
BAS_TRACE_INIT;

BASstream& operator<<(BASstream& Stream, const BASlink& Link){
   Stream << "L: P=" << Link.pPrevious << " N=" << Link.pNext;
   return Stream;
}


BASlinkedListBase::BASlinkedListBase() : m_pHead(NULL), m_pTail(NULL), m_Size(0) {
   BAS_METHOD(BASlinkedListBase::BASlinkedListBase);

}

BASlinkedListBase::~BASlinkedListBase(){
   BAS_METHOD(BASlinkedListBase::~BASlinkedListBase);
}

void BASlinkedListBase::clear(){
   BAS_METHOD(BASlinkedListBase::clear);
   BASlink* pLink = m_pHead;
   while (pLink != NULL){
      BAS_TRC("Deleting " << pLink);
      BASlink* pOld = pLink;
      pLink = pLink->pNext;
      destroyLink(pOld);
   }
   m_Size = 0;
   m_pHead = NULL;
   m_pTail = NULL;
}
   
void BASlinkedListBase::removeItem(BASlink* pLink){
   BAS_METHOD(BASlinkedListBase::removeItem);
   BAS_VAR4(*pLink, m_pTail, m_pHead, m_Size);  
   if (pLink->pPrevious){
      pLink->pPrevious->pNext = pLink->pNext;
   } else {
      BAS_TRC("Have to move head.");
      m_pHead = pLink->pNext;
   } 
   if (pLink->pNext){
      pLink->pNext->pPrevious = pLink->pPrevious;
   } else {
      BAS_TRC("Have to change the tail");
      m_pTail = pLink->pPrevious;
   }
   m_Size--;
   
   destroyLink(pLink);
   BAS_VAR3(m_pTail, m_pHead, m_Size);
}

void BASlinkedListBase::addItem(BASlink* pLink){
   BAS_METHOD(BASlinkedListBase::addItem);
   BAS_VAR4(*pLink, m_pTail, m_pHead, m_Size);
   pLink->pPrevious = m_pTail;
   if (m_pTail == NULL){
      m_pHead = pLink;
   } else {
      m_pTail->pNext = pLink;
   }
   m_pTail = pLink;
   m_Size++;
   BAS_VAR4(*pLink, m_pTail, m_pHead, m_Size);
}

void BASlinkedListIteratorBase::operator++(){
   BAS_METHOD(BASlinkedListIteratorBase::operator++);
   m_pLink = m_pLink->pNext;
}
