//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASdictSortedTree
// 
// Implementation
//-------------------------------------------------------

#include "BASdictSorted.h"

#include "BASstring.h"

#ifndef NULL
#define NULL 0
#endif

BASdictSortedNode::BASdictSortedNode() : m_pLeft(NULL), m_pRight(NULL){
}

BASdictSortedNode::~BASdictSortedNode(){
   delete m_pLeft;
   delete m_pRight;
}


BASdictSortedBase::BASdictSortedBase(BASavlCompare pCompareFunc, BASavlExtractKey pKey) : m_pRoot(NULL), m_pCompareFunc(pCompareFunc), m_pKeyFunc(pKey), m_Size(0){
}

BASdictSortedBase::~BASdictSortedBase(){
   delete m_pRoot;
}

int BASdictSortedHeight(BASdictSortedNode* pNode) {
   if (pNode == NULL){ return 0; } // empty node - zero height
   int LeftHeight  = BASdictSortedHeight(pNode->m_pLeft);
   int RightHeight = BASdictSortedHeight(pNode->m_pRight);
   return LeftHeight > RightHeight ? LeftHeight +1 : RightHeight + 1;
}

int BASdictSortedDifference(BASdictSortedNode* pNode){
   return BASdictSortedHeight(pNode->m_pLeft) - BASdictSortedHeight(pNode->m_pRight);
}

static BASdictSortedNode* rr_rotat(BASdictSortedNode* pParent) {
   BASdictSortedNode* t;
   t = pParent->m_pRight;
   pParent->m_pRight = t->m_pLeft;
   t->m_pLeft = pParent;
   return t;
}

static BASdictSortedNode* ll_rotat(BASdictSortedNode* pParent) {
   BASdictSortedNode* t;
   t = pParent->m_pLeft;
   pParent->m_pLeft = t->m_pRight;
   t->m_pRight = pParent;
   return t;
}

static BASdictSortedNode* lr_rotat(BASdictSortedNode* pParent) {
   BASdictSortedNode* t;
   t = pParent->m_pLeft;
   pParent->m_pLeft = rr_rotat(t);
   return ll_rotat(pParent);
}

static BASdictSortedNode* rl_rotat(BASdictSortedNode* pParent) {
   BASdictSortedNode* t;
   t = pParent->m_pRight;
   pParent->m_pRight = ll_rotat(t);
   return rr_rotat(pParent);
}

BASdictSortedNode* BASdictSortedBalance(BASdictSortedNode* pNode) {
   int Balance = BASdictSortedDifference(pNode);
   if (Balance > 1) {
      if (BASdictSortedDifference(pNode->m_pLeft) > 0)
         pNode = ll_rotat(pNode);
      else
         pNode = lr_rotat(pNode);
   } else if (Balance < -1) {
      if (BASdictSortedDifference(pNode->m_pRight) > 0)
         pNode = rl_rotat(pNode);
      else
         pNode = rr_rotat(pNode);
   }
   return pNode;
}

BASdictSortedNode* BASinsert(BASdictSortedNode* pNode, BASdictSortedNode* pNewNode, BASavlCompare pCompFunc, BASavlExtractKey pKeyFunc, int* pSize){
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
      pNode->m_pLeft = BASinsert(pNode->m_pLeft, pNewNode, pCompFunc,pKeyFunc, pSize);
      return BASdictSortedBalance(pNode);
   }
   pNode->m_pRight = BASinsert(pNode->m_pRight, pNewNode, pCompFunc,pKeyFunc, pSize);
   return BASdictSortedBalance(pNode);
}


void BASdictSortedBase::insert(BASdictSortedNode* pNewNode){
   m_pRoot = BASinsert(m_pRoot, pNewNode, m_pCompareFunc, m_pKeyFunc, &m_Size);
}

BASdictSortedNode* BASfind(BASdictSortedNode* pNode, const void* pKey, BASavlCompare pCompFunc, BASavlExtractKey pKeyFunc){
   if (pNode == NULL){
      return NULL;
   }
   int Compare = (*pCompFunc)((*pKeyFunc)(pNode), pKey);
   if (Compare == 0){
      return pNode;
   }
   if (Compare > 0){
      return BASfind(pNode->m_pLeft, pKey, pCompFunc, pKeyFunc);
   } else {
      return BASfind(pNode->m_pRight, pKey, pCompFunc, pKeyFunc);
   }
}

BASdictSortedNode* BASdictSortedBase::find(const void* pKey) const{
   return BASfind((BASdictSortedNode*)m_pRoot, pKey, m_pCompareFunc, m_pKeyFunc);
}

int BASsCompare(const BASstring& Rhs, const BASstring& Lhs){
   return Rhs.compare(Lhs);
}

BASstream& operator<<(BASstream& Stream, const BASdictSortedNode& Node){
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

BASdictSortedIterator::BASdictSortedIterator(BASdictSortedNode* pRoot) : m_StackPos(1){
   m_Stack[0] = NULL;
   m_Stack[1] = pRoot;
}

void BASdictSortedIterator::first(){
   downLeft();
}

bool BASdictSortedIterator::next(){
   if (root()->m_pRight != NULL){
      goRight();
      return downLeft();
   }  
   return upRight();
}

void BASdictSortedIterator::goRight(){
   m_StackPos++;
   m_Stack[m_StackPos] = parent()->m_pRight; 
}

bool BASdictSortedIterator::downLeft(){
   while (root()->m_pLeft != NULL){
      m_StackPos++;
      m_Stack[m_StackPos] = parent()->m_pLeft; 
   }
   return true;
}

bool BASdictSortedIterator::upRight(){
   while (m_StackPos > 1 && parent()->m_pRight == root()){
      pop();
   }
   pop();
   return root() != NULL;
}

void BASdictSortedIterator::pop(){
   m_StackPos--;
}
