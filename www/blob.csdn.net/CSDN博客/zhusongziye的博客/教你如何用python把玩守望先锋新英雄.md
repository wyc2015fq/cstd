# 教你如何用python把玩守望先锋新英雄 - zhusongziye的博客 - CSDN博客





2018年12月27日 21:30:12[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：138








本文约1500字，阅读需要5分钟

**关键词：Python P图  OpenCV**

本文讲述了借助Python语言和OpenCV工具库完成一个简单的P图操作的过程~

p.s. **文末有源代码分享**



作为一名可视化工程师，如果只会使用PS，那么你一定OUT了。在专注各种花式P图多年，在学习PS,AI,PR,AE,XD……无数设计软件之后，本人终于悟到了一门独步江湖的绝技——“代码P图”。



今天，我就把这门P图界的无上秘籍，传授给你！希望你在炫技（zuo si）的道路上越走越远。



来看一下今天的素材：守望先锋新上线的英雄——艾什的正面照：

![](https://img-blog.csdnimg.cn/20181227212459775.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

然后这是一张背景图：

![](https://img-blog.csdnimg.cn/20181227212527871.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

今天的任务很简单，**我要对第一张图中的人物进行抠图，然后贴在背景图上**。

这个操作用PS并不复杂，让我们来看一下这一过程如何用代码来实现~

****▼****

** 素材处理 **



**首先，导入一些工具包**



> 
opencv(cv2)，用于图像处理

numpy，用于数据计算。

matplotlib用于出图。




```
import cv2
import numpy as np
import matplotlib.pyplot as plt
```



**建立一个显示图片的函数**，便于在P图过程中，实时查看效果



> 
使用的是matplotlib的功能，和制作图表的过程一致。




```
#建立显示图片的函数
def show(image):
    plt.imshow(image)
    plt.axis('off')
    plt.show()
```



**然后，导入前景图**。



> 
因为opencv的图片默认使用BGR图像格式，而我们通常使用的图片是RGB（红，绿，蓝），所以，需要再转换一下格式，否则查看时颜色会失真。




最后**打印图片规格和图片本身**

```
#导入前景图
img=cv2.imread('img.png') #图片导入
img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB) #转换颜色模型
print(img.shape) #打印图片规格
show(img) #显示图片
```



来看一下效果，高1054像素，长703像素，3通道。

![](https://img-blog.csdnimg.cn/20181227212626894.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



同样的方法，**导入背景图**

```
#导入背景图
back_img = cv2.imread('back_img.jpg') #图片导入
back_img = cv2.cvtColor(back_img,cv2.COLOR_BGR2RGB) #转换颜色模型
print(back_img.shape) #打印图片规格
show(back_img) #显示图片
```



效果如下，高1079，长1920,3通道。

![](https://img-blog.csdnimg.cn/20181227212639631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

我们发现人物图高度和背景高度差不多，且我们只要中间的人像即可，那么我们先来适当地裁剪一下图片

```
#裁剪图片
img = img[0:1000,150:550] #裁剪图片大小
show(img) #显示图片
```



**通过切片，裁去了logo**

![](https://img-blog.csdnimg.cn/20181227212653861.png)



**再对图片缩小10%**，这样大小最为合适



```
#缩放图片
print(img.shape) #打印图片规格
img=cv2.resize(img,None,fx=0.9,fy=0.9) #图片缩小10%
print(img.shape) #打印图片规格
```



**打印一下图片尺寸，发现裁剪成功**

![](https://img-blog.csdnimg.cn/20181227212713655.png)

> 
图片在计算机中是用数字矩阵形式保存的，红、绿、蓝三个颜色通道每种色各分为256阶，分别由0-255这256个数表示。比如900*360的图片，可以理解为900行360列的像素矩阵，而每个像素又是由R，G，B三个数字确认其颜色的。于是，我们先把图片的行，列数记录下来，稍后可以用诸如遍历的方法读取每个像素，再对其进行矩阵计算。




```
#拆分图片信息
rows,cols,channels = img.shape #拆分图片信息
```





****▼****

** 抠图：三种效果 **



抠图的方法雷同PS，我们要先建立个蒙版。在开始之前，我们先需要把图片转换成HSV格式，这是一种比较直观的颜色模型，可以更好的数字化处理颜色。

```
#转换格式
img_hsv = cv2.cvtColor(img,cv2.COLOR_RGB2HSV) #把图片转换成HSV格式，用于抠图
show(img_hsv) #显示图片
```



看下效果：

![](https://img-blog.csdnimg.cn/20181227212736688.png)



虽然不能直视，但做法显而易见，只要把非蓝色的部分提取出来。我们设定一个阈值，在最小阈值以下和最大阈值以上，图像变为0，而在阈值之间的变为255。

```
#抠图
lower_blue=np.array([0,0,0]) #获取最小阈值
upper_blue=np.array([0,255,255]) #获取最大阈值
mask = cv2.inRange(img_hsv, lower_blue, upper_blue) #创建遮罩
show(mask) #显示遮罩
```



然后，遮罩就这么给整了出来。

![](https://img-blog.csdnimg.cn/20181227212747306.png)

不过，我们发现，人物中间有那么多小点点，我需要把它们去掉。这里使用**形态学图像处理**的基本方法，先腐蚀后膨胀。其原理是在原图的小区域内取局部最小值和最大值，背后的逻辑为深度学习中的卷积神经网络。



通过尝试，我发现还可以使用开运算（先腐蚀后膨胀的整合运算）直接完成这一过程，且效果相对较好。



```
erode=cv2.erode(mask,None,iterations=3) #图像腐蚀
show(erode) #显示图片
dilate=cv2.dilate(erode,None,iterations=1) #图像膨胀
show(dilate) #显示图片

opening = cv2.morphologyEx(mask, cv2.MORPH_OPEN, cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (8,8))) #开运算
show(opening) #显示图片
```



大家可以自行比较下**腐蚀**，**腐蚀后膨胀**和**开运算**的效果：

![](https://img-blog.csdnimg.cn/20181227212801220.png)

****▼****

** 图像合并 **



最后，终于到了图像合并环节。先设定人物在背景图中的起始位置。再遍历遮罩中的每个像素，如果是0（代表黑色），则把人物图像上的颜色赋值到背景图像上。

```
center = [70,240] #设置前景图开始位置
for i in range(rows):
    for j in range(cols):
        if opening[i,j]==0: #代表黑色
            back_img[center[0]+i,center[1]+j] =img[i,j] #赋值颜色
show(back_img) #显示图片
```



运行完毕，显示结果：

![](https://img-blog.csdnimg.cn/2018122721281361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

受限于图片质量和简化代码，略显粗糙，但大体已经达到功能~

最后，调整图片格式，并保存。

```
back_img = cv2.cvtColor(back_img,cv2.COLOR_RGB2BGR) #图像格式转换
back_img=cv2.resize(back_img,None,fx=0.8,fy=0.8) #图像缩放20%
cv2.imwrite('result.png',back_img) #保存图像
```



这就是代码抠图的全过程，是不是很简单！

源码：

链接:https://pan.baidu.com/s/1lb-iP1HOIOtPDS0utli0Iw 提取码:khbl



