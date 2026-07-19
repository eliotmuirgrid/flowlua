//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// BASthread
//-------------------------------------------------------

#include <libc/thread/thread.h>

int BASthreadId(){
   return pthread_getthreadid_np();
}
