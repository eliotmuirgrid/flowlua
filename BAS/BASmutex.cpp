//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASmutex
// 
// Mutex wrapper - windows and posix implementations
//-------------------------------------------------------
#include "BASmutex.h"

#ifndef _WIN32

#include <pthread.h>
#include <stdlib.h>


#define m_HANDLE (pthread_mutex_t*)&m_Handle

BASmutex::BASmutex(){
   pthread_mutexattr_t Attribute;
   pthread_mutexattr_init(&Attribute);
   pthread_mutexattr_settype(&Attribute, PTHREAD_MUTEX_RECURSIVE);  // Allow multiple locks from the same thread.
   pthread_mutex_init(m_HANDLE, &Attribute);
   pthread_mutexattr_destroy(&Attribute);   
}

BASmutex::~BASmutex(){ pthread_mutex_destroy(m_HANDLE); }

void BASmutex::lock(){ pthread_mutex_lock   (m_HANDLE); }
void BASmutex::unlock(){pthread_mutex_unlock(m_HANDLE); }

#else // WINDOWS

#define LEAN_AND_MEAN
#include <windows.h>

#define m_HANDLE (CRITICAL_SECTION*)&m_Handle

BASmutex::BASmutex()   { ::InitializeCriticalSection(m_HANDLE); }
BASmutex::~BASmutex()  { ::DeleteCriticalSection(m_HANDLE);     }
void BASmutex::lock()  { ::EnterCriticalSection(m_HANDLE);      }  
void BASmutex::unlock(){ ::LeaveCriticalSection(m_HANDLE);      }

#endif

