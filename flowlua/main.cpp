#include "MOD/MODlua.h"

#include "BAS/BASarg.h"
#include "BAS/BASstring.h"
#include "BAS/BASstream.h"
#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

#include "MOD/MODpathSet.h"


int main (int argc, char *argv[]) {
  BASstring Match;
  if (BASargFlagPresent("trace", &Match, argc, argv)){
     BASout << "Tracing: " << Match << newline;
     BAStrace(Match.data());
  }
  BASout << "Hello world of C++" << newline;

  return EXIT_SUCCESS;
}

