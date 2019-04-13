
# Hough直线检测的理解 - saltriver的专栏 - CSDN博客


2018年06月02日 13:35:51[saltriver](https://me.csdn.net/saltriver)阅读数：2161


我们在前面的《[图像的颜色选择](https://blog.csdn.net/saltriver/article/details/79680859)》、《[图像的感兴趣区域](https://blog.csdn.net/saltriver/article/details/79680973)》中提到了对车道线的检测。
![这里写图片描述](https://img-blog.csdn.net/20180324193622942?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180324193622942?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过对原始行车图像进行颜色选择和感兴趣区域的提取，得到了如下的车道线：
![这里写图片描述](https://img-blog.csdn.net/20180324195408900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180324195408900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们的车道线当然是一左一右两条线。那怎样从这个车道线图像中提取出这2条线呢？
[
](https://img-blog.csdn.net/20180324195408900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)这就要谈到“霍夫变换（Hough Transfrom）”。霍夫变换是1972年提出来的，最开始就是用来在图像中过检测直线，后来扩展能检测圆、曲线等。
[
](https://img-blog.csdn.net/20180324195408900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)我们在初中数学中了解到，一条直线可以用如下的方程来表示：y=kx+b，k是直线的斜率，b是截距。
[
](https://img-blog.csdn.net/20180324195408900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)图像是一个个离散的像素点构成的，如果在图像中有一条直线，那也是一系列的离散点构成的。那么怎样检测这些离散的点构成了直线呢？
[
](https://img-blog.csdn.net/20180324195408900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)我们再看上面的直线方程：y=kx+b,(x,y)就是点。我们转换下变成：b=-kx+y。我们是不是也可以把（k，b）看作另外一个空间中的点？这就是k-b参数空间。
![这里写图片描述](https://img-blog.csdn.net/20180602132729141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180602132729141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们看到，在x-y图像空间中的一个点，变成了k-b参数空间中的一条直线，而x-y图像空间中的2点连成的直线，变成了k-b参数空间中的一个交点。
[
](https://img-blog.csdn.net/20180602132729141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**如果x-y图像空间中有很多点在k-b空间中相交于一点，那么这个交点就是我们要检测的直线。**这就是霍夫变换检测直线的基本原理。
[
](https://img-blog.csdn.net/20180602132729141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)当然，有一个问题需要注意，图像空间中如果一条直线是垂直的，那么斜率k是没有定义的（或者说无穷大）。为了避免这个问题，霍夫变换采用了另一个参数空间：距离-角度参数空间。
[
](https://img-blog.csdn.net/20180602132729141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)我们在中学中学过，平面上的一个点也可以用距离-角度来定义，也就是极坐标：
![这里写图片描述](https://img-blog.csdn.net/20180602132856731?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180602132856731?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
那么在图像中，每一个点都可以用距离和角度来表达：
![这里写图片描述](https://img-blog.csdn.net/20180602132904904?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180602132904904?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是，使用距离-角度后，点（x，y）与距离，角度的关系变成了：

$$
\rho=x cos\theta + y sin\theta
$$
于是，在新的距离-角度参数空间中，图像中的一个点变成了一个正弦曲线，而不是k-b参数空间中的直线了。这些正弦曲线的交点就是图像空间中我们要检测的直线了。
对于最开始的图像，我们先用Canny进行边缘检测，较少图像空间中需要检测的点数量：
```python
lane = cv2
```
```python
.imread
```
```python
(
```
```python
"final_roi.png"
```
```python
)
```
```python
# 高斯模糊，Canny边缘检测需要的
```
```python
lane = cv2
```
```python
.GaussianBlur
```
```python
(lane, (
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
# 进行边缘检测，减少图像空间中需要检测的点数量
```
```python
lane = cv2
```
```python
.Canny
```
```python
(lane,
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
"lane"
```
```python
, lane)
cv2
```
```python
.waitKey
```
```python
()
```
![这里写图片描述](https://img-blog.csdn.net/20180602133341305?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
import numpy
```
```python
as
```
```python
np
rho =
```
```python
1
```
```python
# 距离分辨率
```
```python
theta = np.
```
```python
pi
```
```python
/
```
```python
180
```
```python
# 角度分辨率
```
```python
threshold =
```
```python
10
```
```python
# 霍夫空间中多少个曲线相交才算作正式交点
```
```python
min_line_len =
```
```python
10
```
```python
# 最少多少个像素点才构成一条直线
```
```python
max_line_gap =
```
```python
50
```
```python
# 线段之间的最大间隔像素
```
```python
lines
```
```python
= cv2.HoughLinesP(lane, rho, theta, threshold, maxLineGap=max_line_gap)
line_img = np.zeros_like(lane)
```
```python
for
```
```python
line
```
```python
in
```
```python
lines
```
```python
:
```
```python
for
```
```python
x1, y1, x2, y2
```
```python
in
```
```python
line
```
```python
:
        cv2.
```
```python
line
```
```python
(line_img, (x1, y1), (x2, y2),
```
```python
255
```
```python
,
```
```python
1
```
```python
)
cv2.imshow(
```
```python
"line_img"
```
```python
, line_img)
cv2.waitKey()
```
![这里写图片描述](https://img-blog.csdn.net/20180602133427350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NhbHRyaXZlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

