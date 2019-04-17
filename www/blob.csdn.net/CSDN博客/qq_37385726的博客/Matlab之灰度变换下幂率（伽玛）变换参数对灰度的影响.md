# Matlab之灰度变换下幂率（伽玛）变换参数对灰度的影响 - qq_37385726的博客 - CSDN博客





2018年09月07日 22:50:29[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：697








# **目录**

[一、参数对灰度的影响](#%E4%B8%80%E3%80%81%E5%8F%82%E6%95%B0%E5%AF%B9%E7%81%B0%E5%BA%A6%E7%9A%84%E5%BD%B1%E5%93%8D)

[二、程序验证](#%E4%BA%8C%E3%80%81%E7%A8%8B%E5%BA%8F%E9%AA%8C%E8%AF%81)

## 一、参数对灰度的影响

> 
**s = c*r^γ   （c为常数，r为原像素，γ为伽玛）**


当**γ**小于1时，灰度低的变亮，高的相对变暗。 **γ**越小，灰度低的越亮

当**γ**大于1时，灰度高的变亮，低的相对变暗**。γ**越大，灰度高的越亮



## 二、程序验证

```python
tif = imread('C:\\Program Files\\MATLAB\\R2017a\\toolbox\\images\\imdata\\moon.tif');
x = 0:255;n =2;
subplot(2,6,1),plot(x);
subplot(2,6,1+6),imshow(tif),title('initial'),impixelinfo;

flag = 2;
while n~=0 && flag<=6
    n = input('n=');
    c = 255/(255^n);
    y = c*x.^n;
    mi_tif = uint8(y(uint16(tif)+1));
    subplot(2,6,flag),plot(x,y);
    subplot(2,6,flag+6),imshow(mi_tif),title(strcat('n=',num2str(n))),impixelinfo;
    flag = flag + 1;
end
```

![](https://img-blog.csdn.net/20180907224630875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



