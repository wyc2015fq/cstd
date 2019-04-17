# Opencv之输出opencv对象属性 - qq_37385726的博客 - CSDN博客





2018年08月23日 09:42:13[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：61
个人分类：[Opencv](https://blog.csdn.net/qq_37385726/article/category/7962192)

所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、opencv对象属性](#%E4%B8%80%E3%80%81opencv%E5%AF%B9%E8%B1%A1%E5%B1%9E%E6%80%A7)

[二、代码](#%E4%BA%8C%E3%80%81%E4%BB%A3%E7%A0%81)



# 一、opencv对象属性
- type() 输出opencv对象所属类型
- dtype()输出opencv对象数据类型
- shape输出opencv对象的形状
- size输出opencv对象的元素个数



# 二、代码

```python
import cv2

img1 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_COLOR)

print('type:',type(img1))
print('dtype:',img1.dtype)
print('shape:',img1.shape)
print('size:',img1.size)
```

type: <class 'numpy.ndarray'>

dtype: uint8

shape: (300, 540, 3)

size: 486000 

> 
**可以看到，通过imread读入的图片对象实际上是ndarray对象，由原来的RGB三通道，转成了BGR三通道**

**故shape和size属性包括dtype属性都和在numpy中的一样**




