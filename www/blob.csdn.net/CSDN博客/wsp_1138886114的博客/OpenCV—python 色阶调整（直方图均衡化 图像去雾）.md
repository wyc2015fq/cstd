# OpenCV—python 色阶调整（直方图均衡化 图像去雾） - wsp_1138886114的博客 - CSDN博客





2018年11月06日 20:48:40[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1073











### 文章目录
- [一、色阶调整( Levels Adjustment )原理](#_Levels_Adjustment__1)
- [二、自动色阶图像处理算法](#_23)




### 一、色阶调整( Levels Adjustment )原理

色阶：就是用直方图描述出的整张图片的明暗信息。如图

从左至右是从暗到亮的像素分布，

黑色三角代表最暗地方（纯黑—黑点值为０），

白色三角代表最亮地方（纯白—白点为 255）。

灰色三角代表中间调。（灰点为1.00）
对于一个RGB图像, 可以对R, G, B 通道进行独立的色阶调整，即对三个通道分别使用三个色阶定义值。还可以再对 三个通道进行整体色阶调整。

因此，对一个图像，可以用四次色阶调整。最终的结果，是四次调整后合并产生的结果。
**cv2.equalizeHist(img) 函数**
```python
img = cv2.imread('./20181106194742.png',0)
equ = cv2.equalizeHist(img) # 只能传入灰度图

res = np.hstack((img,equ))  # 图像列拼接（用于显示）

cv2.imshow('res',res)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181106203235568.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

### 二、自动色阶图像处理算法

**cv2.createCLAHE()** 对比度有限自适应直方图均衡

直方图均衡后背景对比度有所改善。但导致亮度过高，我们丢失了大部分信息。这是因为它的直方图并不局限于特定区域。

因此，为了解决这个问题，使用**自适应直方图均衡**。在此，图像被分成称为“图块”的小块（在OpenCV中，tileSize默认为8x8）。然后像往常一样对这些块中的每一个进行直方图均衡。所以在一个小区域内，直方图会限制在一个小区域（除非有噪音）。如果有噪音，它会被放大。为避免这种情况，应用对比度限制。如果任何直方图区间高于指定的对比度限制（在OpenCV中默认为40），则在应用直方图均衡之前，将这些像素剪切并均匀分布到其他区间。均衡后，为了去除图块边框中的瑕疵，应用双线性插值。
```python
import numpy as np
import cv2

img = cv2.imread('20181106194742.png',0)
clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8,8))
cl1 = clahe.apply(img)

cv2.imshow('img',img)
cv2.imshow('cl1',cl1)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181106204534124.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

自适应色阶去雾气
```python
import numpy as np
import cv2
 
def ComputeHist(img):
    h,w = img.shape
    hist, bin_edge = np.histogram(img.reshape(1,w*h), bins=list(range(257)))
    return hist
    
def ComputeMinLevel(hist, rate, pnum):
    sum = 0
    for i in range(256):
        sum += hist[i]
        if (sum >= (pnum * rate * 0.01)):
            return i
            
def ComputeMaxLevel(hist, rate, pnum):
    sum = 0
    for i in range(256):
        sum += hist[255-i]
        if (sum >= (pnum * rate * 0.01)):
            return 255-i
            
def LinearMap(minlevel, maxlevel):
    if (minlevel >= maxlevel):
        return []
    else:
        newmap = np.zeros(256)
        for i in range(256):    #获取阈值外的像素值 i< minlevel，i> maxlevel
            if (i < minlevel):
                newmap[i] = 0
            elif (i > maxlevel):
                newmap[i] = 255
            else:
                newmap[i] = (i-minlevel)/(maxlevel-minlevel) * 255
        return newmap
        
def CreateNewImg(img):
    h,w,d = img.shape
    newimg = np.zeros([h,w,d])
    for i in range(d):
        imgmin = np.min(img[:,:,i])
        imgmax = np.max(img[:,:,i])
        imghist = ComputeHist(img[:,:,i])
        minlevel = ComputeMinLevel(imghist, 8.3, h*w)
        maxlevel = ComputeMaxLevel(imghist, 2.2, h*w)
        newmap = LinearMap(minlevel,maxlevel)
        if (newmap.size ==0 ):
            continue
        for j in range(h):
            newimg[j,:,i] = newmap[img[j,:, i]]
    return newimg


if __name__ == '__main__':
    img = cv2.imread('./20181106194742.png')
    newimg = CreateNewImg(img)
    cv2.imshow('original_img', img)
    cv2.imshow('new_img', newimg/255)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181106210115331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)







