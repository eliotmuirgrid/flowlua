require "COL/COLtrace"
require "FUNC/FUNCsplit"
require "FUNC/FUNCluaCppExtension"
require "MUSIC/MUSICplay"

local Commands={
  ["music:play"]=MUSICplay,
  ["add:clua"]=FUNCluaCppExtension,
  ["code:function:split"]=FUNCsplit
}

function main(Arg)
   COL_VAR(Arg)
   if not Commands[Arg[2]] then
      print "Please use a supported command:"
      for K, _ in pairs(Commands) do
         print("   "..K);
      end
      return;
   end
   Commands[Arg[2]](Arg);
end
