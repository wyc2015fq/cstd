# Opencv之图像放缩 - qq_37385726的博客 - CSDN博客





2018年08月23日 10:31:37[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：65
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、指定放缩比例](#%E4%B8%80%E3%80%81%E6%8C%87%E5%AE%9A%E6%94%BE%E7%BC%A9%E6%AF%94%E4%BE%8B)

[二、指定放缩到的长宽](#%E4%BA%8C%E3%80%81%E6%8C%87%E5%AE%9A%E6%94%BE%E7%BC%A9%E5%88%B0%E7%9A%84%E9%95%BF%E5%AE%BD)

[三、interpolation插值](#%E4%B8%89%E3%80%81interpolation%E6%8F%92%E5%80%BC)

[四、代码](#%E5%9B%9B%E3%80%81%E4%BB%A3%E7%A0%81)

# 一、指定放缩比例

fx和fy是放缩的比例，前面的None占据的是明确规定的最后的大小，因为我们定义了比例，所以就不用规定最后的大小。

```python
res = cv2.resize(img1,None,fx=2,fy=1,interpolation=cv2.INTER_CUBIC)
```

# 二、指定放缩到的长宽

规定的最后的大小需要是int型的。

**（列，行）**

```python
res2 = cv2.resize(img1,(int(img1.shape[0]/2),int(img1.shape[1]/2)),interpolation=cv2.INTER_AREA)
```



# 三、interpolation插值

缩放有几种不同的插值（interpolation）方法
- 在缩小时推荐cv2.INTER_ARER
- 扩大是推荐cv2.INTER_CUBIC和cv2.INTER_LINEAR
- 默认都是cv2.INTER_LINEAR



# 四、代码

```python
import cv2


img1 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_COLOR)


res1 = cv2.resize(img1,None,fx=2,fy=1,interpolation=cv2.INTER_CUBIC)

res2 = cv2.resize(img1,(int(img1.shape[1]/2),int(img1.shape[0/2)),interpolation=cv2.INTER_AREA)

cv2.imshow('initial',img1)
cv2.imshow('large',res1)
cv2.imshow('small',res2)
cv2.waitKey(0)
```



![](https://img-blog.csdn.net/20180823102711757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



