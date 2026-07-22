#include "COL/COLtrace.h"
COL_TRACE_INIT


COLstring FILpathName(const COLstring& Path) {
   COL_FUNCTION(FILpathName);
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
            return COLstring(pRead, pEnd - pRead);
      }
   }
   return Path;
}
