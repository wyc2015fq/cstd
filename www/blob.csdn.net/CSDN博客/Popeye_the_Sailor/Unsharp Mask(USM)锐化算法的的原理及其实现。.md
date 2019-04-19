# Unsharp Mask(USM)锐化算法的的原理及其实现。 - Popeye_the_Sailor - CSDN博客
2018年07月08日 22:48:00[_Sailor_](https://me.csdn.net/lz0499)阅读数：786
### [Unsharp Mask(USM)锐化算法的的原理及其实现](https://www.cnblogs.com/Imageshop/archive/2013/05/19/3086388.html)
通过增强图像的高频部分的内容，图像的视觉效果可以极大的得到改观。为达到这个目的，通常可以使用经典USM技术来实现。这个技术的流程可用下图来实现:
![](https://images0.cnblogs.com/blog/349293/201305/18191527-d11db65ad6124e78949a28c301732eac.jpg)
      用具体的公式表达即为：
                    y(n,m)= x(n,m)+ λz(n,m)                                                       （1）
      其中， x(n,m)为输入图像，y(n,m)为输出图像，而z(n,m)为校正信号，一般是通过对x进行高通滤波获取。λ是用于控制增强效果的的一个缩放因子。
      在传统的USM算法中，z(n,m)一般可以通过下式获取：
              z(n,m)=4x(n,m)-x(n-1,m) -x(n +1,m)-x(n, m-1) -x(n,m+1)             （2）
      当然也可以用如下的模板：
![](https://images0.cnblogs.com/blog/349293/201305/18192532-446cd9638d8041d7b138b88ad363164a.jpg)
缩放因子为100时对应的代码：
```python
```
#!/usr/bin/env python
# -*- coding:utf-8 -*-
import cv2
from numpy import *
image = cv2.imread('LenaRGB.bmp') # 根据路径读取一张图片
reflect_img = cv2.copyMakeBorder(image,1,1,1,1,cv2.BORDER_REPLICATE)#填充边界像素
x,y=reflect_img.shape[:2]
for depth in range(0,3):
    for rows in range(1,x-1):
        for colums in range(1,y-1):
            HighPass = (reflect_img.item(rows,colums,depth)<<2)-reflect_img.item(rows-1,colums,depth)\
                        -reflect_img.item(rows+1,colums,depth)- reflect_img.item(rows,colums-1,depth)\
                        -reflect_img.item(rows,colums+1,depth)
            Value = image.item(rows-1,colums-1,depth)+100*HighPass//100
if(Value > 255):
                Value = 255
elif(Value < 0):
                Value = 0
image.itemset((rows-1,colums-1,depth),Value)
cv2.namedWindow("reFlect image")
cv2.imshow("reFlect image",reflect_img)
""" the second method
x,y = image.shape[:2]
image_resize = zeros((x+2,y+2), dtype=uint8)
img_resize = cv2.cvtColor(image_resize, cv2.COLOR_GRAY2BGR)
#拷贝图像到缓冲区，以及填充边缘像素
img_resize[1:513,1:513]   = image[0:512,0:512]
img_resize[0:1,0:512]     = image[0:1,0:512]
img_resize[513:514,0:512] = image[511:512,0:512]
img_resize[0:512,0:1]     = image[0:512,0:1]
img_resize[0:512,513:514] = image[0:512,511:512]
#对三个通道实现UMS
for depth in range(0,3):
    for rows in range(1,x+1):
        for colums in range(1,y+1):
            HighPass = (img_resize.item(rows,colums,depth)<<2)-img_resize.item(rows-1,colums,depth)\
                        -img_resize.item(rows+1,colums,depth)- img_resize.item(rows,colums-1,depth)\
                        -img_resize.item(rows,colums+1,depth)
            Value = image.item(rows-1,colums-1,depth)+100*HighPass//100
            if(Value > 255):
                Value = 255
            elif(Value < 0):
                Value = 0
            image.itemset((rows-1,colums-1,depth),Value)
cv2.namedWindow("resize_Image")          
cv2.imshow("resize_Image",img_resize)
"""
cv2.namedWindow("Image") # 初始化一个名为Image的窗口
cv2.imshow("Image", image) # 显示图片
cv2.imwrite("image_Amout100.bmp",image)
cv2.waitKey(0) # 等待键盘触发事件，释放窗口
Lena测试实际效果图：
![](https://img-blog.csdn.net/20180708224503674?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x6MDQ5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从左至右，分别是原图，缩放因子为25，缩放因子为100
参考：
[Unsharp Mask(USM)锐化算法的的原理及其实现。](http://www.cnblogs.com/Imageshop/archive/2013/05/19/3086388.html)
