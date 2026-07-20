-- git.publish
Command = function(Args)
   local Comment;
   if table.getn(Args) > 0 then
       Comment = table.concat(Args, ' ') or "Update"
   else
       Comment = "Update"
   end
   print ("git:publish "..Comment);
end

return Command
