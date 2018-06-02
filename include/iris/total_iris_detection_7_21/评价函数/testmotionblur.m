%测试运动模糊
clear
close all
clc
%读取图片
I1 = imread('pic12.jpg');
if ndims(I1)==3    
    I1=rgb2gray(I1);
end
I1=double(I1);
I2 = imread('pic12M.jpg');
if ndims(I2)==3    
    I2=rgb2gray(I2);
end
I2=double(I2);
I3 = imread('pic11.jpg');
if ndims(I3)==3    
    I3=rgb2gray(I3);
end
I3=double(I3);
I4= imread('pic11M.jpg');
if ndims(I4)==3    
    I4=rgb2gray(I4);
end
I4=double(I4);
I5= imread('pic00607M.jpg');
if ndims(I5)==3    
    I5=rgb2gray(I5);
end
I5=double(I5);
%生成卷积核
b=ones(1,8)*-1;
c=ones(1,8);
a=[b;c];
%卷积滤波
Img_n1 = conv2(I1,a,'same');
Img_n2 = conv2(I2,a,'same');
Img_n3 = conv2(I3,a,'same');
Img_n4 = conv2(I4,a,'same');
Img_n5 = conv2(I5,a,'same');
%灰度缩放
img1=mat2gray(Img_n1);
img2=mat2gray(Img_n2);
img3=mat2gray(Img_n3);
img4=mat2gray(Img_n4);
img5=mat2gray(Img_n5);
%计算垂直高频得分
%模糊图像的分数较高，清晰图片的得分较低.
%清晰图片大概在0.47以下，模糊图片在0.48以上
score_motion1=mean2(img1);
score_motion2=mean2(img2);
score_motion3=mean2(img3);
score_motion4=mean2(img4);
score_motion5=mean2(img5);
%测试主动加模糊的图片通过卷积滤波后的分数
originalRGB = imread('peppers.png');
originalRGB=rgb2gray(originalRGB);
originalRGB=originalRGB-mean2(originalRGB);
h = fspecial('motion', 50, 45);
filteredRGB = imfilter(originalRGB, h);
filteredRGB=double(filteredRGB);
filteredRGB1 = conv2(filteredRGB, a,'same');
% filteredRGB1=rgb2gray(filteredRGB1);
It=mat2gray(filteredRGB1);
score=mean2(It);%score=0.49,
% figure, imshow(filteredRGB1)
