require "COL/COLshow"

function COLtrace(T)
   print(T);
end	

function COL_VAR (V) COLtrace(COLshow(V))                                                       ; end	
function COL_VAR2(V1, V2) 
   COLtrace(COLshow(V1)..", "..COLshow(V2)); 
end	
--function COL_VAR3(V) COLtrace(COLshow(V)..", "..COLshow(V))..", "..COLshow(V))                ; end	


