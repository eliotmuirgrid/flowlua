//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASavlTree
// 
// Implementation
//-------------------------------------------------------

#include "BASavlTree.h"

#include "BASstring.h"

#ifndef NULL
#define NULL 0
#endif

BASavlNode::BASavlNode() : m_pLeft(NULL), m_pRight(NULL){
}

BASavlNode::~BASavlNode(){
   delete m_pLeft;
   delete m_pRight;
}


BASavlTreeBase::BASavlTreeBase(BASavlCompare pCompareFunc, BASavlExtractKey pKey) : m_pRoot(NULL), m_pCompareFunc(pCompareFunc), m_pKeyFunc(pKey), m_Size(0){
}

BASavlTreeBase::~BASavlTreeBase(){
   delete m_pRoot;
}

int BASavlHeight(BASavlNode* pNode) {
   if (pNode == NULL){ return 0; } // empty node - zero height
   int LeftHeight  = BASavlHeight(pNode->m_pLeft);
   int RightHeight = BASavlHeight(pNode->m_pRight);
   return LeftHeight > RightHeight ? LeftHeight +1 : RightHeight + 1;
}

int BASavlDifference(BASavlNode* pNode){
   return BASavlHeight(pNode->m_pLeft) - BASavlHeight(pNode->m_pRight);
}

static BASavlNode* rr_rotat(BASavlNode* pParent) {
   BASavlNode* t;
   t = pParent->m_pRight;
   pParent->m_pRight = t->m_pLeft;
   t->m_pLeft = pParent;
   return t;
}

static BASavlNode* ll_rotat(BASavlNode* pParent) {
   BASavlNode* t;
   t = pParent->m_pLeft;
   pParent->m_pLeft = t->m_pRight;
   t->m_pRight = pParent;
   return t;
}

static BASavlNode* lr_rotat(BASavlNode* pParent) {
   BASavlNode* t;
   t = pParent->m_pLeft;
   pParent->m_pLeft = rr_rotat(t);
   return ll_rotat(pParent);
}

static BASavlNode* rl_rotat(BASavlNode* pParent) {
   BASavlNode* t;
   t = pParent->m_pRight;
   pParent->m_pRight = ll_rotat(t);
   return rr_rotat(pParent);
}

BASavlNode* BASavlBalance(BASavlNode* pNode) {
   int Balance = BASavlDifference(pNode);
   if (Balance > 1) {
      if (BASavlDifference(pNode->m_pLeft) > 0)
         pNode = ll_rotat(pNode);
      else
         pNode = lr_rotat(pNode);
   } else if (Balance < -1) {
      if (BASavlDifference(pNode->m_pRight) > 0)
         pNode = rl_rotat(pNode);
      else
         pNode = rr_rotat(pNode);
   }
   return pNode;
}

BASavlNode* BASinsert(BASavlNode* pNode, BASavlNode* pNewNode, BASavlCompare pCompFunc, BASavlExtractKey pKeyFunc, int* pSize){
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
      return BASavlBalance(pNode);
   }
   pNode->m_pRight = BASinsert(pNode->m_pRight, pNewNode, pCompFunc,pKeyFunc, pSize);
   return BASavlBalance(pNode);
}


void BASavlTreeBase::insert(BASavlNode* pNewNode){
   m_pRoot = BASinsert(m_pRoot, pNewNode, m_pCompareFunc, m_pKeyFunc, &m_Size);
}

BASavlNode* BASfind(BASavlNode* pNode, const void* pKey, BASavlCompare pCompFunc, BASavlExtractKey pKeyFunc){
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

BASavlNode* BASavlTreeBase::find(const void* pKey) const{
   return BASfind((BASavlNode*)m_pRoot, pKey, m_pCompareFunc, m_pKeyFunc);
}

int BASsCompare(const BASstring& Rhs, const BASstring& Lhs){
   return Rhs.compare(Lhs);
}

BASstream& operator<<(BASstream& Stream, const BASavlNode& Node){
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

BASavlIterator::BASavlIterator(BASavlNode* pRoot) : m_StackPos(1){
   m_Stack[0] = NULL;
   m_Stack[1] = pRoot;
}

void BASavlIterator::first(){
   downLeft();
}

bool BASavlIterator::next(){
   if (root()->m_pRight != NULL){
      goRight();
      return downLeft();
   }  
   return upRight();
}

void BASavlIterator::goRight(){
   m_StackPos++;
   m_Stack[m_StackPos] = parent()->m_pRight; 
}

bool BASavlIterator::downLeft(){
   while (root()->m_pLeft != NULL){
      m_StackPos++;
      m_Stack[m_StackPos] = parent()->m_pLeft; 
   }
   return true;
}

bool BASavlIterator::upRight(){
   while (m_StackPos > 1 && parent()->m_pRight == root()){
      pop();
   }
   pop();
   return root() != NULL;
}

void BASavlIterator::pop(){
   m_StackPos--;
}
