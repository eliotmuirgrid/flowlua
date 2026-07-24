function FILErename(Old, New)
   print("Rename '"..Old.."' to '"..New.."'")
   local ok, err = os.rename(Old, New)
   if ok then
      print("Renamed '"..Old.."' to '"..New.."'")
   else
      print("Failed to rename '"..Old.."' to '"..New.."': "..err)
   end
end
