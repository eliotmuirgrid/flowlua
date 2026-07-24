require "FILE/FILEread"

function FUNCsplit(FileName)
   print("FileName="..FileName);
   local Content = FILEread(FileName);
   print("Content="..Content);


end
