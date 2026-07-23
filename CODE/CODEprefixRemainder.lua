function CODEprefixRemainder(s)
   local prefix = ""
   local i = 1
   while i <= s:len()) do
      local byte = s:byte(i)
      if byte >= 65 and byte <= 90 then -- ASCII A-Z
         prefix = prefix..s:sub(i,i)
         i = i + 1
      else
         break
      end
   end
   return prefix, s:sub(i)
end
