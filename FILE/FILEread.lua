function FILEread(FileName)
   local File, Err = io.open(FileName, "r");
   if not File then
      return nil, err
   end
   local Content = File:read("*a");
   File:close()
   return Content
end
