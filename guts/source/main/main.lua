require "COL/COLtrace"
require "FUNC/FUNCsplit"
require "FUNC/FUNCluaCppExtension"
require "MUSIC/MUSICplay"

  -- FUNCsplit(Arg[2]);
function main(Arg)
   FUNCluaCppExtension(Arg[2]);
   --MUSICplay(Arg)
end
