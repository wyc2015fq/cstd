# opencv之图像直方图均衡化cv2.equalizeHist - qq_37385726的博客 - CSDN博客





2018年09月02日 11:35:23[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1347
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、图像直方图](#%E4%B8%80%E3%80%81%E5%9B%BE%E5%83%8F%E7%9B%B4%E6%96%B9%E5%9B%BE)

[二、绘制直方图](#%E4%BA%8C%E3%80%81%E7%BB%98%E5%88%B6%E7%9B%B4%E6%96%B9%E5%9B%BE)

[三、直方图均衡化](#%E4%B8%89%E3%80%81%E7%9B%B4%E6%96%B9%E5%9B%BE%E5%9D%87%E8%A1%A1%E5%8C%96)

[四、直方图均衡化效果展示](#%E5%9B%9B%E3%80%81%E7%9B%B4%E6%96%B9%E5%9B%BE%E5%9D%87%E8%A1%A1%E5%8C%96%E6%95%88%E6%9E%9C%E5%B1%95%E7%A4%BA)



## 一、图像直方图

  图像的构成是有像素点构成的，每个像素点的值代表着该点的颜色（灰度图或者彩色图）。所谓直方图就是对图像的中的这些像素点的值进行统计，**得到一个统一的整体的灰度概念**。**直方图的好处就在于可以清晰了解图像的整体灰度分布，这对于后面依据直方图处理图像来说至关重要。**

**一般情况下直方图都是灰度图像，直方图x轴是灰度值（一般0~255），y轴就是图像中每一个灰度级对应的像素点的个数。**

![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/w%3D268%3Bg%3D0/sign=de05aeab54df8db1bc2e7b623118ba69/7af40ad162d9f2d3d5498bdca3ec8a136327ccad.jpg)



## 二、绘制直方图

**①利用cv2.calcHist()**

![](https://img-blog.csdn.net/20180902111143568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- **函数原型：cv2.calcHist(image,channels,mask,histSize,ranges)**
- **image**为待计算直方图的图像，需用[]包裹
- **channels**指定待计算直方图的图像的哪一通道用来计算直方图，RGB图像可以指定[0,1,2]，灰度图像只有[0],需用[]包裹,
- **mask**为掩码，可以指定图像的范围，如果是全图，默认为none
- **hitsize**为直方图的灰度级数，例如[0,255]一共256级，故参数为256，需用[]包裹
- **range**为像素值范围，为[0,255]
- **返回值**为hist，直方图；接着使用
	
matplotlib.pyplot.plot(hist,color)进行绘制

```python
hist = cv2.calcHist([res],[0],None,[256],[0,255])
plt.plot(hist,'r')
plt.show()
```

![](https://img-blog.csdn.net/20180902112055224?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



**②使用plt.hist()，进行绘制**

**plt.hist(img,ravel(),hitsizes,ranges,color=)**
- **img.ravel()**将原图像的array数组转成一维的数组
- **hitsizes**为直方图的灰度级数
- **ranges**为灰度范围[0,255]
- color是参数，需要使用color=''来指定颜色

```python
plt.hist(res.ravel(), 256, [0, 256],color='r')
plt.show()
```

![](https://img-blog.csdn.net/20180902112812909?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 三、直方图均衡化

**图像的直方图是对图像对比度效果上的一种处理，旨在使得图像整体效果均匀，黑与白之间的各个像素级之间的点更均匀一点。 **

通过这种方法，亮度可以更好地在直方图上分布。这样就可以用于增强局部的对比度而不影响整体的对比度，直方图均衡化通过有效地扩展常用的亮度来实现这种功能。

这种方法对于背景和前景都太亮或者太暗的图像非常有用，这种方法尤其是可以带来**X光****图像中更好的骨骼结构显示以及曝光过度或者曝光不足照片****中更好的细节**。

**①实现方法**

利用

cv2.equalizeHist(img)，将要均衡化的原图像**【要求是灰度图像】**作为参数传入，则返回值即为均衡化后的图像。
原图像直方图

![](https://img-blog.csdn.net/20180902113240843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
res = cv2.equalizeHist(res)
```

处理后的直方图

![](https://img-blog.csdn.net/20180902112812909?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 四、直方图均衡化效果展示

![](https://upload-images.jianshu.io/upload_images/3209607-53a2d0f41f06008a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800/format/webp)

![](https://upload-images.jianshu.io/upload_images/3209607-cd814e47edc3ac6a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800/format/webp)





