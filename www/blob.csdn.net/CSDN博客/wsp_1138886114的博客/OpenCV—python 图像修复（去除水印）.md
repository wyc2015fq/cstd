# OpenCV—python 图像修复（去除水印） - wsp_1138886114的博客 - CSDN博客





2018年11月15日 21:19:25[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1557








基于OpenCV的两种去水印方案(不具有普适性)

可以使用深度学习方法来去修复图像

### 一、基于 inpaint 方法（网上的方法，处理质量较低）

```
算法理论：基于Telea在2004年提出的基于快速行进的修复算法（FMM算法），先处理待修复区域边缘上的像素点，然后层层向内推进，直到修复完所有的像素点
处理方式：由ui人员制作出黑底白色水印且相同位置的水印蒙版图(必须单通道灰度图)，然后使用inpaint方法处理原始图像，具体使用时可把水印区放粗，这样处理效果会好点
```

```python
# -*- coding: utf-8 -*-

"""
cv2.inpaint(src, inpaintMask, 3, cv2.INPAINT_TELEA)
参数：
目标修复图像;
蒙版图（定位修复区域）;
选取邻域半径;
修复算法(INPAINT_TELEA：基于快速行进算法 算法效果较好
        INPAINT_NS:基于流体动力学并使用了偏微分方程)
"""

import cv2

src_ = cv2.imread('1111.png')
mask = cv2.imread('2222.png', cv2.IMREAD_GRAYSCALE)
res_ = cv2.resize(src_,None,fx=0.6, fy=0.6, interpolation = cv2.INTER_CUBIC)
mask = cv2.resize(mask,None,fx=0.6, fy=0.6, interpolation = cv2.INTER_CUBIC)
dst = cv2.inpaint(res_, mask, 3, cv2.INPAINT_TELEA)

cv2.imshow('res_', res_)
cv2.imshow('mask', mask)
cv2.imshow('dst', dst)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115205626167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

### 二、基于像素的反色中和（处理质量较高）

参考自ps去水印原理，通过一张白底的反色水印图来中和原图水印

```python
# -*- coding: utf-8 -*-
import cv2
import numpy

src = cv2.imread('1111.png')
mask = cv2.imread('2222.png')
src = cv2.resize(src,None,fx=0.6, fy=0.6, interpolation = cv2.INTER_CUBIC)
mask = cv2.resize(mask,None,fx=0.6, fy=0.6, interpolation = cv2.INTER_CUBIC)
save = numpy.zeros(src.shape, numpy.uint8) #创建一张空图像用于保存

for row in range(src.shape[0]):
    for col in range(src.shape[1]):
        for channel in range(src.shape[2]):
            if mask[row, col, channel] == 0:
                val = 0
            else:
                reverse_val = 255 - src[row, col, channel]
                val = 255 - reverse_val * 256 / mask[row, col, channel]
                if val < 0: val = 0
            save[row, col, channel] = val

cv2.imshow('src', src)
cv2.imshow('mask', mask)
cv2.imshow('save', save)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115211634706.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

鸣谢
[https://my.oschina.net/u/2400083/blog/732321](https://my.oschina.net/u/2400083/blog/732321)
[https://www.cnblogs.com/lgh344902118/p/7928054.html](https://www.cnblogs.com/lgh344902118/p/7928054.html)



