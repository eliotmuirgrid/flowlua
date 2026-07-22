#include <FIL/FILpathName.h>

#include <BAS/BASstring.h>
#include <BAS/BAStrace.h>
BAS_TRACE_INIT;


BASstring FILpathNameNoExt(const BASstring& Path){
   BAS_FUNCTION(FILpathNameWithoutLastExt);
   BASstring FileNameOnly = FILpathName(Path);
   int DotIndex = FileNameOnly.reverseFind('.');
   if(DotIndex < 0){
      return FileNameOnly; // no dot - return entire filename
   }
   BAS_VAR(DotIndex);
   // got dot - return everything up to, but not including, the last dot
   return FileNameOnly.substr(0, DotIndex);
};
