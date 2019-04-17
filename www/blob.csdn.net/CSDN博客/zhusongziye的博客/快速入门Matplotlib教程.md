# 快速入门Matplotlib教程 - zhusongziye的博客 - CSDN博客





2018年04月12日 20:14:52[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：467








> 
原文链接:http://www.labri.fr/perso/nrougier/teaching/matplotlib/#id12
译文链接：https://liam0205.me/2014/09/11/matplotlib-tutorial-zh-cn/


## 介绍

`Matplotlib`可能是 Python 2D-绘图领域使用最广泛的套件。它能让使用者很轻松地将数据图形化，并且提供多样化的输出格式。这里将会探索 matplotlib 的常见用法。

#### IPython 以及 pylab 模式

IPython 是 Python 的一个增强版本。它在下列方面有所增强：命名输入输出、使用系统命令（shell commands）、排错（debug）能力。我们在命令行终端给 IPython 加上参数`-pylab`（0.12 以后的版本是`--pylab`）之后，就可以像 Matlab 或者 Mathematica 那样以交互的方式绘图。

#### pylab

pylab 是 matplotlib 面向对象绘图库的一个接口。它的语法和 Matlab 十分相近。也就是说，它主要的绘图命令和 Matlab 对应的命令有相似的参数。

#### 初级绘制

这一节中，我们将从简到繁：先尝试用默认配置在同一张图上绘制正弦和余弦函数图像，然后逐步美化它。

第一步，是取得正弦函数和余弦函数的值：

```
1from pylab import *
2X = np.linspace(-np.pi, np.pi, 256,endpoint=True)
3C,S = np.cos(X), np.sin(X)
```

`X`是一个`numpy`数组，包含了从 −π−π 到 +π+π 等间隔的 256 个值。`C`和`S`则分别是这 256 个值对应的余弦和正弦函数值组成的 numpy 数组。

你可以在 IPython 的交互模式下测试代码，也可以下载代码（下载链接就是这些示例图），然后执行：

```
1python exercise_1.py
```

#### 使用默认配置

Matplotlib 的默认配置都允许用户自定义。你可以调整大多数的默认配置：图片大小和分辨率（dpi）、线宽、颜色、风格、坐标轴、坐标轴以及网格的属性、文字与字体属性等。不过，matplotlib 的默认配置在大多数情况下已经做得足够好，你可能只在很少的情况下才会想更改这些默认配置。

```
1from pylab import *
2X = np.linspace(-np.pi, np.pi, 256,endpoint=True)
3C,S = np.cos(X), np.sin(X)
4plot(X,C)
5plot(X,S)
6show()
```

![](https://img-blog.csdn.net/20180412200803431?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 默认配置的具体内容

下面的代码中，我们展现了 matplotlib 的默认配置并辅以注释说明，这部分配置包含了有关绘图样式的所有配置。代码中的配置与默认配置完全相同，你可以在交互模式中修改其中的值来观察效果。

```
1# 导入 matplotlib 的所有内容（nympy 可以用 np 这个名字来使用）
 2from pylab import *
 3# 创建一个 8 * 6 点（point）的图，并设置分辨率为 80
 4figure(figsize=(8,6), dpi=80)
 5# 创建一个新的 1 * 1 的子图，接下来的图样绘制在其中的第 1 块（也是唯一的一块）
 6subplot(1,1,1)
 7X = np.linspace(-np.pi, np.pi, 256,endpoint=True)
 8C,S = np.cos(X), np.sin(X)
 9# 绘制余弦曲线，使用蓝色的、连续的、宽度为 1 （像素）的线条
10plot(X, C, color="blue", linewidth=1.0, linestyle="-")
11# 绘制正弦曲线，使用绿色的、连续的、宽度为 1 （像素）的线条
12plot(X, S, color="green", linewidth=1.0, linestyle="-")
13# 设置横轴的上下限
14xlim(-4.0,4.0)
15# 设置横轴记号
16xticks(np.linspace(-4,4,9,endpoint=True))
17# 设置纵轴的上下限
18ylim(-1.0,1.0)
19# 设置纵轴记号
20yticks(np.linspace(-1,1,5,endpoint=True))
21# 以分辨率 72 来保存图片
22# savefig("exercice_2.png",dpi=72)
23# 在屏幕上显示
24show()
```

![](https://img-blog.csdn.net/20180412200818767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 改变线条的颜色和粗细

首先，我们以蓝色和红色分别表示余弦和正弦函数，而后将线条变粗一点。接下来，我们在水平方向拉伸一下整个图。

```
1...
2figure(figsize=(10,6), dpi=80)
3plot(X, C, color="blue", linewidth=2.5, linestyle="-")
4plot(X, S, color="red",  linewidth=2.5, linestyle="-")
5...
```

![](https://img-blog.csdn.net/20180412200833189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 设置图片边界

当前的图片边界设置得不好，所以有些地方看得不是很清楚。

```
1...
2xlim(X.min()*1.1, X.max()*1.1)
3ylim(C.min()*1.1, C.max()*1.1)
4...
```

更好的方式是这样：

```
1xmin ,xmax = X.min(), X.max()
2ymin, ymax = Y.min(), Y.max()
3dx = (xmax - xmin) * 0.2
4dy = (ymax - ymin) * 0.2
5xlim(xmin - dx, xmax + dx)
6ylim(ymin - dy, ymax + dy)
```

![](https://img-blog.csdn.net/2018041220085518?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 设置记号

我们讨论正弦和余弦函数的时候，通常希望知道函数在 ±π±π 和 ±π2±π2 的值。这样看来，当前的设置就不那么理想了。

```
1...
2xticks( [-np.pi, -np.pi/2, 0, np.pi/2, np.pi])
3yticks([-1, 0, +1])
4...
```

![](https://img-blog.csdn.net/20180412200906253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 设置记号的标签

记号现在没问题了，不过标签却不大符合期望。我们可以把 3.1423.142 当做是 ππ，但毕竟不够精确。当我们设置记号的时候，我们可以同时设置记号的标签。注意这里使用了 LaTeX。

```
1...
2xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi],
3       [r'$-\pi$', r'$-\pi/2$', r'$0$', r'$+\pi/2$', r'$+\pi$'])
4yticks([-1, 0, +1],
5       [r'$-1$', r'$0$', r'$+1$'])
6...
```

![](https://img-blog.csdn.net/20180412200918370?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 移动脊柱

坐标轴线和上面的记号连在一起就形成了脊柱（Spines，一条线段上有一系列的凸起，是不是很像脊柱骨啊~），它记录了数据区域的范围。它们可以放在任意位置，不过至今为止，我们都把它放在图的四边。

实际上每幅图有四条脊柱（上下左右），为了将脊柱放在图的中间，我们必须将其中的两条（上和右）设置为无色，然后调整剩下的两条到合适的位置——数据空间的 0 点。

```
1...
2ax = gca()
3ax.spines['right'].set_color('none')
4ax.spines['top'].set_color('none')
5ax.xaxis.set_ticks_position('bottom')
6ax.spines['bottom'].set_position(('data',0))
7ax.yaxis.set_ticks_position('left')
8ax.spines['left'].set_position(('data',0))
9...
```

![](https://img-blog.csdn.net/20180412200932684?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 添加图例

我们在图的左上角添加一个图例。为此，我们只需要在 plot 函数里以「键 - 值」的形式增加一个参数。

```
1...
2plot(X, C, color="blue", linewidth=2.5, linestyle="-", label="cosine")
3plot(X, S, color="red",  linewidth=2.5, linestyle="-", label="sine")
4legend(loc='upper left')
5...
```

![](https://img-blog.csdn.net/20180412200944353?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 给一些特殊点做注释

我们希望在 2π/32π/3 的位置给两条函数曲线加上一个注释。首先，我们在对应的函数图像位置上画一个点；然后，向横轴引一条垂线，以虚线标记；最后，写上标签。

```
1...
 2t = 2*np.pi/3
 3plot([t,t],[0,np.cos(t)], color ='blue', linewidth=2.5, linestyle="--")
 4scatter([t,],[np.cos(t),], 50, color ='blue')
 5annotate(r'$\sin(\frac{2\pi}{3})=\frac{\sqrt{3}}{2}$',
 6         xy=(t, np.sin(t)), xycoords='data',
 7         xytext=(+10, +30), textcoords='offset points', fontsize=16,
 8         arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))
 9plot([t,t],[0,np.sin(t)], color ='red', linewidth=2.5, linestyle="--")
10scatter([t,],[np.sin(t),], 50, color ='red')
11annotate(r'$\cos(\frac{2\pi}{3})=-\frac{1}{2}$',
12         xy=(t, np.cos(t)), xycoords='data',
13         xytext=(-90, -50), textcoords='offset points', fontsize=16,
14         arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))
15...
```

![](https://img-blog.csdn.net/2018041220095943?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 精益求精

坐标轴上的记号标签被曲线挡住了，作为强迫症患者（雾）这是不能忍的。我们可以把它们放大，然后添加一个白色的半透明底色。这样可以保证标签和曲线同时可见。

```
1...
2for label in ax.get_xticklabels() + ax.get_yticklabels():
3    label.set_fontsize(16)
4    label.set_bbox(dict(facecolor='white', edgecolor='None', alpha=0.65 ))
5...
```

![](https://img-blog.csdn.net/2018041220100951?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 图像、子图、坐标轴和记号

到目前为止，我们都用隐式的方法来绘制图像和坐标轴。快速绘图中，这是很方便的。我们也可以显式地控制图像、子图、坐标轴。Matplotlib 中的「图像」指的是用户界面看到的整个窗口内容。在图像里面有所谓「子图」。子图的位置是由坐标网格确定的，而「坐标轴」却不受此限制，可以放在图像的任意位置。我们已经隐式地使用过图像和子图：当我们调用`plot`函数的时候，matplotlib 调用`gca()`函数以及`gcf()`函数来获取当前的坐标轴和图像；如果无法获取图像，则会调用`figure()`函数来创建一个——严格地说，是用`subplot(1,1,1)`创建一个只有一个子图的图像。

#### 图像

所谓「图像」就是 GUI 里以「Figure #」为标题的那些窗口。图像编号从 1 开始，与 MATLAB 的风格一致，而于 Python 从 0 开始编号的风格不同。以下参数是图像的属性：

![](https://img-blog.csdn.net/20180412201019620?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这些默认值可以在源文件中指明。不过除了图像数量这个参数，其余的参数都很少修改。

你在图形界面中可以按下右上角的 X 来关闭窗口（OS X 系统是左上角）。Matplotlib 也提供了名为`close`的函数来关闭这个窗口。`close`函数的具体行为取决于你提供的参数：
- 
不传递参数：关闭当前窗口；

- 
传递窗口编号或窗口实例（instance）作为参数：关闭指定的窗口；

- 
`all`：关闭所有窗口。


和其他对象一样，你可以使用`setp`或者是`set_something`这样的方法来设置图像的属性。

#### 子图

你可以用子图来将图样（plot）放在均匀的坐标网格中。用`subplot`函数的时候，你需要指明网格的行列数量，以及你希望将图样放在哪一个网格区域中。此外，`gridspec`的功能更强大，你也可以选择它来实现这个功能。

![](https://img-blog.csdn.net/20180412201035139?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180412201044759?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180412201055869?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180412201103361?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 坐标轴

坐标轴和子图功能类似，不过它可以放在图像的任意位置。因此，如果你希望在一副图中绘制一个小图，就可以用这个功能。

![](https://img-blog.csdn.net/20180412201115799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180412201123287?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 记号

良好的记号是图像的重要组成部分。Matplotlib 里的记号系统里的各个细节都是可以由用户个性化配置的。你可以用 Tick Locators 来指定在那些位置放置记号，用 Tick Formatters 来调整记号的样式。主要和次要的记号可以以不同的方式呈现。默认情况下，每一个次要的记号都是隐藏的，也就是说，默认情况下的次要记号列表是空的——`NullLocator`。

Tick Locators

下面有为不同需求设计的一些 Locators。

![](https://img-blog.csdn.net/20180412201137157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这些 Locators 都是`matplotlib.ticker.Locator`的子类，你可以据此定义自己的 Locator。以日期为 ticks 特别复杂，因此 Matplotlib 提供了`matplotlib.dates`来实现这一功能。

#### 其他类型的图

接下来的内容是练习。请运用你学到的知识，从提供的代码开始，实现配图所示的效果。具体的答案可以点击配图下载。

普通图

```
1from pylab import *
2n = 256
3X = np.linspace(-np.pi,np.pi,n,endpoint=True)
4Y = np.sin(2*X)
5plot (X, Y+1, color='blue', alpha=1.00)
6plot (X, Y-1, color='blue', alpha=1.00)
7show()
```



![](https://img-blog.csdn.net/20180412201148494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

散点图

```
1from pylab import *
2n = 1024
3X = np.random.normal(0,1,n)
4Y = np.random.normal(0,1,n)
5scatter(X,Y)
6show()
```



![](https://img-blog.csdn.net/2018041220120365?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

条形图

```
1from pylab import *
 2n = 12
 3X = np.arange(n)
 4Y1 = (1-X/float(n)) * np.random.uniform(0.5,1.0,n)
 5Y2 = (1-X/float(n)) * np.random.uniform(0.5,1.0,n)
 6bar(X, +Y1, facecolor='#9999ff', edgecolor='white')
 7bar(X, -Y2, facecolor='#ff9999', edgecolor='white')
 8for x,y in zip(X,Y1):
 9    text(x+0.4, y+0.05, '%.2f' % y, ha='center', va= 'bottom')
10ylim(-1.25,+1.25)
11show()
12
```



![](https://img-blog.csdn.net/20180412201218556?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

等高线图


```
1from pylab import *
2def f(x,y): return (1-x/2+x**5+y**3)*np.exp(-x**2-y**2)
3n = 256
4x = np.linspace(-3,3,n)
5y = np.linspace(-3,3,n)
6X,Y = np.meshgrid(x,y)
7contourf(X, Y, f(X,Y), 8, alpha=.75, cmap='jet')
8C = contour(X, Y, f(X,Y), 8, colors='black', linewidth=.5)
9show()
```



![](https://img-blog.csdn.net/20180412201229334?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

灰度图（Imshow）

```
1from pylab import *
2def f(x,y): return (1-x/2+x**5+y**3)*np.exp(-x**2-y**2)
3n = 10
4x = np.linspace(-3,3,4*n)
5y = np.linspace(-3,3,3*n)
6X,Y = np.meshgrid(x,y)
7imshow(f(X,Y)), show()
```



![](https://img-blog.csdn.net/20180412201239195?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

饼状图

```
1from pylab import *
2n = 20
3Z = np.random.uniform(0,1,n)
4pie(Z), show()
```



![](https://img-blog.csdn.net/20180412201248509?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

量场图（Quiver Plots）

```
1from pylab import *
2n = 8
3X,Y = np.mgrid[0:n,0:n]
4quiver(X,Y), show()
```



![](https://img-blog.csdn.net/20180412201257656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

网格

```
1from pylab import *
2axes = gca()
3axes.set_xlim(0,4)
4axes.set_ylim(0,3)
5axes.set_xticklabels([])
6axes.set_yticklabels([])
7show()
```



![](https://img-blog.csdn.net/20180412201324915?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

多重网格

```
1from pylab import *
2subplot(2,2,1)
3subplot(2,2,3)
4subplot(2,2,4)
5show()
```



![](https://img-blog.csdn.net/20180412201332650?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

极轴图

```
1from pylab import *
 2axes([0,0,1,1])
 3N = 20
 4theta = np.arange(0.0, 2*np.pi, 2*np.pi/N)
 5radii = 10*np.random.rand(N)
 6width = np.pi/4*np.random.rand(N)
 7bars = bar(theta, radii, width=width, bottom=0.0)
 8for r,bar in zip(radii, bars):
 9    bar.set_facecolor( cm.jet(r/10.))
10    bar.set_alpha(0.5)
11show()
```



![](https://img-blog.csdn.net/20180412201341281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3D 图

```
1from pylab import *
 2from mpl_toolkits.mplot3d import Axes3D
 3fig = figure()
 4ax = Axes3D(fig)
 5X = np.arange(-4, 4, 0.25)
 6Y = np.arange(-4, 4, 0.25)
 7X, Y = np.meshgrid(X, Y)
 8R = np.sqrt(X**2 + Y**2)
 9Z = np.sin(R)
10ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='hot')
11show()
```

![](https://img-blog.csdn.net/2018041220135253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 手稿

![](https://img-blog.csdn.net/20180412201403976?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







欢迎大家阅读！



