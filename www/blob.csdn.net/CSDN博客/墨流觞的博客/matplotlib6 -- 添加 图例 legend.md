# matplotlib6 -- 添加 图例 legend - 墨流觞的博客 - CSDN博客





2018年11月24日 22:11:20[墨氲](https://me.csdn.net/dss_dssssd)阅读数：52标签：[matplotlib																[legend](https://so.csdn.net/so/search/s.do?q=legend&t=blog)](https://so.csdn.net/so/search/s.do?q=matplotlib&t=blog)
个人分类：[matplotlib](https://blog.csdn.net/dss_dssssd/article/category/8401939)

所属专栏：[matplotlib一直在入门](https://blog.csdn.net/column/details/30379.html)








先看一下比较简单的legend：
- 在plot()函数中使用label关键字参数指明图例的文字内容
- axes.legend()函数说明图例的位置等相关属性，

```python
fig = plt.figure(1)
ax = plt.subplot(111)
ax.plot(x, cosx, 'r',label='y=cos(x)')
ax.plot(x, sinx, 'c',label='y=sin(x)')
ax.legend()
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124221151342.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

legend最常用的参数`loc`,默认: 对于Axes， `‘best’`, 对于Figure,  `'upper right'`
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125105745476.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
```python
fig = plt.figure(1)
ax = plt.subplot(111)
ax.plot(x, cosx, 'r',label='y=cos(x)')
ax.plot(x, sinx, 'c',label='y=sin(x)')
ax.legend(loc=1)
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125105937100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

参考文档：
[Legned guide](https://matplotlib.org/users/legend_guide.html)
[Legend API](https://matplotlib.org/api/legend_api.html#matplotlib.legend.Legend)
[axes.legend()](https://matplotlib.org/api/pyplot_api.html#matplotlib.pyplot.legend)
主要讲解一下Legend guide：

legend主要涉及一下四个术语：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125103745178.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)- legend entry： 图例条目， 一个图例由一个或多个条目组成，而每个条目由键和值（标签组成）
- legend key：图例键， 每个图例标签左边的颜色或图案标记
- legend label：图例标签， 每个图例键右边的文字描述
- legend handle：图例句柄， 用于在图例中生成合适的图例条目的原始对象

直接调用`legend()`,不传入任何参数，将会自动获取图例句柄和与之相对应的图例标签，与下列函数等效：

```python
handles, labels = ax.get_legend_handles_labels()
ax.legend(handles, labels)
```

函数`get_legend_handles_labels()`返回handles/artists的列表的元组`(handles, labels)`,

```python
fig = plt.figure(1)
ax = plt.subplot(111)
ax.plot(x, cosx, 'r',label='y=cos(x)')
ax.plot(x, sinx, 'c',label='y=sin(x)')
ax.legend(loc='upper left')
handles, labels = ax.get_legend_handles_labels()

print(ax.get_legend_handles_labels())
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125105058262.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

但是注意的是并非所有的artists都可以直接添加到图例中，此时需要创建’**proxy**’, [see Creating artists specifically for adding to the legend (aka. Proxy artists)](https://matplotlib.org/users/legend_guide.html#proxy-legend-handles)

想要完全控制添加到图例中的内容，通常传递将句柄handles作为参数传递给`legend()`

```python
fig = plt.figure(1)
ax = plt.subplot(111)
line1, = ax.plot(x, cosx, 'r',label='y=cos(x)')
line2, = ax.plot(x, sinx, 'c',label='y=sin(x)')
# 此时是一个Line2D对象，包含各种属性， color， label等
ax.legend(handles=[line1, line2])



plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125110856346.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

有时， 不好直接设置图例的标签，也可以通过参数传递给`legend()`

```python
fig = plt.figure(1)
ax = plt.subplot(111)
line1, line2 = ax.plot(x, cosx, 'r', x, sinx, 'c')
ax.legend([line1, line2], ['y=cos(x)', 'y=sin(x)'])
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125111231243.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### 创建特殊的artists，添加到图例中。

并不是所有的handles都能自动转化为图例，经常需要创建artist，使用在Axes或Figure上不存在的句柄。

假设我们要创建一个图例，该图例的legend key为黄色的颜色块，**而此`对象`并不存在与Axes上。**

```python
import matplotlib.patches as mpatches
fig = plt.figure(1)
ax = plt.subplot(111)
red_patch = mpatches.Patch(color='yellow', label='The yellow data')

ax.legend(handles=[red_patch])
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112511193028.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

当然还有很多其他的图例，比如创建line with dot

```python
import matplotlib.lines as mlines
fig = plt.figure(1)
ax = plt.subplot(111)
cyan_line = mlines.Line2D([], [], color='c', marker='o',
                          markersize=15, label='Cyan dot')
plt.legend(handles=[cyan_line])


plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125112535874.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### Legend location

可以使用`legend()`函数参数`loc`指定位置。

也可以通过`bbox_to_anchor`和`bbox_transform`来指定准确的位置
- bbox_to_anchoe:  (0,0)左下角， （1,1）右上角
- bbox_transform： 默认是Axes，`plt.gcf().transFigure`使用Figure

比如讲图标放在Figure的右下角

```python
fig = plt.figure(1)
axes0 = plt.subplot(211)
axes0.plot(x, cosx, 'k',linewidth=1, label='y=cos(x)')
axes0.legend(bbox_to_anchor=(1, 1),
           )
axes1 = plt.subplot(212)
axes1.plot(x, sinx, 'r', linewidth=1, label='y=sin(x)')


axes1.legend(bbox_to_anchor=(1, 1),
           bbox_transform=plt.gcf().transFigure)


plt.show()
```

注意两个图例 ‘y=cos(x), y=sin(x)’的位置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125121219767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
更多的例子：

```python
import matplotlib.pyplot as plt


plt.subplot(211)
plt.plot([1,2,3], label="test1")
plt.plot([3,2,1], label="test2")
# Place a legend above this subplot, expanding itself to
# fully use the given bounding box.
plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
           ncol=2, mode="expand", borderaxespad=0.)

plt.subplot(223)
plt.plot([1,2,3], label="test1")
plt.plot([3,2,1], label="test2")
# Place a legend to the right of this smaller subplot.
plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)

plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125121601322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### 在同一个Axes上添加多个legend

有时候将legend entries 分成多个legends更加清晰，而如果多次调用legend，你会发现只会出现最新的legend，所以我们要手动将旧的图例添加到Axes上。

```python
fig = plt.figure(1)
ax = plt.subplot(111)
line1,  = ax.plot(x, cosx, 'r-.',label = 'y=cos(x)' )
line2, =ax.plot(x, sinx, 'c',label = 'y=sin(x)')

# 创建第一个图例：y=cosx
first_legend = plt.legend(handles=[line1], loc='lower right')
# 添加第一个图例
ax.add_artist(first_legend)

# 添加第二个图例
ax.legend(handles=[line2], loc='upper right')

plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125122516425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)








