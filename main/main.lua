require "COL/COLtrace"
require "FUNC/FUNCsplit"
require "FUNC/FUNCluaCppExtension"

  -- FUNCsplit(Arg[2]);
function main(Arg)
   FUNCluaCppExtension(Arg[2]);
end
