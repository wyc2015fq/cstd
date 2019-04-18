# 霍夫变换hough、houghpeaks、houghlines - 心纯净，行致远 - CSDN博客





2018年04月15日 14:46:57[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：369








霍夫变换在图像处理里常用来在黑白图像里检测直线，matlab里有相应的几个函数，使用方便，这里把matlab帮助里介绍的例子演示一下。

    matlab里霍夫变换主要包含一下三个函数：

hough：实现霍夫变换，得到霍夫变换矩阵，用法如下

> 

[H, theta, rho] = hough(BW)

[H, theta, rho] = hough(BW, ParameterName,ParameterValue)



houghpeaks：在霍夫变换矩阵里找极值点

> 

peaks = houghpeaks(H, numpeaks)

peaks = houghpeaks(..., param1, val1,param2, val2)



houghlines：从霍夫变换矩阵中提取线段

> 

lines = houghlines(BW, theta, rho,peaks)

lines = houghlines(..., param1, val1,param2, val2)



    下面以一个例子来看看霍夫变换的效果，代码如下


```
% 测试霍夫变换
clc
clear
close all 
% 读取图像
I  = imread('circuit.tif');
rotI = imrotate(I,33,'crop'); % 旋转33度，保持原图片大小
fig1 = imshow(rotI);
% 提取边
BW = edge(rotI,'canny');
figure, imshow(BW); 
% 霍夫变换
[H,theta,rho] = hough(BW); % 计算二值图像的标准霍夫变换，H为霍夫变换矩阵，theta,rho为计算霍夫变换的角度和半径值
figure, imshow(imadjust(mat2gray(H)),[],'XData',theta,'YData',rho,...
    'InitialMagnification','fit');
xlabel('\theta (degrees)'), ylabel('\rho');
axis on, axis normal, hold on;
colormap(hot) 
% 显示霍夫变换矩阵中的极值点
P = houghpeaks(H,5,'threshold',ceil(0.3*max(H(:)))); % 从霍夫变换矩阵H中提取5个极值点
x = theta(P(:,2));
y = rho(P(:,1));
plot(x,y,'s','color','black'); 
% 找原图中的线
lines = houghlines(BW,theta,rho,P,'FillGap',5,'MinLength',7);
figure, imshow(rotI), hold on
max_len = 0;
for k = 1:length(lines)
    % 绘制各条线
    xy = [lines(k).point1; lines(k).point2];
    plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');   
    % 绘制线的起点（黄色）、终点（红色）
    plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
    plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');   
    % 计算线的长度，找最长线段
    len = norm(lines(k).point1 - lines(k).point2);
    if ( len > max_len)
        max_len = len;
        xy_long = xy;
    end
end
% 以红色线高亮显示最长的线
plot(xy_long(:,1),xy_long(:,2),'LineWidth',2,'Color','red');
```




