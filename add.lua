
local a,b=...
local a1=tonumber(a)
local b1=tonumber(b)
if not a1 then error(string.format("arg1 ('%s') is not a number!", a)) end
if not b1 then error(string.format("arg2 ('%s') is not a number!", b)) end
print(a1+b1)
