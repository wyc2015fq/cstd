%%%%定序核为3个极子，距离为10
clear all;
clc;
delta=1.7;  %尺度
dis=10;     %距离
width=31;
z=makekernel(width,-dis,0,delta);
z1=makekernel(width,0,0,delta);
z2=makekernel(width,dis,0,delta);
ordifilter=z-2*z1+z2;
save OM3 ordifilter;
%%%%提取特征%%示例程序%%
load OM3;
im=imread('1.bmp');
fimg=makeom(im,ordifilter); %提取定序特征图
aa=ordifilter(16, :);
bb=ordifilter./(ones(31, 1)*aa);
round([bb(:,16)';aa]*(2^8))
imshow(fimg);
