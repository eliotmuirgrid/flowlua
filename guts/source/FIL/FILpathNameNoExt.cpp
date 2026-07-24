#include <FIL/FILpathName.h>

#include <COL/COLstring.h>
#include <COL/COLtrace.h>
COL_TRACE_INIT;


COLstring FILpathNameNoExt(const COLstring& Path){
   COL_FUNCTION(FILpathNameWithoutLastExt);
   COLstring FileNameOnly = FILpathName(Path);
   int DotIndex = FileNameOnly.reverseFind('.');
   if(DotIndex < 0){
      return FileNameOnly; // no dot - return entire filename
   }
   COL_VAR(DotIndex);
   // got dot - return everything up to, but not including, the last dot
   return FileNameOnly.substr(0, DotIndex);
};
