# OpenCV—python 颜色空间（RGB，HSV，Lab）与 颜色直方图 - wsp_1138886114的博客 - CSDN博客





2018年07月13日 08:57:38[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：3493











### 文章目录
- [一、图像分类的过程](#_3)
- [二、开发环境：（安装）OpenCV 计算机视觉库](#OpenCV__23)
- [三、颜色空间（颜色模型：RGB，HSV，Lab）](#RGBHSVLab_41)
- [四、OpenCV处理图片](#OpenCV_68)
- [4.1 直接读取图片](#41__69)
- [4.2 读取图片并简单处理—图像特征：颜色直方图](#42__103)
- [RGB颜色空间](#RGB_148)
- [HSV颜色空间](#HSV_191)
- [L*a*b*颜色空间](#Lab_225)
- [灰度图与其颜色直方图](#_260)
- [带蒙版的颜色直方图](#_293)
- [多个颜色直方图](#_324)
- [按多个通道计算颜色直方图](#_348)



开发环境 jupyter notebook

### 一、图像分类的过程

![这里写图片描述](https://img-blog.csdn.net/20180808210306345?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
图片必须经过标注

图片的类别为有限的集合，如{猫、狗、牛、马、狼}

每个类别的图片数量接近，如果不同类别图片的数量差异很大，训练出来的分类器容易倾向于图片数量多的类别- 
划分训练/测试集

在训练之前，要把数据集划分为训练集（学习分类器）和测试集（对分类器进行评估）

训练集和测试集相互独立，不重合！- 
特征表示 ：对图片进行数值处理，转换成计算机可理解的特征，常用特征：

颜色直方图（`Color Histograms`）

方向梯度直方图（`Histogram of Oriented Gradients`，HOG）

局部二值模式（`Local Binary Pattern`，LBP）- 
分类算法 ：一般分类算法即可：逻辑回归，支持向量机（SVM），随机森林

- 
模型评估 ： 混淆矩阵（`Confusion Matrix`）

精确度（`Precision`）

召回率（`Recall`）

F1值（`F1 score`） recall与Precision的调和均值
### 二、开发环境：（安装）OpenCV 计算机视觉库

```python
```python
安装：
方法1：conda install -c menpo opencv 
方法2：conda install --channel https://conda.anaconda.org/menpo opencv3 
方法2：pip/conda install opencv-python
方法4：使用 whl 文件进行安装，进入网站 http://www.lfd.uci.edu/~gohlke/pythonlibs/。 
      下载与当前环境兼容的 opencv 文件。切换到该文件所在的目录，在命令行环境下使用 
      例如：安装文件：opencv_python-3.2.0-cp35-cp35m-win_amd64.whl
      pip install opencv_python-3.2.0-cp35-cp35m-win_amd64.whl

opencv 的导入： 
import cv2
cv2.__version__
	
	OpenCV的使用
		在线文档：http://docs.opencv.org/2.4.11/
```
```

### 三、颜色空间（颜色模型：RGB，HSV，Lab）

```
常见颜色空间有：
	RGB	:模型容易理解，连续变换颜色时不直观
		(红red    绿green    蓝blue)三原色：
		RGB取值范围[[0,255][0,255][0,255]]

HSV 是为了数字化图像提出来了，不能很好的表示人眼解释图像过程
	H (Hue)色相:[0,360]
	S (Saturation)饱和度，即色彩纯净度，0饱和度为白色
	V (Value/Brightness):明度	0明度为纯黑色
	
	在OpenCV中
		H = [0,179]
		S = [0,255]
		V = [0,255]
Lab：颜色之间的欧式距离有具体含义--距离越大，人眼感官两种颜色差距越远
	L 通道：像素亮度，上白下黑 中间灰
	a 通道：左绿 右红
	b 通道：一端纯蓝，一端纯黄
	
灰度图：每个像素[0,255]
	根据人眼敏感度，把RGB图片转换为灰度图，不是简单把RGB每个通道取平均值
	而是：
		Y = 0.299*R + 0.587*G + 0.114*B
```

![这里写图片描述](https://img-blog.csdn.net/20180808211311922?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 四、OpenCV处理图片

##### 4.1 直接读取图片

```python
```python
import cv2                      #导入 Opencv
import os 
import numpy as np
import matplotlib.pyplot as plt


output_dir = 'output2'	        #设置输出文件夹，若不存在则创建
if not os.path.exists(output_dir):
	os.mkdir(output_dir)	

img_file = './../image.png'     #读取图片
img = cv2.imread(img_file)

type(img)                       #读入图片后得到ndarray 对象
img.shape                       #ndarray的三个维度分别是图片的：高，宽，通道

# pyplot.imgshow 在显示图片时是按照RGB通道顺序显示，cv2则相反
# 需要通过 np.flip(img,axis = 2) 调整3个通道的顺序（若不调整图片颜色失真）
plt.imshow(np.flip(img,axis = 2))
plt.axis('off')
plt.show()                      #图1

plt.imshow(img)
plt.axis('off')                 #不显示坐标
plt.show()                      #图2

#输出并保存图片
output_image = os.path.join(output_dir,'image.png')
cv2.imwrite(output_image,img)
```
```

![这里写图片描述](https://img-blog.csdn.net/20180811141509322?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 4.2 读取图片并简单处理—图像特征：颜色直方图

OpenCV-Python中调用的直方图计算函数为`cv2.calcHist`。

```python
```python
"""
hist = cv2.calcHist([image],             # 传入图像（列表）
                    [0],                 # 使用的通道（使用通道：可选[0],[1],[2]）
                    None,                # 没有使用mask(蒙版)
                    [256],               # HistSize
                    [0.0,255.0])         # 直方图柱的范围
                                         # return->list
"""

import cv2    
import numpy as np
import matplotlib.pyplot as plt

def calcAndDrawHist(image, color):  
    hist= cv2.calcHist([image], [0], None, [256], [0.0,255.0])  
    minVal, maxVal, minLoc, maxLoc = cv2.minMaxLoc(hist)  
    histImg = np.zeros([256,256,3], np.uint8)  
    hpt = int(0.9* 256);  
      
    for h in range(256):  
        intensity = int(hist[h]*hpt/maxVal)  
        cv2.line(histImg,(h,256), (h,256-intensity), color)  
    return histImg


if __name__ == '__main__':  
    original_img = cv2.imread("666.png")
    img = cv2.resize(original_img,None,fx=0.6,fy=0.6,interpolation = cv2.INTER_CUBIC)
    b, g, r = cv2.split(img)  
  
    histImgB = calcAndDrawHist(b, [255, 0, 0])  
    histImgG = calcAndDrawHist(g, [0, 255, 0])  
    histImgR = calcAndDrawHist(r, [0, 0, 255])  

    cv2.imshow("histImgB", histImgB)  
    cv2.imshow("histImgG", histImgG)  
    cv2.imshow("histImgR", histImgR)  
    cv2.imshow("Img", img)  
    cv2.waitKey(0)  
    cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20181004091601257?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### RGB颜色空间

```python
```python
import cv2
import numpy as np
from matplotlib import pyplot as plt
%matplotlib inline 

pic_file = '../data/images/image_crocus_0003.png'

img_bgr = cv2.imread(pic_file, cv2.IMREAD_COLOR) #OpenCV读取颜色顺序：BRG 
img_b = img_bgr[..., 0]
img_g = img_bgr[..., 1]
img_r = img_bgr[..., 2]
fig = plt.gcf()                                  #图片详细信息


fig = plt.gcf()                                  #分通道显示图片
fig.set_size_inches(10, 15)

plt.subplot(221)
plt.imshow(np.flip(img_bgr, axis=2))             #展平图像数组并显示
plt.axis('off')
plt.title('Image')

plt.subplot(222)
plt.imshow(img_r, cmap='gray')
plt.axis('off')
plt.title('R')

plt.subplot(223)
plt.imshow(img_g, cmap='gray')
plt.axis('off')
plt.title('G')

plt.subplot(224)
plt.imshow(img_b, cmap='gray')
plt.axis('off')
plt.title('B')

plt.show()
```
```

![这里写图片描述](https://img-blog.csdn.net/2018081114291652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### HSV颜色空间

```python
```python
img_hsv = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2HSV)
img_h = img_hsv[..., 0]
img_s = img_hsv[..., 1]
img_v = img_hsv[..., 2]

fig = plt.gcf()                      # 分通道显示图片
fig.set_size_inches(10, 15)

plt.subplot(221)
plt.imshow(img_hsv)
plt.axis('off')
plt.title('HSV')

plt.subplot(222)
plt.imshow(img_h, cmap='gray')
plt.axis('off')
plt.title('H')

plt.subplot(223)
plt.imshow(img_s, cmap='gray')
plt.axis('off')
plt.title('S')

plt.subplot(224)
plt.imshow(img_v, cmap='gray')
plt.axis('off')
plt.title('V')

plt.show()
```
```

![这里写图片描述](https://img-blog.csdn.net/20180811143245569?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### L*a*b*颜色空间

```python
```python
img_lab = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2LAB)
img_ls = img_lab[..., 0]
img_as = img_lab[..., 1]
img_bs = img_lab[..., 2] 

# 分通道显示图片
fig = plt.gcf()
fig.set_size_inches(10, 15)

plt.subplot(221)
plt.imshow(img_lab)
plt.axis('off')
plt.title('L*a*b*')

plt.subplot(222)
plt.imshow(img_ls, cmap='gray')
plt.axis('off')
plt.title('L*')

plt.subplot(223)
plt.imshow(img_as, cmap='gray')
plt.axis('off')
plt.title('a*')

plt.subplot(224)
plt.imshow(img_bs, cmap='gray')
plt.axis('off')
plt.title('b*')

plt.show()
```
```

![这里写图片描述](https://img-blog.csdn.net/20180811143638824?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 灰度图与其颜色直方图

```python
```python
img_gray = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2GRAY)
fig = plt.gcf()
fig.set_size_inches(5, 7.5)

plt.imshow(img_gray, cmap='gray')
plt.axis('off')
plt.title('Gray')
plt.show()
```
```

```python
```python
"""
	cv2.calcHist(images, channels, mask, histSize, ranges[, hist[, accumulate]]) → hist
	参数说明
	images:   图片列表
	channels: 需要计算直方图的通道。[0]表示计算通道0的直方图，[0,1,2]表示计算通道0,1,2所表示颜色的直方图
	mask:     蒙版，只计算值>0的位置上像素的颜色直方图，取None表示无蒙版
	histSize: 每个维度上直方图的大小，[8]表示把通道0的颜色取值等分为8份后计算直方图
	ranges:   每个维度的取值范围，[lower0, upper0, lower1, upper1, ...]，lower可以取到，upper无法取到
	hist:     保存结果的ndarray对象
	accumulate: 是否累积，如果设置了这个值，hist不会被清零，直方图结果直接累积到hist中
""" 

img_gray_hist = cv2.calcHist([img_gray], [0], None, [256], [0, 256])

plt.plot(img_gray_hist)
plt.title('Grayscale Histogram')
plt.xlabel('Bins')
plt.ylabel('# of Pixels')
plt.show()
```
```

![这里写图片描述](https://img-blog.csdn.net/20180811144734426?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 带蒙版的颜色直方图

```python
```python
# 读取模板
mask_file = '../data/masks/mask_crocus_0003.png'
mask = cv2.imread(mask_file, cv2.IMREAD_UNCHANGED) 
help(cv2.calcHist)
img_gray_hist_with_mask = cv2.calcHist([img_gray], [0], mask, [256], [0, 256])
"""
	图片按位与
	cv2.bitwise_and(src1, src2[, dst[, mask]]) → dst
	参数说明
		src1: 图片1
		src2: 图片2
		dst: 保存结果的ndarray对象
		mask: 蒙版 
"""
img_masked = cv2.bitwise_and(img_gray, img_gray, mask=mask)

plt.imshow(img_masked, cmap='gray')
plt.axis('off')
plt.title('Image with mask')
plt.show()

plt.plot(img_gray_hist_with_mask)
plt.title('Grayscale Histogram')
plt.xlabel('Bins')
plt.ylabel('# of Pixels')

plt.show()
```
```

![这里写图片描述](https://img-blog.csdn.net/20180811150309107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 多个颜色直方图

```python
```python
# 按R、G、B三个通道分别计算颜色直方图
b_hist = cv2.calcHist([img_bgr], [0], None, [256], [0, 256])
g_hist = cv2.calcHist([img_bgr], [1], None, [256], [0, 256])
r_hist = cv2.calcHist([img_bgr], [2], None, [256], [0, 256])

# 显示3个通道的颜色直方图
plt.plot(b_hist, label='B', color='blue')
plt.plot(g_hist, label='G', color='green')
plt.plot(r_hist, label='R', color='red')
plt.legend(loc='best')
plt.xlim([0, 256])
plt.show()

# 显示3个通道的颜色直方图
plt.plot(b_hist, label='B', color='blue')
plt.plot(g_hist, label='G', color='green')
plt.plot(r_hist, label='R', color='red')
plt.legend(loc='best')
plt.xlim([0, 256])
plt.show()
```
```

![这里写图片描述](https://img-blog.csdn.net/20180811150535706?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 按多个通道计算颜色直方图

```python
```python
# 把一个像素的多个通道合在一起看作一个值
hist = cv2.calcHist([img_bgr], [0, 1, 2], None, [8, 8, 8], [0, 256, 0, 256, 0, 256])

hist.shape
# 含义：B取值在[0, 32), G取值在[0, 32), R取值在[0, 32)的像素个数
hist[0, 0, 0]
```
```











