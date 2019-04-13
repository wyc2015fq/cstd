
# 为什么matplotlib显示opencv图像不正常 - saltriver的专栏 - CSDN博客


2017年10月08日 09:41:13[saltriver](https://me.csdn.net/saltriver)阅读数：4370


在前面的《[数字图像的加载、显示和输出](http://blog.csdn.net/saltriver/article/details/78167393)》中提到，opencv对多个图片输出在同一个窗口并没有直接的支持手段，但有时候我们会有这个需求，这时可以用matplotlib搭配使用。
下面将4张图片放在一个窗口中显示。原始图像如下：
![这里写图片描述](https://img-blog.csdn.net/20171008090256132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171008090256132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
使用matplotlib将4幅图像显示在一个窗口的代码如下：
[
](https://img-blog.csdn.net/20171008090256132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
# coding=utf-8
```
```python
import cv2
import matplotlib
```
```python
.pyplot
```
```python
as plt
scooter_path =
```
```python
"scooter.png"
```
```python
# 带alpha通道的彩色图像
```
```python
lenna_path =
```
```python
"lenna.bmp"
```
```python
# 3通道彩色图像
```
```python
baboon_path =
```
```python
"baboon.bmp"
```
```python
# 3通道彩色图像
```
```python
moon_path =
```
```python
"moon.bmp"
```
```python
# 灰度图像
```
```python
pic = [scooter_path, lenna_path, baboon_path, moon_path]
plt
```
```python
.figure
```
```python
(figsize=(
```
```python
8
```
```python
,
```
```python
6
```
```python
))
for i
```
```python
in
```
```python
range(
```
```python
4
```
```python
):
    img = cv2
```
```python
.imread
```
```python
(pic[i])
    plt
```
```python
.subplot
```
```python
(
```
```python
2
```
```python
,
```
```python
2
```
```python
, i +
```
```python
1
```
```python
)
    plt
```
```python
.imshow
```
```python
(img)
    plt
```
```python
.title
```
```python
(pic[i])
plt
```
```python
.show
```
```python
()
```
[
](https://img-blog.csdn.net/20171008090256132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)显示如下：
[
](https://img-blog.csdn.net/20171008090256132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20171008090531900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看出，图像的颜色显示不正常。
这是因为matplotlib使用的颜色模式是我们现在流行的RGB模式，而opencv使用的是BGR模式，即RGB的倒序模式，与我们通常的RGB是反向的。因此在使用matplotlib显示之前需要做一下图像颜色的转换。
注意下面代码中如下这一行：
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
```python
import cv2
import matplotlib
```
```python
.pyplot
```
```python
as plt
scooter_path =
```
```python
"scooter.png"
```
```python
# 带alpha通道的彩色图像
```
```python
lenna_path =
```
```python
"lenna.bmp"
```
```python
# 3通道彩色图像
```
```python
baboon_path =
```
```python
"baboon.bmp"
```
```python
# 3通道彩色图像
```
```python
moon_path =
```
```python
"moon.bmp"
```
```python
# 灰度图像
```
```python
pic = [scooter_path, lenna_path, baboon_path, moon_path]
plt
```
```python
.figure
```
```python
(figsize=(
```
```python
8
```
```python
,
```
```python
6
```
```python
))
for i
```
```python
in
```
```python
range(
```
```python
4
```
```python
):
    img = cv2
```
```python
.imread
```
```python
(pic[i])
    img = cv2
```
```python
.cvtColor
```
```python
(img, cv2
```
```python
.COLOR
```
```python
_BGR2RGB)
```
```python
# 注意这行。
```
```python
plt
```
```python
.subplot
```
```python
(
```
```python
2
```
```python
,
```
```python
2
```
```python
, i +
```
```python
1
```
```python
)
    plt
```
```python
.imshow
```
```python
(img)
    plt
```
```python
.title
```
```python
(pic[i])
plt
```
```python
.show
```
```python
()
```
再次输出，这次正常了：
![这里写图片描述](https://img-blog.csdn.net/20171008094449530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
那么问题来了，为什么opencv要采用BGR顺序的模式呢？这又是因为历史原因，早期的opencv开发者采用了BGR顺序，所以一直遗留下来了。那么早期的opencv开发者为什么要用BGR呢，因为那时的许多照相机生产厂家和软件开发商就是采用的BGR模式。

