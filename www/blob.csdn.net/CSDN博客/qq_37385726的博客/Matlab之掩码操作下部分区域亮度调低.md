# Matlab之掩码操作下部分区域亮度调低 - qq_37385726的博客 - CSDN博客





2018年09月05日 20:10:40[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：303








# **目录**

[一、roipoly](#%E4%B8%80%E3%80%81roipoly)

[二、代码](#%E4%BA%8C%E3%80%81%E4%BB%A3%E7%A0%81)

[三、效果](#%E4%B8%89%E3%80%81%E6%95%88%E6%9E%9C)

## 一、roipoly

roipoly(img)可以提取掩码区域，创建掩码图像。



## 二、代码

```python
clear
jpg = imread('C:\\Program Files\\MATLAB\\R2017a\\toolbox\\images\\imdata\\baby.jpg');

roi = im2uint8(roipoly(jpg));
roi_cmp = bitcmp(roi);
interest = bitand(jpg,roi);
non_interest = bitand(roi_cmp,imdivide(jpg,1.5));
combine = bitor(interest,non_interest);
subplot(2,2,1),imshow(jpg),title('initial')
subplot(2,2,2),imshow(interest),title('interest')
subplot(2,2,3),imshow(non_interest),title('non_interest')
subplot(2,2,4),imshow(combine),title('combine')
```



## 三、效果



![](https://img-blog.csdn.net/2018090520102969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



