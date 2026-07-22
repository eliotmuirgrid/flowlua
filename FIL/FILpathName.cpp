#include "BAS/BAStrace.h"
BAS_TRACE_INIT


BASstring FILpathName(const BASstring& Path) {
   BAS_FUNCTION(FILpathName);
   const char* pBegin = Path.data();
   const char* pEnd = pBegin + Path.size();
   const char* pRead = pEnd;

   while (--pRead >= pBegin)
   {
      switch (*pRead)
      {
         case '\\':
         case '/':
            pRead++;
            return BASstring(pRead, pEnd - pRead);
      }
   }
   return Path;
}
