# Matlab之只加盐噪声或只加椒噪声 - qq_37385726的博客 - CSDN博客





2018年09月15日 10:46:45[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：542








# **目录**

[一、关于imnoise](#%E4%B8%80%E3%80%81%E5%85%B3%E4%BA%8Eimnoise)

[二、只加椒噪声](#%E4%BA%8C%E3%80%81%E5%8F%AA%E5%8A%A0%E6%A4%92%E5%99%AA%E5%A3%B0)

[三、只加盐噪声](#%E4%B8%89%E3%80%81%E5%8F%AA%E5%8A%A0%E7%9B%90%E5%99%AA%E5%A3%B0)



## 一、关于imnoise

MATLAB内置函数**imnoise**中有属性**'salt & pepper'**，添加的是椒盐噪声。

注意**'salt & pepper'**中间的空格不能少，不然会报错。

![](https://img-blog.csdn.net/20180915103711529?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 二、只加椒噪声

我们无法通过内置的imnoise函数得到只加椒噪声的图像，我们需要手动给图片加椒噪声。

代码如下：

```python
% Pepper Noise
pepper_ind = randperm(uint16(numel(tif)),uint8(0.1*numel(tif)));
tif_pepper = tif1;  % tif1为uint8类型
tif_pepper(pepper_ind) = 0;
tif_pepper = im2double(tif_pepper);  %将得到的椒图像转化为double类型
```
- randperm(n,k)返回1：n的一维向量中k个随机不重复数的随机permutation（排列）
- numel的返回值是double的，要将其转成int类型，为了防止溢出，我们选择uint16

![](https://img-blog.csdn.net/20180915104437707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 三、只加盐噪声

类似于只加椒噪声，也不能通过imnoise得到，也需要手动去加，只不过区别在于盐噪声我们设置的值是255而已

代码如下：

```python
% Salt Noise
salt_ind = randperm(uint16(numel(tif)),uint8(0.1*numel(tif)));
tif_salt = tif1;
tif_salt(salt_ind) = 255;
tif_salt = im2double(tif_salt);
```

![](https://img-blog.csdn.net/20180915104608329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



