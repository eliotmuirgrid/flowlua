-- TODO require cannot handle circular requires.

function COLshowTable(V, Indent)
   local R = ''
   local Indent = Indent or "";
   Indent = Indent..'  ';
   R = "{\n";
   for k, v in pairs(V) do
      R=R..Indent..tostring(k).." = "..COLshow(v, Indent).."\n";
   end
   R = R.."}\n";
   return R;
end
