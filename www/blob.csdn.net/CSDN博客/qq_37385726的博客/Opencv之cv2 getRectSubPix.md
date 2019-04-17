# Opencv之cv2.getRectSubPix - qq_37385726的博客 - CSDN博客





2018年09月02日 11:43:02[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：274
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、函数原型](#%E4%B8%80%E3%80%81%E5%87%BD%E6%95%B0%E5%8E%9F%E5%9E%8B)

[二、minAreaRect矩形截取实现](#%E4%BA%8C%E3%80%81minAreaRect%E7%9F%A9%E5%BD%A2%E6%88%AA%E5%8F%96%E5%AE%9E%E7%8E%B0)

## 一、函数原型

**cv2.getRectSubPix(img,(height,width),(x,y))**
- img为要截取的原图像
- (height,width)为要截取图像的高和宽，**也就是columns和rows，切记不要弄成行和列了**
- (x,y)为要截取矩形的中心
- 返回值为截取后的 图像

也就是说，利用该函数截取的矩形要求是水平的（竖直的），对于minArearect求出的矩形可能还有旋转角度，得利用warpAffine旋转成水平方向。

## 二、minAreaRect矩形截取实现

```python
rotate = cv2.getRotationMatrix2D((x,y),90+angel,1)
res = cv2.warpAffine(initial_car,rotate,initial_car.shape[:2])
#img,(列，行)，（中心）
res = cv2.getRectSubPix(res,(int(new_height),int(new_width)),(x,y))
```





