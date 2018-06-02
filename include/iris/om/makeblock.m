
%clc
%%clear 分块提取定序特征
function result=makeblock(im)
%im的大小为360*30，分割的图像小块大小为64*16
m=64;
n=16;
z=0;
result=[];
for i=1:8:360
    for j=1:2:30
        if (i+m)<=360&(j+n)<=30
            z=z+1;
            subimg=im(j:(j+n-1),i:(i+m-1));
            result=[result;sum(subimg(:))];
        end
    end
end

