function CODEprefixRemainder(s)
   local prefix = ""
   local i = 1
   while i <= string.len(s) do
      local byte = string.byte(s, i) -- TODO put meta table into Lua for strings.
      if byte >= 65 and byte <= 90 then -- ASCII A-Z
         prefix = prefix .. string.sub(s, i,i)
         i = i + 1
      else
         break
      end
   end
   return prefix, string.sub(s, i)
end
