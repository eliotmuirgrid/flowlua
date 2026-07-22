function PATHfile(File)
    -- Remove directory path
    local filename = string.gsub(File, "^.*/", "")  -- Keeps only the file name
    -- Remove extension
    local name = string.gsub(filename, "%.[^.]*$", "")  -- Removes the last extension (if any)
    return name
end
