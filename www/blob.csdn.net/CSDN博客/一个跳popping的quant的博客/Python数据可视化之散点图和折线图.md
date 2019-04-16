# Python数据可视化之散点图和折线图 - 一个跳popping的quant的博客 - CSDN博客





2017年12月02日 21:39:23[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：9441
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









最近一直用到Python的matplotlib包，一些画简单的散点图和折线图的用法老是记不住，所以从最基础的地方开始，稍微总结了一下。

**一、散点图**

画散点图时主要用到的是scatter函数。

随机产生两组length相同的数据。

```
x = np.random.uniform(10, 20, 10)
y = np.random.random_integers(10, 50, 10)
```

通过plt.figure()相当于生成了一个画布，然后通过add_subplot()函数来划分这个画布，当这个画布中只有一个图像的时候，参数默认1，1，1，相当于1row x 1 col中的第一块区域。所以同理若是2，1，1那么就是2rows x 1 col中的第一块区域了。其中ax1就相当于第一块区域。

```
fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)
```

这里给ax1区域的图像设置了标题，设置了x、y坐标的标签。 

然后通过scatter函数来画图像。 

其中scatter函数参数的一些介绍如下： 
![这里写图片描述](https://img-blog.csdn.net/20171202211747355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

点的形状选择 
![这里写图片描述](https://img-blog.csdn.net/20171202212019645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

颜色的选择： 
![](https://img-blog.csdn.net/20171202212116550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

plt.legend()函数的作用就是来设置标注。
```
ax1.set_title('Scatter Plot')
plt.xlabel('X')
plt.ylabel('Y')

ax1.scatter(x, y, c='r', marker='o')
ax1.scatter(y, x, c='g', marker='o')
plt.legend(['x','y'])
plt.show()
```

效果如下： 
![这里写图片描述](https://img-blog.csdn.net/20171202212357490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
以上部分图来自：[http://blog.csdn.net/u013634684/article/details/49646311](http://blog.csdn.net/u013634684/article/details/49646311)

**二、折线图**

同样是数据的初始化：

```
x = np.random.random_integers(1, 20, 10)
y = range(len(x))
```

这里和画散点图的做法一样，也是划分区域，然后在相应的区域中进行绘图。 

其中plot()函数也有几个常用到的参数color、marker之类的。 

其中color值跟scatter中的一样。 

marker参数值如下： 
![这里写图片描述](https://img-blog.csdn.net/20171202213731478?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```bash
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
ax.plot(y, x)
plt.xlabel('x')
plt.ylabel('y')
plt.legend(['line1','line2'])
plt.show()
```

效果如下： 
![这里写图片描述](https://img-blog.csdn.net/20171202213801618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






