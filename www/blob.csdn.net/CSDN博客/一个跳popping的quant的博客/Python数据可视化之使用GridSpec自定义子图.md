# Python数据可视化之使用GridSpec自定义子图 - 一个跳popping的quant的博客 - CSDN博客





2018年03月20日 16:32:46[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：3323
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









使用了很长时间的python，每次画图时都会忘记很多画图的方式，这里整理一下怎么在一张画布上定义多张子图。

引用模块：

```python
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import gridspec
```

生成几个随机点：

```python
random_cp = np.random.random_integers(1, 100, (100, 2))
random_nums = np.random.random_integers(1, 100, 100)
random_cp1 = np.random.random_integers(1, 100, (100, 2))
random_nums1 = np.random.random_integers(1, 100, 100)
```

**1、使用add_subplot**


通过add_subplot就可以实现子图的划分：

先是通过plt.figure()创建画布，之后再通过add_subplot()在画布上创建不同的区域。

其中add_subplot()的参数，类似221就是将画布分为2行2列的也就是2×2的区域，使用第1个区域。


```python
fig1 = plt.figure()

ax1 = fig1.add_subplot(221)
ax2 = fig1.add_subplot(222)

ax3 = fig1.add_subplot(223)
ax4 = fig1.add_subplot(224)

ax1.plot(range(len(random_nums)), random_nums)
ax3.scatter(random_cp[:, 0], random_cp[:, 1])
ax4.plot(range(len(random_nums1)), random_nums1)
ax2.scatter(random_cp1[:, 0], random_cp1[:, 1])

plt.show()
```




**2、使用GridSpec**

先是通过gridspec.GridSpec()创建区域，参数5，5的意思就是每行五个，每列五个，最后就是一个5×5的画布，相比于add_subplot()，使用网格布局的话可以更加灵活的控制占用多少空间。


```python
gs = gridspec.GridSpec(5,5)
fig1 = plt.figure()
ax1 = fig1.add_subplot(gs[0:2, 0:2])
ax2 = fig1.add_subplot(gs[3:5, 0:2])

ax3 = fig1.add_subplot(gs[0:2, 3:5])
ax4 = fig1.add_subplot(gs[3:5, 3:5])

ax1.plot(range(len(random_nums)), random_nums)
ax3.scatter(random_cp[:, 0], random_cp[:, 1])
ax4.plot(range(len(random_nums1)), random_nums1)
ax2.scatter(random_cp1[:, 0], random_cp1[:, 1])

ax1.plot(range(len(random_nums)), random_nums)
ax3.scatter(random_cp[:, 0], random_cp[:, 1])
ax4.plot(range(len(random_nums1)), random_nums1)
ax2.scatter(random_cp1[:, 0], random_cp1[:, 1])

plt.show()
```


![](https://img-blog.csdn.net/20180320163138751)







