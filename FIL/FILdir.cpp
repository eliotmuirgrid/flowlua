// ---------------------------------------------------------------------------
// Copyright (C) Eliot.  All Rights Reserved
//
// Module: FILdir
//
// Description:
//
// Implementation
//
// Author: Eliot Muir 
// Date:   Wednesday 01 July 2026 - 07:40PM
// ---------------------------------------------------------------------------

#include "FIL/FILdir.h"

#include "BAS/BASvector.h"

#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

BASstring FILdirCurrent(){
   BAS_FUNCTION(FILdirCurrent);
   BASstring Result;
   char* pCwd = getcwd(NULL, 0);
   if (pCwd) {
      Result = pCwd;
      free(pCwd);
   }
   BAS_VAR(Result);
   return Result;
}


BASstring FILpathSimplify(const BASstring& Path) {
   BAS_FUNCTION(FILpathSimplify);
   BAS_VAR(Path);

   const char* p = Path.data();
   const size_t n = Path.size();

   char Separator = '/';  // TODO - hope this works for WINDOZE
   BASstring Prefix;

   // Detect absolute path style.
   if (n >= 1 && p[0] == '/') {
      Separator = '/';
      Prefix = "/";
      p += 1;
   }
   else if (n >= 2 && ::memcmp(p, "\\\\", 2) == 0) {
      Separator = '\\';
      Prefix = "\\\\";
      p += 2;
   }
   else if (n >= 3 &&
            BASisThisAlpha(p[0]) &&
            p[1] == ':' &&
            (p[2] == '/' || p[2] == '\\')) {
      Separator = '\\';
      Prefix += p[0];
      Prefix += ':';
      Prefix += Separator;
      p += 3;
   }

   BASvector<BASstring> Parts;

   while (*p) {
      while (*p == '/' || *p == '\\') {
         ++p;
      }

      const char* Start = p;

      while (*p && *p != '/' && *p != '\\') {
         ++p;
      }

      if (p == Start) {
         continue;
      }

      BASstring Part(Start, p - Start);

      if (Part == ".") {
         continue;
      }

      if (Part == "..") {
         if (Parts.size() > 0) {
            Parts.pop();
         }
         else if (Prefix.size() == 0) {
            Parts.push(Part);
         }
         continue;
      }

      Parts.push(Part);
   }

   BASstring Result = Prefix;

   for (size_t i = 0; i < Parts.size(); ++i) {
      if (i > 0 || Result.size() > 0 && Result[Result.size() - 1] != Separator) {
         Result += Separator;
      }
      Result += Parts[i];
   }

   BAS_VAR(Result);
   return Result;
}
