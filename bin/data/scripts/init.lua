local mainlib = require "ludo"

print("mainlib is:")
for k, v in pairs(mainlib) do
    print(k, v)
    if (type(v) == "table") then
        for sk, sv in pairs(v) do
            print(sk, sv)
        end
    end
end
print("Package path is: " .. package.path);

while 1 do

end
