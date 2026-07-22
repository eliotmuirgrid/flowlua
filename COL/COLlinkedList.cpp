//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLlinkedList
// 
// Implementation
//-------------------------------------------------------
#include "COLlinkedList.h"

#include "COLtrace.h"
COL_TRACE_INIT;

COLstream& operator<<(COLstream& Stream, const COLlink& Link){
   Stream << "L: P=" << Link.pPrevious << " N=" << Link.pNext;
   return Stream;
}


COLlinkedListBase::COLlinkedListBase() : m_pHead(NULL), m_pTail(NULL), m_Size(0) {
   COL_METHOD(COLlinkedListBase::COLlinkedListBase);

}

COLlinkedListBase::~COLlinkedListBase(){
   COL_METHOD(COLlinkedListBase::~COLlinkedListBase);
}

void COLlinkedListBase::clear(){
   COL_METHOD(COLlinkedListBase::clear);
   COLlink* pLink = m_pHead;
   while (pLink != NULL){
      COL_TRC("Deleting " << pLink);
      COLlink* pOld = pLink;
      pLink = pLink->pNext;
      destroyLink(pOld);
   }
   m_Size = 0;
   m_pHead = NULL;
   m_pTail = NULL;
}
   
void COLlinkedListBase::removeItem(COLlink* pLink){
   COL_METHOD(COLlinkedListBase::removeItem);
   COL_VAR4(*pLink, m_pTail, m_pHead, m_Size);  
   if (pLink->pPrevious){
      pLink->pPrevious->pNext = pLink->pNext;
   } else {
      COL_TRC("Have to move head.");
      m_pHead = pLink->pNext;
   } 
   if (pLink->pNext){
      pLink->pNext->pPrevious = pLink->pPrevious;
   } else {
      COL_TRC("Have to change the tail");
      m_pTail = pLink->pPrevious;
   }
   m_Size--;
   
   destroyLink(pLink);
   COL_VAR3(m_pTail, m_pHead, m_Size);
}

void COLlinkedListBase::addItem(COLlink* pLink){
   COL_METHOD(COLlinkedListBase::addItem);
   COL_VAR4(*pLink, m_pTail, m_pHead, m_Size);
   pLink->pPrevious = m_pTail;
   if (m_pTail == NULL){
      m_pHead = pLink;
   } else {
      m_pTail->pNext = pLink;
   }
   m_pTail = pLink;
   m_Size++;
   COL_VAR4(*pLink, m_pTail, m_pHead, m_Size);
}

void COLlinkedListIteratorBase::operator++(){
   COL_METHOD(COLlinkedListIteratorBase::operator++);
   m_pLink = m_pLink->pNext;
}
