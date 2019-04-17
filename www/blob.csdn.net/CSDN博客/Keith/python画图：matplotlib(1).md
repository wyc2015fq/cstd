# python画图：matplotlib(1) - Keith - CSDN博客





2017年01月10日 12:55:10[ke1th](https://me.csdn.net/u012436149)阅读数：763标签：[python																[面向对象](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/u012436149/article/category/6462204)








# python matplotlib

`matplotlib`是`python`中用来绘图的一个库，提供非常强大的绘图功能。

## 安装

```python
pip install matplotlib pillow
```

## 面向对象风格

首先要理解两个对象，一个是`Figure`，一个是`AxesSubplot`,`Figure`是一个窗口，他是`AxesSubplot`的容器，真正的绘图要在`AxesSubplot`上进行。

```python
import matplotlib.pyplot as plt
#如何看待plt呢，我觉着plt是一个包含fig的容器

fig = plt.figure() #创建Figure对象
axes1 = fig.add_subplot(2,2,1) #在fig中创建axes，用来画图
#参数的意思是，在fig中创建了2行2列的axes，将第一个axes返回
axes2 = fig.add_subplot(2,2,2) #将第二个返回

#使用axes绘图
axe1.hist(randn(100), bins=20, color='k', alpha=0.3)
axe2.hist(randn(100), bins=20, color='k', alpha=0.3)

#显示,最后一定要加个plt.show()，不然不会显示
plt.show()
```

**一个更加简单创建fig和axes的方法**

```python
#创建一个2*3个axes的fig图像，并返回。axes是一个2*3的对象数组
fig, axes = plt.subplots(2,3)
...
...
plt.show()
```

## 面向过程风格

一个`plt`吃遍天

```python
import matplotlib.pyplot as plt

#可以直接使用plt绘图.
plt.hist(randn(100), bins=20, color='k', alpha=0.3)

#多个axes
fig = plt.figure()
axes1 = fig.add_subplot(2,2,1)
axes2 = fig.add_subplot(2,2,1)
plt.sca(axes1)#设定当前的axes
plt.hist(...) #在当前axes上绘图
plt.sca(axes2)#
plt.hist(...)# 在axes2上绘图

plt.show()
```

可以看出，面向过程就是通过`plt`设置当前的`axes`，然后，用`plt`在当前`axes`上画图

## axes

```python
axes对象都有拥有几个函数:
axes.xlabel("...") #x轴 label
axes.ylabel("...") #y轴 label
axes.title("...")  #axes的title
axes.xlim(start, end)
axes.ylim(start, end)
```




