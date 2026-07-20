
#include "FIL/FILdir.h"

#include "BAS/BASdictSorted.h"
#include "BAS/BASstring.h"
#include "BAS/BASstream.h"
#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

void APPinstall(){
   BAS_FUNCTION(APPinstall);
   BASdictSorted<BASstring, BASfile> List;
   FILdirList("code/commands", &List);
   for (auto i=List.begin(); i != List.end(); ++i){
      BAS_TRC(i.key());
   }
}

