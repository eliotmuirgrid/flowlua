require "COL/COLshowTable"

function COLshow(V, Indent)
   local Indent = Indent or '';
   if (type(V) == 'table') then return COLshowTable(V) end;
   return tostring(V); 
end
