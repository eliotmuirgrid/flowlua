#ifndef _BAS_THREAD_
#define _BAS_THREAD_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASthread
// 
// Minimal cross platform wrapper around threading APIs.
//
// Leaves most of the rest of the work to the application
// writer to decide what ever else they wany to do with 
// this API
//-------------------------------------------------------

typedef void (*BASthreadProc)(void* pData); 

void BASthread(BASthreadProc pProcedure, void* pData);

#endif
