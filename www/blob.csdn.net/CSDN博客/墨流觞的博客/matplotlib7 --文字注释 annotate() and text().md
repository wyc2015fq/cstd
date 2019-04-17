# matplotlib7 --文字注释  annotate() and text() - 墨流觞的博客 - CSDN博客





2018年11月26日 16:11:14[墨氲](https://me.csdn.net/dss_dssssd)阅读数：265
所属专栏：[matplotlib一直在入门](https://blog.csdn.net/column/details/30379.html)









图的文字注释：
- annotate()
- text()

## 1.  [Axes.annotate(s, xy, *args, **kwargs)](https://matplotlib.org/api/_as_gen/matplotlib.axes.Axes.annotate.html#matplotlib.axes.Axes.annotate)

用text `s`注释point `xy`

参数：
- s : str   注释文本
- xy: (float, float)  要注释的点(x, y)
- xytext:  (float, float) optional   s显示的位置。 默认xy
- xycoords：  xy所采用的坐标系， 默认是’data’
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181126144050286.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)- textcoords: xytext所采用的位置确定方法
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181126144252138.png)
默认是xycoords,既采用和xycoords一样的坐标系统。
- 
arrowprops： 在xy和xytext之间指向箭头的属性 参数类型为dict
- 
如果 字典中没有’arrowstyle’ 键，则使用一下键值
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181126161319423.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
[matplotlib.patches.FancyArrowPatch](https://matplotlib.org/api/_as_gen/matplotlib.patches.FancyArrowPatch.html#matplotlib.patches.FancyArrowPatch)
- 
如果包含’arrowstyle’, 则使用一下键值

允许的arrowstyle包括：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181126145246310.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
有效的键包括：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181126145314878.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
[matplotlib.patches.PathPatch](https://matplotlib.org/api/_as_gen/matplotlib.patches.PathPatch.html#matplotlib.patches.PathPatch)
- 
annotation_clip

当xy在Axes区域外时，是否还要注释
- True： 只有当xy在Axes内是才注释
- False： 注释总是会被添加
- None：只有xy在Axes内并且xycoords是’data’才注释

**kwargs

其余的参数都传给[Text](https://matplotlib.org/api/text_api.html#matplotlib.text.Text)
在之后的文章中会进一步介绍箭头的属性设置等更深入的话题
- 先看一个简单的例子：

```python
fig = plt.figure()
ax = fig.add_subplot(111)

t = np.arange(0.0, 5.0, 0.01)
s = np.cos(2*np.pi*t)
line, = ax.plot(t, s, lw=2)


ann = ax.annotate("local max",
                  xy=(2, 1), xycoords='data',
                  xytext=(3, 1.5), textcoords='data',
                  
                  # 设置字体相关属性
                  size=20, va="center", ha="center",
                  color='r',
                  bbox=dict(boxstyle="round4", fc="w"),

                  arrowprops=dict(arrowstyle="-|>",
                                  connectionstyle="arc3,rad=-0.2",
                                  fc="w"), 
                  )

ax.set_ylim(-2,2)
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181126155014725.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
- 一个相对完整的作图例子

```python
import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(-np.pi, np.pi, 128,endpoint=True)
cosx,sinx,x_3 = np.cos(x), np.sin(x), x / 3

#%%
fig = plt.figure(1)
axes0 = plt.subplot(111)
line1, line2 = axes0.plot(x, cosx, 'r',x, sinx, 'c')

line1.set_linewidth(2.5)
line2.set_linewidth(1)
# plt.xlim(x.min() *2, x.max()*2)
axes0.set_xlim(x.min() *1.2, x.max()*1.2)
axes0.set_ylim(cosx.min() * 1.2, cosx.max() * 1.2)

axes0.set_xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi])
axes0.set_xticklabels([r'$-\pi$', r'$-\frac{\pi}{2}$', 0,  r'$+\frac{\pi}{2}$', r'$+\pi$'])
axes0.set_yticks([-1, 0, 1])
axes0.set_yticklabels([r'-1', r'0', r'+1'])

# add legend
axes0.legend([line1, line2], ['y=cos(x)', 'y=sin(x)'])

# 轴居中
axes0.spines['right'].set_color('none')
axes0.spines['top'].set_color('none')
axes0.xaxis.set_ticks_position('bottom')
axes0.spines['bottom'].set_position(('data',0))
axes0.yaxis.set_ticks_position('left')
axes0.spines['left'].set_position(('data',0))
# 添加注释
t = 2 * np.pi / 3

# 通过将[t, 0], [t, np.sin(t)]连接起来使得图更好看一些。
axes0.plot([t, t], [0, np.sin(t)], color='c', linewidth=1.5, linestyle="--")
axes0.scatter([t],[np.sin(t)] ,s=50, c='c')
axes0.annotate(r'$\sin(\frac{2\pi}{3})=\frac{\sqrt{3}}{2}$',
              xy=(t, np.sin(t)), xycoords='data',
              xytext=(+10, +30), textcoords='offset points',
              fontsize=16, color= 'c',
              arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))

# 同样对cosx做处理
axes0.plot([t, t], [0, np.cos(t)], color='r', linewidth=1.5, linestyle="--")
axes0.scatter([t],[np.cos(t)] ,s=50, c='r')
axes0.annotate(r'$\cos(\frac{2\pi}{3})=-\frac{1}{2}$',
              xy=(t, np.cos(t)), xycoords='data',
              xytext=(-90, -50), textcoords='offset points',
              fontsize=16, color= 'r',
              arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181126153154468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
- 看一个极坐标系的作图

```python
fig = plt.figure()
ax = fig.add_subplot(111, polar=True)

r = np.arange(0, 1, 0.001)

theta = 2 *2 * np.pi * r

# 前两个是特例
# line, = ax.plot([4*np.pi]*1000, r, color='#ee8d18', lw=3)
# line, = ax.plot(theta, [1]*1000, color='#ee8d18', lw=3)

line, = ax.plot(theta, r, color='#ee8d18', lw=3)

# 在第800个点出注释
ind = 800
thisr, thistheta = r[ind], theta[ind]
ax.plot([thistheta], [thisr], 'o')

ax.annotate('a polar annotation',
            xy=(thistheta, thisr),  # theta, radius
            xytext=(0.1, 0.1),    # fraction, fraction
            textcoords='figure fraction',  # 从左下角开始，注释文字起始位置所占Figure总长度的比例(0~1)
            horizontalalignment='left',
            verticalalignment='bottom',
            arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2")
            )
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181126161012661.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)













