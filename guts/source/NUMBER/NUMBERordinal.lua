require "NUMBER/NUMBERmod"

function NUMBERordinal(Num)
   if Num == 11 or 
      Num == 12	or 
      Num == 13                then return "th" 
   end
   local  LastDigit =  NUMBERmod(Num, 10);
   if     LastDigit == 1       then return "st"
   elseif LastDigit == 2       then return "nd"
   elseif LastDigit == 3       then return "rd"
   else                             return "th"
   end
end
