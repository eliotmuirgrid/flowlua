#ifndef _BAS_ERROR_
#define _BAS_ERROR_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASerror
// 
// Basic cross platform function to translating operating
// system error codes into the description given by the operating system.
//-------------------------------------------------------

class BASstring;

// This returns the error code from the operating system translation table.  May be quite incomprehensible :-)
BASstring BASerrorMessage(int ErrorCode);

#endif
