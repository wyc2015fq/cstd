
# Canny边缘检测 - saltriver的专栏 - CSDN博客


2018年06月02日 10:08:37[saltriver](https://me.csdn.net/saltriver)阅读数：4328


Canny边缘检测是一种非常流行的边缘检测算法，是John Canny在1986年提出的。它是一个多阶段的算法，即由多个步骤构成。
**1.图像降噪**
**2.计算图像梯度**
**3.非极大值抑制**
**4.阈值筛选**
我们就事后诸葛亮，分析下这个步骤的缘由。
**首先**，图像降噪。我们知道梯度算子可以用于增强图像，本质上是通过增强边缘轮廓来实现的，也就是说是可以检测到边缘的。但是，它们受噪声的影响都很大。那么，我们第一步就是想到要先去除噪声，因为噪声就是灰度变化很大的地方，所以容易被识别为伪边缘。
**第二步**，计算图像梯度，得到可能边缘。我们在前面的关于《图像梯度》文章中有所介绍，计算图像梯度能够得到图像的边缘，因为梯度是灰度变化明显的地方，而边缘也是灰度变化明显的地方。当然这一步只能得到可能的边缘。因为灰度变化的地方可能是边缘，也可能不是边缘。这一步就有了所有可能是边缘的集合。
**第三步**，非极大值抑制。通常灰度变化的地方都比较集中，将局部范围内的梯度方向上，灰度变化最大的保留下来，其它的不保留，这样可以剔除掉一大部分的点。将有多个像素宽的边缘变成一个单像素宽的边缘。即“胖边缘”变成“瘦边缘”。
**第四步**，双阈值筛选。通过非极大值抑制后，仍然有很多的可能边缘点，进一步的设置一个双阈值，即低阈值（low），高阈值（high）。灰度变化大于high的，设置为强边缘像素，低于low的，剔除。在low和high之间的设置为弱边缘。进一步判断，如果其领域内有强边缘像素，保留，如果没有，剔除。
这样做的目的是只保留强边缘轮廓的话，有些边缘可能不闭合，需要从满足low和high之间的点进行补充，使得边缘尽可能的闭合。
我们看看效果：
![这里写图片描述](https://img-blog.csdn.net/20180602094955984?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
# 读入图像
```
```python
lenna = cv2
```
```python
.imread
```
```python
(
```
```python
"images\\lenna.png"
```
```python
,
```
```python
0
```
```python
)
```
```python
# 图像降噪
```
```python
lenna = cv2
```
```python
.GaussianBlur
```
```python
(lenna, (
```
```python
5
```
```python
,
```
```python
5
```
```python
),
```
```python
0
```
```python
)
```
```python
# Canny边缘检测，50为低阈值low，150为高阈值high
```
```python
canny = cv2
```
```python
.Canny
```
```python
(lenna,
```
```python
50
```
```python
,
```
```python
150
```
```python
)
cv2
```
```python
.imshow
```
```python
(
```
```python
"canny"
```
```python
, canny)
cv2
```
```python
.waitKey
```
```python
()
```
在OpenCV中，Canny函数本身应该没有将图像降噪包含在内。因此，实施Canny边缘检测时，需要在Canny函数外面执行图像降噪的过程。
![这里写图片描述](https://img-blog.csdn.net/20180602100302203?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
调整low和high双阈值，能够得到不同的边缘效果。
```python
canny
```
```python
=
```
```python
cv2.Canny(lenna, 100, 200)
```
![这里写图片描述](https://img-blog.csdn.net/20180602100411651?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再看看有噪声的情况：
![这里写图片描述](https://img-blog.csdn.net/2018060210045089?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
# 读入图像
```
```python
lenna = cv2
```
```python
.imread
```
```python
(
```
```python
"images\\lenna_gauss.png"
```
```python
,
```
```python
0
```
```python
)
```
```python
# 图像降噪
```
```python
lenna = cv2
```
```python
.GaussianBlur
```
```python
(lenna, (
```
```python
5
```
```python
,
```
```python
5
```
```python
),
```
```python
0
```
```python
)
```
```python
# Canny边缘检测
```
```python
canny = cv2
```
```python
.Canny
```
```python
(lenna,
```
```python
50
```
```python
,
```
```python
150
```
```python
)
cv2
```
```python
.imshow
```
```python
(
```
```python
"canny"
```
```python
, canny)
cv2
```
```python
.waitKey
```
```python
()
```
![这里写图片描述](https://img-blog.csdn.net/20180602100533469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
效果很差啊，我们调整下高斯模糊的核大小，效果立即好了很多很多。
```python
lenna
```
```python
=
```
```python
cv2.GaussianBlur(lenna, (9, 9), 0)
```
![这里写图片描述](https://img-blog.csdn.net/20180602100557608?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所以，Canny在有噪声的情况下表现好不好，取决于前面的降噪过程，这也是为什么OpenCV将图像降噪放在Canny函数外面的原因吧，需要你自己精心调整它。

