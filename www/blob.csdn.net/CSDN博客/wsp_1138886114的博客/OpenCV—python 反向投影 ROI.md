# OpenCV—python 反向投影 ROI - wsp_1138886114的博客 - CSDN博客





2018年11月18日 21:24:13[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：78











### 文章目录
- [一、反向投影概念](#_1)
- [1.1 直方图交叉](#11__5)
- [1.2 直方图反向投影：](#12__13)
- [二、彩色图像高斯反向投影](#_47)
- [三、算法步骤与代码实现](#_57)




### 一、反向投影概念

在计算机视觉这一块，图像反向投影的最终目的是获取ROI然后实现对ROI区域的标注、识别、测量等图像处理与分析，是计算机视觉与人工智能的常见方法之一。 如果一幅图像的区域中显示的是一种结构纹理或者一个独特的物体，那么这个区域的直方图可以看做是一个概率函数，其表现形式是某个像素属于该纹理或物体的概率。而反向投影就是一种记录给定图像中的像素点如何适应直方图模型像素分布方式的一种方法。

反向投影就是首先计算某一特征的直方图模型，然后使用模型去寻找图像中存在的该特征的方法。例如，有一个颜色直方图，可以利用反向投影在图像中找到该区域。

##### 1.1 直方图交叉

实现对象背景区分、复杂场景中查找对象、不同光照条件影响等。

假设 $M$ : **模型直方图数据**, $I$ : **图像直方图数据**、直方图交叉匹配可以被描述为如下：
$$\sum_{j=0}^n \min(I_j,M_j)$$

其中 $J$ 表示直方图的范围，即 bin 的个数。最终得到结果是表示多少个模型颜色像素与图像中的像素相同或者相似，值越大，表示越相似。归一化表示如下：
$$H(I,M) = \frac{\sum_{j=0}^n  \min(I_j,M_j)}{\sum_{j=1}^n M_j}$$

这种方法对背景像素变换可以保持稳定性、同时对尺度变换也有一定抗干扰作用，但是无法做到尺度不变性特征。**通过该方法可以定位图像中已知物体的位置**，这个方法叫做**直方图反向投影(Back Projection)**。
##### 1.2 直方图反向投影：

查找的方式就是不断的在输入图像中切割跟模板图像大小一致的图像块，并用直方图对比的方式与模板图像进行比较。
- 对每个直方图bin  ，直方图的范围$J$ ：$R_j = \frac{M_j}{I}$
- 对图像每个像素点 $I(x,y)$ 根据像素值获取对应的直方图分布概率 $\rm b_{(x,y)} = \min (R_{h(x,y)},I)$
- 对得到分布概率图像做卷积
- 求取局部最大值，即得到已知物体位置信息

正是因为直方图反向投影有这样能力，用在经典的MeanShift与CAMeanShift跟踪算法中来实现已知对象物体的定位。
- 
假设我们有一张100x100的输入图像，有一张10x10的模板图像，查找的过程是这样的：（使用单通道图）

（1）从输入图像的左上角(0,0)开始，切割一块(0,0)至(10,10)的临时图像；

（2）生成临时图像的直方图；

（3）用临时图像的直方图和模板图像的直方图对比，对比结果记为c；

（4）直方图对比结果c，就是结果图像(0,0)处的像素值；

（5）切割输入图像从(0,1)至(10,11)的临时图像，对比直方图，并记录到结果图像；

（6）重复（1）～（5）步直到输入图像的右下角。- 
注意点

输入图像和模板图像大小： $\rm size_{original\_img} \geqslant \rm size_{model\_img}$ 否则可能报错。

- 
反向投影函数：


```python
void cvCalcBackProjectPatch()
	IplImage** image,     输入图像：是一个单通道图像数组，而非实际图像
	CvArr* dst,           输出结果：单通道32位浮点图像，宽度为W-w+1，高度为H-h+1，
	                      其中W和H是输入图像的宽度和高度，w和h是模板图像的宽度和高度
	CvSize patch_size,    模板图像的大小：宽度和高度
	CvHistogram* hist,    模板图像的直方图：直方图的维数和输入图像的个数相同，并且次序要一致；             
	                      例如：输入图像包含色调和饱和度，那么直方图的第0维是色调，第1维是饱和度
	int method,           对比方式：跟直方图对比中的方式类似，可以是：CORREL（相关）、
	                      CHISQR（卡方）、INTERSECT（相交）、BHATTACHARYYA
	float~factor          归一化因子，一般都设置成1，否则很可能会出错；这个参数的类型是double
```

还有最需要注意的地方：这个函数的执行效率非常的低，在使用之前尤其需要注意图像的大小，直方图的维数，对比方式。如果说对比单个直方图对现在的电脑来说是清风拂面，那么反向投影是狂风海啸。对于1010x1010的RGB输入图像，10x10的模板图像，需要生成1百万次3维直方图，对比1百万次3维直方图。

### 二、彩色图像高斯反向投影

图像反向投影通常是更多对象细节信息的彩色图，而转为灰度图像会导致这些细节信息丢失、从而导致分割失败。最常见的是基于图像直方图特征的反向投影。我们这里介绍一种跟直方图反向投影不一样的彩色图像反向投影方法，通过基于高斯的概率分布公式（PDF）估算，反向投影得到对象区域，该方法也可以看做最简单的图像分割方法。缺点是对象颜色光照改变和尺度改变不具备不变性特征。所以需要在光照度稳定情况下成像采集图像数据。 在这种情况下使用的高斯概率密度公式为：
$$P(r) = \frac{1}{\sigma_r \sqrt{2\pi}}\exp\left \{ -\frac{(r-\mu)^2)}{2\sigma_r^2 } \right \}$$

其中：$\mu$: 均值 、$\sigma$ :标准方差- 输入模型M，对M的每个像素点（R,G,B）计算I=R+G+B r=R/I, g=G/I, b=B/I
- 根据得到权重比例值，计算得到对应的均值 与标准方差
- 对输入图像的每个像素点计算根据高斯公式计算P®与P(g)的乘积
- 归一化之后输出结果，即为最终基于高斯PDF的反向投影图像

### 三、算法步骤与代码实现
- 首先加载模型图像与测试图像
- 根据模型图像计算得到每个通道对应的均值与标准方差参数
- 根据参数方差计算每个像素点的PDF值
- 归一化概率分布图像-即为反向投影图像，显示
- 根据Mask得到最终颜色模型对象分割

原图为下，roi 模型图片为蓝框区域
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111821225140.png)
```python
import cv2
import numpy as np


#roi 模型图片
roi = cv2.imread('timg_1.png')
hsv = cv2.cvtColor(roi,cv2.COLOR_BGR2HSV)

#目标图片
target = cv2.imread('timg.png')
hsvt = cv2.cvtColor(target,cv2.COLOR_BGR2HSV)

#计算目标直方图 >> 归一化直方图并应用反投影
roi_hist = cv2.calcHist([hsv],[0,1],None,[180,256],[0,180,0,256])
roi_normalize = cv2.normalize(roi_hist,0,255,cv2.NORM_MINMAX)
calc_Back_Project = cv2.calcBackProject([hsvt],[0,1],roi_normalize,[0,180,0,256],1)

#卷积(构建椭圆结构)
kernel_disc = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))
filter2D_img = cv2.filter2D(calc_Back_Project,-1,kernel_disc)

#阈值二值化 >> 使用merge变成通道图像 >> 蒙板
ret,thresh = cv2.threshold(filter2D_img,50,255,0)
thresh = cv2.merge((thresh,thresh,thresh))
mask = cv2.bitwise_and(target,thresh)

#矩阵按列拼接
result = np.hstack((target,thresh,mask))

#添加文本
w,h = target.shape[:2]
font = cv2.FONT_HERSHEY_SIMPLEX
img_word0 = cv2.putText(result, "target", (10, 25), font, 0.8, (0, 0, 255), 2,)
img_word1 = cv2.putText(img_word0, "thresh", (w-30, 25), font, 0.8, (0, 0, 255), 2,)
img_result = cv2.putText(img_word1, "mask", (2*w-100, 25), font, 0.8, (0, 0, 255), 2,)

#显示图像
cv2.imshow('img_result',img_result)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181118212004323.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

```python
import cv2
import numpy as np


def read_img(roi_img,target_img):
    roi = cv2.imread(roi_img)
    roi = cv2.resize(roi,None,fx=0.5, fy=0.5, interpolation = cv2.INTER_CUBIC)
    hsv_roi = cv2.cvtColor(roi,cv2.COLOR_BGR2HSV)

    target_img = cv2.imread(target_img)
    target = cv2.resize(target_img,None,fx=0.5, fy=0.5, interpolation = cv2.INTER_CUBIC)
    hsv_target = cv2.cvtColor(target,cv2.COLOR_BGR2HSV)
    return target,hsv_roi,hsv_target

def calcHist(hsv_roi,hsv_target):
    #计算目标直方图 >> 归一化直方图并应用反投影
    roi_hist = cv2.calcHist([hsv_roi],[0,1],None,[180,256],[0,180,0,256])
    roi_normalize = cv2.normalize(roi_hist,0,255,cv2.NORM_MINMAX)
    calc_Back_Project = cv2.calcBackProject([hsv_target],[0,1],roi_normalize,[0,180,0,256],1)
    return calc_Back_Project

def filter2D(target_img,calc_Back_Project):
    kernel_disc = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))
    filter2D_img = cv2.filter2D(calc_Back_Project,-1,kernel_disc)

    ret,thresh = cv2.threshold(filter2D_img,50,255,0)
    thresh = cv2.merge((thresh,thresh,thresh))
    mask = cv2.bitwise_and(target_img,thresh)
    return thresh,mask



def Img_Outline(input_dir):
    original_img = cv2.imread(input_dir)
    gray_img = cv2.cvtColor(original_img, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray_img, (9, 9), 0)                     # 高斯模糊去噪（设定卷积核大小影响效果）
    _, RedThresh = cv2.threshold(blurred, 165, 255, cv2.THRESH_BINARY)  # 设定阈值165（阈值影响开闭运算效果）
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))          # 定义矩形结构元素
    closed = cv2.morphologyEx(RedThresh, cv2.MORPH_CLOSE, kernel)       # 闭运算（链接块）
    opened = cv2.morphologyEx(closed, cv2.MORPH_OPEN, kernel)           # 开运算（去噪点）
    return original_img, gray_img, RedThresh, closed, opened

def findContours_img(target_img, opened):
    image, contours, hierarchy = cv2.findContours(opened, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    c = sorted(contours, key=cv2.contourArea, reverse=True)[1] # 计算最大轮廓的旋转包围盒
    rect = cv2.minAreaRect(c)                                  # 获取包围盒（中心点，宽高，旋转角度）
    box = np.int0(cv2.boxPoints(rect))
    draw_img = cv2.drawContours(target_img.copy(), [box], -1, (0, 0, 255), 3)
    return box,draw_img
    
def Add_text(target,thresh,mask):
    result = np.hstack((target,thresh,mask))
    w,h = target.shape[:2]
    font = cv2.FONT_HERSHEY_SIMPLEX
    img_word0 = cv2.putText(result, "target", (10, 25), font, 0.8, (0, 0, 255), 2,)
    img_word1 = cv2.putText(img_word0, "thresh", (w-30, 25), font, 0.8, (0, 0, 255), 2,)
    img_result = cv2.putText(img_word1, "mask", (2*w-100, 25), font, 0.8, (0, 0, 255), 2,)
    return img_result

if __name__ =="__main__":
    roi_img = "./timg_1.png"
    target_img = "./timg.png"
    target,hsv_roi, hsv_target = read_img(roi_img,target_img)
    calc_Back_Project = calcHist(hsv_roi,hsv_target)
    thresh, mask = filter2D(target_img,calc_Back_Project)

    original_img, gray_img, RedThresh, closed, opened = Img_Outline(target_img)
    box,draw_img = findContours_img(original_img, opened)
    
    img_result = Add_text(target, thresh, mask)
    cv2.imshow('img_result',img_result)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
```

鸣谢
[https://mp.weixin.qq.com/s?__biz=MzA4MDExMDEyMw==&mid=2247483990&idx=1&sn=95d61a4a005e349bbb4022f9f3a95a29&chksm=9fa87512a8dffc04a4ef0126404c88e494f37dd78ed6a8561a6d7f675d015548a69c6dd471e1&scene=21#wechat_redirect](https://mp.weixin.qq.com/s?__biz=MzA4MDExMDEyMw==&mid=2247483990&idx=1&sn=95d61a4a005e349bbb4022f9f3a95a29&chksm=9fa87512a8dffc04a4ef0126404c88e494f37dd78ed6a8561a6d7f675d015548a69c6dd471e1&scene=21#wechat_redirect)








