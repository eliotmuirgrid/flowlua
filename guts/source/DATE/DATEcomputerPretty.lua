function DATEcomputerPretty(Time)
    local t = Time or os.time()
    return string.format("0x%X", t)
end

