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

#include <dirent.h>

#include "COL/COLarray.h"
#include "COL/COLdictSorted.h"

#include "COL/COLtrace.h"
COL_TRACE_INIT;

COLstring FILdirCurrent(){
   COL_FUNCTION(FILdirCurrent);
   COLstring Result;
   char* pCwd = getcwd(NULL, 0);
   if (pCwd) {
      Result = pCwd;
      free(pCwd);
   }
   COL_VAR(Result);
   return Result;
}

#define MAX_NESTED_DEPTH 256

static bool FILisThisAlpha(const char C){
   return ((0x41 <= C && C <= 0x5a) || (0x61 <= C && C <= 0x7A));
}

static bool FILcheckDirDepthLimit(int DirectoryDepth){
   // COL_FUNCTION(FILcheckDirDepthLimit);      
   if (DirectoryDepth >= MAX_NESTED_DEPTH){                     
      COL_TRC("Directory depth exceeds 256");
      return false;                      
   } else if (DirectoryDepth < 0){
      COL_TRC("Directory depth below 0");
      return false;
   }                 
   return true;              
}

bool FILpathSimplify(COLstring* pPath) {
   COL_FUNCTION(FILpathSimplify);
   COLstring& Path = *pPath;
   COL_VAR(Path);
   // This function will attempt to 'simplify' a path via the following methods:
   //   * Stripping out any './' which exist
   //   * Removing any '../' which are resolveable
   //   * Removing any extraneous slashes (i.e. 'foo//bar')
   //
   // The algorithm itself is pretty much what would happen if you did it by
   // hand. It works its way through the string, and instead of using substring
   // to cut out extraneous directories, it just moves the write pointer back 
   // and overwrites them. 

   const char* pInput = Path.data();

   // This holds our output. It must be big enough for us to have the entire 
   // Path in it, and pre-allocating means we can pretty much treat it as
   // a big block of data.
   COLstring Buffer;
   Buffer.setCapacity(Path.size() + 1);

   char* pOutputStart = Buffer.data();

   // This is our actual write pointer, which we move around as we pass over
   // the input.
   char* pOutput = pOutputStart;

   // Our stack of directories, which are just pointers within the output
   // Buffer.
   char* pDirectoryOffsets[MAX_NESTED_DEPTH];

   // How many directories are in the directory stack.
   int DirectoryDepth = 0;

   // The separator we wish to use. This code will use the 'proper' separators
   // when it identifies a Windows, *NIX or Samba absolute path, otherwise it 
   // uses the system default.
   char Separator = FIL_DIR_SEPARATOR[0];

   enum {
      ST_START,  // Beginning of a directory name, we're at the first char after the slash.
      ST_DATA,   // Any old directory name content which isn't something we specifically care about.
      ST_DOT,    // We've seen a single . at the start of a dir name so far
      ST_DOTDOT  // Two dots!
   } State = ST_START;

   // Figure out what kind of path we're dealing with here
   if (Path.size() >= 1 && 
       *pInput == '/')
   {
      COL_TRC("POSIX style absolute path.");
      // POSIX-style absolute path
      Separator = '/';
      *pOutput++ = *pInput++;
   }
   else if (Path.size() >= 2 &&
            ::memcmp(pInput, "\\\\", 2) == 0)
   {
      COL_TRC("Samba style absolute path.");
      // Samba-style absolute path
      Separator = '\\';
      ::memcpy(pOutput, pInput, 2);
      pOutput += 2;
      pInput += 2;
   }
   else if (Path.size() >= 3 && 
            FILisThisAlpha(pInput[0]) &&
            pInput[1]==':' &&
            (pInput[2]=='/' || pInput[2]=='\\')
         )
   {
      COL_TRC("Windows style absolute path.");
      // Windows-style absolute path
      Separator = '\\';
      pOutput[0] = pInput[0];
      pOutput[1] = pInput[1];
      pOutput[2] = Separator;
      //::memcpy(pOutput, pInput, 3);
      pOutput += 3;
      pInput += 3;
   }
   else
   {
      COL_TRC("Relative path.");
      // Relative path of some sort, we use the current platform's default
      // separator.
   }

   // Set up where our 'root' directory is.
   pDirectoryOffsets[DirectoryDepth++] = pOutput;

   while (*pInput)
   {
      // LastChar is the value we've seen. We move the pointers ahead before
      // we look at its value for non-separator cases, so it's more sane to 
      // have it in a variable.
      char LastChar = *pInput;
      switch (LastChar)
      {
         case '\\':
         case '/':
            LastChar = Separator;
            break;
         default:
            break;
      }

      pInput++;
      *pOutput++ = LastChar;

      switch (State)
      {
         case ST_START:
            switch (LastChar)
            {
               case '.':
                  State = ST_DOT;
                  break;
               case '\\':
               case '/':
                  // Since we're at the start of a directory name, this means 
                  // that we have multiple slashes, which are redundant and can
                  // be stripped out.
                  pOutput--;
                  State = ST_START;
                  break;
               default:
                  State = ST_DATA;
                  break;
            }
            break;
         case ST_DATA:
            switch (LastChar)
            {
               case '\\':
               case '/':
                  // We've got a non-special directory name, so we just keep
                  // going after adding our write position to the stack.
                  State = ST_START;
                  if (!FILcheckDirDepthLimit(DirectoryDepth)) { return false; }  // IX-4181 
                  pDirectoryOffsets[DirectoryDepth++] = pOutput;
                  break;
               default:
                  break;
            }
            break;
         case ST_DOT:
            switch (LastChar)
            {
               case '\\':
               case '/':
                  // We've seen a reference to the current directory, so we 
                  // rewind to the character after the previous slash.
                  if (!FILcheckDirDepthLimit(DirectoryDepth)) { return false; } // IX-4181
                  pOutput = pDirectoryOffsets[DirectoryDepth - 1];
                  State = ST_START;
                  break;
               case '.':
                  State = ST_DOTDOT;
                  break;
               default:
                  State = ST_DATA;
                  break;
            }
            break;
         case ST_DOTDOT:
            switch (LastChar)
            {
               case '\\':
               case '/':
                  // We've seen a reference to the parent directory. Make sure
                  // that we have enough directories to be able to resolve the
                  // parent and back ourselves up.
                  if (DirectoryDepth >= 2)
                  {
                     pOutput = pDirectoryOffsets[(--DirectoryDepth) - 1];
                  }
                  State = ST_START;
                  break;
               default:
                  State = ST_DATA;
                  break;
            }
            break;
      }
   }
   
   // Make sure we handle a .. or . at the end of a string without a trailing
   // slash correctly.
   switch (State)
   {
      case ST_DOT:
         if (!FILcheckDirDepthLimit(DirectoryDepth)) { return false; } // IX-4181
         pOutput = pDirectoryOffsets[--DirectoryDepth];
         break;
      case ST_DOTDOT:
         if (DirectoryDepth >= 2)
         {
            pOutput = pDirectoryOffsets[(--DirectoryDepth) - 1];
         }
         break;
      default:;
   }

   // Not NULL-terminated, use the special constructor.
   COLstring SimplePath(pOutputStart, pOutput - pOutputStart);
   COL_VAR(SimplePath);
   Path = SimplePath; 
   return true;
}

bool FILdirList(const COLstring& Dir,COLdictSorted<COLstring, COLfile>* pList){
   DIR* dir = opendir(Dir.data());

   if (!dir) { return false; }

   struct dirent* ent;

   while ((ent = readdir(dir)) != NULL) {
      pList->add(ent->d_name, COLfile()); 
   }
   closedir(dir);
   return true;
}

