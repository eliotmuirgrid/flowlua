#include <COL/COLstring.h>
#include <COL/COLtrace.h>
COL_TRACE_INIT


COLstring FILdirCurrent(){
   COL_FUNCTION(FILdirCurrent);
   COLstring Result;
   char* pCwd = getcwd(NULL, 0);
   if (pCwd) {
      Result = pCwd;
      Result += "/";
      free(pCwd);
   }
   COL_VAR(Result);
   return Result;
}

