# Matplotlib—数据可视化 - wsp_1138886114的博客 - CSDN博客





2018年06月02日 15:36:25[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：212
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)











- - - [一、Matplotlib 入门](#一matplotlib-入门)- - [初识mtaplotlib](#初识mtaplotlib)


- [二、 Matplotlib 进阶](#二-matplotlib-进阶)- - - [1 折线图](#1-折线图)
- [2 柱状图/条形图](#2-柱状图条形图)
- [3 直方图](#3-直方图)
- [4 饼图](#4-饼图)
- [5 散点图](#5-散点图)
- [6 堆叠图](#6-堆叠图)
- [7 重构](#7-重构)









### 一、Matplotlib 入门

> 
##### 初识mtaplotlib

```
1 简介：Matplotlib 是开源项目
    官网：http://matplotlib.org
2 Matplotlib 基本绘图
    调用figure 创建一个绘图对象：plt.figure(figsize=(8,4))
    figsize:指定绘图对象的宽度和高度 单位，英寸
    dpi:    绘图分辨率，每英寸多少个像素，缺省值80。本例宽度8*80=640 高度4*80像素

3 安装 matplotlib: pip install matplotlib
    代码实现：import matplotlib.pyplot as plt
              plt.figure(figsize=(8,4))

    画图1：  import matplotlib.pyplot as plt
            plt.figure(figsize = (8,4))
            plt.plot([1,2,3],[5,7,4])   #坐标（1，5），（2，7），（3，4）
            plt.show()                  #默认折线图

4 美化图标-加入图例、标题与标签
    4.1 定义(x,y)坐标
    4.2 绘制两条图，同时添加一个参数label。为线条指定名称，可以在图例中显示它
```

```python
代码：
    import matplotlib.pyplot as plt
    plt.rcParams['font.sans-serif'] = ['SimHei']    #用来正常显示中文标签
    plt.rcParams['axes.unicode_minus'] = False      #用来正常显示负号

    x = [1,2,3]
    y = [4,7,5]

    x2 = [1,2,3]
    y2 = [10,14,12]

    plt.plot(x,y,label = '第一条线')
    plt.plot(x2,y2,label = '第二条线')

    plt.xlabel('xaxis')     #x轴
    plt.ylabel('yaxis')     #y轴

    plt.title('为图标加入\n图例、标题、标签')
    plt.legend()           #显示图例
    plt.show()
```

```
4.4 中文乱码解决问题1：
        http://segmentfault.com/a/1190000005144275

        在代码中动态设置（推荐方式）
        这种方式不需要修改配置文件，比较方便，推荐该方法，下面是具体步骤：
        首先要再python脚本中的开头加上后面的内容：#encoding:utf-8，即用utf8编码
        然后在代码中动态设置字体，下面是主要的几行代码
```

```python
frommatplotlib.font_manager import FontProperties  
import matplotlib.pyplot as plt  
font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=14)  

plt.xlabel(u"电压差(V)", fontproperties=font)  
plt.ylabel(u"介质损耗角差(度)", fontproperties=font)  
plt.title(u"介质损耗角和荷电状态SOC关系图",fontproperties=font)
```

```
4.5 中文乱码解决问题2：  

python中的matplotlib仅支持Unicode编码，默认是不显示中文的，如果让其默认显示中文，有下面方法：
1、在python的安装目录中找到配置文件： %Python_Home%\Lib\site-packages\matplotlib\mpl-data\matplotlibrc    
(如，我的是在C:\Python34\Lib\site-packages\matplotlib\mpl-data)，用任意文本编辑器打开。
2、找到139行的font.family         : sans-serif将其前面的#注释号去掉
3、找到151行的font.sans-serif     :AR PL UMing CN, SimHei, Bitstream Vera Sans, Lucida Grande,
    Verdana, Geneva, Lucid, Arial, Helvetica, Avant Garde, sans-serif
    将【AR PL UMing CN, SimHei】添加在最前面，其中AR PL UMing CN代表：宋体。
    SimHei代表：黑体。并将前面的#注释号去掉，重启编辑器后，便可显示中文了。
4、同时需要更改264行的axes.unicode_minus  : False；使其值为False；否则无法显示负号
5、代码如下：
    import matplotlib.pyplot as plt
    plt.xlabel('x轴')
    plt.ylabel('y轴')
    plt.bar(left = (0,1),height =(1,0.5),width = 0.35)
    plt.show()

4.6 中文乱码解决问题3
    直接在中文前加u  例如：label=u'柱状图-01'
```


### 二、 Matplotlib 进阶

> 
###### 1 折线图

```
plt.plot 默认绘制折线图，最简单绘图方式
```

###### 2 柱状图/条形图

```python
代码：
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号

plt.bar([1, 3, 5, 7,9], [5, 2, 7, 8, 2], label='柱状图-01')
plt.bar([2, 4, 6, 8, 10], [8, 6, 2, 5, 6], label='柱状图-02',color = 'g')

plt.legend()     # 显示图例

plt.xlabel('x轴')
plt.ylabel('y轴')

plt.title('Matplotlib绘制柱状图')
plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/20180626122924843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


###### 3 直方图

```python
代码：
import matplotlib.pyplot as plt   
plt.rcParams['font.sans-serif'] = ['SimHei']    #用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False     #用来正常显示负号

population_ages = [22,55,62,45,21,34,42,42,4,99,102,110,120,121,122,
                       130,111,115,112,80,75,65,54,44,43,42,48]
bins = [0,10,20,30,40,50,60,70,80,90,100,110,120,130]

◆ 绘制频率直方图方法 plt.hist()
plt.hist(population_ages,bins,histtype = 'bar',label = '年龄分布图',rwidth = 0.8)
        #rwidth = 1
plt.xlabel('x') 
plt.ylabel('y') 

plt.title('绘制直方图')
plt.legend()                                  #显示图例
plt.show()
```

###### 4 饼图

```python
代码：
import matplotlib.pyplot as plt   
plt.rcParams['font.sans-serif'] = ['SimHei']    #用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False      #用来正常显示负号
slice = [7,2,2,13]
activities = ['sleeping','eating','working','playing']
cols = ['c','m','r','b']

◆ #绘制饼图方法 plt.pie()
plt.pie(slice,
    labels = activities,    #图例文字说明
    colors = cols,          #颜色
    startangle = 90,        #起始绘图位置
    shadow = True,          #是否显示阴影
    explode = (0,0.1,0,0),  #是否显示偏移（0：不偏移，0.1：偏移0.1个数值）
    autopct = '%1.1f%%'     #是否显示百分比
    )   
plt.title('绘制饼图')
plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/20180626123857678?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


###### 5 散点图

```python
代码
 #encoding:utf-8
import matplotlib.pyplot as plt   
plt.rcParams['font.sans-serif'] = ['SimHei']    #用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False      #用来正常显示负号

x = [1,2,3,4,5,6,7,8]
y = [5,2,4,2,1,4,5,2]

plt.scatter(x,y,label = 'skitcat',color = 'k',s = 25,marker = "o")

plt.xlabel('x')
plt.ylabel('y')
plt.title('interesting Garde')
plt.legend()
plt.show
```

###### 6 堆叠图

```
import matplotlib.pyplot as plt   
plt.rcParams['font.sans-serif'] = ['SimHei']     #用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False       #用来正常显示负号
plt.figure(figsize = (8,6))                      #创建画布

 #一周的时间安排
days = [1,2,3,4,5]                               

sleeping = [7,8,6,11,7]
eating = [2,3,4,3,2]
working  = [7,8,7,2,2]
playing = [8,5,7,8,13]

 # 显示图例
plt.plot([],[],color='m', label='Sleeping',linewidth=5)
plt.plot([],[],color='c', label='Eating',  linewidth=5)
plt.plot([],[],color='r', label='Working', linewidth=5)
plt.plot([],[],color='y', label='Playing', linewidth=5)

 # 绘制堆叠图
plt.stackplot(days,
              sleeping,eating,working,playing,
              colors = ['m','c','r','k'])

plt.xlabel('x')
plt.ylabel('y')
plt.title('堆叠图')
plt.legend()
plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/20180626133022553?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


###### 7 重构

```python
plt.plot([],[],color='m',label="sleeping",linewidth=5)
plt.plot([],[],color='c',label="eating",linewidth=5)
plt.plot([],[],color='r',label="working",linewidth=5)
plt.plot([],[],color='k',label="playing",linewidth=5)

plt.stackplot(days,sleeping,working,playing,colors = ['m','c','r','k'])
plt.xlabel('x')
plt.ylabel('y')
plt.title(u'堆叠图(重构)')
plt.legend()
plt.show()
```




