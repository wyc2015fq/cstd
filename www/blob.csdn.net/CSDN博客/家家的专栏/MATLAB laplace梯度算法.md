# MATLAB laplace梯度算法 - 家家的专栏 - CSDN博客





2011年07月12日 17:05:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2946








                 首先，使用fspecial(T,P)产生一个laplace算子。
h1=fspecial('laplacian')%laplacian代表laplace滤波器
其次，进行滤波
A=imread('1.bmp');
 B=imfilter(A,h1);
subplot(1,2,1),imshow(A);
subplot(1,2,2),imshow(B);            


