# matplotlib5---设置轴范围limits，标度tick， 刻度符号 tick label， 轴居中 - 墨流觞的博客 - CSDN博客





2018年11月24日 17:52:19[墨氲](https://me.csdn.net/dss_dssssd)阅读数：277
所属专栏：[matplotlib一直在入门](https://blog.csdn.net/column/details/30379.html)









本文只要讲述绘图中轴的相关设置
- 设置轴的范围 setting limits
- 设置轴的刻度 setting ticks
- 设置轴刻度的标记 setting tick labels
- 将x，y轴居中
- 设置x, y轴的label

```python
x = np.linspace(-np.pi, np.pi, 128,endpoint=True)
cosx,sinx,x_3 = np.cos(x), np.sin(x), x / 3
```

### 1. 设置轴的范围

[matplotlib.pyplot.xlim(*args, **kwargs)](https://matplotlib.org/api/pyplot_api.html#matplotlib.pyplot.xlim)
[matplotlib.pyplot.ylim(*args, **kwargs)](https://matplotlib.org/api/pyplot_api.html#matplotlib.pyplot.ylim)

参数传递：
- `xlim((xmin, xmax))`
- `xlim(xmin, xmax)`
- `xlim(xmin=xmin, xmax=xmax)`

**个人推荐使用axes.set_xlim()**, 显然是对于每一个axes设置轴属性，用法与上述相同

在使用关键字参数时，可以只指定一个值，而保持另一个值使用默认值

```python
fig = plt.figure(1)
axes0 = plt.subplot(111)
line1, line2 = axes0.plot(x, cosx, 'w',x, x_3, 'c')

line1.set_linewidth(2.5)
line2.set_linewidth(1)
# plt.xlim(x.min() *2, x.max()*2)
axes0.set_xlim(x.min() *2, x.max()*2)
axes0.set_ylim(cosx.min() * 1.5, cosx.max() * 1.5)
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124170916894.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### 2. 设置刻度 setting ticks

[matplotlib.pyplot.xticks(*args, **kwargs)](https://matplotlib.org/api/pyplot_api.html#matplotlib.pyplot.xticks)

[matplotlib.pyplot.yticks(*args, **kwargs)](https://matplotlib.org/api/pyplot_api.html#matplotlib.pyplot.yticks)

[Tick containers](https://matplotlib.org/users/artists.html#axis-container)

[Tick containers](https://matplotlib.org/users/artists.html#axis-containers)

```python
fig = plt.figure(1)
axes0 = plt.subplot(111)
line1, line2 = axes0.plot(x, cosx, 'w',x, x_3, 'c')

line1.set_linewidth(2.5)
line2.set_linewidth(1)
# plt.xlim(x.min() *2, x.max()*2)
axes0.set_xlim(x.min() *1.2, x.max()*1.2)
axes0.set_ylim(cosx.min() * 1.2, cosx.max() * 1.2)

axes0.set_xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi])
axes0.set_yticks([-1, 0, 1])
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124171844305.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### 3. 设置tick labels
- 可以使用`plt.xticks(tick, label)`, 也可以使用`axes.set_xticklabels(label)`,

**推荐使用后者**
**label的长度与tick相同，并且是可索引对象**

[matplotlib.axes.Axes.set_xticklabels](https://matplotlib.org/api/_as_gen/matplotlib.axes.Axes.set_xticklabels.html#matplotlib.axes.Axes.set_xticklabels)

2. 可以通过使用`r'$math$`的形式使用markdown math公式
[text](https://matplotlib.org/users/index_text.html)
```python
fig = plt.figure(1)
axes0 = plt.subplot(111)
line1, line2 = axes0.plot(x, cosx, 'w',x, x_3, 'c')

line1.set_linewidth(2.5)
line2.set_linewidth(1)
# plt.xlim(x.min() *2, x.max()*2)
axes0.set_xlim(x.min() *1.2, x.max()*1.2)
axes0.set_ylim(cosx.min() * 1.2, cosx.max() * 1.2)

axes0.set_xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi])
axes0.set_xticklabels([r'$-\pi$', r'$-\frac{\pi}{2}$', 0,  r'$+\frac{\pi}{2}$', r'$+\pi$'])
axes0.set_yticks([-1, 0, 1])
axes0.set_yticklabels([r'-1', r'0', r'+1'])
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124173229391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### 4. 设置x,yz轴居中  spines

[spine](https://matplotlib.org/api/spines_api.html#matplotlib.spines)

axes有4个边界，上，下，左，右；将上和有的边界颜色设为无色，然后将x轴设为下边界，将x轴数据标记‘0’，同理对左边界做相同处理。
[set_position(position)](https://matplotlib.org/api/spines_api.html#matplotlib.spines.Spine.set_position)

```python
fig = plt.figure(1)
axes0 = plt.subplot(111)
line1, line2 = axes0.plot(x, cosx, 'w',x, x_3, 'c')

line1.set_linewidth(2.5)
line2.set_linewidth(1)
# plt.xlim(x.min() *2, x.max()*2)
axes0.set_xlim(x.min() *1.2, x.max()*1.2)
axes0.set_ylim(cosx.min() * 1.2, cosx.max() * 1.2)

axes0.set_xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi])
axes0.set_xticklabels([r'$-\pi$', r'$-\frac{\pi}{2}$', 0,  r'$+\frac{\pi}{2}$', r'$+\pi$'])
axes0.set_yticks([-1, 0, 1])
axes0.set_yticklabels([r'-1', r'0', r'+1'])

# 居中xy轴
axes0.spines['right'].set_color('none')
axes0.spines['top'].set_color('none')
axes0.xaxis.set_ticks_position('bottom')
axes0.spines['bottom'].set_position(('data',0))
axes0.yaxis.set_ticks_position('left')
axes0.spines['left'].set_position(('data',0))

plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124174445563.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

一下代码将倒数第二行代码`axes0.spines['left'].set_position(('data',0))`改为`axes0.spines['left'].set_position(('data',np.pi))`

下列为绘制的图形：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124174659733.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### 5. 设置x,y轴的label

[axes.set_xlabel()](https://matplotlib.org/api/_as_gen/matplotlib.axes.Axes.set_xlabel.html)

axes.set_ylabel()

```python
fig = plt.figure(1)
axes0 = plt.subplot(111)
line1, line2 = axes0.plot(x, cosx, 'w',x, x_3, 'c')

line1.set_linewidth(2.5)
line2.set_linewidth(1)
# plt.xlim(x.min() *2, x.max()*2)
axes0.set_xlim(x.min() *1.2, x.max()*1.2)
axes0.set_ylim(cosx.min() * 1.2, cosx.max() * 1.2)

axes0.set_xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi])
axes0.set_xticklabels([r'$-\pi$', r'$-\frac{\pi}{2}$', 0,  r'$+\frac{\pi}{2}$', r'$+\pi$'])
axes0.set_yticks([-1, 0, 1])
axes0.set_yticklabels([r'-1', r'0', r'+1'])


axes0.set_xlabel('x-values')
axes0.set_ylabel('y-values')
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124175113737.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)




