#ifndef _BAS_AVL_TREE_H__
#define _BAS_AVL_TREE_H__
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASavlTree
//
// An AVL tree.
// https://www.youtube.com/watch?v=vRwi_UcZGjU
//-------------------------------------------------------

class BASstring;

class BASstream;

class BASavlNode{
public:
   BASavlNode();
   virtual ~BASavlNode();

   virtual void copy(BASavlNode& Orig)=0;
   BASavlNode* m_pLeft;
   BASavlNode* m_pRight;
};

template<class KType, class VType>
class BASavlNodeT : public BASavlNode{
public:
   BASavlNodeT(KType Key, VType Value) : BASavlNode(), m_Key(Key), m_Value(Value) {}
   BASavlNodeT(KType Key) : BASavlNode(), m_Key(Key) {}   

   virtual void copy(BASavlNode& Orig){
      m_Value = ((BASavlNodeT<KType, VType>&)Orig).m_Value;
   }

   KType m_Key;
   VType m_Value;
};

class BASavlIterator{
public:
   BASavlIterator(BASavlNode* pRoot);

   struct Link{
      Link(Link* pParent, BASavlNode* pNode) : m_pParent(pParent), m_pNode(pNode) {}
      Link* m_pParent;
      BASavlNode* m_pNode;
   };
   void first();
   void end() { m_StackPos = 0;}
   bool next();

   bool operator++() { return next(); }

   bool operator==(const BASavlIterator& Rhs) const { return root() == Rhs.root(); }
   bool operator!=(const BASavlIterator& Rhs) const { return root() != Rhs.root(); }

   inline BASavlNode* root() { return m_Stack[m_StackPos];}
   inline const BASavlNode* root() const { return m_Stack[m_StackPos];}
private:
   void pop();
   void goRight();
   bool downLeft();
   bool upRight();

   
   inline BASavlNode* parent() { return m_Stack[m_StackPos-1];}

   BASavlNode* m_Stack[32];
   int m_StackPos;
};

template<class KType, class VType>
class BASavlIteratorT : public BASavlIterator{
public:
   BASavlIteratorT(BASavlNode* pRoot) : BASavlIterator(pRoot) {}
   
   KType& key() { return ((BASavlNodeT<KType, VType>*)root())->m_Key; }
   VType& value() { return ((BASavlNodeT<KType, VType>*)root())->m_Value; }
};

typedef int (*BASavlCompare)(const void* pRKey, const void* pLKey);
typedef const void* (*BASavlExtractKey)(const BASavlNode* pNode);


class BASavlTreeBase{
public:
   BASavlTreeBase(BASavlCompare pCompFunc, BASavlExtractKey pFunc);
   virtual ~BASavlTreeBase();

   int size() const { return m_Size; }

protected:
   void insert(BASavlNode* pNewNode);
   BASavlNode* find(const void* Key) const;

   BASavlNode* m_pRoot;
private:
   BASavlCompare    m_pCompareFunc;
   BASavlExtractKey m_pKeyFunc;
   int              m_Size;
};

BASstream& operator<<(BASstream& Stream, const BASavlNode& Node);

template<class KType>
int BASsCompare(const KType& Rhs, const KType& Lhs){
   return Rhs - Lhs;
}

int BASsCompare(const BASstring& Rhs, const BASstring& Lhs);

template<class KType, class VType>
class BASavlTree : public BASavlTreeBase{
public:
   BASavlTree() : BASavlTreeBase(&compare, &key) {}

   static const void* key(const BASavlNode* pNode){
      return &((BASavlNodeT<KType, VType>*)pNode)->m_Key; 
   }
   
   static int compare(const void* pRKey, const void* pLKey){
      return BASsCompare( *((const KType*)pRKey), *((const KType*)pLKey)); 
   }

   void add(KType Key, VType Value){
      insert(new BASavlNodeT<KType, VType>(Key, Value));
   }

   bool has(const KType& Key){
      return find((const void*)&Key) != 0;
   }

   const VType& value(const KType& Key) const{
      return ((BASavlNodeT<KType, VType>*)find((const void*)&Key))->m_Value;
   }

   VType& value(const KType& Key)     { return ((BASavlNodeT<KType, VType>*)find((const void*)&Key))->m_Value;}
   VType& operator[](const KType& Key){
      BASavlNodeT<KType, VType>* pNode = ((BASavlNodeT<KType, VType>*)find((const void*)&Key));
      if (!pNode){
         pNode = new BASavlNodeT<KType, VType>(Key);
         insert(pNode); // TODO - could optimize this!    
      }
      return pNode->m_Value; 
   }

   BASavlIteratorT<KType, VType> begin() { BASavlIteratorT<KType, VType> i(m_pRoot); i.first(); return i; }
   BASavlIteratorT<KType, VType> end()   { BASavlIteratorT<KType, VType> i(m_pRoot); i.end(); return i; }

   BASavlIteratorT<KType, VType> cbegin() const { BASavlIteratorT<KType, VType> i(m_pRoot); i.first(); return i; }
   BASavlIteratorT<KType, VType> cend()   const { BASavlIteratorT<KType, VType> i(m_pRoot); i.end(); return i; }
};

#endif
