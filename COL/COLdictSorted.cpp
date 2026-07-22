//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLdictSortedTree
// 
// Implementation
//-------------------------------------------------------

#include "COLdictSorted.h"

#include "COLstring.h"

#ifndef NULL
#define NULL 0
#endif

COLdictSortedNode::COLdictSortedNode() : m_pLeft(NULL), m_pRight(NULL){
}

COLdictSortedNode::~COLdictSortedNode(){
   delete m_pLeft;
   delete m_pRight;
}


COLdictSortedBase::COLdictSortedBase(COLavlCompare pCompareFunc, COLavlExtractKey pKey) : m_pRoot(NULL), m_pCompareFunc(pCompareFunc), m_pKeyFunc(pKey), m_Size(0){
}

COLdictSortedBase::~COLdictSortedBase(){
   delete m_pRoot;
}

int COLdictSortedHeight(COLdictSortedNode* pNode) {
   if (pNode == NULL){ return 0; } // empty node - zero height
   int LeftHeight  = COLdictSortedHeight(pNode->m_pLeft);
   int RightHeight = COLdictSortedHeight(pNode->m_pRight);
   return LeftHeight > RightHeight ? LeftHeight +1 : RightHeight + 1;
}

int COLdictSortedDifference(COLdictSortedNode* pNode){
   return COLdictSortedHeight(pNode->m_pLeft) - COLdictSortedHeight(pNode->m_pRight);
}

static COLdictSortedNode* rr_rotat(COLdictSortedNode* pParent) {
   COLdictSortedNode* t;
   t = pParent->m_pRight;
   pParent->m_pRight = t->m_pLeft;
   t->m_pLeft = pParent;
   return t;
}

static COLdictSortedNode* ll_rotat(COLdictSortedNode* pParent) {
   COLdictSortedNode* t;
   t = pParent->m_pLeft;
   pParent->m_pLeft = t->m_pRight;
   t->m_pRight = pParent;
   return t;
}

static COLdictSortedNode* lr_rotat(COLdictSortedNode* pParent) {
   COLdictSortedNode* t;
   t = pParent->m_pLeft;
   pParent->m_pLeft = rr_rotat(t);
   return ll_rotat(pParent);
}

static COLdictSortedNode* rl_rotat(COLdictSortedNode* pParent) {
   COLdictSortedNode* t;
   t = pParent->m_pRight;
   pParent->m_pRight = ll_rotat(t);
   return rr_rotat(pParent);
}

COLdictSortedNode* COLdictSortedBalance(COLdictSortedNode* pNode) {
   int Balance = COLdictSortedDifference(pNode);
   if (Balance > 1) {
      if (COLdictSortedDifference(pNode->m_pLeft) > 0)
         pNode = ll_rotat(pNode);
      else
         pNode = lr_rotat(pNode);
   } else if (Balance < -1) {
      if (COLdictSortedDifference(pNode->m_pRight) > 0)
         pNode = rl_rotat(pNode);
      else
         pNode = rr_rotat(pNode);
   }
   return pNode;
}

COLdictSortedNode* COLinsert(COLdictSortedNode* pNode, COLdictSortedNode* pNewNode, COLavlCompare pCompFunc, COLavlExtractKey pKeyFunc, int* pSize){
   if (pNode == NULL){
      (*pSize)++;
      return pNewNode;
   } 
   int Compare = (*pCompFunc)((*pKeyFunc)(pNode),(*pKeyFunc)(pNewNode));
   if (Compare == 0){
      pNode->copy(*pNewNode);
      return pNode;
   }
   if (Compare > 0){
      pNode->m_pLeft = COLinsert(pNode->m_pLeft, pNewNode, pCompFunc,pKeyFunc, pSize);
      return COLdictSortedBalance(pNode);
   }
   pNode->m_pRight = COLinsert(pNode->m_pRight, pNewNode, pCompFunc,pKeyFunc, pSize);
   return COLdictSortedBalance(pNode);
}


void COLdictSortedBase::insert(COLdictSortedNode* pNewNode){
   m_pRoot = COLinsert(m_pRoot, pNewNode, m_pCompareFunc, m_pKeyFunc, &m_Size);
}

COLdictSortedNode* COLfind(COLdictSortedNode* pNode, const void* pKey, COLavlCompare pCompFunc, COLavlExtractKey pKeyFunc){
   if (pNode == NULL){
      return NULL;
   }
   int Compare = (*pCompFunc)((*pKeyFunc)(pNode), pKey);
   if (Compare == 0){
      return pNode;
   }
   if (Compare > 0){
      return COLfind(pNode->m_pLeft, pKey, pCompFunc, pKeyFunc);
   } else {
      return COLfind(pNode->m_pRight, pKey, pCompFunc, pKeyFunc);
   }
}

COLdictSortedNode* COLdictSortedBase::find(const void* pKey) const{
   return COLfind((COLdictSortedNode*)m_pRoot, pKey, m_pCompareFunc, m_pKeyFunc);
}

int COLsCompare(const COLstring& Rhs, const COLstring& Lhs){
   return Rhs.compare(Lhs);
}

COLstream& operator<<(COLstream& Stream, const COLdictSortedNode& Node){
   //Stream << Node.m_Key; 
   if (!Node.m_pRight && !Node.m_pLeft){
      return Stream; // empty node
   }
   Stream << "-->[";
   if (Node.m_pLeft){
      Stream << *(Node.m_pLeft);
   } 
   Stream << ",";
   if (Node.m_pRight){
      Stream << *(Node.m_pRight);
   } 
   Stream << "]";
   return Stream;
}

COLdictSortedIterator::COLdictSortedIterator(COLdictSortedNode* pRoot) : m_StackPos(1){
   m_Stack[0] = NULL;
   m_Stack[1] = pRoot;
}

void COLdictSortedIterator::first(){
   downLeft();
}

bool COLdictSortedIterator::next(){
   if (root()->m_pRight != NULL){
      goRight();
      return downLeft();
   }  
   return upRight();
}

void COLdictSortedIterator::goRight(){
   m_StackPos++;
   m_Stack[m_StackPos] = parent()->m_pRight; 
}

bool COLdictSortedIterator::downLeft(){
   while (root()->m_pLeft != NULL){
      m_StackPos++;
      m_Stack[m_StackPos] = parent()->m_pLeft; 
   }
   return true;
}

bool COLdictSortedIterator::upRight(){
   while (m_StackPos > 1 && parent()->m_pRight == root()){
      pop();
   }
   pop();
   return root() != NULL;
}

void COLdictSortedIterator::pop(){
   m_StackPos--;
}
