# OpenCV—Python 图像滤波（均值、中值、高斯、高斯双边、高通等滤波） - wsp_1138886114的博客 - CSDN博客





2018年09月27日 20:41:41[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：2588











### 文章目录
- [一、图像处理——滤波](#_1)
- [1.1 均值模糊（低通滤波）、中值模糊（中值滤波）](#11__7)
- [1.2  边缘保留滤波EPF](#12__EPF_48)
- [二、图像处理——噪点与滤波](#_91)
- [2.1 椒盐噪点（使用中值滤波去除）](#21__92)
- [2.1 高斯滤波/模糊（去噪效果好）](#21__119)
- [三、高通过滤/滤波（边缘检测/高反差保留）](#_160)




### 一、图像处理——滤波

**过滤** ：是信号和图像处理中基本的任务。其目的是根据应用环境的不同，选择性的提取图像中某些认为是重要的信息。过滤可以移除图像中的噪音、提取感兴趣的可视特征、允许图像重采样等等。
**频域分析** ：将图像分成从低频到高频的不同部分。低频对应图像强度变化小的区域，而高频是图像强度变化非常大的区域。

在频率分析领域的框架中，滤波器是一个用来增强图像中某个波段或频率并阻塞（或降低）其他频率波段的操作。低通滤波器是消除图像中高频部分，但保留低频部分。高通滤波器消除低频部分。- 滤波(高通、低通、带通、带阻) 、模糊、去噪、平滑等。
![在这里插入图片描述](https://img-blog.csdn.net/20180927190558256?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 1.1 均值模糊（低通滤波）、中值模糊（中值滤波）
- 均值滤波：典型的线性滤波算法，它是指在图像上对目标像素给一个模板，该模板包括了其周围的临近像素（以目标像素为中心的周围8个像素，构成一个滤波模板，即去掉目标像素本身），再用模板中的全体像素的平均值来代替原来像素值。
- 中值滤波法是一种非线性平滑技术，它将每一像素点的灰度值设置为该点某邻域窗口内的所有像素点灰度值的中值。

```python
import cv2
import numpy as np


def blur_demo(image):            
	"""
	均值模糊 : 去随机噪声有很好的去噪效果
	（1, 15）是垂直方向模糊，（15， 1）是水平方向模糊
	"""
    dst = cv2.blur(image, (1, 15))    
    cv2.imshow("avg_blur_demo", dst)

def median_blur_demo(image):    # 中值模糊  对椒盐噪声有很好的去燥效果
    dst = cv2.medianBlur(image, 5)
    cv2.imshow("median_blur_demo", dst)

def custom_blur_demo(image):
	"""
	用户自定义模糊
	下面除以25是防止数值溢出
	"""
    kernel = np.ones([5, 5], np.float32)/25   
    dst = cv2.filter2D(image, -1, kernel)
    cv2.imshow("custom_blur_demo", dst)

src = cv2.imread("./fapiao.png")
img = cv2.resize(src,None,fx=0.8,fy=0.8,interpolation=cv2.INTER_CUBIC)
cv2.imshow('input_image', img)

blur_demo(img)
median_blur_demo(img)
custom_blur_demo(img)

cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdn.net/20180927192806190?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 1.2  边缘保留滤波EPF

进行边缘保留滤波通常用到两个方法：高斯双边滤波和均值迁移滤波。
- 双边滤波（Bilateral filter）是一种非线性的滤波方法，是**结合图像的空间邻近度和像素值相似度的一种折中处理**，同时考虑空域信息和灰度相似性，达到保边去噪的目的。

双边滤波器顾名思义比高斯滤波多了一个高斯方差 $\sigma－d$，它是基于空间分布的高斯滤波函数，所以在边缘附近，离的较远的像素不会太多影响到边缘上的像素值，这样就保证了边缘附近像素值的保存。但是由于保存了过多的高频信息，对于彩色图像里的高频噪声，双边滤波器不能够干净的滤掉，只能够对于低频信息进行较好的滤波
- 双边滤波函数原型：

```python
"""
	bilateralFilter(src, d, sigmaColor, sigmaSpace[, dst[, borderType]]) -> dst
	  - src: 输入图像。
	  - d:   在过滤期间使用的每个像素邻域的直径。如果输入d非0，则sigmaSpace由d计算得出，如果sigmaColor没输入，则sigmaColor由sigmaSpace计算得出。
	  - sigmaColor: 色彩空间的标准方差，一般尽可能大。
	                较大的参数值意味着像素邻域内较远的颜色会混合在一起，
	                从而产生更大面积的半相等颜色。
	  - sigmaSpace: 坐标空间的标准方差(像素单位)，一般尽可能小。
	                参数值越大意味着只要它们的颜色足够接近，越远的像素都会相互影响。
	                当d > 0时，它指定邻域大小而不考虑sigmaSpace。 
	                否则，d与sigmaSpace成正比。
"""
```

```python
import cv2

def bi_demo(image):      #双边滤波
    dst = cv2.bilateralFilter(image, 0, 100, 5)
    cv2.imshow("bi_demo", dst)

def shift_demo(image):   #均值迁移
    dst = cv2.pyrMeanShiftFiltering(image, 10, 50)
    cv2.imshow("shift_demo", dst)

src = cv2.imread('./100.png')
img = cv2.resize(src,None,fx=0.8,fy=0.8,
                 interpolation=cv2.INTER_CUBIC)
cv2.imshow('input_image', img)

bi_demo(img)
shift_demo(img)

cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdn.net/20180927212216332?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 二、图像处理——噪点与滤波

#### 2.1 椒盐噪点（使用中值滤波去除）

```python
import cv2
import numpy as np

def salt(img, n):
    for k in range(n):
        i = int(np.random.random() * img.shape[1])
        j = int(np.random.random() * img.shape[0])
        if img.ndim == 2:
            img[j,i] = 255
        elif img.ndim == 3:
            img[j,i,0]= 255
            img[j,i,1]= 255
            img[j,i,2]= 255
        return img

img = cv2.imread("./original_img.png",cv2.IMREAD_GRAYSCALE)
result = salt(img, 500)
median = cv2.medianBlur(result, 5)
cv2.imshow("original_img", img)
cv2.imshow("Salt", result)
cv2.imshow("Median", median)
cv2.waitKey(0)
cv2.destroyWindow()
```

![在这里插入图片描述](https://img-blog.csdn.net/20180927200950887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 2.1 高斯滤波/模糊（去噪效果好）

高斯模糊实质上就是一种均值模糊，只是高斯模糊是按照加权平均的，距离越近的点权重越大，距离越远的点权重越小。

通俗的讲，高斯滤波就是对整幅图像进行加权平均的过程，每一个像素点的值，都由其本身和邻域内的其他像素值经过加权平均后得到。

我们可以从其函数表达式可知：
![在这里插入图片描述](https://img-blog.csdn.net/20180927204059734?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
import cv2
import numpy as np

def clamp(pv):
    if pv > 255:
        return 255
    if pv < 0:
        return 0
    else:
        return pv

def gaussian_noise(image):           # 加高斯噪声
    h, w, c = image.shape
    for row in range(h):
        for col in range(w):
            s = np.random.normal(0, 20, 3)
            b = image[row, col, 0]   # blue
            g = image[row, col, 1]   # green
            r = image[row, col, 2]   # red
            image[row, col, 0] = clamp(b + s[0])
            image[row, col, 1] = clamp(g + s[1])
            image[row, col, 2] = clamp(r + s[2])
    cv2.imshow("noise image", image)

src = cv2.imread('888.png')
cv2.imshow('input_image', src)

gaussian_noise(src)
dst = cv2.GaussianBlur(src, (15,15), 0) #高斯模糊
cv2.imshow("Gaussian_Blur2", dst)

cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdn.net/20180927202421686?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 三、高通过滤/滤波（边缘检测/高反差保留）

使用的函数有：`cv2.Sobel()` , `cv2.Schar()` , `cv2.Laplacian()`

Sobel,scharr其实是求一阶或者二阶导数。scharr是对Sobel的优化。

Laplacian是求二阶导数。- cv2.Sobel() 是一种带有方向过滤器

```python
"""
dst = cv2.Sobel(src, ddepth, dx, dy[, dst[, ksize[, scale[, delta[, borderType]]]]])
src:    需要处理的图像；
ddepth: 图像的深度，-1表示采用的是与原图像相同的深度。 
        目标图像的深度必须大于等于原图像的深度；
dx和dy: 求导的阶数，0表示这个方向上没有求导，一般为0、1、2。

dst     不用解释了；
ksize： Sobel算子的大小，必须为1、3、5、7。  ksize=-1时，会用3x3的Scharr滤波器，
        它的效果要比3x3的Sobel滤波器要好 
scale： 是缩放导数的比例常数，默认没有伸缩系数；
delta： 是一个可选的增量，将会加到最终的dst中， 默认情况下没有额外的值加到dst中
borderType： 是判断图像边界的模式。这个参数默认值为cv2.BORDER_DEFAULT。

"""
```

```python
import cv2

img=cv2.imread('888.png',cv2.IMREAD_COLOR)
x=cv2.Sobel(img,cv2.CV_16S,1,0)
y=cv2.Sobel(img,cv2.CV_16S,0,1)

absx=cv2.convertScaleAbs(x)
absy=cv2.convertScaleAbs(y)
dist=cv2.addWeighted(absx,0.5,absy,0.5,0)

cv2.imshow('original_img',img)
cv2.imshow('y',absy)
cv2.imshow('x',absx)
cv2.imshow('dsit',dist)

cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdn.net/20180927213859549?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

参考博客：
[https://blog.csdn.net/sunny2038/article/details/9155893](https://blog.csdn.net/sunny2038/article/details/9155893)
[https://www.cnblogs.com/FHC1994/p/9097231.html](https://www.cnblogs.com/FHC1994/p/9097231.html)
[https://www.cnblogs.com/FHC1994/p/9097231.html](https://www.cnblogs.com/FHC1994/p/9097231.html)
[https://blog.csdn.net/wangleixian/article/details/78237597](https://blog.csdn.net/wangleixian/article/details/78237597)







