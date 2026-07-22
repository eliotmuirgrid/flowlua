//-------------------------------------------------------
// Copyright (C) Forever Eliot Muir.  All Rights Reserved
//
// COLthread
//-------------------------------------------------------

#include <libc/thread/thread.h>

int COLthreadId(){
   return pthread_getthreadid_np();
}
