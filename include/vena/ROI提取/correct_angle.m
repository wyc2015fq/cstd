function rotaimg=correct_angle(shuru)
%%%%%%%%将倾斜图像根据倾斜角校正%%%%%输入：图像边缘%%%
if ndims(shuru)==3
        shuru=rgb2gray(shuru);
end 
% [mm nn]=size(shuru);
% if mm<nn
%     shuru=rot90(shuru);
% end
%  shuru=shuru(40:300,:);%%利用矩形框框定图片
%%均值滤波
% h=fspecial('average'); 
% shuru1=filter2(h,shuru); 
% shuru1=medfilt2(shuru); %%中值滤波
%边缘检测 
BW2=edge(shuru1);
se=strel('disk',2); 
BW2=imclose(BW2,se);%闭运算 
%BW2=bwmorph(BW2,'thin',Inf);%骨骼化 
BW2=lvboo(BW2,100);%去出噪声 
%%求倾斜角，并校正图片（纠正倾斜角）%%%%%%%
angle=radon(BW2);%利用radon变换求倾斜角
if abs(angle)>1
% rotaimg=imrotate(shuru,-angle,'bilinear','crop');
rotaimg=imrotate(shuru,-angle,'bilinear','crop');  %%角度纠正
 else
     rotaimg=shuru;
end

