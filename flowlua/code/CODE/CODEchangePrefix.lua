require "FILE/FILErename"
require "CODE/CODEprefixRemainder"

function CODEchangePrefix(File, Old, New)
   local OldPrefix, Remainder = CODEprefixRemainder(File);
   local NewFile = New..Remainder;
   local From    = CODEroot()..Old.."/"..Old..Remainder;
   local To      = CODEroot()..New.."/"..New..Remainder;
   FILErename(From, To) 
end
