require "TEST/TESThello"

function main(Arg)
   TESThello()
   local C = table.getn(Arg)

   if C < 2 then
      print "No command given"
      return;
   end

--[[   local ok, Command = pcall(require, "commands/"..Arg[2])

   if not ok then
      print(module);
      print("No such command:", Arg[2])
      return
   end
   table.remove(Arg,1)
   table.remove(Arg,1)
   Command(Arg);]]
end
