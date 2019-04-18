# 利用python进行折线图，直方图和饼图的绘制 - 李鑫o_O - CSDN博客

置顶2016年03月10日 21:04:04[hustlx](https://me.csdn.net/HUSTLX)阅读数：3032


# 我用10个国家某年的GDP来绘图，数据如下：

labels   = [*'USA'*, *'China'*, *'India'*, *'Japan'*, *'Germany'*, *'Russia'*, *'Brazil'*, *'UK'*, *'France'*, *'Italy'*]

quants   = [15094025.0, 11299967.0, 4457784.0, 4440376.0, 3099080.0, 2383402.0, 2293954.0, 2260803.0, 2217900.0,1846950.0]

首先绘制折线图，代码如下：

```python
def draw_line(labels,quants):
    ind = np.linspace(0,9,10)
    fig = plt.figure(1)
    ax  = fig.add_subplot(111)
    ax.plot(ind,quants)
    ax.set_title('Top 10 GDP Countries', bbox={'facecolor':'0.8', 'pad':5})
    ax.set_xticklabels(labels)
    plt.grid(True)
plt.show()
```

最后如下图：

![clip_image002[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311102306397-1204312421.jpg)

再画柱状图，代码如下：

```python
def draw_bar(labels,quants):
    width = 0.4
    ind = np.linspace(0.5,9.5,10)
    # make a square figure
    fig = plt.figure(1)
    ax  = fig.add_subplot(111)
    # Bar Plot
    ax.bar(ind-width/2,quants,width,color='green')
    # Set the ticks on x-axis
    ax.set_xticks(ind)
    ax.set_xticklabels(labels)
    # labels
    ax.set_xlabel('Country')
    ax.set_ylabel('GDP (Billion US dollar)')
    # title
    ax.set_title('Top 10 GDP Countries', bbox={'facecolor':'0.8', 'pad':5})
    plt.grid(True)
plt.show()
```

![clip_image004[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311102307304-1031821523.jpg)

最后画饼图，代码如下：

```python
def draw_pie(labels,quants):
    plt.figure(1, figsize=(6,6))
    # For China, make the piece explode a bit
    expl = [0,0.1,0,0,0,0,0,0,0,0]
    # Colors used. Recycle if not enough.
    colors  = ["blue","red","coral","green","yellow","orange"]
    # autopct: format of "percent" string;
    plt.pie(quants, explode=expl, colors=colors, labels=labels, autopct='%1.1f%%',pctdistance=0.8, shadow=True)
    plt.title('Top 10 GDP Countries', bbox={'facecolor':'0.8', 'pad':5})
plt.show()
```

![clip_image006[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311102308210-2005820231.jpg)

# 三、实验小结

Python的安装比较简单，但是numpy、matplotlib、scipy的安装并没有预期的简单，首先版本得对应安装的python版本，而且分32和64位，资源不容易找，安装成功后还要装其他的东西。至于matplitlib的画图感觉还是比较方便的，初学python，虽然整体简洁了很多，但是python的格式的要求过于严格，尤其是缩进等，初学者查了好久都检查不出错误但后来就又稀里糊涂运行成功了，比较抓狂。

附录：完整代码：

```python
# -*- coding: gbk -*-
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
 
def draw_pie(labels,quants):
    # make a square figure
    plt.figure(1, figsize=(6,6))
    # For China, make the piece explode a bit
    expl = [0,0.1,0,0,0,0,0,0,0,0]
    # Colors used. Recycle if not enough.
    colors  = ["blue","red","coral","green","yellow","orange"]
    # Pie Plot
    # autopct: format of "percent" string;
    plt.pie(quants, explode=expl, colors=colors, labels=labels, autopct='%1.1f%%',pctdistance=0.8, shadow=True)
    plt.title('Top 10 GDP Countries', bbox={'facecolor':'0.8', 'pad':5})
    plt.show()
def draw_bar(labels,quants):
    width = 0.4
    ind = np.linspace(0.5,9.5,10)
    # make a square figure
    fig = plt.figure(1)
    ax  = fig.add_subplot(111)
    # Bar Plot
    ax.bar(ind-width/2,quants,width,color='green')
    # Set the ticks on x-axis
    ax.set_xticks(ind)
    ax.set_xticklabels(labels)
    # labels
    ax.set_xlabel('Country')
    ax.set_ylabel('GDP (Billion US dollar)')
    # title
    ax.set_title('Top 10 GDP Countries', bbox={'facecolor':'0.8', 'pad':5})
    plt.grid(True)
    plt.show()
def draw_line(labels,quants):
    ind = np.linspace(0,9,10)
    fig = plt.figure(1)
    ax  = fig.add_subplot(111)
    ax.plot(ind,quants)
    ax.set_title('Top 10 GDP Countries', bbox={'facecolor':'0.8', 'pad':5})
    ax.set_xticklabels(labels)
    plt.grid(True)
    plt.show()
# quants: GDP
# labels: country name
labels   = ['USA', 'China', 'India', 'Japan', 'Germany', 'Russia', 'Brazil', 'UK', 'France', 'Italy']
quants   = [15094025.0, 11299967.0, 4457784.0, 4440376.0, 3099080.0, 2383402.0, 2293954.0, 2260803.0, 2217900.0,1846950.0]
draw_pie(labels,quants)
#draw_bar(labels,quants)
#draw_line(labels,quants)
```

