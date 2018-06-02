clc;clear;close all;
chdir('F:\pub\bin\photo\yiwei');
Image=imread('Fotor_LomoOrg.png');
figure(1);
imshow(Image);

R=double(Image(:,:,1));
G=double(Image(:,:,2));
B=double(Image(:,:,3));

%调整参数
HighLight=238;
Shadow=159;
Midtones=0.51;

Diff=HighLight-Shadow;
rDiff=R-Shadow;
gDiff=G-Shadow;
bDiff=B-Shadow;

rDiff(rDiff<0)=0;
gDiff(gDiff<0)=0;
bDiff(bDiff<0)=0;

R=(rDiff/Diff).^(1/Midtones)*255;
G=(gDiff/Diff).^(1/Midtones)*255;
B=(bDiff/Diff).^(1/Midtones)*255;

R(R>255)=255;
G(G>255)=255;
B(B>255)=255;

img(:,:,1)=uint8(R);
img(:,:,2)=uint8(G);
img(:,:,3)=uint8(B);
figure(2);
imshow(img);
