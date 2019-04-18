# OpenCV—python 形态学处理（腐蚀、膨胀、开闭运算、边缘检测） - wsp_1138886114的博客 - CSDN博客





2018年10月01日 17:52:56[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：2032











### 文章目录
- [一、定义结构元素](#_1)
- [二、腐蚀和膨胀](#_27)
- [三、开运算和闭运算](#_80)
- [四、礼帽/顶帽，黑帽 算法](#__125)
- [五、形态学运算 检测边和 角点](#___174)
- [5.1 检测边缘](#51__177)
- [5.2检测拐角](#52_211)




### 一、定义结构元素

形态学处理的核心就是定义结构元素，在OpenCV-Python中，可以使用其自带的`getStructuringElement`函数，也可以直接使用NumPy的ndarray来定义一个结构元素。(形象图如下：)
![在这里插入图片描述](https://img-blog.csdn.net/2018100117033846?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如下代码：为上图的十字形结构

当然还可以定义椭圆/矩形等：

椭圆：cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))

矩形：cv2.getStructuringElement(cv2.MORPH_RECT,(5,5))
```python
element = cv2.getStructuringElement(cv2.MORPH_CROSS,(5,5))
print("elemrnt":element )
------------------------------------------------
import numpy as np
NpKernel = np.uint8(np.zeros((5,5)))
for i in range(5):
	NpKernel[2, i] = 1
	NpKernel[i, 2] = 1
print("NpKernel ",NpKernel )

上述结果输出（相同）：
array([[0, 0, 1, 0, 0],
       [0, 0, 1, 0, 0],
       [1, 1, 1, 1, 1],
       [0, 0, 1, 0, 0],
       [0, 0, 1, 0, 0]], dtype=uint8)
```

### 二、腐蚀和膨胀
- 
腐蚀：腐蚀会把物体的边界腐蚀掉，卷积核沿着图象滑动，如果卷积核对应的原图的所有像素值为1，那么中心元素就保持原来的值，否则变为零。主要应用在去除白噪声，也可以断开连在一起的物体。

- 
膨胀：卷积核所对应的原图像的像素值只要有一个是1，中心像素值就是1。一般在除噪是，先腐蚀再膨胀，因为腐蚀在去除白噪声的时候也会使图像缩小，所以我们之后要进行膨胀。当然也可以用来将两者物体分开。


```python
"""
腐蚀
cv2.erode(src,                     # 输入图像
	  kernel,                  # 卷积核
	  dst=None, 
	  anchor=None,
	  iterations=None,         # 迭代次数，默认1
	  borderType=None,
	  borderValue=None) 

膨胀
cv2.dilate(src,                    # 输入图像
           kernel,                 # 卷积核
           dst=None, 
           anchor=None, 
           iterations=None,        # 迭代次数，默认1
           borderType=None, 
           borderValue=None)
"""

import cv2
import numpy as np
original_img = cv2.imread('flower.png')
res = cv2.resize(original_img,None,fx=0.6, fy=0.6,
                 interpolation = cv2.INTER_CUBIC) #图形太大了缩小一点
B, G, R = cv2.split(res)                    #获取红色通道
img = R
_,RedThresh = cv2.threshold(img,160,255,cv2.THRESH_BINARY)
#OpenCV定义的结构矩形元素
kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(3, 3))
eroded = cv2.erode(RedThresh,kernel)        #腐蚀图像
dilated = cv2.dilate(RedThresh,kernel)      #膨胀图像

cv2.imshow("original_img", res)             #原图像
cv2.imshow("R_channel_img", img)            #红色通道图
cv2.imshow("RedThresh", RedThresh)          #红色阈值图像
cv2.imshow("Eroded Image",eroded)           #显示腐蚀后的图像
cv2.imshow("Dilated Image",dilated)         #显示膨胀后的图像

#NumPy定义的结构元素
NpKernel = np.uint8(np.ones((3,3)))
Nperoded = cv2.erode(RedThresh,NpKernel)       #腐蚀图像
cv2.imshow("Eroded by NumPy kernel",Nperoded)  #显示腐蚀后的图像
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114191259360.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

### 三、开运算和闭运算

开运算和闭运算就是将腐蚀和膨胀按照一定的次序进行处理。

但这两者并不是可逆的，即先开后闭并不能得到原先的图像。

为了获取图像中的主要对象：对一副二值图连续使用闭运算和开运算，或者消除图像中的噪声，也可以对图像先用开运算后用闭运算，不过这样也会消除一些破碎的对象。- 开运算：先腐蚀后膨胀，用于移除由图像噪音形成的斑点。
- 闭运算：先膨胀后腐蚀，用来连接被误分为许多小块的对象；

```python
"""
cv2.morphologyEx(src,      # 输入图片
                 op,       # 需要处理类型的函数：(cv2.MORPH_OPEN,cv2.MORPH_CLOSE,cv2.MORPH_GRADIENT)
                 kernel,   # 卷积核大小
                 dst=None, 
                 anchor=None, 
                 iterations=None,     #迭代次数，默认1次
                 borderType=None, 
                 borderValue=None)
"""
import cv2
import numpy as np
original_img = cv2.imread('original_img.png',0)
gray_res = cv2.resize(original_img,None,fx=0.8,fy=0.8,
                 interpolation = cv2.INTER_CUBIC)                #图形太大了缩小一点
# B, G, img = cv2.split(res)
# _,RedThresh = cv2.threshold(img,160,255,cv2.THRESH_BINARY)     #设定红色通道阈值160（阈值影响开闭运算效果）
kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))         #定义矩形结构元素

closed1 = cv2.morphologyEx(gray_res, cv2.MORPH_CLOSE, kernel,iterations=1)    #闭运算1
closed2 = cv2.morphologyEx(gray_res, cv2.MORPH_CLOSE, kernel,iterations=3)    #闭运算2
opened1 = cv2.morphologyEx(gray_res, cv2.MORPH_OPEN, kernel,iterations=1)     #开运算1
opened2 = cv2.morphologyEx(gray_res, cv2.MORPH_OPEN, kernel,iterations=3)     #开运算2
gradient = cv2.morphologyEx(gray_res, cv2.MORPH_GRADIENT, kernel)             #梯度

#显示如下腐蚀后的图像
cv2.imshow("gray_res", gray_res)
cv2.imshow("Close1",closed1)
cv2.imshow("Close2",closed2)
cv2.imshow("Open1", opened1)
cv2.imshow("Open2", opened2)
cv2.imshow("gradient", gradient)

cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114193621437.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

### 四、礼帽/顶帽，黑帽 算法

```python
import cv2

original_img0 = cv2.imread('original_img.png')
original_img = cv2.imread('original_img.png',0)

kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))                  #定义矩形结构元素
TOPHAT_img = cv2.morphologyEx(original_img, cv2.MORPH_TOPHAT, kernel)     #顶帽运算
BLACKHAT_img = cv2.morphologyEx(original_img, cv2.MORPH_BLACKHAT, kernel) #黒帽运算

#显示图像
cv2.imshow("original_img0", original_img0)
cv2.imshow("original_img", original_img)
cv2.imshow("TOPHAT_img", TOPHAT_img)
cv2.imshow("BLACKHAT_img", BLACKHAT_img)

cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114200238630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

显然该算法可以图像识别的预处理，用于图像二值化后去除孤立点,如下图所示
```python
import cv2

original_img = cv2.imread('original_img.png',0)
gray_img = cv2.resize(original_img,None,fx=0.8, fy=0.8,
                 interpolation = cv2.INTER_CUBIC) #图形太大了缩小一点


kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))              #定义矩形结构元素(核大小为3效果好)
TOPHAT_img = cv2.morphologyEx(gray_img, cv2.MORPH_TOPHAT, kernel)     #顶帽运算
BLACKHAT_img = cv2.morphologyEx(gray_img, cv2.MORPH_BLACKHAT, kernel) #黒帽运算

bitwiseXor_gray = cv2.bitwise_xor(gray_img,TOPHAT_img)


#显示如下腐蚀后的图像
cv2.imshow("gray_img", gray_img)
cv2.imshow("TOPHAT_img", TOPHAT_img)
cv2.imshow("BLACKHAT_img", BLACKHAT_img)
cv2.imshow("bitwiseXor_gray",bitwiseXor_gray)


cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114202202435.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)显然噪点少了许多

### 五、形态学运算 检测边和 角点

形态学算子检测图像中的边缘和拐角（实际用：Canny或Harris等算法）

##### 5.1 检测边缘

形态学检测边缘的原理很简单：在膨胀时，图像中的物体会想周围“扩张”；腐蚀时，图像中的物体会“收缩”。由于这两幅图像其变化的区域只发生在边缘。所以这时将两幅图像相减，得到的就是图像中物体的边缘。

```python
import cv2
import numpy 

image = cv2.imread("jianzhu.png",cv2.IMREAD_GRAYSCALE)

kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(3, 3))
dilate_img = cv2.dilate(image, kernel)
erode_img = cv2.erode(image, kernel) 

"""
我选了一张较好的图片，有的图片要去噪（高斯模糊）
将两幅图像相减获得边；cv2.absdiff参数：(膨胀后的图像，腐蚀后的图像)
上面得到的结果是灰度图，将其二值化以便观察结果
反色，对二值图每个像素取反
"""
absdiff_img = cv2.absdiff(dilate_img,erode_img);
retval, threshold_img = cv2.threshold(absdiff_img, 40, 255, cv2.THRESH_BINARY); 
result = cv2.bitwise_not(threshold_img); 

cv2.imshow("jianzhu",image)
cv2.imshow("dilate_img",dilate_img)
cv2.imshow("erode_img",erode_img)
cv2.imshow("absdiff_img",absdiff_img)
cv2.imshow("threshold_img",threshold_img)
cv2.imshow("result",result)

cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdn.net/20181003081946242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 5.2检测拐角

拐角的检测的过程稍稍有些复杂。原理:

先用十字形的结构元素膨胀像素，这种情况下只会在边缘处“扩张”，角点不发生变化。

接着用菱形的结构元素腐蚀原图像，导致只有在拐角处才会“收缩”，而直线边缘都未发生变化。

第二步是用X形膨胀原图像，角点膨胀的比边要多。这样第二次用方块腐蚀时，角点恢复原状，而边要腐蚀的更多。所以当两幅图像相减时，只保留了拐角处。
```python
import cv2 

image = cv2.imread("./jianzhu.png",0)
original_image = image.copy()
#构造5×5的结构元素，分别为十字形、菱形、方形和X型
cross = cv2.getStructuringElement(cv2.MORPH_CROSS,(5, 5))
diamond = cv2.getStructuringElement(cv2.MORPH_RECT,(5, 5))
diamond[0, 0] = 0
diamond[0, 1] = 0
diamond[1, 0] = 0
diamond[4, 4] = 0
diamond[4, 3] = 0
diamond[3, 4] = 0
diamond[4, 0] = 0
diamond[4, 1] = 0
diamond[3, 0] = 0
diamond[0, 3] = 0
diamond[0, 4] = 0
diamond[1, 4] = 0
square = cv2.getStructuringElement(cv2.MORPH_RECT,(5, 5))  #构造方形结构元素
x = cv2.getStructuringElement(cv2.MORPH_CROSS,(5, 5))     

dilate_cross_img = cv2.dilate(image,cross)                #使用cross膨胀图像
erode_diamond_img = cv2.erode(dilate_cross_img, diamond)  #使用菱形腐蚀图像

dilate_x_img = cv2.dilate(image, x)                       #使用X膨胀原图像 
erode_square_img = cv2.erode(dilate_x_img,square)         #使用方形腐蚀图像 

result = cv2.absdiff(erode_square_img, erode_diamond_img)          #将两幅闭运算的图像相减获得角
retval, result = cv2.threshold(result, 40, 255, cv2.THRESH_BINARY) #使用阈值获得二值图

#在原图上用半径为5的圆圈将点标出。
for j in range(result.size):
    y = int(j / result.shape[0])
    x = int(j % result.shape[0])
    if result[x, y] == 255:                                        #result[] 只能传入整型
        cv2.circle(image,(y,x),5,(255,0,0))

cv2.imshow("original_image", original_image)
cv2.imshow("Result", image)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

![在这里插入图片描述](https://img-blog.csdn.net/20181004080137894?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

特别鸣谢
[https://blog.csdn.net/sunny2038/article/details/9137759](https://blog.csdn.net/sunny2038/article/details/9137759)
[https://blog.csdn.net/on2way/article/details/46850813](https://blog.csdn.net/on2way/article/details/46850813)







