# PIL之ImageFilter下的基本操作 - qq_37385726的博客 - CSDN博客





2018年08月18日 17:25:02[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：868








# **目录**

[1.模糊滤镜  BLUR](#1.%E6%A8%A1%E7%B3%8A%E6%BB%A4%E9%95%9C%C2%A0%20BLUR)

[2.轮廓滤镜 CONTOUR](#2.%E8%BD%AE%E5%BB%93%E6%BB%A4%E9%95%9C%20CONTOUR)

[3.细节滤镜 DETAIL](#3.%E7%BB%86%E8%8A%82%E6%BB%A4%E9%95%9C%20DETAIL)

[4.边界增强滤镜 EDGE_ENHANCE](#4.%E8%BE%B9%E7%95%8C%E5%A2%9E%E5%BC%BA%E6%BB%A4%E9%95%9C%20EDGE_ENHANCE)

[5.边界增强加强版滤镜 EDGE_ENHANCE_MORE](#5.%E8%BE%B9%E7%95%8C%E5%A2%9E%E5%BC%BA%E5%8A%A0%E5%BC%BA%E7%89%88%E6%BB%A4%E9%95%9C%20EDGE_ENHANCE_MORE)

[6.浮雕滤镜 EMBOSS](#6.%E6%B5%AE%E9%9B%95%E6%BB%A4%E9%95%9C%20EMBOSS)

[7寻找边界滤镜 FUND_EDGES](#7%E5%AF%BB%E6%89%BE%E8%BE%B9%E7%95%8C%E6%BB%A4%E9%95%9C%20FUND_EDGES)

[8.平滑滤镜 SMOOTH](#8.%E5%B9%B3%E6%BB%91%E6%BB%A4%E9%95%9C%20SMOOTH)

[9.平滑滤镜加强版  SOOTH_MORE](#9.%E5%B9%B3%E6%BB%91%E6%BB%A4%E9%95%9C%E5%8A%A0%E5%BC%BA%E7%89%88%C2%A0%20SOOTH_MORE)

[10.锐化滤镜 SHARPEN](#10.%E9%94%90%E5%8C%96%E6%BB%A4%E9%95%9C%20SHARPEN)

[11.核滤镜 Kernel](#11.%E6%A0%B8%E6%BB%A4%E9%95%9C%20Kernel)

[12.等级滤波 RankFilter](#12.%E7%AD%89%E7%BA%A7%E6%BB%A4%E6%B3%A2%20RankFilter)

[13.最大值滤波  MaxFilter](#13.%E6%9C%80%E5%A4%A7%E5%80%BC%E6%BB%A4%E6%B3%A2%C2%A0%20MaxFilter)

[14.最小值滤波  MinFilter](#14.%E6%9C%80%E5%B0%8F%E5%80%BC%E6%BB%A4%E6%B3%A2%C2%A0%20MinFilter)

[15.中值滤波滤镜 MedianFilter(size=3)](#15.%E4%B8%AD%E5%80%BC%E6%BB%A4%E6%B3%A2%E6%BB%A4%E9%95%9C%20MedianFilter(size%3D3))

[16.波形滤镜 ModeFilter](#16.%E6%B3%A2%E5%BD%A2%E6%BB%A4%E9%95%9C%C2%A0ModeFilter)

[写在最后](#%E5%86%99%E5%9C%A8%E6%9C%80%E5%90%8E)

参考自[这里](https://www.aliyun.com/jiaocheng/470850.html)

> 
**添加滤镜**

**PILImage对象调用filter函数，参数传递ImageFilter下的滤镜参数**


**原图**

![](https://img-blog.csdn.net/20180818141908794?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 1.模糊滤镜  BLUR

> 
**ImageFilter.BLUR**为模糊滤波,处理之后的图像会整体变得模糊。


```python
#滤镜，模糊效果
img1 = img.filter(ImageFilter.BLUR)
img1.show()
```

![](https://img-blog.csdn.net/20180818142145384?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 2.轮廓滤镜 CONTOUR

> 
**ImageFilter.CONTOUR**为轮廓滤波,将图像中的轮廓信息全部提取出来。


```python
#滤镜，勾勒轮廓
img2 = img.filter(ImageFilter.CONTOUR)
img2.show()
```

![](https://img-blog.csdn.net/20180818142200400?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 3.细节滤镜 DETAIL

> 
**ImageFilter.DETAIL**为细节增强滤波,会使得图像中细节更加明显。


```python
#滤镜，细节
img3 = img.filter(ImageFilter.DETAIL)
img3.show()
```

![](https://img-blog.csdn.net/20180818142216248?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 4.边界增强滤镜 EDGE_ENHANCE

> 
**ImageFilter.EDGE_ENHANCE**为边缘增强滤波,突出、加强和改善图像中不同灰度区域之间的边界和轮廓的图像增强方法。经处理使得边界和边缘在图像上表现为图像灰度的突变,用以提高人眼识别能力。


```python
#滤镜，边界增强
img4 = img.filter(ImageFilter.EDGE_ENHANCE)
img4.show()
```

![](https://img-blog.csdn.net/20180818142233829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 5.边界增强加强版滤镜 EDGE_ENHANCE_MORE

> 
**ImageFilter.EDGE_ENHANCE_MORE**为深度边缘增强滤波,会使得图像中边缘部分更加明显


```python
#滤镜，边界增强的加强版
img5 = img.filter(ImageFilter.EDGE_ENHANCE_MORE)
img5.show()
```

![](https://img-blog.csdn.net/20180818142310867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 6.浮雕滤镜 EMBOSS

> 
**ImageFilter.EMBOSS**为浮雕滤波,会使图像呈现出浮雕效果。


```python
#滤镜，浮雕处理
img6 = img.filter(ImageFilter.EMBOSS)
img6.show()
```

![](https://img-blog.csdn.net/20180818142334108?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 7寻找边界滤镜 FUND_EDGES

> 
**ImageFilter.FIND_EDGES**为寻找边缘信息的滤波,会找出图像中的边缘信息。


```python
#滤镜，寻找边界滤镜
img7 = img.filter(ImageFilter.FIND_EDGES)
img7.show()
```

![](https://img-blog.csdn.net/20180818142756653?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 8.平滑滤镜 SMOOTH

> 
**ImageFilter.SMOOTH**为平滑滤波,突出图像的宽大区域、低频成分、主干部分或**抑制图像噪声和干扰高频成分,使图像亮度平缓渐变,减小突变梯度,改善图像质量。**


# ![](https://img-blog.csdn.net/20180818145117514?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 9.平滑滤镜加强版  SOOTH_MORE

> 
**ImageFilter.SMOOTH_MORE**为深度平滑滤波,会使得图像变得更加平滑。


```python
#滤镜，平滑滤镜加强版
img9 = img.filter(ImageFilter.SMOOTH_MORE)
img9.show()
```

![](https://img-blog.csdn.net/20180818145250114?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 10.锐化滤镜 SHARPEN

> 
**ImageFilter.SHARPEN**为锐化滤波,**补偿图像的轮廓,增强图像的边缘及灰度跳变的部分**,使图像变得清晰。

【和EDGE_ENHANCE的区别】

**对比输出的结果图能够发现，边缘加强是会将边缘在原图上凸显出来，以强调边缘。**

**但是锐化更多的是补充原图边缘轮廓的信息，是图片更为清晰，并不会使边缘凸显，整个图片还是很和谐。**


```python
#滤镜，锐化滤镜
img10 = img.filter(ImageFilter.SHARPEN)
img10.show()
```

![](https://img-blog.csdn.net/20180818150255449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





# 11.核滤镜 Kernel

> 
**ImageFilter.Kernel**

定义:Kernel(size,kernel, scale=None, offset=0)

含义:(New in 1.1.5)生成一个给定尺寸的卷积核。在当前的版本中,变量size必须为(3,3)或者(5,5)。变量kernel与变量size对应地必须为包含9个或者25个整数或者浮点数的序列。

如果设置了变量scale,那将卷积核作用于每个像素值之后的数据,都需要除以这个变量。默认值为卷积核的权重之和。

如果设置变量offset,这个值将加到卷积核作用的结果上,然后再除以变量scale。


```python
img11 = img.filter(ImageFilter.Kernel((3,3),(1,1,1,0,0,0,2,0,2)))
img11.show()
```

![](https://img-blog.csdn.net/20180818151521979?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 12.等级滤波 RankFilter

> 
**ImageFilter.RankFilter**

定义:RankFilter(size,rank)

含义:(New in 1.1.5)生成给定尺寸的等级滤波器。对于输入图像的每个像素点,等级滤波器根据像素值,在(size,size)的区域中对所有像素点进行排序,然后拷贝对应等级的值存储到输出图像中。

**例如**

 im=im02.filter(ImageFilter.RankFilter(5,24))

图像im为等级滤波后的图像,在每个像素点为中心的5x5区域25个像素点中选择排序第24位的像素作为新的值。


```python
#等级滤镜
img12 = img.filter(ImageFilter.RankFilter(5,0))
img12.show()
```

![](https://img-blog.csdn.net/20180818152127963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 13.最大值滤波  MaxFilter

> 
**ImageFilter.MaxFilter**

最大值滤波是一种比较保守的图像处理手段.

定义:MaxFilter(size=3)

含义:(New in 1.1.5)生成给定尺寸的最大滤波器。对于输入图像的每个像素点,该滤波器从(size,size)的区域中拷贝最大的像素值存储到输出图像中。


```python
#MaxFilter
img13 = img.filter(ImageFilter.MaxFilter(3))
img13.show()
```

![](https://img-blog.csdn.net/20180818154241379?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 14.最小值滤波  MinFilter

> 
**ImageFilter.MinFilter**

最小值滤波是一种比较保守的图像处理手段.

定义:MinFilter(size=3)

含义:(New in 1.1.5)生成给定尺寸的最小滤波器。对于输入图像的每个像素点,该滤波器从(size,size)的区域中拷贝最小的像素值存储到输出图像中。


```python
img14 = img.filter(ImageFilter.MinFilter(3))
img14.show()
```

![](https://img-blog.csdn.net/20180818154432281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 15.中值滤波滤镜 MedianFilter(size=3)

> 
**ImageFilter.MedianFilter**

[中值](https://baike.baidu.com/item/%E4%B8%AD%E5%80%BC/9661467)滤波是基于排序统计理论的一种**能有效抑制噪声的非线性信号处理技术（**特别是消除**椒盐噪声**，中值滤波的效果要比均值滤波更好**）**

中值滤波的基本原理是把数字图像或数字序列中**一点的值用该点的一个邻域中各点值的中值代替，让周围的[像素](https://baike.baidu.com/item/%E5%83%8F%E7%B4%A0)值接近的真实值，从而消除孤立的噪声点。**

![](http://hi.csdn.net/attachment/201202/25/0_13301799280L9y.gif)


```python
# #滤镜，中值滤波滤镜
img15 = img.filter(ImageFilter.MedianFilter(3))
img15.show()
```

![](https://img-blog.csdn.net/20180818154617772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 16.波形滤镜 ModeFilter

> 
**ImageFilter.ModeFilter**

定义:ModeFilter(size=3)

含义:(New in 1.1.5)生成给定尺寸的模式滤波器。对于输入图像的每个像素点,该滤波器从(size,size)的区域中拷贝出现次数最多的像素值存储到输出图像中。如果没有一个像素值出现过两次极其以上,则使用原始像素值。


```python
img16 = img.filter(ImageFilter.ModeFilter(3))
img16.show()
```

![](https://img-blog.csdn.net/20180818154937913?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 写在最后

> 
ImageFilter下有很多滤波参数，如上所介绍的。

的确也会对原始图像做出改变，但是在实际应用上，效果可能并不大，例如图像去噪上。

举个最简单的例子，**图像去雾**。


![åå¾](https://img-blog.csdn.net/20160328130809575)

![å»é¾å¾å](https://img-blog.csdn.net/20160328131016718)



