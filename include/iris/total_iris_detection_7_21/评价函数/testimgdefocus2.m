clear 
clc
close all
%读取图片
I = imread('D:\pub\bin\iris\评价函数\1.jpg');%清晰
if ndims(I)==3    
    I=rgb2gray(I);
end
I=double(I);
%得到卷积核
a=ones(5,5)*(-1);
b=ones(3,3)*2;
b(2,2)=0;
a(2:4,2:4)=b;
%图片卷积滤波
Img_n1 = conv2(I,a,'valid');
Img_n11=uint8(Img_n1);
Img_n11=abs(Img_n1);
score1=mean2(Img_n11)
%图片灰度缩放便于后面计算得分
%Img_n1=Img_n1(3:end-3, 3:end-3);
%img1=mat2gray(Img_n1);
%计算高频能力得分
%score1=mean2(img1)
%score3>score1>score2;表明越清晰的图片分数越大.通过实验可以得到阈值
% figure;imshow(Img_n1,[]);title('卷积运算图')
