function angle=radoning(img)
%%利用radon内函数寻找倾斜角%输入为一个图像的边缘图edge%
theta=1:179;
[R,XP]=radon(img,theta);
[index,jj]=find(R>=max(R(:)));
if jj<=90
    angle=jj;
else
    angle=jj-180;
end


