-- TODO require cannot handle circular requires.

function COLshowTable(V)
   local R = ''
   R = "{\n";
   for k, v in pairs(V) do
      R=R..tostring(k).." = "..COLshow(v).."\n";
   end
   R = R.."}\n";
   return R;
end
