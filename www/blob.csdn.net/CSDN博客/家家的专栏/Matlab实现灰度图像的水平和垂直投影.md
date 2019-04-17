# Matlab实现灰度图像的水平和垂直投影 - 家家的专栏 - CSDN博客





2015年12月15日 19:17:52[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：9249
个人分类：[图像处理算法](https://blog.csdn.net/yihaizhiyan/article/category/715109)








clear all
clc
close all
%% 读入图像数据
I=imread('2.bmp');
% I=rgb2gray(I);
[m n]=size(I);
% 求垂直投影
for y=1:n
 S(y)=sum(I(1:m,y));
end
y=1:n;
figure
subplot(211),plot(y,S(y));
title('垂直投影');
% 求水平投影
for x=1:m
 S(x)=sum(I(x,:));
end
x=1:m;
subplot(212),plot(x,S(x));
title('水平投影');


