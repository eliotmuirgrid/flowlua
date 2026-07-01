#ifndef _BAS_FILE_IMP_H_
#define _BAS_FILE_IMP_H_
//-------------------------------------------------------
// Copyright (C) 2021 Eliot Muir.  All Rights Reserved
//
// BASfileImp
// 
// Defines for file implementation
//-------------------------------------------------------

#ifdef _WIN32
   #include <io.h>
   #define BASfsync _commit
   #define BASwrite _write
   #define BASopen _open
   #define BASseek _lseeki64
#else
   #include <unistd.h>
   #define BASfsync ::fsync
   #define BASwrite ::write
   #define BASopen  ::open
   #define BASseek  ::lseek
#endif

#ifdef _WIN32
  #define O_RDONLY _O_RDONLY
  #define S_IREAD _S_IREAD
#endif

#endif
