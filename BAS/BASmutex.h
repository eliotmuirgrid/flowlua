#ifndef _BAS_MUTEX_H_
#define _BAS_MUTEX_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASmutex
// 
// Mutex wrapper - windows and posix implemenations
//-------------------------------------------------------

class BASmutex{
public:
   BASmutex();
   ~BASmutex();

   void lock();
   void unlock();
private:
   char m_Handle[64];   // 64 is the size for OS X pthreads 
};

class BASlocker{
public:
   BASlocker(BASmutex& Mutex) : m_pMutex(&Mutex) { m_pMutex->lock(); }
   ~BASlocker()                                  { m_pMutex->unlock(); }
private:
   BASmutex* m_pMutex;   
};

#endif
