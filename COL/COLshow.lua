require "COL/COLshowTable"

function COLshow(V)
   if (type(V) == 'table') then return COLshowTable(V) end;
   return tostring(V); 
end
