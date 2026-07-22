require 'CODE/CODEprefixRemainder'
require 'CODE/CODEchangePrefix'
require 'CODE/CODEroot'
require 'PATH/PATHfile'


function CODErenamePrefix(Old, New)
   local Dir    = CODEroot()..Old.."/"
   local NewDir = CODEroot()..New.."/"

   os.execute("mkdir -p "..NewDir);

   print(Dir.." change prefix to "..New);
   local List = dir(Dir);
   for i=1, table.getn(List) do
      local File = PATHfile(List[i]);
      if (Old == CODEprefixRemainder(File)) then
         CODEchangePrefix(List[i], Old, New);
      end
   end	
end	
