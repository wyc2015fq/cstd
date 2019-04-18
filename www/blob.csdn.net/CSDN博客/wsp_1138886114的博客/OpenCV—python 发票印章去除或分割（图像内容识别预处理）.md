# OpenCV—python 发票印章去除或分割（图像内容识别预处理） - wsp_1138886114的博客 - CSDN博客





2018年09月26日 20:01:10[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1706








### 前言

在做发票内容识别时，发现发票的印章遮挡住文字信息，尤其在合计金额处，导致这一处的信息提取出问题，于是想到去除发票印章。

##### 思路一：通过颜色通道获取所需信息图

（代码主要是参考 [http://www.deanhan.com/2018/06/15/channel/）](http://www.deanhan.com/2018/06/15/channel/%EF%BC%89)
- 读取原始图像A
- 提取图像的红色通道，得到红色通道灰度值图片B
- 计算B的统计直方图C，确定最佳的阈值threshold
- 根据阈值，对B进行二值化，得到最终图片D
- （可选）应用膨胀算子对D进行操作，得到图片E

```python
```python
#去除印章
import cv2
import numpy as np
import matplotlib.pyplot as plt


image0=cv2.imread("fapiao.png",cv2.IMREAD_COLOR)   # 以BGR色彩读取图片
image = cv2.resize(image0,None,fx=0.5,fy=0.5,
                   interpolation=cv2.INTER_CUBIC)  # 缩小图片0.5倍（图片太大了）
cols,rows,_=image.shape                            # 获取图片高宽
B_channel,G_channel,R_channel=cv2.split(image)     # 注意cv2.split()返回通道顺序

cv2.imshow('Blue channel',B_channel)
cv2.imshow('Green channel',G_channel)
cv2.imshow('Red channel',R_channel)

pixelSequence=R_channel.reshape([rows*cols,])     # 红色通道的histgram 变换成一维向量
numberBins=256                                    # 统计直方图的组数
plt.figure()                                      # 计算直方图
manager = plt.get_current_fig_manager()
histogram,bins,patch=plt.hist(pixelSequence,
                              numberBins,
                              facecolor='black',
                              histtype='bar')     # facecolor设置为黑色
#设置坐标范围
y_maxValue=np.max(histogram)
plt.axis([0,255,0,y_maxValue])
#设置坐标轴
plt.xlabel("gray Level",fontsize=20)
plt.ylabel('number of pixels',fontsize=20)
plt.title("Histgram of red channel", fontsize=25)
plt.xticks(range(0,255,10))
#显示直方图
plt.pause(0.05)
plt.savefig("histgram.png",dpi=260,bbox_inches="tight")
plt.show()


#红色通道阈值(调节好函数阈值为160时效果最好，太大一片白，太小干扰点太多)
_,RedThresh = cv2.threshold(R_channel,160,255,cv2.THRESH_BINARY)

#膨胀操作（可以省略）
element = cv2.getStructuringElement(cv2.MORPH_RECT,(3, 3)) 
erode = cv2.erode(RedThresh, element)

#显示效果
cv2.imshow('original color image',image)
cv2.imshow("RedThresh",RedThresh)
cv2.imshow("erode",erode)

# 保存图像
cv2.imwrite('scale_image.jpg',image)
cv2.imwrite('RedThresh.jpg',RedThresh)
cv2.imwrite("erode.jpg",erode)

cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180926200025102?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 思路二：通过HSV颜色空间先提取印章，再获取所需信息

关于颜色空间查看：[https://blog.csdn.net/wsp_1138886114/article/details/80660014](https://blog.csdn.net/wsp_1138886114/article/details/80660014)

```python
```python
---------------------提取印章---------------------
import cv2
import numpy as np

np.set_printoptions(threshold=np.inf)
image=cv2.imread("fapiao.png")

hue_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
low_range = np.array([150, 103, 100])
high_range = np.array([180, 255, 255])
th = cv2.inRange(hue_image, low_range, high_range)
index1 = th == 255

img = np.zeros(image.shape, np.uint8)
img[:, :] = (255,255,255)
img[index1] = image[index1]#(0,0,255)
cv2.imshow('original_img', image)
cv2.imshow('extract_img', img)
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180926212220833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



