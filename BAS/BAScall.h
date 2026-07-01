#ifndef _BAS_CALL_H_
#define _BAS_CALL_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BAScall
//
// Simulates a closure in C++.  Use it for callbacks.
//-------------------------------------------------------

template<int idx, typename T>
struct GetHelper { };
template <typename... T>
struct BAStuple { };
template<typename T, typename... Rest>
struct GetHelper<0, BAStuple<T, Rest...>> { // Specialization for index 0
    static T get(BAStuple<T, Rest...> &data) { return data.first; }
};

template<int idx,typename T,typename... Rest>
struct GetHelper<idx, BAStuple<T, Rest...>> { // GetHelper Implementation
    static auto get(BAStuple<T, Rest...> &data) { return GetHelper<idx - 1, BAStuple<Rest...>>::get(data.rest); }
};
template<typename T,typename... Rest>
struct BAStuple<T, Rest...> {
    T                   first;
    BAStuple<Rest...>      rest;
    BAStuple(const T &f, const Rest &... r)  : first(f), rest(r...)  {}
};
template<int idx, template <typename...> class BAStuple, typename... Args>
auto get(BAStuple<Args...> &t) { return GetHelper<idx, BAStuple<Args...>>::get(t); }

template <int ... Ns> struct sequence {};
// First define the template signature
template <int ... Ns> struct seq_gen;
// Recursion case
template <int I, int ... Ns> struct seq_gen<I, Ns...>{
    // Take front most number of sequence, decrement it, and prepend it twice.
    // First I - 1 goes into the counter, Second I - 1 goes into the sequence.
    using type = typename seq_gen<I - 1, I - 1, Ns...>::type;
};
// Recursion abort
template <int ... Ns> struct seq_gen<0, Ns...>{ using type = sequence<Ns...>; };
template <int N> using sequence_t = typename seq_gen<N>::type;

template<typename...BArgs>
class BAScall{
public:
   BAScall() {}
   virtual ~BAScall() {}
   virtual void call(BArgs...args)=0;

   template<typename...Args>
   class BAScallT : public BAScall<BArgs...>{ // This is for calling functions
   public:
      typedef void (*BASfuncP)(BArgs...bargs, Args...args);
      BAScallT(BASfuncP pFunc, Args...args) : m_pFunc(pFunc), m_Tuple(args...) {}
      virtual ~BAScallT() {}
      template <typename F, typename TUP,int ... INDICES>
      static void tuple_call_(F* f, TUP tup, sequence<INDICES...>, BArgs...bargs){ (*f)(bargs..., get<INDICES>(tup) ...);}
      template <typename F, typename ... Ts>
      static void tuple_call(F* f, BAStuple<Ts...> Tup, BArgs...bargs){ tuple_call_(f, Tup, sequence_t<sizeof...(Ts)>{}, bargs...); }
      virtual void call(BArgs...bargs){ tuple_call(m_pFunc, m_Tuple, bargs...);}

      BASfuncP m_pFunc;
      BAStuple<Args...> m_Tuple;
   };
   template<class TObject, typename...Args>
   class BAScallM : public BAScall<BArgs...>{  // This is for calling methods
   public:
      typedef void (TObject::*BASfuncM)(BArgs...bargs, Args...args);
      BAScallM(TObject* pObject, BASfuncM pMethod, Args...args) : m_pMethod(pMethod), m_pObject(pObject), m_Tuple(args...) {}
      virtual ~BAScallM() {}
      template <typename TUP,int ... INDICES>
      static void tuple_call_(TObject* pObject, BASfuncM pMethod, TUP tup, sequence<INDICES...>, BArgs...bargs){  (pObject->*pMethod)(bargs..., get<INDICES>(tup) ...); } 
      template <typename ... Ts>
      static void tuple_call(TObject* pObject, BASfuncM pMethod, BAStuple<Ts...> Tup, BArgs...bargs){ tuple_call_(pObject, pMethod, Tup, sequence_t<sizeof...(Ts)>{}, bargs...); }
      virtual void call(BArgs...bargs){ tuple_call(m_pObject, m_pMethod, m_Tuple, bargs...); }

      BASfuncM m_pMethod;
      TObject* m_pObject;
      BAStuple<Args...> m_Tuple;
   };

   template<typename...Args>
   static BAScall<BArgs...>* create(void (*pFunc)(BArgs...bargs, Args...args),Args...args) { return new typename BAScall<BArgs...>::template BAScallT<Args...>(pFunc, args...); }
  
   template<class TObject, typename...Args>
   static BAScall<BArgs...>* create(TObject* pObject, void (TObject::*pMethod)(BArgs...bargs, Args...args),Args...args) { return new typename BAScall<BArgs...>::template BAScallM<TObject, Args...>(pObject, pMethod, args...); } 
};

#endif