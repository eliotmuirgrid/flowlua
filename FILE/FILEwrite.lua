function FILEwrite(FileName, Content)
    local F = io.open(FileName, "w") -- open the file for writing ("w" mode)
    if not F then
        return false, "Error opening file"
    end
    F:write(Content)
    F:close()
    return true
end
