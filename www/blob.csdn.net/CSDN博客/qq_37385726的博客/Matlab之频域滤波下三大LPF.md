# Matlab之频域滤波下三大LPF - qq_37385726的博客 - CSDN博客





2018年09月14日 18:46:32[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：565








# **目录**

[一、初始知识](#%E4%B8%80%E3%80%81%E5%88%9D%E5%A7%8B%E7%9F%A5%E8%AF%86)

[二、理想LPF](#%E4%BA%8C%E3%80%81%E7%90%86%E6%83%B3LPF)

[三、高斯LPF](#%E4%B8%89%E3%80%81%E9%AB%98%E6%96%AFLPF)

[四、巴特沃斯（Butterworth）LPF](#%E5%9B%9B%E3%80%81%E5%B7%B4%E7%89%B9%E6%B2%83%E6%96%AF%EF%BC%88Butterworth%EF%BC%89LPF)

[五、滤波器和频谱的卷积](#%E4%BA%94%E3%80%81%E6%BB%A4%E6%B3%A2%E5%99%A8%E5%92%8C%E9%A2%91%E8%B0%B1%E7%9A%84%E5%8D%B7%E7%A7%AF)

[六、完整代码](#%E3%80%81%E5%AE%8C%E6%95%B4%E4%BB%A3%E7%A0%81)



Python中OpenCV2下实现参见[这里](https://blog.csdn.net/qq_37385726/article/details/82697648)

## 一、初始知识
- 关于 distmatrix 的作用，见[https://blog.csdn.net/qq_37385726/article/details/82697648](https://blog.csdn.net/qq_37385726/article/details/82697648)，看源代码可以知道函数的意义。
- 但是，不懂函数的意义也没关系，只要记住，**fftshift(distmatrix)生成的是各个坐标到中心坐标的距离**即可

```python
[M,N] = size(tif);
D = distmatrix(M,N);  
dist = fftshift(D);   %距离矩阵（距中心）

figure
subplot(1,2,1),mesh(D),title('distmatrix');
subplot(1,2,2),mesh(fftshift(D)),title('fftshift(distmatrix)');
```

![](https://img-blog.csdn.net/20180914183654698?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 关于傅里叶变换，知道频域滤波就知道傅里叶变换实现的是时域（空域）到频率域的转变，对于2D傅里叶变换，坐标（1，1） %Python中为（0，0）%是频率为0的直流信号，坐标（u,v）是在x方向频率为u，y方向频率为v的信号



## 二、理想LPF

![](https://img-blog.csdn.net/20180914183806594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由于生硬的过度，图像会产生振铃伪影。

```python
% idea LPF
H1 = zeros(M,N);
radius = 35;
ind = dist <= radius;
H1(ind) = 1;
Hd1 = double(H1);
```

![](https://img-blog.csdn.net/20180914184502915?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





## 三、高斯LPF

![](https://img-blog.csdn.net/20180914184014701?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180914184042565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/2018091418405299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
% Gaussion LPF
sigma = 30;
H2 = exp(-dist.^2/2/sigma^2);
Hd2 = double(H2);
```

## ![](https://img-blog.csdn.net/20180914184516412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180914184527647?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







## 四、巴特沃斯（Butterworth）LPF

![](https://img-blog.csdn.net/20180914183919118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从函数图上看，更圆滑，用幂系数n可以改变滤波器的形状。n越大，则该滤波器越接近于理想滤波器

![](https://images2018.cnblogs.com/blog/1308890/201803/1308890-20180318213537364-35648855.png)![](https://images2018.cnblogs.com/blog/1308890/201803/1308890-20180318213615439-907127474.png)

```python
% Butterworth LPF
D0 = 30;
n = 3;
H3 = 1./(1+(dist/D0).^(2*n));
Hd3 = double(H3);
```

![](https://img-blog.csdn.net/20180914184543866?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180914184554300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





## 五、滤波器和频谱的卷积

```
I_dft = fft2(tif);   %原频谱
fts = fftshift(I_dft);  %shift后的频谱
ft_img1 = fts .* Hd1;
ft_img2 = fts .* Hd2;
ft_img3 = fts .* Hd3;
```

## 六、完整代码

```python
tif = imread('C:\\Program Files\\MATLAB\\R2017a\\toolbox\\images\\imdata\\AT3_1m4_01.tif');
tif = im2double(tif);
I_dft = fft2(tif);   %原频谱
fts = fftshift(I_dft);  %shift后的频谱

[M,N] = size(tif);
D = distmatrix(M,N);  
dist = fftshift(D);   %距离矩阵（距中心）

% idea LPF
H1 = zeros(M,N);
radius = 35;
ind = dist <= radius;
H1(ind) = 1;
Hd1 = double(H1);

% Gaussion LPF
sigma = 30;
H2 = exp(-dist.^2/2/sigma^2);
Hd2 = double(H2);

% Butterworth LPF
D0 = 30;
n = 3;
H3 = 1./(1+(dist/D0).^(2*n));
Hd3 = double(H3);


ft_img1 = fftshift(I_dft) .* Hd1;
ft_img2 = fftshift(I_dft) .* Hd2;
ft_img3 = fftshift(I_dft) .* Hd3;

subplot(1,4,1),imshow(tif),axis tight,title('initial');
subplot(1,4,2),imshow(real(ifft2(fftshift(ft_img1)))),axis tight,title('Ideal LPF');
subplot(1,4,3),imshow(real(ifft2(fftshift(ft_img2)))),axis tight,title('Guassion LPF');
subplot(1,4,4),imshow(real(ifft2(fftshift(ft_img3)))),axis tight,title('Butterworth LPF');
```

![](https://img-blog.csdn.net/20180914184623829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

三类滤波器mesh后的形状：

![](https://img-blog.csdn.net/20180914185839770?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



频谱的变化如下：



![](https://img-blog.csdn.net/20180914185531648?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



