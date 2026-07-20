#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASdictSorted
//
// An AVL tree.
// https://www.youtube.com/watch?v=vRwi_UcZGjU
//-------------------------------------------------------

class BASstring;
class BASstream;

class BASdictSortedNode{
public:
   BASdictSortedNode();
   virtual ~BASdictSortedNode();

   virtual void copy(BASdictSortedNode& Orig)=0;
   BASdictSortedNode* m_pLeft;
   BASdictSortedNode* m_pRight;
};

template<class KType, class VType>
class BASdictSortedNodeT : public BASdictSortedNode{
public:
   BASdictSortedNodeT(KType Key, VType Value) : BASdictSortedNode(), m_Key(Key), m_Value(Value) {}
   BASdictSortedNodeT(KType Key) : BASdictSortedNode(), m_Key(Key) {}   

   virtual void copy(BASdictSortedNode& Orig){
      m_Value = ((BASdictSortedNodeT<KType, VType>&)Orig).m_Value;
   }

   KType m_Key;
   VType m_Value;
};

class BASdictSortedIterator{
public:
   BASdictSortedIterator(BASdictSortedNode* pRoot);

   struct Link{
      Link(Link* pParent, BASdictSortedNode* pNode) : m_pParent(pParent), m_pNode(pNode) {}
      Link* m_pParent;
      BASdictSortedNode* m_pNode;
   };
   void first();
   void end() { m_StackPos = 0;}
   bool next();

   bool operator++() { return next(); }

   bool operator==(const BASdictSortedIterator& Rhs) const { return root() == Rhs.root(); }
   bool operator!=(const BASdictSortedIterator& Rhs) const { return root() != Rhs.root(); }

   inline BASdictSortedNode* root() { return m_Stack[m_StackPos];}
   inline const BASdictSortedNode* root() const { return m_Stack[m_StackPos];}
private:
   void pop();
   void goRight();
   bool downLeft();
   bool upRight();

   
   inline BASdictSortedNode* parent() { return m_Stack[m_StackPos-1];}

   BASdictSortedNode* m_Stack[32];
   int m_StackPos;
};

template<class KType, class VType>
class BASdictSortedIteratorT : public BASdictSortedIterator{
public:
   BASdictSortedIteratorT(BASdictSortedNode* pRoot) : BASdictSortedIterator(pRoot) {}
   
   KType& key() { return ((BASdictSortedNodeT<KType, VType>*)root())->m_Key; }
   VType& value() { return ((BASdictSortedNodeT<KType, VType>*)root())->m_Value; }
};

typedef int (*BASavlCompare)(const void* pRKey, const void* pLKey);
typedef const void* (*BASavlExtractKey)(const BASdictSortedNode* pNode);


class BASdictSortedBase{
public:
   BASdictSortedBase(BASavlCompare pCompFunc, BASavlExtractKey pFunc);
   virtual ~BASdictSortedBase();

   int size() const { return m_Size; }

protected:
   void insert(BASdictSortedNode* pNewNode);
   BASdictSortedNode* find(const void* Key) const;

   BASdictSortedNode* m_pRoot;
private:
   BASavlCompare    m_pCompareFunc;
   BASavlExtractKey m_pKeyFunc;
   int              m_Size;
};

BASstream& operator<<(BASstream& Stream, const BASdictSortedNode& Node);

template<class KType>
int BASsCompare(const KType& Rhs, const KType& Lhs){
   return Rhs - Lhs;
}

int BASsCompare(const BASstring& Rhs, const BASstring& Lhs);

template<class KType, class VType>
class BASdictSorted : public BASdictSortedBase{
public:
   BASdictSorted() : BASdictSortedBase(&compare, &key) {}

   static const void* key(const BASdictSortedNode* pNode){
      return &((BASdictSortedNodeT<KType, VType>*)pNode)->m_Key; 
   }
   
   static int compare(const void* pRKey, const void* pLKey){
      return BASsCompare( *((const KType*)pRKey), *((const KType*)pLKey)); 
   }

   void add(KType Key, VType Value){
      insert(new BASdictSortedNodeT<KType, VType>(Key, Value));
   }

   bool has(const KType& Key){
      return find((const void*)&Key) != 0;
   }

   const VType& value(const KType& Key) const{
      return ((BASdictSortedNodeT<KType, VType>*)find((const void*)&Key))->m_Value;
   }

   VType& value(const KType& Key)     { return ((BASdictSortedNodeT<KType, VType>*)find((const void*)&Key))->m_Value;}
   VType& operator[](const KType& Key){
      BASdictSortedNodeT<KType, VType>* pNode = ((BASdictSortedNodeT<KType, VType>*)find((const void*)&Key));
      if (!pNode){
         pNode = new BASdictSortedNodeT<KType, VType>(Key);
         insert(pNode); // TODO - could optimize this!    
      }
      return pNode->m_Value; 
   }

   BASdictSortedIteratorT<KType, VType> begin() { BASdictSortedIteratorT<KType, VType> i(m_pRoot); i.first(); return i; }
   BASdictSortedIteratorT<KType, VType> end()   { BASdictSortedIteratorT<KType, VType> i(m_pRoot); i.end(); return i; }

   BASdictSortedIteratorT<KType, VType> cbegin() const { BASdictSortedIteratorT<KType, VType> i(m_pRoot); i.first(); return i; }
   BASdictSortedIteratorT<KType, VType> cend()   const { BASdictSortedIteratorT<KType, VType> i(m_pRoot); i.end(); return i; }
};

