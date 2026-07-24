require "URL/URLopen"

function MUSICplay(Args)
   COL_VAR(Args)
   
   local Url = "https://open.spotify.com/search/query="..table.concat(Args, " ", 3);
   COL_VAR(Url)
   URLopen(Url);
end
