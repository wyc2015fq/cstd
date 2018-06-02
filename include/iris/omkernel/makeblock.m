
clc
clear
m=64;
n=16;
z=0;
for i=1:8:360
    for j=1:2:30
        if (i+m)<=360&(j+n)<=30
            z=z+1;
        end
    end
end
