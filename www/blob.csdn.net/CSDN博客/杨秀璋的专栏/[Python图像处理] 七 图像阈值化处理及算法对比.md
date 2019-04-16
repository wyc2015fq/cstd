# [Python图像处理] 七.图像阈值化处理及算法对比 - 杨秀璋的专栏 - CSDN博客





2018年10月30日 22:19:36[Eastmount](https://me.csdn.net/Eastmount)阅读数：1516
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
本篇文章主要讲解Python调用OpenCV实现图像阈值化处理操作，包括二进制阈值化、反二进制阈值化、截断阈值化、反阈值化为0、阈值化为0。全文均是基础知识，希望对您有所帮助。
**1.阈值化**
**2.二进制阈值化**
**3.反二进制阈值化**
**4.截断阈值化**
**5.反阈值化为0**
**6.阈值化为0**
PS：文章参考自己以前系列图像处理文章及OpenCV库函数，同时部分参考网易云视频，推荐大家去学习。同时，本篇文章涉及到《计算机图形学》基础知识，请大家下来补充。

PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。


# 一. 阈值化

（注：该部分参考作者的论文《基于苗族服饰的图像锐化和边缘提取技术研究》）

图像的二值化或阈值化（Binarization）旨在提取图像中的目标物体，将背景以及噪声区分开来。通常会设定一个阈值T，通过T将图像的像素划分为两类：大于T的像素群和小于T的像素群。

灰度转换处理后的图像中，每个像素都只有一个灰度值，其大小表示明暗程度。二值化处理可以将图像中的像素划分为两类颜色，常用的二值化算法如公式1所示：

$$\begin{cases}Y=0，gray&lt;T\\Y=255，gray&gt;=T\\\end{cases}$$

当灰度Gray小于阈值T时，其像素设置为0，表示黑色；当灰度Gray大于或等于阈值T时，其Y值为255，表示白色。

Python OpenCV中提供了阈值函数threshold()实现二值化处理，其公式及参数如下图所示：
**retval, dst = cv2.threshold(src, thresh, maxval, type)**![](https://img-blog.csdnimg.cn/20181030211150489.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
常用的方法如下表所示，其中函数中的参数Gray表示灰度图，参数127表示对像素值进行分类的阈值，参数255表示像素值高于阈值时应该被赋予的新像素值，最后一个参数对应不同的阈值处理方法。
![](https://img-blog.csdnimg.cn/20181030211333173.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
对应OpenCV提供的五张图如下所示，第一张为原图，后面依次为：二进制阈值化、反二进制阈值化、截断阈值化、反阈值化为0、阈值化为0。
![](https://img-blog.csdnimg.cn/20181030212012630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
二值化处理广泛应用于各行各业，比如生物学中的细胞图分割、交通领域的车牌设别等。在文化应用领域中，通过二值化处理将所需民族文物图像转换为黑白两色图，从而为后面的图像识别提供更好的支撑作用。下图表示图像经过各种二值化处理算法后的结果，其中“BINARY”是最常见的黑白两色处理。
![](https://img-blog.csdnimg.cn/20181030211808848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 二. 二进制阈值化

该方法先要选定一个特定的阈值量，比如127。新的阈值产生规则如下：
$$dst(x,y) = \begin{cases}maxVal， if src(x,y)&gt;thresh\\0，otherwise\\\end{cases}$$

(1) 大于等于127的像素点的灰度值设定为最大值（如8位灰度值最大为255）

(2) 灰度值小于127的像素点的灰度值设定为0

例如，163->255，86->0，102->0，201->255。
关键字为 cv2.THRESH_BINARY，完整代码如下：

```python
#encoding:utf-8
import cv2  
import numpy as np  

#读取图片
src = cv2.imread('test.jpg')

#灰度图像处理
GrayImage = cv2.cvtColor(src,cv2.COLOR_BGR2GRAY)

#二进制阈值化处理
r, b = cv2.threshold(GrayImage, 127, 255, cv2.THRESH_BINARY)
print r

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", b)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出为两个返回值，r为127，b为处理结果（大于127设置为255，小于设置为0）。如下图所示：
![](https://img-blog.csdnimg.cn/20181030215716828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 三. 反二进制阈值化

该方法与二进制阈值化方法相似，先要选定一个特定的灰度值作为阈值，比如127。新的阈值产生规则如下：
$$dst(x,y) = \begin{cases}0， if src(x,y)&gt;thresh\\maxVal，otherwise\\\end{cases}$$

(1) 大于127的像素点的灰度值设定为0（以8位灰度图为例）

(2) 小于该阈值的灰度值设定为255

例如，163->0，86->255，102->255，201->0。
关键字为 cv2.THRESH_BINARY_INV，完整代码如下：

```python
#encoding:utf-8
import cv2  
import numpy as np  

#读取图片
src = cv2.imread('test.jpg')

#灰度图像处理
GrayImage = cv2.cvtColor(src,cv2.COLOR_BGR2GRAY)

#反二进制阈值化处理
r, b = cv2.threshold(GrayImage, 127, 255, cv2.THRESH_BINARY_INV)
print r

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", b)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20181030220119698.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
该方法得到的结果正好与二进制阈值化方法相反，亮色元素反而处理为黑色，暗色处理为白色。


# 四. 截断阈值化

该方法需要选定一个阈值，图像中大于该阈值的像素点被设定为该阈值，小于该阈值的保持不变，比如127。新的阈值产生规则如下：
$$dst(x,y) = \begin{cases}threshold， if src(x,y)&gt;thresh\\src(x,y)，otherwise\\\end{cases}$$

(1) 大于等于127的像素点的灰度值设定为该阈值127

(2) 小于该阈值的灰度值不改变

例如，163->127，86->86，102->102，201->127。
关键字为 cv2.THRESH_TRUNC，完整代码如下：

```python
#encoding:utf-8
import cv2  
import numpy as np  

#读取图片
src = cv2.imread('test.jpg')

#灰度图像处理
GrayImage = cv2.cvtColor(src,cv2.COLOR_BGR2GRAY)

#截断阈值化处理
r, b = cv2.threshold(GrayImage, 127, 255, cv2.THRESH_TRUNC)
print r

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", b)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：：
![](https://img-blog.csdnimg.cn/20181030220405196.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
该处理方法相当于把图像中比较亮（大于127，偏向于白色）的像素值处理为阈值。


# 五. 反阈值化为0

该方法先选定一个阈值，比如127，接着对图像的灰度值进行如下处理：
$$dst(x,y) = \begin{cases}0， if src(x,y)&gt;thresh\\src(x,y)，otherwise\\\end{cases}$$

(1) 大于等于阈值127的像素点变为0

(2) 小于该阈值的像素点值保持不变

例如，163->0，86->86，102->102，201->0。
关键字为 cv2.THRESH_TOZERO_INV，完整代码如下：

```python
#encoding:utf-8
import cv2  
import numpy as np  

#读取图片
src = cv2.imread('test.jpg')

#灰度图像处理
GrayImage = cv2.cvtColor(src,cv2.COLOR_BGR2GRAY)

#反阈值化为0处理
r, b = cv2.threshold(GrayImage, 127, 255, cv2.THRESH_TOZERO_INV)
print r

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", b)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20181030220813247.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 六. 阈值化为0

该方法先选定一个阈值，比如127，接着对图像的灰度值进行如下处理：
$$dst(x,y) = \begin{cases}src(x,y)， if src(x,y)&gt;thresh\\0，otherwise\\\end{cases}$$

(1) 大于等于阈值127的像素点，值保持不变

(2) 小于该阈值的像素点值设置为0

例如，163->163，86->0，102->0，201->201。
关键字为 cv2.THRESH_TOZERO，完整代码如下：

```python
#encoding:utf-8
import cv2  
import numpy as np  

#读取图片
src = cv2.imread('test.jpg')

#灰度图像处理
GrayImage = cv2.cvtColor(src,cv2.COLOR_BGR2GRAY)

#阈值化为0处理
r, b = cv2.threshold(GrayImage, 127, 255, cv2.THRESH_TOZERO)
print r

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", b)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20181030221043790.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
该算法把比较亮的部分不变，比较暗的部分处理为0。

完整五个算法的对比代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt

#读取图像
img=cv2.imread('test.jpg')
lenna_img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
GrayImage=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)  

#阈值化处理
ret,thresh1=cv2.threshold(GrayImage,127,255,cv2.THRESH_BINARY)  
ret,thresh2=cv2.threshold(GrayImage,127,255,cv2.THRESH_BINARY_INV)  
ret,thresh3=cv2.threshold(GrayImage,127,255,cv2.THRESH_TRUNC)  
ret,thresh4=cv2.threshold(GrayImage,127,255,cv2.THRESH_TOZERO)  
ret,thresh5=cv2.threshold(GrayImage,127,255,cv2.THRESH_TOZERO_INV)

#显示结果
titles = ['Gray Image','BINARY','BINARY_INV','TRUNC','TOZERO','TOZERO_INV']  
images = [GrayImage, thresh1, thresh2, thresh3, thresh4, thresh5]  
for i in xrange(6):  
   plt.subplot(2,3,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20181030221502568.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。最近经历的事情太多，有喜有悲，关闭了朋友圈，希望通过不断学习和写文章来忘记烦劳，将忧郁转换为动力。哎，总感觉自己在感动这个世界，帮助所有人，而自己却…谁有关心秀璋？晚安。

（By：Eastmount 2018-10-30 晚上10点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)












