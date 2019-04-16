# 深度之眼课程打卡-python入门06 - 去向前方的博客 - CSDN博客





2019年03月14日 11:36:55[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：102








# 目录




### 文章目录
- [目录](#_0)
- [前言](#_2)
- [简单演示](#_6)
- [显示多个图像](#_31)
- [指定轴的名称](#_59)
- [同时绘制多条曲线](#_79)
- [多条曲线之曲线说明](#_122)
- [多个figure，并加上特殊点注释](#figure_162)
- [tick能见度设置](#tick_211)
- [散点图](#_241)
- [条形图](#_258)
- [contour等高线图](#contour_286)
- [image图片显示](#image_315)
- [3D数据图](#3D_341)
- [Subplot多合一显示](#Subplot_370)
- [分格显示](#_390)
- [gridspec](#gridspec_425)
- [easy to define structure分格显示](#easy_to_define_structure_455)
- [图中图](#_474)
- [主次坐标轴](#_507)
- [Animation动画](#Animation_527)




# 前言

为了增加实战经验，选择了开通深度之眼vip，先试试水，效果好的话，推荐给大家。

入门第二三课的内容以及在其他bolg里，所以就不打卡了。

本节课主要是练习matplotlib的相应语法，熟悉相关操作。
# 简单演示

```
import matplotlib.pyplot as plt
import numpy as np
#从[-1,1]中等距去50个数作为x的取值
x = np.linspace(-1, 1, 50)
print(x)
y = 2*x + 1
#第一个是横坐标的值，第二个是纵坐标的值
plt.plot(x, y)
#必要方法，用于将设置好的figure对象显示
plt.show()
```

里写图片描述

```
import matplotlib.pyplot as plt
import numpy as np
x = np.linspace(-1, 1, 50)
y = 2**x + 1
#第一个是横坐标的值，第二个是纵坐标的值
plt.plot(x, y)  
plt.show()
```

这里写图片描述

# 显示多个图像

```
import matplotlib.pyplot as plt
import numpy as np
# 多个figure
x = np.linspace(-1, 1, 50)
y1 = 2*x + 1
y2 = 2**x + 1
# 使用figure()函数重新申请一个figure对象
# 注意，每次调用figure的时候都会重新申请一个figure对象
plt.figure()
# 第一个是横坐标的值，第二个是纵坐标的值
plt.plot(x, y1)
# 第一个参数表示的是编号，第二个表示的是图表的长宽
plt.figure(num = 3, figsize=(8, 5))
# 当我们需要在画板中绘制两条线的时候，可以使用下面的方法：
plt.plot(x, y2)
plt.plot(x, y1, 
         color='red',   # 线颜色
         linewidth=1.0,  # 线宽 
         linestyle='--'  # 线样式
        )
plt.show()
```

这里会显示两个图像：

这里写图片描述

这里写图片描述
# 指定轴的名称

```
import matplotlib.pyplot as plt
import numpy as np
# 从[-1,1]中等距去50个数作为x的取值
x = np.linspace(-1, 1, 50)
y1 = 2*x + 1
y2 = 2**x + 1
# 请求一个新的figure对象
plt.figure()
# 第一个是横坐标的值，第二个是纵坐标的值
plt.plot(x, y1)  
# 设置轴线的lable（标签）
plt.xlabel("I am x")
plt.ylabel("I am y")
plt.show()
```

这里写图片描述

# 同时绘制多条曲线

```
import matplotlib.pyplot as plt
import numpy as np
# 从[-1,1]中等距去50个数作为x的取值
x = np.linspace(-1, 1, 50)
y1 = 2*x + 1
y2 = 2**x + 1
# num表示的是编号，figsize表示的是图表的长宽
plt.figure(num = 3, figsize=(8, 5))  
plt.plot(x, y2)
# 设置线条的样式
plt.plot(x, y1, 
         color='red',  # 线条的颜色
         linewidth=1.0,  # 线条的粗细
         linestyle='--'  # 线条的样式
        )
# 设置取值参数范围
plt.xlim((-1, 2))  # x参数范围
plt.ylim((1, 3))  # y参数范围
# 设置点的位置
new_ticks = np.linspace(-1, 2, 5)
plt.xticks(new_ticks)
# 为点的位置设置对应的文字。
# 第一个参数是点的位置，第二个参数是点的文字提示。
plt.yticks([-2, -1.8, -1, 1.22, 3],
          [r'$really\ bad$', r'$bad$', r'$normal$', r'$good$', r'$readly\ good$'])
# gca = 'get current axis'
ax = plt.gca()
# 将右边和上边的边框（脊）的颜色去掉
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
# 绑定x轴和y轴
ax.xaxis.set_ticks_position('bottom')
ax.yaxis.set_ticks_position('left')
# 定义x轴和y轴的位置
ax.spines['bottom'].set_position(('data', 0))
ax.spines['left'].set_position(('data', 0))
plt.show()
```

这里写图片描述

# 多条曲线之曲线说明

```
import matplotlib.pyplot as plt
import numpy as np
# 从[-1,1]中等距去50个数作为x的取值
x = np.linspace(-1, 1, 50)
y1 = 2*x + 1
y2 = 2**x + 1
# 第一个参数表示的是编号，第二个表示的是图表的长宽
plt.figure(num = 3, figsize=(8, 5))  
plt.plot(x, y2)
plt.plot(x, y1, color='red', linewidth=1.0, linestyle='--')
# 设置取值参数
plt.xlim((-1, 2))
plt.ylim((1, 3))
# 设置lable
plt.xlabel("I am x")
plt.ylabel("I am y")
# 设置点的位置
new_ticks = np.linspace(-1, 2, 5)
plt.xticks(new_ticks)
plt.yticks([-2, -1.8, -1, 1.22,3],
          [r'$really\ bad$', r'$bad$', r'$normal$', r'$good$', r'$readly\ good$'])
l1, = plt.plot(x, y2, 
               label='aaa'
              )
l2, = plt.plot(x, y1, 
               color='red',  # 线条颜色
               linewidth = 1.0,  # 线条宽度
               linestyle='-.',  # 线条样式
               label='bbb'  #标签
              )
# 使用ｌｅｇｅｎｄ绘制多条曲线
plt.legend(handles=[l1, l2], 
           labels = ['aaa', 'bbb'], 
           loc = 'best'
          )
plt.show()
```

这里写图片描述

# 多个figure，并加上特殊点注释

```
import matplotlib.pyplot as plt
import numpy as np
# 从[-1,1]中等距去50个数作为x的取值
x = np.linspace(-1, 1, 50)
y1 = 2*x + 1
y2 = 2**x + 1
plt.figure(figsize=(12, 8))  # 第一个参数表示的是编号，第二个表示的是图表的长宽
plt.plot(x, y2)
plt.plot(x, y1, color='red', linewidth=1.0, linestyle='--')
# gca = 'get current axis'
ax = plt.gca()
# 将右边和上边的边框（脊）的颜色去掉
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
# 绑定x轴和y轴
ax.xaxis.set_ticks_position('bottom')
ax.yaxis.set_ticks_position('left')
# 定义x轴和y轴的位置
ax.spines['bottom'].set_position(('data', 0))
ax.spines['left'].set_position(('data', 0))
# 显示交叉点
x0 = 1
y0 = 2*x0 + 1
# s表示点的大小，默认rcParams['lines.markersize']**2
plt.scatter(x0, y0, s = 66, color = 'b')
# 定义线的范围，X的范围是定值，y的范围是从y0到0的位置
# lw的意思是linewidth,线宽
plt.plot([x0, x0], [y0, 0], 'k-.', lw= 2.5)
# 设置关键位置的提示信息
plt.annotate(r'$2x+1=%s$' % 
             y0, 
             xy=(x0, y0), 
             xycoords='data',
             xytext=(+30, -30),
             textcoords='offset points',
             fontsize=16,  # 这里设置的是字体的大小
             # 这里设置的是箭头和箭头的弧度         arrowprops=dict(arrowstyle='->',connectionstyle='arc3,rad=.2')
            )
# 在figure中显示文字信息
# 可以使用\来输出特殊的字符\mu\ \sigma\ \alpha
plt.text(0, 3, 
         r'$This\ is\ a\ good\ idea.\ \mu\ \sigma_i\ \alpha_t$',
         fontdict={'size':16,'color':'r'})
plt.show()
```

这里写图片描述

# tick能见度设置

```
import matplotlib.pyplot as plt
import numpy as np
# 从[-1,1]中等距去50个数作为x的取值
x = np.linspace(-1, 1, 50)
y = 2*x - 1
plt.figure(figsize=(12, 8))  # 第一个参数表示的是编号，第二个表示的是图表的长宽
# alpha是设置透明度的
plt.plot(x, y, color='r', linewidth=10.0, alpha=0.5)
# gca = 'get current axis'
ax = plt.gca()
# 将右边和上边的边框（脊）的颜色去掉
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
# 绑定x轴和y轴
ax.xaxis.set_ticks_position('bottom')
ax.yaxis.set_ticks_position('left')
# 定义x轴和y轴的位置
ax.spines['bottom'].set_position(('data', 0))
ax.spines['left'].set_position(('data', 0))
# 可以使用tick设置透明度
for label in ax.get_xticklabels() + ax.get_yticklabels():
    label.set_fontsize(12)
    label.set_bbox(dict(facecolor='y', edgecolor='None', alpha=0.7))
plt.show()
```

这里写图片描述

# 散点图

```
import matplotlib.pyplot as plt
import numpy as np
n = 1024
# 从[0]
X = np.random.normal(0, 1, n)
Y = np.random.normal(0, 1, n)
T = np.arctan2(X, Y)
plt.scatter(np.arange(5), np.arange(5))
plt.xticks(())
plt.yticks(())
plt.show()
```

这里写图片描述

# 条形图

```
import matplotlib.pyplot as plt
import numpy as np
n = 12
X = np.arange(n)
Y1 = (1 - X/float(n)) * np.random.uniform(0.5, 1.0, n)
Y2 = (1 - X/float(n)) * np.random.uniform(0.5, 1.0, n)
plt.figure(figsize=(12, 8))
plt.bar(X, +Y1, facecolor='#9999ff', edgecolor='white')
plt.bar(X, -Y2, facecolor='#ff9999', edgecolor='white')
for x, y in zip(X,Y1):
    # ha: horizontal alignment水平方向
    # va: vertical alignment垂直方向
    plt.text(x, y+0.05, '%.2f' % y, ha='center', va='bottom')
for x, y in zip(X,-Y2):
    # ha: horizontal alignment水平方向
    # va: vertical alignment垂直方向
    plt.text(x, y-0.05, '%.2f' % y, ha='center', va='top')
# 定义范围和标签
plt.xlim(-.5, n)
plt.xticks(())
plt.ylim(-1.25, 1.25)
plt.yticks(())
plt.show()
```

这里写图片描述

# contour等高线图

```
import matplotlib.pyplot as plt
import numpy as np
def get_height(x, y):
    # the height function
    return (1-x/2+x**5+y**3)*np.exp(-x**2-y**2)
n = 256
x = np.linspace(-3, 3, n)
y = np.linspace(-3, 3, n)
X, Y = np.meshgrid(x, y)
plt.figure(figsize=(14, 8))
# use plt.contourf to filling contours
# X, Y and value for (X, Y) point
# 横坐标、纵坐标、高度、 、透明度、cmap是颜色对应表
# 等高线的填充颜色
plt.contourf(X, Y, get_height(X, Y), 16, alpah=0.7, cmap=plt.cm.hot)  
# use plt.contour to add contour lines
# 这里是等高线的线
C = plt.contour(X, Y, get_height(X, Y), 16, color='black', linewidth=.5)
# adding label
plt.clabel(C, inline=True, fontsize=16)
plt.xticks(())
plt.yticks(())
plt.show()
```

这里写图片描述

# image图片显示

```
import matplotlib.pyplot as plt
import numpy as np

# image data
a = np.array([0.313660827978, 0.365348418405, 0.423733120134,
              0.365348418405, 0.439599930621, 0.525083754405,
              0.423733120134, 0.525083754405, 0.651536351379]).reshape(3,3)

"""
for the value of "interpolation", check this:
http://matplotlib.org/examples/images_contours_and_fields/interpolation_methods.html
for the value of "origin"= ['upper', 'lower'], check this:
http://matplotlib.org/examples/pylab_examples/image_origin.html
"""
# 这是颜色的标注
# 主要使用imshow来显示图片，这里暂时不适用图片来显示，采用色块的方式演示。
plt.imshow(a, interpolation='nearest', cmap='bone', origin='lower')
plt.colorbar(shrink=.90)  # 这是颜色深度的标注，shrink表示压缩比例
plt.xticks(())
plt.yticks(())
plt.show()
```

这里写图片描述

# 3D数据图

```
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
fig = plt.figure(figsize=(12, 8))
ax = Axes3D(fig)
# 生成X，Y
X = np.arange(-4, 4, 0.25)
Y = np.arange(-4, 4, 0.25)
X,Y = np.meshgrid(X, Y)
R = np.sqrt(X**2 + Y**2)
# height value
Z = np.sin(R)
# 绘图
# rstride（row）和cstride(column)表示的是行列的跨度
ax.plot_surface(X, Y, Z, 
                rstride=1,  # 行的跨度
                cstride=1,  # 列的跨度
                cmap=plt.get_cmap('rainbow')  # 颜色映射样式设置
               )
# offset 表示距离zdir的轴距离
ax.contourf(X, Y, Z, zdir='z', offest=-2, cmap='rainbow')
ax.set_zlim(-2, 2)
plt.show()
```

这里写图片描述

# Subplot多合一显示

```
import matplotlib.pyplot as plt
import numpy as np
plt.figure()
# 将整个figure分成两行两列
plt.subplot(2, 2, 1)
# 第一个参数表示X的范围，第二个是y的范围
plt.plot([0, 1], [0, 1])
plt.subplot(222)
plt.plot([0, 1], [0, 2])
plt.subplot(223)
plt.plot([0, 1], [0, 3])
plt.subplot(224)
plt.plot([0, 1], [0, 4])
plt.show()
```

这里写图片描述

# 分格显示

subplot2grid

```
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.gridspec as gridspec
plt.figure()
# 第一个元素表示将总的面板进行划分，划分为3行3列，
# 第二个元素表示该面板从0行0列开始，列的跨度（colspan）为3列，行的跨度（rowspan）为1
ax1 = plt.subplot2grid((3, 3), (0, 0), colspan=3, rowspan=1)
# 第一个元素的表示X的范围为[1,2]，第二个元素表示Y的范围为[1,2]
ax1.plot([1, 2], [1, 2])
ax1.set_title(r'$ax1\_title$')
# 第一个元素表示将总的面板进行划分，划分为3行3列，
# 第二个元素表示该面板从1行0列开始，列的跨度（colspan）为2列，行的跨度（rowspan）取默认值1
ax2 = plt.subplot2grid((3, 3), (1, 0), colspan=2)
ax2.set_title(r'$ax2\_title$')
# 第一个元素表示将总的面板进行划分，划分为3行3列，
# 第二个元素表示该面板从1行2列开始，行的跨度（rowspan）为2列，列的跨度（colspan）取默认值1
ax3 = plt.subplot2grid((3, 3), (1, 2), rowspan=2)
ax3.set_title(r'$ax3\_title$')
# 第一个元素表示将总的面板进行划分，划分为3行3列，
# 第二个元素表示该面板从2行0列开始,行的跨度（rowspan）为2列，列的跨度（colspan）取默认值1
ax4 = plt.subplot2grid((3, 3), (2, 0))
ax4.set_title(r'$ax4\_title$')
# 第一个元素表示将总的面板进行划分，划分为3行3列，
# 第二个元素表示该面板从2行1列开始,行的跨度（rowspan）为2列，列的跨度（colspan）取默认值1
ax5 = plt.subplot2grid((3, 3), (2, 1))
ax5.set_title(r'$ax5\_title$')
plt.tight_layout()
plt.show()
```

这里写图片描述

# gridspec

```
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.gridspec as gridspec
plt.figure()
# 首先，定义网格的布局为3行3列
gs = gridspec.GridSpec(3, 3)
# 这里表示从0行全部都是ax1的
ax1 = plt.subplot(gs[0, :])
ax1.set_title(r'$ax1\_title$')
# 这里表示第一行中0列和1列都是ax2的
ax2 = plt.subplot(gs[1, :2])
ax2.set_title(r'$ax2\_title$')
# 这里表示第一行中2列是ax3的
ax3 = plt.subplot(gs[1:, 2])
ax3.set_title(r'$ax3\_title$')
# 这里表示最后一行中0列是ax4的
ax4 = plt.subplot(gs[-1, 0])
ax4.set_title(r'$ax4\_title$')

# 这里表示最后一行中倒数第二列是ax5的
ax5 = plt.subplot(gs[-1, -2])
ax5.set_title(r'$ax5\_title$')
plt.tight_layout()
plt.show()
```

这里写图片描述

# easy to define structure分格显示

```
import matplotlib.pyplot as plt
import numpy as np
plt.figure()
# sharex表示共享X轴，sharey表示共享y轴
f, ((ax11, ax12), (ax21, ax22)) = plt.subplots(2, 2, sharex=True, sharey=True)
# 显示点（1, 2）, （1, 2）
ax11.scatter([1, 2], [1, 2])
ax11.set_title('11')
ax12.set_title('11')
ax21.set_title('21')
ax22.set_title('22')
plt.tight_layout()
plt.show()
```

这里写图片描述

# 图中图

```
import matplotlib.pyplot as plt
import numpy as np
fig = plt.figure(figsize=(10, 6))
x = [1, 2, 3, 4, 5, 6, 7]
y = [1, 3, 4, 2, 5, 8, 6]
# 大图
left, bottom, width, weight = 0.1, 0.1, 0.8, 0.8
ax1 = fig.add_axes([left, bottom, width, weight])
ax1.plot(x, y, 'r')
ax1.set_xlabel(r'$x$')
ax1.set_ylabel(r'$y$')
ax1.set_title(r'$××Interesting××$')
# 左上小图
left, bottom, width, weight = 0.2, 0.6, 0.25, 0.25
ax2 = fig.add_axes([left, bottom, width, weight])
ax2.plot(y, x, 'b')
ax2.set_xlabel(r'$x$')
ax2.set_ylabel(r'$y$')
ax2.set_title(r'$title\ inside\ 1$')
# 右下小图
plt.axes([0.6, 0.2, 0.25, 0.25])
# 将y的数据逆序输出[::1]
plt.plot(y[::-1],x, 'g')
plt.xlabel('x')
plt.ylabel('y')
plt.title(r'$title\ inside\ 2$')
plt.show()
```

这里写图片描述

# 主次坐标轴

```
import matplotlib.pyplot as plt
import numpy as np
# 从[0, 10]以0.1为间隔，形成一个列表
x = np.arange(0, 10, 0.1)
y1 = 0.05 * x**2
y2 = -1 * y1
fig, ax1 = plt.subplots()
# 镜像（上下左右颠倒）
ax2 = ax1.twinx()
ax1.plot(x, y1, 'g-')
ax2.plot(x, y2, 'b--')
# 为轴进行命名
ax1.set_xlabel(r'$X\ data$', fontsize=16)
ax1.set_ylabel(r'$Y1$', color='g', fontsize=16)
ax2.set_ylabel(r'$Y2$', color='b', fontsize=16)
plt.show()
```

这里写图片描述

# Animation动画

```
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import animation
fig, ax = plt.subplots()
#从[0, 2*np.pi]以0.01为间隔，形成一个列表
x = np.arange(0, 2*np.pi, 0.01)
#这里只需要列表的第一个元素，所以就用逗号“,”加空白的形式省略了列表后面的元素
line, = ax.plot(x, np.sin(x))
def animate(i):
    line.set_ydata(np.sin(x + i/100))
    return line, 
def init():
    line.set_ydata(np.sin(x))
    # 这里由于仅仅需要列表的第一个参数，所以后面的就直接用空白省略了
    return line,  
ani = animation.FuncAnimation(fig=fig, 
                              func=animate,  # 动画函数
                              frames=100,   # 帧数
                              init_func=init,  # 初始化函数
                              interval=20,  # 20ms
                              blit=True)
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190314113625212.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)





