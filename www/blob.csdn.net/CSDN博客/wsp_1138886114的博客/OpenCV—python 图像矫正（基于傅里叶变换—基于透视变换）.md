# OpenCV—python 图像矫正（基于傅里叶变换—基于透视变换） - wsp_1138886114的博客 - CSDN博客





2018年10月29日 15:28:56[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：2429











### 文章目录
- [一、基于傅里叶变换的图像矫正](#_9)
- [1.1 傅里叶变换原理](#11__13)
- [1.2 傅里叶变换过程一系列函数](#12__26)
- [1.3 图像矫正处理流程](#13__68)
- [二、旋转图像矫正](#_148)
- [三、基于透视的图像矫正](#_201)
- [3.1 直接变换](#31__202)
- [3.2 自动获取图像顶点变换](#32__236)
- [直接获取图像轮廓矫正](#_243)
- [获取图像轮廓顶点矫正](#_286)



图像校正运行环境 `Anaconda| pycharm/jupyter notebook`

你需要安装如下库


```
pip install numpy
pip install matplotlib
pip install opencv-python
```

### 一、基于傅里叶变换的图像矫正

由于拍照时，图像被旋转，为方便观看或使用，需要对旋转图像进行校正。

旋转图像矫正流程

思路借鉴 [http://johnhany.net/2013/11/dft-based-text-rotation-correction/](http://johnhany.net/2013/11/dft-based-text-rotation-correction/)
##### 1.1 傅里叶变换原理

2维图像的傅立叶变换可以用以下数学公式表达:
$$F(k,l) = \displaystyle\sum\limits_{i=0}^{N-1}\sum\limits_{j=0}^{N-1} f(i,j)e^{-i2\pi(\frac{ki}{N}+\frac{lj}{N})}$$

$$e^{ix} = \cos{x} + i\sin {x}$$

式中 $f$ 是空间域(spatial domain)值， $F$ 则是频域(frequency domain)值。

转换之后的频域值是复数， 因此，显示傅立叶变换之后的结果需要使用实数图像(real image) 加虚数图像(complex image), 或者幅度图像(magitude image)加相位图像(phase image)。 在实际的图像处理过程中，仅仅使用了幅度图像，因为幅度图像包含了原图像的几乎所有我们需要的几何信息。然而，如果你想通过修改幅度图像或者相位图像的方法来间接修改原空间图像，你需要使用逆傅立叶变换得到修改后的空间图像，这样你就必须同时保留幅度图像和相位图像了。

在此示例中，我将展示如何计算以及显示傅立叶变换后的幅度图像。由于数字图像的离散性，像素值的取值范围也是有限的。比如在一张灰度图像中，像素灰度值一般在0到255之间。 因此，我们这里讨论的也仅仅是离散傅立叶变换(DFT)。 如果你需要得到图像中的几何结构信息，那你就要用到它了。

在频域里面，对于一幅图像，高频部分代表了图像的细节、纹理信息；低频部分代表了图像的轮廓信息。如果对一幅精细的图像使用低通滤波器，那么滤波后的结果就剩下了轮廓了。这与信号处理的基本思想是相通的。如果图像受到的噪声恰好位于某个特定的“频率”范围内，则可以通过滤波器来恢复原来的图像。傅里叶变换在图像处理中可以做到：图像增强与图像去噪，图像分割之边缘检测，图像特征提取，图像压缩等等。

##### 1.2 傅里叶变换过程一系列函数

**1 . 将图像延展到最佳尺寸（提高运行速度）**
- getOptimalDFTSize():返回给定尺寸的傅里叶最优尺寸大小，提高运行速度（需要扩充图像，填充多少由此函数计算得到：被计算的数字序列长度vecsize为2的n次幂或者能够分解成2,3,5的乘积）
- copyMakeBorder() 边界扩充函数

**2 . 为傅立叶变换的结果(实部和虚部)分配存储空间.** 傅立叶变换的结果是复数，这就是说对于每个原图像值，结果是两个图像值。 此外，频域值范围远远超过空间值范围， 因此至少要将频域储存在 float 格式中。 结果我们将输入图像转换成浮点类型，并多加一个额外通道来储存复数部分
**3 . 进行离散傅立叶变换**. 支持图像原地计算 (输入输出为同一图像):
- dtf() ：对一维或者二维浮点数数组进行正向或反向傅里叶变换

**4 . 将复数转换为幅度**复数包含实数部分(Re)和复数部分 (imaginary - Im)。 离散傅立叶变换的结果是复数，对应的幅度可以表示为:
$$M = \sqrt[2]{ {Re(DFT(I))}^2 + {Im(DFT(I))}^2}$$
- magnitude() 计算二维矢量的幅值

**5 . 对数尺度(logarithmic scale)缩放**. 傅立叶变换的幅度值范围大到不适合在屏幕上显示。高值在屏幕上显示为白点，而低值为黑点，高低值的变化无法有效分辨。为了在屏幕上凸显出高低变化的连续性，我们可以用对数尺度来替换线性尺度:
$$M_1 = \log{(1 + M)}$$
- log() 自然对数函数

**6 . 剪切和重分布幅度图象限**.将新添加的像素剔除。为了方便显示，我们也可以重新分布幅度图象限位置(注：将第五步得到的幅度图从中间划开得到四张1/4子图像，将每张子图像看成幅度图的一个象限，重新分布即将四个角点重叠到图片中心)。 这样的话原点(0,0)就位移到图像中心。

**7 . 归一化**. 将像素值归一到 float(0,1) 再乘以 255用于显示 。
- normalize() 矩阵归一化函数

```python
cv2.getOptimalDFTSize(vecsize)
# vecsize： 传入：image.shape[0]/image.shape[1]

cv2.copyMakeBorder(src,top,bottom,left,right,borderType,dst=None)
"""
	src：  图像
	top,bottom,left,right：  上/下/左/右边扩充像素（int）
	borderType： 边界类型：
		BORDER_CONSTANT:   常量，增加的变量通通为value
		BORDER_REFLICATE:  直接用边界的颜色填充,比如 : aaaaaa | abcdefgh | hhhhhhh
		BORDER_REFLECT:    镜像：比如 : fedcba | abcdefgh | hgfedcb
		BORDER_REFLECT_101:倒映，和上面类似，但在倒映时，会把边界空开：比如 : gfedcb | abcdefgh  |gfedcba
		BORDER_WRAP:       没有规律的，比如: cdefgh | abcdefgh | abcdefg
"""
cv2.magnitude(InputArray x, InputArray y, OutPutArray magnitude)
"""
计算输入矩阵x和y对应该的每个像素平方求和后开根号保存在输出矩阵magnitude中。
"""
```

##### 1.3 图像矫正处理流程
- 获取图像的傅里叶变换图
- 二值化
- Hough直线检测
- 计算倾斜角度
- 旋转校正

```python
import cv2
import numpy as np
import math

def fourier_demo():
    #1、灰度化读取文件，
    img = cv2.imread('english_rotation.jpg',0)

    #2、图像延扩
    h, w = img.shape[:2]
    new_h = cv2.getOptimalDFTSize(h)
    new_w = cv2.getOptimalDFTSize(w)
    right = new_w - w
    bottom = new_h - h
    nimg = cv2.copyMakeBorder(img, 0, bottom, 0, right, borderType=cv2.BORDER_CONSTANT, value=0)
    cv2.imshow('new image', nimg)

    #3、执行傅里叶变换，并过得频域图像
    f = np.fft.fft2(nimg)
    fshift = np.fft.fftshift(f)
    magnitude = np.log(np.abs(fshift))


    #二值化
    magnitude_uint = magnitude.astype(np.uint8)
    ret, thresh = cv2.threshold(magnitude_uint, 11, 255, cv2.THRESH_BINARY)
    print(ret)

    cv2.imshow('thresh', thresh)
    print(thresh.dtype)
    #霍夫直线变换
    lines = cv2.HoughLinesP(thresh, 2, np.pi/180, 30, minLineLength=40, maxLineGap=100)
    print(len(lines))

    #创建一个新图像，标注直线
    lineimg = np.ones(nimg.shape,dtype=np.uint8)
    lineimg = lineimg * 255

    piThresh = np.pi/180
    pi2 = np.pi/2
    print(piThresh)

    for line in lines:
        x1, y1, x2, y2 = line[0]
        cv2.line(lineimg, (x1, y1), (x2, y2), (0, 255, 0), 2)
        if x2 - x1 == 0:
            continue
        else:
            theta = (y2 - y1) / (x2 - x1)
        if abs(theta) < piThresh or abs(theta - pi2) < piThresh:
            continue
        else:
            print(theta)

    angle = math.atan(theta)
    print(angle)
    angle = angle * (180 / np.pi)
    print(angle)
    angle = (angle - 90)/(w/h)
    print(angle)

    center = (w//2, h//2)
    M = cv2.getRotationMatrix2D(center, angle, 1.0)
    rotated = cv2.warpAffine(img, M, (w, h), flags=cv2.INTER_CUBIC, borderMode=cv2.BORDER_REPLICATE)
    cv2.imshow('line image', lineimg)
    cv2.imshow('rotated', rotated)

fourier_demo()
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181029163321404.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_27,color_FFFFFF,t_70)

### 二、旋转图像矫正

图像旋转校正思路如下
- 读入,灰度化
- 高斯模糊
- 二值化图像
- 闭开运算
- 获取图像顶点
- 旋转矫正

```python
import cv2
import numpy as np

def Img_Outline(input_dir):
    original_img = cv2.imread(input_dir)
    gray_img = cv2.cvtColor(original_img, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray_img, (9, 9), 0)                     # 高斯模糊去噪（设定卷积核大小影响效果）
    _, RedThresh = cv2.threshold(blurred, 165, 255, cv2.THRESH_BINARY)  # 设定阈值165（阈值影响开闭运算效果）
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))          # 定义矩形结构元素
    closed = cv2.morphologyEx(RedThresh, cv2.MORPH_CLOSE, kernel)       # 闭运算（链接块）
    opened = cv2.morphologyEx(closed, cv2.MORPH_OPEN, kernel)           # 开运算（去噪点）
    return original_img, gray_img, RedThresh, closed, opened


def findContours_img(original_img, opened):
    image, contours, hierarchy = cv2.findContours(opened, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    c = sorted(contours, key=cv2.contourArea, reverse=True)[1]          # 计算最大轮廓的旋转包围盒
    rect = cv2.minAreaRect(c)
    angle = rect[2]
    print("angle",angle)
    box = np.int0(cv2.boxPoints(rect))
    draw_img = cv2.drawContours(original_img.copy(), [box], -1, (0, 0, 255), 3)
    rows, cols = original_img.shape[:2]
    M = cv2.getRotationMatrix2D((cols / 2, rows / 2), angle, 1)
    result_img = cv2.warpAffine(original_img, M, (cols, rows))
    return result_img,draw_img


if __name__ == "__main__":
    input_dir = "gongjiaoka.png"
    original_img, gray_img, RedThresh, closed, opened = Img_Outline(input_dir)
    result_img,draw_img = findContours_img(original_img,opened)

    cv2.imshow("original_img", original_img)
    cv2.imshow("gray_img", gray_img)
    cv2.imshow("RedThresh", RedThresh)
    cv2.imshow("Close", closed)
    cv2.imshow("Open", opened)
    cv2.imshow("draw_img", draw_img)
    cv2.imshow("result_img", result_img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181029112219470.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_27,color_FFFFFF,t_70)

### 三、基于透视的图像矫正

##### 3.1 直接变换
- 获取图像四个顶点
- 形成变换矩阵
- 透视变换

```python
import cv2
import numpy as np
import matplotlib.pyplot as plt
img = cv2.imread('original_img.jpg')
H_rows, W_cols= img.shape[:2]
print(H_rows, W_cols)

# 原图中书本的四个角点(左上、右上、左下、右下),与变换后矩阵位置
pts1 = np.float32([[161, 80], [449, 12], [1, 430], [480, 394]])
pts2 = np.float32([[0, 0],[W_cols,0],[0, H_rows],[H_rows,W_cols],])

# 生成透视变换矩阵；进行透视变换
M = cv2.getPerspectiveTransform(pts1, pts2)
dst = cv2.warpPerspective(img, M, (500,470))

"""
注释代码同效
# img[:, :, ::-1]是将BGR转化为RGB
# plt.subplot(121), plt.imshow(img[:, :, ::-1]), plt.title('input')
# plt.subplot(122), plt.imshow(dst[:, :, ::-1]), plt.title('output')
# plt.show
"""

cv2.imshow("original_img",img)
cv2.imshow("result",dst)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181026143251246.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_27,color_FFFFFF,t_70)

##### 3.2 自动获取图像顶点变换
- 以灰度图读入
- 腐蚀膨胀，闭合等操作
- 二值化图像
- 获取图像顶点
- 透视矫正

###### 直接获取图像轮廓矫正

该方法不具有普适性，只针对比较干净对比度高的图像，只提供一种思路

```python
from imutils.perspective import four_point_transform
import imutils
import cv2

def Get_Outline(input_dir):
    image = cv2.imread(input_dir)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (5,5),0)
    edged = cv2.Canny(blurred,75,200)
    return image,gray,edged

def Get_cnt(edged):
    cnts = cv2.findContours(edged.copy(), cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
    cnts = cnts[0] if  imutils.is_cv2()  else   cnts[1]
    docCnt =None

    if len(cnts) > 0:
        cnts =sorted(cnts,key=cv2.contourArea,reverse=True)
        for c in cnts:
            peri = cv2.arcLength(c,True)                   # 轮廓按大小降序排序
            approx = cv2.approxPolyDP(c,0.02 * peri,True)  # 获取近似的轮廓
            if len(approx) ==4:                            # 近似轮廓有四个顶点
                docCnt = approx
                break
    return docCnt

if __name__=="__main__":
    input_dir = "gongjiaoka.png"
    image,gray,edged = Get_Outline(input_dir)
    docCnt = Get_cnt(edged)
    result_img = four_point_transform(image, docCnt.reshape(4,2)) # 对原始图像进行四点透视变换
    cv2.imshow("original", image)
    cv2.imshow("gray", gray)
    cv2.imshow("edged", edged)
    cv2.imshow("result_img", result_img)

    cv2.waitKey(0)
    cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181029105011891.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_27,color_FFFFFF,t_70)

###### 获取图像轮廓顶点矫正

```python
import cv2
import math
import numpy as np

def Img_Outline(input_dir):
    original_img = cv2.imread(input_dir)
    gray_img = cv2.cvtColor(original_img, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray_img, (9, 9), 0)                     # 高斯模糊去噪（设定卷积核大小影响效果）
    _, RedThresh = cv2.threshold(blurred, 165, 255, cv2.THRESH_BINARY)  # 设定阈值165（阈值影响开闭运算效果）
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))          # 定义矩形结构元素
    closed = cv2.morphologyEx(RedThresh, cv2.MORPH_CLOSE, kernel)       # 闭运算（链接块）
    opened = cv2.morphologyEx(closed, cv2.MORPH_OPEN, kernel)           # 开运算（去噪点）
    return original_img, gray_img, RedThresh, closed, opened


def findContours_img(original_img, opened):
    image, contours, hierarchy = cv2.findContours(opened, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    c = sorted(contours, key=cv2.contourArea, reverse=True)[1]   # 计算最大轮廓的旋转包围盒
    rect = cv2.minAreaRect(c)                                    # 获取包围盒（中心点，宽高，旋转角度）
    box = np.int0(cv2.boxPoints(rect))                           # box
    draw_img = cv2.drawContours(original_img.copy(), [box], -1, (0, 0, 255), 3)

    print("box[0]:", box[0])
    print("box[1]:", box[1])
    print("box[2]:", box[2])
    print("box[3]:", box[3])
    return box,draw_img

def Perspective_transform(box,original_img):
    # 获取画框宽高(x=orignal_W,y=orignal_H)
    orignal_W = math.ceil(np.sqrt((box[3][1] - box[2][1])**2 + (box[3][0] - box[2][0])**2))
    orignal_H= math.ceil(np.sqrt((box[3][1] - box[0][1])**2 + (box[3][0] - box[0][0])**2))

    # 原图中的四个顶点,与变换矩阵
    pts1 = np.float32([box[0], box[1], box[2], box[3]])
    pts2 = np.float32([[int(orignal_W+1),int(orignal_H+1)], [0, int(orignal_H+1)], [0, 0], [int(orignal_W+1), 0]])

    # 生成透视变换矩阵；进行透视变换
    M = cv2.getPerspectiveTransform(pts1, pts2)
    result_img = cv2.warpPerspective(original_img, M, (int(orignal_W+3),int(orignal_H+1)))

    return result_img

if __name__=="__main__":
    input_dir = "gongjiaoka.png"
    original_img, gray_img, RedThresh, closed, opened = Img_Outline(input_dir)
    box, draw_img = findContours_img(original_img,opened)
    result_img = Perspective_transform(box,original_img)
    cv2.imshow("original", original_img)
    cv2.imshow("gray", gray_img)
    cv2.imshow("closed", closed)
    cv2.imshow("opened", opened)
    cv2.imshow("draw_img", draw_img)
    cv2.imshow("result_img", result_img)

    cv2.waitKey(0)
    cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018102915241829.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_27,color_FFFFFF,t_70)




