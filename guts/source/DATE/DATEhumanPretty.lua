require "NUMBER/NUMBERordinal"

function DATEhumanPretty(Time)
   Time = Time or os.time()
   local Date = os.date("*t", Time)
   local R = os.date("%A", Time).." "..Date.day..NUMBERordinal(Date.day)..os.date(" %B %Y", Time)
   COL_VAR(R)
   return R;
end

