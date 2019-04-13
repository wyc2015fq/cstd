
# [Python图像处理] 五.图像融合、加法运算及图像类型转换 - 杨秀璋的专栏 - CSDN博客

2018年09月03日 14:44:43[Eastmount](https://me.csdn.net/Eastmount)阅读数：6565所属专栏：[Python图像处理及图像识别](https://blog.csdn.net/column/details/26274.html)[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)



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
本篇文章主要讲解Python调用OpenCV实现图像融合及加法运算，包括三部分知识：图像融合、图像加法运算、图像类型转换。全文均是基础知识，希望对您有所帮助。
**1.图像加法运算**
**2.图像融合**
**3.图像类型转换**
PS：文章参考自己以前系列图像处理文章及OpenCV库函数，同时部分参考网易云视频，推荐大家去学习。
PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)[
](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C\#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。
---

# 一.图像加法运算
**1.Numpy库加法**
其运算方法是：目标图像 = 图像1 + 图像2，运算结果进行取模运算。
当像素值<=255时，结果为“图像1+图像2”，例如：120+48=168
当像素值>255时，结果为对255取模的结果，例如：(255+64)%255=64
**2.OpenCV加法运算**
另一种方法是直接调用OpenCV库实现图像加法运算，方法如下：
目标图像 = cv2.add(图像1, 图像2)
此时结果是饱和运算，即：
当像素值<=255时，结果为“图像1+图像2”，例如：120+48=168
当像素值>255时，结果为255，例如：(255+64) = 255
两种方法对应的代码如下所示：
```python
#encoding:utf-8
```
```python
import
```
```python
cv2
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
import
```
```python
matplotlib
```
```python
.
```
```python
pyplot
```
```python
as
```
```python
plt
```
```python
#读取图片
```
```python
img
```
```python
=
```
```python
cv2
```
```python
.
```
```python
imread
```
```python
(
```
```python
'picture.bmp'
```
```python
)
```
```python
test
```
```python
=
```
```python
img
```
```python
#方法一：Numpy加法运算
```
```python
result1
```
```python
=
```
```python
img
```
```python
+
```
```python
test
```
```python
#方法二：OpenCV加法运算
```
```python
result2
```
```python
=
```
```python
cv2
```
```python
.
```
```python
add
```
```python
(
```
```python
img
```
```python
,
```
```python
test
```
```python
)
```
```python
#显示图像
```
```python
cv2
```
```python
.
```
```python
imshow
```
```python
(
```
```python
"original"
```
```python
,
```
```python
img
```
```python
)
```
```python
cv2
```
```python
.
```
```python
imshow
```
```python
(
```
```python
"result1"
```
```python
,
```
```python
result1
```
```python
)
```
```python
cv2
```
```python
.
```
```python
imshow
```
```python
(
```
```python
"result2"
```
```python
,
```
```python
result2
```
```python
)
```
```python
#等待显示
```
```python
cv2
```
```python
.
```
```python
waitKey
```
```python
(
```
```python
0
```
```python
)
```
```python
cv2
```
```python
.
```
```python
destroyAllWindows
```
```python
(
```
```python
)
```
输出结果如下图所示，其中result1为第一种方法，result2为第二种方法，白色点255更多。
![](https://img-blog.csdn.net/20180903135909945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180903135909945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)注意：参与运算的图像大小和类型必须一致。下面是对彩色图像进行加法运算的结果。
![](https://img-blog.csdn.net/20180903140223309?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180903140223309?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

# 二.图像融合
图像融合通常是指将2张或2张以上的图像信息融合到1张图像上，融合的图像含有更多的信息，能够更方便人们观察或计算机处理。如下图所示，将两张不清晰的图像融合得到更清晰的图。
![](https://img-blog.csdn.net/2018090314173253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/2018090314173253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)图像融合是在图像加法的基础上增加了系数和亮度调节量。
图像加法：目标图像 = 图像1 + 图像2
图像融合：目标图像 = 图像1 * 系数1 + 图像2 * 系数2 + 亮度调节量
主要调用的函数是addWeighted，方法如下：
dst = cv2.addWeighter(scr1, alpha, src2, beta, gamma)
dst = src1 * alpha + src2 * beta + gamma
其中参数gamma不能省略。
代码如下：
\#encoding:utf-8importcv2importnumpyasnpimportmatplotlib.pyplotasplt\#读取图片src1=cv2.imread('test22.jpg')src2=cv2.imread('picture.bmp')\#图像融合result=cv2.addWeighted(src1,1,src2,1,0)\#显示图像cv2.imshow("src1",src1)cv2.imshow("src2",src2)cv2.imshow("result",result)\#等待显示cv2.waitKey(0)cv2.destroyAllWindows()需要注意的是，两张融合的图像像素大小需要一致，如下图所示，将两张RGB且像素410*410的图像融合。
![](https://img-blog.csdn.net/20180903142846357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180903142846357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)设置不同的比例的融合如下所示：
result = cv2.addWeighted(src1, 0.6, src2, 0.8, 10)
![](https://img-blog.csdn.net/20180903143331165?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180903143331165?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

三.图像类型转换
图像类型转换是指将一种类型转换为另一种类型，比如彩色图像转换为灰度图像、BGR图像转换为RGB图像。OPenCV提供了200多种不同类型之间的转换，其中最常用的包括3类，如下：
cv2.COLOR_BGR2GRAY
cv2.COLOR_BGR2RGB
cv2.COLOR_GRAY2BGR
代码如下所示：
\#encoding:utf-8importcv2importnumpyasnpimportmatplotlib.pyplotasplt\#读取图片src=cv2.imread('01.bmp')\#图像类型转换result=cv2.cvtColor(src,cv2.COLOR_BGR2GRAY)\#显示图像cv2.imshow("src",src)cv2.imshow("result",result)\#等待显示cv2.waitKey(0)cv2.destroyAllWindows()输出结果如下图所示：
![](https://img-blog.csdn.net/20180903143942834?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180903143942834?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)如果使用通道转化，则结果如下图所示：
result = cv2.cvtColor(src, cv2.COLOR_BGR2RGB)
![](https://img-blog.csdn.net/2018090314411279?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/2018090314411279?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)图像处理通常需要将彩色图像转换为灰度图像再进行后续的操作，更多知识后续将继续分享，希望对着喜欢，尤其是做图像识别、图像处理的同学。
希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。九月份准备出去休婚假了，好好和她享受最幸福的时光，不被工作所烦扰，但每当自己写完一篇文章或解答一个问题，这种分享知识的快感，真的让我着迷，这就是知识的魅力，老师的快乐吧！
（By：Eastmount 2018-09-03 下午14点[https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)

