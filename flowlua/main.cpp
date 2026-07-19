#include "MOD/MODlua.h"

#include "BAS/BASargFlagPresent.h"
#include "BAS/BASstring.h"
#include "BAS/BASstream.h"
#include "BAS/BASarray.h"
#include "BAS/BASarrayCopy.h"
#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

#include "MOD/MODpathSet.h"

void APPhello(BASstream& Stream, const BASarray<BASstring>& Args){
   BAS_FUNCTION(APPhello);
   BAS_VAR(Args);
   Stream << "Hello world!" << newline;
}

int main (int argc, const char** argv) {
  BAS_FUNCTION(main);
  BASstring Match;
  BASarray<BASstring> Args;
  BASarrayCopy(argc, argv, &Args);

  if (BASargFlagPresent("trace", &Match, &Args)){
     BAStrace(Match.data());
  }
  APPhello(BASout, Args);

  return 0;  // 0 means success.  Nothing is success apparently.
}

