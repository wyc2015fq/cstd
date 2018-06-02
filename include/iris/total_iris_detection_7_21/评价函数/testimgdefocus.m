clear 
clc
close all
%读取图片
I = imread('1.bmp');%清晰
if ndims(I)==3    
    I=rgb2gray(I);
end
I=double(I);%满足卷积运算需要
I1 = imread('2.bmp');%有散焦
if ndims(I1)==3    
    I1=rgb2gray(I1);
end
I1=double(I1);%卷积运算要求为double
I2 = imread('pic11.jpg');
if ndims(I2)==3    
    I2=rgb2gray(I2);
end
I2=double(I2);
%得到卷积核
a=ones(5,5)*(-1);
b=ones(3,3)*2;
b(2,2)=0;
a(2:4,2:4)=b;
%图片卷积滤波
Img_n1 = conv2(I,a,'same');
Img_n2 = conv2(I1,a,'same');
Img_n3 = conv2(I2,a,'same');
%图片灰度缩放便于后面计算得分
img1=mat2gray(Img_n1);
img2=mat2gray(Img_n2);
img3=mat2gray(Img_n3);
%计算高频能力得分
score1=mean2(img1);
score2=mean2(img2);
score3=mean2(img3);
%score3>score1>score2;表明越清晰的图片分数越大.通过实验可以得到阈值
% figure;imshow(Img_n1,[]);title('卷积运算图')
% figure;imshow(Img_n2,[]);title('卷积运算图')
% figure;imshow(Img_n3,[]);title('卷积运算图')