# MATLAB图像增强程序举例 - maopig的专栏 - CSDN博客
2012年04月28日 13:41:32[maopig](https://me.csdn.net/maopig)阅读数：12959

## **1.灰度变换增强程序：**
% GRAY TRANSFORM
clc;
I=imread('pout.tif');
imshow(I);
J=imadjust(I,[0.3 0.7],[0 1],1);  %transforms the walues in the %intensity image I to values in J by linealy mapping %values between 0.3 and 0.7 to values between 0 and 1.
figure;
imshow(J);
J=imadjust(I,[0.3 0.7],[0 1],0.5);  % if GAMMA is less than 1,the  mapping si weighted toward higher (brighter)
%output values.
figure;
imshow(J);
J=imadjust(I,[0.3 0.7],[0 1],1.5);  % if GAMMA is greater than 1,the mapping si weighted toward lower (darker)
%output values.
figure;
imshow(J)
J=imadjust(I,[0.3 0.7],[0 1],1);  % If TOP<BOTTOM,the output image is reversed,as in a photographic negative.
figure;
imshow(J);
## 2.直方图灰度变换
%直方图灰度变换
[X,map]=imread('forest.tif');
I=ind2gray(X,map);%把索引图像转换为灰度图像
imshow(I);
title('原图像');
improfile%用鼠标选择一条对角线，显示线段的灰度值
figure;subplot(121)
plot(0:0.01:1,sqrt(0:0.01:1))
axis square
title('平方根灰度变换函数')
subplot(122)
maxnum=double(max(max(I)));%取得二维数组最大值
J=sqrt(double(I)/maxnum);%把数据类型转换成double,然后进行平方根变换
%sqrt函数不支持uint8类型
J=uint8(J*maxnum);%把数据类型转换成uint8类型
imshow(J)
title('平方根变换后的图像')
## 3.直方图均衡化程序举例
% HISTGRAM EAQUALIZATION
clc;
% Clear command window
I=imread('tire.tif');
% reads the image in tire.tif into I
imshow(I);
% displays the intensity image I with 256 gray levels
figure;
%creates a new figure window
imhist(I);
% displays a histogram for the intensity image I
J=histeq(I,64);
% transforms the intensity image I,returning J an intensity
figure;
%image with 64 discrete levels
imshow(J);
figure;
imhist(J);
J=histeq(I,32);
%transforms the intensity image ,returning in % J an intensity
figure;
%image with 32 discrete levels
imshow(J);
figure;
imhist(J);
## 4.直方图规定化程序举例
% HISTGRAM REGULIZATION
clc;
%Clear command window
I=imread('tire.tif');
%reads the image in tire.tif into I
J=histeq(I,32);
%transforms the intensity image I,returning in
%J an intensity image with 32 discrete levels
[counts,x]=imhist(J);
%displays a histogram for the intensity image I
Q=imread('pout.tif');
%reads the image in tire.tif into I
figure;
imshow(Q);
figure;
imhist(Q);
M=histeq(Q,counts);
%transforms the intensity image Q so that the
%histogram of the output image M approximately matches counts
figure;
imshow(M);
figure;
imhist(M);
## 空域滤波增强部分程序
**1.线性平滑滤波**
I=imread('eight.tif');
J=imnoise(I,'salt & pepper',0.02);
subplot(221),imshow(I)
title('原图像')
subplot(222),imshow(J)
title('添加椒盐噪声图像')
K1=filter2(fspecial('average',3),J)/255;%应用3*3邻域窗口法
subplot(223),imshow(K1)
title('3x3窗的邻域平均滤波图像')
K2=filter2(fspecial('average',7),J)/255;%应用7*7邻域窗口法
subplot(224),imshow(K2)
title('7x7窗的邻域平均滤波图像')
**2.中值滤波器**
MATLAB中的二维中值滤波函数medfit2来进行图像中椒盐躁声的去除
%IMAGE NOISE REDUCTION WITH MEDIAN FILTER
clc;
hood=3;%滤波窗口
[I,map]=imread('eight.tif');
imshow(I,map);
noisy=imnoise(I,'salt & pepper',0.05);
figure;
imshow(noisy,map);
filtered1=medfilt2(noisy,[hood hood]);
figure;
imshow(filtered1,map);
hood=5;
filtered2=medfilt2(noisy,[hood hood]);
figure;
imshow(filtered2,map);
hood=7;
filtered3=medfilt2(noisy,[hood hood]);
figure;
imshow(filtered3,map);
**3. 4邻域8邻域平均滤波算法**% IMAGE NOISE REDUCTION WITH MEAN ALGORITHM
clc;
[I,map]=imread('eight.tif');
noisy=imnoise(I,'salt & pepper',0.05);
myfilt1=[0 1 0;1 1 1;0 1 0];%4邻域平均滤波模版
myfilt1=myfilt1/9;%对模版归一化
filtered1=filter2(myfilt1,noisy);
imshow(filtered1,map);
myfilt2=[1 1 1;1 1 1;1 1 1];
myfilt2=myfilt2/9;
filtered2=filter2(myfilt2,noisy);
figure;
imshow(filtered2,map);
## 频域增强程序举例
**1.低通滤波器**% LOWPASS FILTER
clc;
[I,map]=imread('eight.tif');
noisy=imnoise(I,'gaussian',0.05);
imshow(noisy,map);
myfilt1=[1 1 1;1 1 1;1 1 1];
myfilt1=myfilt1/9;
filtered1=filter2(myfilt1,noisy);
figure;
imshow(filtered1,map);
myfilt2=[1 1 1;1 2 1;1 1 1];
myfilt2=myfilt2/10;
filtered2=filter2(myfilt2,noisy);
figure;
imshow(filtered2,map);
myfilt3=[1 2 1;2 4 2; 1 2 1];
myfilt3=filter2(myfilt3,noisy);
figure;
imshow(filtered3,map);
**2.布特沃斯低通滤波器图像实例**
I=imread('saturn.png');
J=imnoise(I,'salt & pepper',0.02);
subplot(121),imshow(J)
title('含噪声的原图像')
J=double(J);
f=fft2(J);
g=fftshift(f);
[M,N]=size(f);
n=3;d0=20;
n1=floor(M/2);n2=floor(N/2);
for i=1:M;
for j=1:N;
d=sqrt((i-n1)^2+(j-n2)^2);
h=1/(1+0.414*(d/d0)^(2*n));
g(i,j)=h*g(i,j);
end
end
g=ifftshift(g);
g=uint8(real(ifft2(g)));
subplot(122),imshow(g)
title('三阶Butterworth滤波图像')
## 色彩增强程序举例
**1.真彩色增强实例：**%真彩色图像的分解
clc;
RGB=imread('peppers.png');
subplot(221),imshow(RGB)
title('原始真彩色图像')
subplot(222),imshow(RGB(:,:,1))
title('真彩色图像的红色分量')
subplot(223),imshow(RGB(:,:,2))
title('真彩色图像的绿色分量')
subplot(224),imshow(RGB(:,:,3))
title('真彩色图像的蓝色分量')
**2.伪彩色增强举例：**I=imread('cameraman.tif');
imshow(I);
X=grayslice(I,16);%thresholds the intensity image I using
%threshold values 1/16,2/16,…..,15/16,returning an indexed %image in X
figure;
imshow(X,hot(16));
**3.假彩色增强处理程序举例**
[RGB]=imread('ghost.bmp');
imshow(RGB);
RGBnew(:,:,1)=RGB(:,:,3);
RGBnew(:,:,2)=RGB(:,:,1);
RGBnew(:,:,3)=RGB(:,:,2);
figure;
subplot(121);
imshow(RGB);
subplot(122);
imshow(RGBnew);

