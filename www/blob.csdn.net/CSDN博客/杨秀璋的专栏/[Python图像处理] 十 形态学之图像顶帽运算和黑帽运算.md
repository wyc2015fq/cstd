# [Python图像处理] 十.形态学之图像顶帽运算和黑帽运算 - 杨秀璋的专栏 - CSDN博客





2018年11月04日 00:03:23[Eastmount](https://me.csdn.net/Eastmount)阅读数：1829
所属专栏：[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)[Python图像处理及图像识别](https://blog.csdn.net/column/details/26274.html)









该系列文章是讲解Python OpenCV图像处理知识，前期主要讲解图像入门、OpenCV基础用法，中期讲解图像处理的各种算法，包括图像锐化算子、图像增强技术、图像分割等，后期结合深度学习研究图像识别、图像分类应用。希望文章对您有所帮助，如果有不足之处，还请海涵~

同时推荐作者的C++图像系列知识：
[[数字图像处理] 一.MFC详解显示BMP格式图片](https://blog.csdn.net/eastmount/article/details/18238863)
[[数字图像处理] 二.MFC单文档分割窗口显示图片](https://blog.csdn.net/eastmount/article/details/18987539)
[[数字图像处理] 三.MFC实现图像灰度、采样和量化功能详解](https://blog.csdn.net/eastmount/article/details/46010637)
[[数字图像处理] 四.MFC对话框绘制灰度直方图](https://blog.csdn.net/eastmount/article/details/46237463)
[[数字图像处理] 五.MFC图像点运算之灰度线性变化、灰度非线性变化、阈值化和均衡化处理详解](https://blog.csdn.net/eastmount/article/details/46312145)
[[数字图像处理] 六.MFC空间几何变换之图像平移、镜像、旋转、缩放详解](https://blog.csdn.net/eastmount/article/details/46345299)
[[数字图像处理] 七.MFC图像增强之图像普通平滑、高斯平滑、Laplacian、Sobel、Prewitt锐化详解](https://blog.csdn.net/eastmount/article/details/46378783)
前文参考：
[[Python图像处理] 一.图像处理基础知识及OpenCV入门函数](https://blog.csdn.net/Eastmount/article/details/81748802)
[[Python图像处理] 二.OpenCV+Numpy库读取与修改像素](https://blog.csdn.net/eastmount/article/details/82120114)
[[Python图像处理] 三.获取图像属性、兴趣ROI区域及通道处理](https://blog.csdn.net/eastmount/article/details/82177300)
[[Python图像处理] 四.图像平滑之均值滤波、方框滤波、高斯滤波及中值滤波](https://blog.csdn.net/Eastmount/article/details/82216380)
[[Python图像处理] 五.图像融合、加法运算及图像类型转换](https://blog.csdn.net/Eastmount/article/details/82347501)
[[Python图像处理] 六.图像缩放、图像旋转、图像翻转与图像平移](https://blog.csdn.net/Eastmount/article/details/82454335)
[[Python图像处理] 七.图像阈值化处理及算法对比](https://blog.csdn.net/Eastmount/article/details/83548652)
[[Python图像处理] 八.图像腐蚀与图像膨胀](https://blog.csdn.net/Eastmount/article/details/83581277)
[[Python图像处理] 九.形态学之图像开运算、闭运算、梯度运算](https://blog.csdn.net/Eastmount/article/details/83651172)
数学形态学（Mathematical morphology）是一门建立在格论和拓扑学基础之上的图像分析学科，是数学形态学图像处理的基本理论。其基本的运算包括：腐蚀和膨胀、开运算和闭运算、骨架抽取、极限腐蚀、击中击不中变换、形态学梯度、Top-hat变换、颗粒分析、流域变换等。

本篇文章主要讲解Python调用OpenCV实现图像形态学转化，包括图像顶帽运算和图像黑帽运算，基础性知识希望对您有所帮助。
**1.图像顶帽运算**
**2.图像黑帽运算**
PS：文章参考自己以前系列图像处理文章及OpenCV库函数，同时部分参考网易云lilizong老师的视频，推荐大家去学习。同时，本篇文章涉及到《计算机图形学》基础知识，请大家下来补充。

PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。


# 一. 图像顶帽运算

**1.基本原理**

图像顶帽（或图像礼帽）运算是原始图像减去图像开运算的结果，得到图像的噪声。如下图所示：

顶帽运算(img) = 原始图像(img) - 开运算(img)
![](https://img-blog.csdnimg.cn/20181103234231406.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.函数原型**

图像开运算主要使用的函数morphologyEx，它是形态学扩展的一组函数，其参数cv2.MORPH_TOPHAT对应开运算。其原型如下：
**dst = cv2.morphologyEx(src, cv2.MORPH_TOPHAT, kernel)**
参数dst表示处理的结果，src表示原图像，cv2.MORPH_TOPHAT表示顶帽运算，kernel表示卷积核。下图表示5*5的卷积核，可以采用函数 np.ones((5,5), np.uint8) 构建。
![](https://img-blog.csdnimg.cn/20181103234508682.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
卷积如下图所示：
![](https://img-blog.csdnimg.cn/20181103234614692.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.代码实现**

完整代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np  

#读取图片
src = cv2.imread('test01.png', cv2.IMREAD_UNCHANGED)

#设置卷积核
kernel = np.ones((5,5), np.uint8)

#图像顶帽运算
result = cv2.morphologyEx(src, cv2.MORPH_TOPHAT, kernel)

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", result)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示，可以看到外部噪声被提取出来。
![](https://img-blog.csdnimg.cn/20181103234956786.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
如果想获取更多的细节，可以将卷积设置为10*10，如下图所示：

kernel = np.ones((10,10), np.uint8)

result = cv2.morphologyEx(src, cv2.MORPH_TOPHAT, kernel)
![](https://img-blog.csdnimg.cn/20181103235122143.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 二. 图像黑帽运算

**1.基本原理**

图像黑帽运算是图像闭运算操作减去原始图像的结果，得到图像内部的小孔，或者前景色中的小黑点。如下图所示：

黑帽运算(img) = 闭运算图像(img) - 原始图像(img)
![](https://img-blog.csdnimg.cn/20181103235502545.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.函数原型**

图像开运算主要使用的函数morphologyEx，它是形态学扩展的一组函数，其参数cv2.MORPH_BLACKHAT对应开运算。其原型如下：
**dst = cv2.morphologyEx(src, cv2.MORPH_BLACKHAT, kernel)**
参数dst表示处理的结果，src表示原图像，cv2.MORPH_BLACKHAT表示黑帽运算，kernel表示卷积核。下图表示5*5的卷积核，可以采用函数 np.ones((5,5), np.uint8) 构建。
![](https://img-blog.csdnimg.cn/20181103235643863.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.代码实现**

完整代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np  

#读取图片
src = cv2.imread('test02.png', cv2.IMREAD_UNCHANGED)

#设置卷积核
kernel = np.ones((5,5), np.uint8)

#图像黑帽运算
result = cv2.morphologyEx(src, cv2.MORPH_BLACKHAT, kernel)

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", result)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示，可以看到图像内部黑点被提取出来。
![](https://img-blog.csdnimg.cn/2018110323583288.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
但内部比较大的四个黑点没有被提取，如果想获取更多的细节，可以将卷积设置为10*10，如下图所示：

kernel = np.ones((10,10), np.uint8)

result = cv2.morphologyEx(src, cv2.MORPH_BLACKHAT, kernel)![](https://img-blog.csdnimg.cn/20181104000133101.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。最近经历的事情太多，有喜有悲，关闭了朋友圈，希望通过不断学习和写文章来忘记烦劳，将忧郁转换为动力，每周学习都记录下来，加油！！！

（By：Eastmount 2018-11-04 深夜12点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)









