#pragma once
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// COLfileImp
// 
// Defines for file implementation
//-------------------------------------------------------

#ifdef _WIN32
   #include <io.h>
   #define COLfsync _commit
   #define COLwrite _write
   #define COLopen _open
   #define COLseek _lseeki64
#else
   #include <unistd.h>
   #define COLfsync ::fsync
   #define COLwrite ::write
   #define COLopen  ::open
   #define COLseek  ::lseek
#endif

#ifdef _WIN32
  #define O_RDONLY _O_RDONLY
  #define S_IREAD _S_IREAD
#endif

