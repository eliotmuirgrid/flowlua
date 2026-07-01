#ifndef _BAS_LOOP_
#define _BAS_LOOP_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASloop
// 
// Cross platform high efficiency event loop.
//-------------------------------------------------------

class BASloop{
public:
   BASloop();
   void start();  // blocks until we exit.
   void shutdown();
};

#endif
