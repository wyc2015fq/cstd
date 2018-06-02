function [BW]=edge(shuru1)
% clear;
% clc;
% shuru1=imread('a.bmp');
shuru1=double(shuru1);
y_mask=[-1 0 1;-2 0 2;-1 0 1];%垂直方向
x_mask=y_mask';
dx=imfilter(shuru1,x_mask);
dy=imfilter(shuru1,y_mask);
grad=sqrt(dx.*dx+dy.*dy); %计算梯度
grad=mat2gray(grad);
level=graythresh(grad);
BW=im2bw(grad,level);
%  BW=guge(BW);
[mm,nn]=size(BW);
BW(1,:)=0;
BW(:,1)=0;
BW(mm,:)=0;
BW(:,nn)=0;
[r,c]=find(BW);

BW=xihua(BW);%骨骼化