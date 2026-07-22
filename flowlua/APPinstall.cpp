
#include "FIL/FILdir.h"

#include "COL/COLdictSorted.h"
#include "COL/COLstring.h"
#include "COL/COLstream.h"
#include "COL/COLtrace.h"
COL_TRACE_INIT;

void APPinstall(){
   COL_FUNCTION(APPinstall);
   COLdictSorted<COLstring, COLfile> List;
   FILdirList("code/commands", &List);
   for (auto i=List.begin(); i != List.end(); ++i){
      COL_TRC(i.key());
   }
}

