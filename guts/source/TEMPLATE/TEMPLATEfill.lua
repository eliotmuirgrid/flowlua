function TEMPLATEfill(template, tokens)
    -- Replace all @@KEY@@ in the template with tokens[KEY]
    local result = template:gsub("@@([A-Z_]+)@@", function(key)
        return tostring(tokens[key]) or ("@@"..key.."@@")
    end)
    return result
end

