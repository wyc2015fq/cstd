# matlab 素描 code - 家家的专栏 - CSDN博客





2014年04月21日 20:55:46[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1718








                
clear all

clc

I = imread('IMG_4374.JPG');  

imggray = rgb2gray(I);  

N = 255-imggray;     

h1=fspecial('gaussian',3,3);  

g=imfilter(N,h1,'same');  

height=size(I,1);

width=size(I,2);

for i=1:height  

    for j=1:width  

        b = double(g(i,j));  

        a = double(imggray(i,j));  

        temp = a+a*b/(255-b);  

        out(i,j)=min(temp,255);  

    end  

end  

imshow(out/255);  




![](https://img-blog.csdn.net/20140421205507000)

![](https://img-blog.csdn.net/20140421205513640)





转自：http://blog.csdn.net/abcd1992719g/article/details/24200551






