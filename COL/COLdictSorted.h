#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLdictSorted
//
// An AVL tree.
// https://www.youtube.com/watch?v=vRwi_UcZGjU
//-------------------------------------------------------

class COLstring;
class COLstream;

class COLdictSortedNode{
public:
   COLdictSortedNode();
   virtual ~COLdictSortedNode();

   virtual void copy(COLdictSortedNode& Orig)=0;
   COLdictSortedNode* m_pLeft;
   COLdictSortedNode* m_pRight;
};

template<class KType, class VType>
class COLdictSortedNodeT : public COLdictSortedNode{
public:
   COLdictSortedNodeT(KType Key, VType Value) : COLdictSortedNode(), m_Key(Key), m_Value(Value) {}
   COLdictSortedNodeT(KType Key) : COLdictSortedNode(), m_Key(Key) {}   

   virtual void copy(COLdictSortedNode& Orig){
      m_Value = ((COLdictSortedNodeT<KType, VType>&)Orig).m_Value;
   }

   KType m_Key;
   VType m_Value;
};

class COLdictSortedIterator{
public:
   COLdictSortedIterator(COLdictSortedNode* pRoot);

   struct Link{
      Link(Link* pParent, COLdictSortedNode* pNode) : m_pParent(pParent), m_pNode(pNode) {}
      Link* m_pParent;
      COLdictSortedNode* m_pNode;
   };
   void first();
   void end() { m_StackPos = 0;}
   bool next();

   bool operator++() { return next(); }

   bool operator==(const COLdictSortedIterator& Rhs) const { return root() == Rhs.root(); }
   bool operator!=(const COLdictSortedIterator& Rhs) const { return root() != Rhs.root(); }

   inline COLdictSortedNode* root() { return m_Stack[m_StackPos];}
   inline const COLdictSortedNode* root() const { return m_Stack[m_StackPos];}
private:
   void pop();
   void goRight();
   bool downLeft();
   bool upRight();

   
   inline COLdictSortedNode* parent() { return m_Stack[m_StackPos-1];}

   COLdictSortedNode* m_Stack[32];
   int m_StackPos;
};

template<class KType, class VType>
class COLdictSortedIteratorT : public COLdictSortedIterator{
public:
   COLdictSortedIteratorT(COLdictSortedNode* pRoot) : COLdictSortedIterator(pRoot) {}
   
   KType& key() { return ((COLdictSortedNodeT<KType, VType>*)root())->m_Key; }
   VType& value() { return ((COLdictSortedNodeT<KType, VType>*)root())->m_Value; }
};

typedef int (*COLavlCompare)(const void* pRKey, const void* pLKey);
typedef const void* (*COLavlExtractKey)(const COLdictSortedNode* pNode);


class COLdictSortedBase{
public:
   COLdictSortedBase(COLavlCompare pCompFunc, COLavlExtractKey pFunc);
   virtual ~COLdictSortedBase();

   int size() const { return m_Size; }

protected:
   void insert(COLdictSortedNode* pNewNode);
   COLdictSortedNode* find(const void* Key) const;

   COLdictSortedNode* m_pRoot;
private:
   COLavlCompare    m_pCompareFunc;
   COLavlExtractKey m_pKeyFunc;
   int              m_Size;
};

COLstream& operator<<(COLstream& Stream, const COLdictSortedNode& Node);

template<class KType>
int COLsCompare(const KType& Rhs, const KType& Lhs){
   return Rhs - Lhs;
}

int COLsCompare(const COLstring& Rhs, const COLstring& Lhs);

template<class KType, class VType>
class COLdictSorted : public COLdictSortedBase{
public:
   COLdictSorted() : COLdictSortedBase(&compare, &key) {}

   static const void* key(const COLdictSortedNode* pNode){
      return &((COLdictSortedNodeT<KType, VType>*)pNode)->m_Key; 
   }
   
   static int compare(const void* pRKey, const void* pLKey){
      return COLsCompare( *((const KType*)pRKey), *((const KType*)pLKey)); 
   }

   void add(KType Key, VType Value){
      insert(new COLdictSortedNodeT<KType, VType>(Key, Value));
   }

   bool has(const KType& Key){
      return find((const void*)&Key) != 0;
   }

   const VType& value(const KType& Key) const{
      return ((COLdictSortedNodeT<KType, VType>*)find((const void*)&Key))->m_Value;
   }

   VType& value(const KType& Key)     { return ((COLdictSortedNodeT<KType, VType>*)find((const void*)&Key))->m_Value;}
   VType& operator[](const KType& Key){
      COLdictSortedNodeT<KType, VType>* pNode = ((COLdictSortedNodeT<KType, VType>*)find((const void*)&Key));
      if (!pNode){
         pNode = new COLdictSortedNodeT<KType, VType>(Key);
         insert(pNode); // TODO - could optimize this!    
      }
      return pNode->m_Value; 
   }

   COLdictSortedIteratorT<KType, VType> begin() { COLdictSortedIteratorT<KType, VType> i(m_pRoot); i.first(); return i; }
   COLdictSortedIteratorT<KType, VType> end()   { COLdictSortedIteratorT<KType, VType> i(m_pRoot); i.end(); return i; }

   COLdictSortedIteratorT<KType, VType> cbegin() const { COLdictSortedIteratorT<KType, VType> i(m_pRoot); i.first(); return i; }
   COLdictSortedIteratorT<KType, VType> cend()   const { COLdictSortedIteratorT<KType, VType> i(m_pRoot); i.end(); return i; }
};

