# Matplotlib——综合演练 - wsp_1138886114的博客 - CSDN博客





2018年05月29日 22:20:32[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：170
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)











- - - [一、绘制三角函数](#一绘制三角函数)- - [1.1快速创建三角函数：](#11快速创建三角函数)
- [1.2 重构（修改颜色和线宽）](#12-重构修改颜色和线宽)
- [1.3 重构（调整坐标）](#13-重构调整坐标)
- [1.4 重构（刻度修改）](#14-重构刻度修改)
- [1.5 重构（刻度修改为π）](#15-重构刻度修改为π)
- [1.6 把坐标轴放在中间](#16-把坐标轴放在中间)
- [1.7 添加图例](#17-添加图例)
- [1.8 添加特殊点注释](#18-添加特殊点注释)
- [1.9 细节修改(设置X轴Y轴 坐标刻度标签字体细节)](#19-细节修改设置x轴y轴-坐标刻度标签字体细节)
- [1.10 完整代码](#110-完整代码)


- [二、绘制条形图](#二绘制条形图)- - [2.1 绘制完整条形图：](#21-绘制完整条形图)
- [2.2 创建两块画板绘制图形](#22-创建两块画板绘制图形)
- [2.3导入csv文件可视化](#23导入csv文件可视化)
- [2.4 从网络中加载数据并可视化](#24-从网络中加载数据并可视化)








### 一、绘制三角函数

> 
##### 1.1快速创建三角函数：

用到了Matplotlib和numpy模块，linspace在(-π，π)之间绘制。

```
代码：
    import matplotlib.pyplot as plt
    import numpy as np

    X = np.linspace(-np.pi,np.pi,256,endpoint = True) #np.linspace在(-π，π)之间分成256小段，并赋予X
    (C,S) = np.cos(X),np.sin(X)     #C，S 分别是cosine/sine 的值（X，C，S都是numpy数组））

    plt.plot(X,C)
    plt.plot(X,S)
    plt.show()
```

##### 1.2 重构（修改颜色和线宽）

```
plt.plot(X,C,'b-',lw=2.5)   # b- 是'color=blue','linestyle= -'简写
plt.plot(X,S,'r-',lw=2.5)   # r- 是'color=red',lw = linewidth
```

##### 1.3 重构（调整坐标）

```
plt.xlim(X.min()*1.5,X.max()*1.5)   #将X，Y轴放大1.5倍        
plt.ylim(C.min()*1.5,C.max()*1.5)
```

##### 1.4 重构（刻度修改）

```
plt.xticks([-np.pi,-np.pi/2,0,np.pi/2,np.pi])
plt.yticks([-1,0,1])
```

##### 1.5 重构（刻度修改为π）

```python
plt.xticks([-np.pi,-np.pi/2,0,np.pi/2,np.pi],
[r'$-\pi$',r'$-\pi/2$',r'$0$',r'$\pi/2$',r'$+\pi$'])
plt.yticks([-1,0,1])
```

##### 1.6 把坐标轴放在中间

```bash
ax = plt.gca()                          #通过plt.gca()获取当前的Axes对象 ax
ax.spines['right'].set_color('none')    #先把右边和上边的边界设置为不可见
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')   #下边界移动到0点
ax.spines['bottom'].set_position(('data',0))
ax.yaxis.set_ticks_position('left')     #左边界移动到0点
ax.spines['left'].set_position(('data',0))
```

##### 1.7 添加图例

```
plt.plot(X,C,'b-',lw=2.5,label='cosine')
plt.plot(X,S,'r-',lw=2.5,label='sine')
plt.legend(loc = 'upper left')
```

##### 1.8 添加特殊点注释

```
t=2*np.pi/3
plt.plot([t,t],[0,np.cos(t)],color='blue',linewidth=2.5,linestyle='--') #画出需要标注的蓝色的线
plt.scatter([t,],[np.cos(t),],50,color='blue')              #画出需要标注的蓝色的点
plt.annotate(r'$\cos(\frac{2\pi}{3}) = -\frac{1}{2}$',      #给蓝色的点添加注释
            xy =(t,np.cos(t)),
            xycoords = 'data',
            xytext = (-90,-50),
            textcoords = 'offset points',
            fontsize = 16,
            arrowprops = dict(arrowstyle ='->',
                              connectionstyle = 'arc3,
                              rad = .2'))

plt.plot([t,t],[0,np.sin(t)],color='red',linewidth=2.5,linestyle='--')  #画出需要标注的红色的线
plt.scatter([t,],[np.sin(t),],50,color='red')          #画出需要标注的红色的点
plt.annotate(r'$\sin(\frac{2\pi}{3}) = \frac{3}{2}$',  #给红色的点添加注释
            xy =(t,np.sin(t)),
            xycoords = 'data',
            xytext = (+10,+30),
            textcoords = 'offset points',
            fontsize = 16,
            arrowprops = dict(arrowstyle ='->',
                              connectionstyle = 'arc3,
                              rad = .2')
        )
```

##### 1.9 细节修改(设置X轴Y轴 坐标刻度标签字体细节)

```
for label in ax.get_xticklabels()+ax.get_yticklabels():
label.set_fontsize(18)
label.set_bbox(dict(facecolor = 'w',edgecolor = 'None',alpha = 0.4))
```

##### 1.10 完整代码

```
import matplotlib.pyplot as plt
import numpy as np
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签 
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号 

▶ np.linspace 在 ( - π,π ) 之间分成 256 小段,并赋予 X 
X = np.linspace(-np.pi, np.pi, 256, endpoint=True) 
(C, S) = np.cos(X), np.sin(X)     # C,S分别是 cosine/sine 的值（ X,C,S 都是 numpy 数 组）） 
plt.figure(figsize = ((8,6)))     # 设置画布大小（可省略）

▶调整线宽和颜色 
plt.plot(X,C,'b-',lw=2.5)         # b - 是 'color=blue','linestyle= - ' 
简写 plt.plot(X,S,'r-',lw=2.5)     # r - 是 'color=red' ,lw = l inewidth 

▶（调整坐标） 
plt.xlim(X.min()*1.5,X.max()*1.2) # 将 X,Y轴放大 1.2 倍 
plt.ylim(C.min()*1.5,C.max()*1.2) 

▶添加图例 
plt.plot(X,C,'b-',lw=2.5,label='cosine') 
plt.plot(X,S,'r-',lw=2.5,label='sine') 

▶（刻度修改） 
plt.xticks([-np.pi,-np.pi/2,0,np.pi/2,np.pi],            
           [r'$-\pi$',r'$-\pi/2$',r'$0$',r'$\pi/2$',r'$+\pi$'])  #（刻度修改为π） 
plt.yticks([-1,0,1]) 

▶把坐标轴放在中间 
ax = plt.gca()                        # 通过 plt.gca() 获取当前的 Axes 对象 ax 
ax.spines['right'].set_color('none')  # 先把右边和上边的边界设置为不可见 
ax.spines['top'].set_color('none') 

ax.xaxis.set_ticks_position('bottom') # 下边界移动到 0 点 
ax.spines['bottom'].set_position(('data', 0)) 
ax.yaxis.set_ticks_position('left')   # 左边界移动到 0 点 
ax.spines['left'].set_position(('data', 0)) 

▶添加特殊点注释 
t=2*np.pi/3 
plt.plot([t,t],[0,np.cos(t)],color='blue',linewidth=2.5,linestyle='--')  # 画出需要标注 的蓝色的线 
plt.scatter([t,],[np.cos(t),],50,color='blue')          # 画出需要标注的蓝色的点 
plt.annotate(r'$\cos(\frac{2\pi}{3}) = -\frac{1}{2}$',  # 给蓝色的点添加注释     
             xy =(t,np.cos(t)),                         # 标注点的位置
             xycoords = 'data',     
             xytext = (-90,-50),                        # 相对于标注点，文字的位置
             textcoords = 'offset points',
             fontsize = 16,     
             arrowprops = dict(arrowstyle ='->',
                              connectionstyle = 'arc3,
                              rad = .2')
             ) 
plt.plot([t,t],[0,np.sin(t)],color='red',linewidth=2.5,linestyle='--') # 画出需要标注的 红色的线 
plt.scatter([t,],[np.sin(t),],50,color='red')           # 画出需要标注的红色的点 
plt.annotate(r'$\sin(\frac{2\pi}{3}) = \frac{3}{2}$',   # 给红色的点添加注释     
             xy =(t,np.sin(t)),xycoords = 'data',     
             xytext = (+10,+30),
             fontsize = 16,     
             arrowprops = dict(arrowstyle ='->',
                              connectionstyle = 'arc3,
                              rad = .2')
             ) 

▶细节修改 ( 设置 X 轴 Y 轴 坐标刻度标签字体细节 ) 
for label in ax.get_xticklabels() + ax.get_yticklabels():     
    label.set_fontsize(18)    
    '''
    set_bbox     #为每个刻度设置边框
    facecolor    # 刻度的背景颜色
    edgecolor    # 边框颜色
    alpha        # 背景透明度
    ''' 
    label.set_bbox(dict(facecolor='w',edgecolor='None',alpha=0.4)) 

plt.legend(loc = 'upper left')            # 图例位置 
plt.show()
```

效果图： 
![这里写图片描述](https://img-blog.csdn.net/20180530090811843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二、绘制条形图

> 
##### 2.1 绘制完整条形图：

```python
# encoding:utf - 8 
import matplotlib.pyplot as plt
import numpy as np 
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签 
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号 

fig = plt.figure(1,figsize=((8,6)))           # 准备一张白纸 ,设置大小

▶确定绘图的范围，设置绘图范围为1行1列,并输出到第 1 块画布（目前只有一块） 
ax1 = plt.subplot(111) 

▶确定绘制条形图的参数 
data = np.array([15,20,18,25]) # 准备输出的数据 
width = 0.5 
x_bar = np.arange(4) 

▶核心图形绘制 ( 通过 plt.bar() 绘制条形图 ) 
rect = ax1.bar(x_bar,data,width = width,color = 'lightblue') 

▶向各条形图上添加数据标签 
for rec in rect:     
    x = rec.get_x()     
    height = rec.get_height()     
    ax1.text(x+0.2,1.01*height,str(height)+'W') # X 轴 /Y 轴写文字位置 

▶绘制 X ， Y 轴坐标刻度及标签，标题 
ax1.set_xticks(x_bar)    # 设置 X 轴刻度 
ax1.set_xticklabels(('第一季度','第二季度','第三季度','第四季度')) 
ax1.set_ylabel('销售量（单位：万）') 
ax1.set_title('2017 年季度销售量统计') 
ax1.grid(True)          # 显示网格 
ax1.set_ylim(0,28)      # 增加留白 

plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/20180626160346271?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 2.2 创建两块画板绘制图形

```python
from matplotlib import pyplot as plt 
import numpy as np 

plt.rcParams['font.sans-serif'] = ['SimHei']       # 用来正常显示中文标签 
plt.rcParams['axes.unicode_minus'] = False         # 用来正常显示负号 

plt.figure(figsize=(6, 8), dpi=80)                 # 设定画板大小
plt.figure(1)                # 创建第一个画板( figure ) 
plt.subplot(211)             # 划分画板 2 行 1 列 两块区域,并获取第 1 块画板区域 
plt.scatter([1, 2, 3],[0.12,0.42,0.62],          # 绘图数据（散点图）
         marker = 'v',       # 图形格式：倒三角
         color = 'r',        # 颜色：红色
         s = 50)             # 图形大小

plt.subplot(212)             # 获取第 2 块画板区域 

plt.plot([4, 5, 6])          # 画图 
-----------------------------------------------------

plt.figure(2)                # 创建第二个画板(figure) 
x = np.arange(4)
y = np.array([23,34,65,78])
plt.plot(x,y)                # 在当前画板(第2个画板)上绘图,默认子图,等价于:subplot(111) 
plt.title('第二块画板')       # 做出当前画板 111 的标题 

plt.figure(1)                # 切换到画板 1 ,注意：当前 subplot(212) 仍然被调用中 
plt.subplot(211)             # 切换到第一块区域 subplot(211) 
plt.title('第一个画板(区域 1)')       # 211 的标题 
plt.subplot(212)                    # 切换到第一块区域 subplot(211) 
plt.title('第一个画板(区域 2)')       # 211 的标题 

plt.tight_layout()                  # 调整每个子图之间的距离 
plt.show()
```

![这里写图片描述](https://img-blog.csdn.net/20180626171359300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 2.3导入csv文件可视化

```python
import matplotlib.pyplot as plt 
import csv 
import numpy as np 

plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签 
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号 

x = [] 
y = [] 
with open('./matplotlib-demo.csv', 'r')as csvfile:     
    plots = csv.reader(csvfile, delimiter=',')     #print plots     
    for row in plots:         
        x.append(int(row[0]))         
        y.append(int(row[1])) 
plt.plot(x, y, label='模拟数据') 
plt.xlabel('x')
plt.ylabel('y') 
plt.title('演示从文件加载数据') 
plt.legend() 
plt.show()
```

```
matplotlib-demo.csv文件数据展示：
1   5
2   3
3   4
4   7
5   4
6   3
7   5
8   7
9   4
10  4
```

![这里写图片描述](https://img-blog.csdn.net/2018062711301718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 2.4 从网络中加载数据并可视化

```python
import urllib
import json

plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签 
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号 

    # 利用 urllib.request 网络存取数据 
    # 通过 json.loads() 
    # 将 json 中的字符串转换为 python 中的字典数据类型 

url = 'https://api.douban.com/v2/book/1220562' 
source_code = urllib2.request.urlopen(url).read().decode() 
data = json.loads(source_code) 
 # data  查看数据
x = []
y = [] 

    # 通过 data['tags'] 获取该键所对应的值 
    # 注意： tags 得到的是列表，列表中的元素是字典 

tags = data['tags'] 
for tag in tags:     
    x.append(tag['name'])     
    y.append(tag['count']) 
plt.bar(x, y, label=u'图书搜索热词') 
plt.legend() 
plt.xlabel('x 轴-搜索热词') 
plt.ylabel('y 轴-搜索热词排行') 
plt.show()
```

```
# 其部分数据展示：
```

```
data:      #查看数据

{'rating': {'max': 10, 'numRaters': 373, 'average': '7.3', 'min': 0},
 'subtitle': '',
 'author': ['[日] 片山恭一'],
 'pubdate': '2005-1',
 'tags': [{'count': 144, 'name': '片山恭一', 'title': '片山恭一'},
  {'count': 70, 'name': '日本', 'title': '日本'},
  {'count': 65, 'name': '日本文学', 'title': '日本文学'},
  {'count': 40, 'name': '小说', 'title': '小说'},
  {'count': 33, 'name': '满月之夜白鲸现', 'title': '满月之夜白鲸现'},
  {'count': 17, 'name': '爱情', 'title': '爱情'},
  {'count': 10, 'name': '純愛', 'title': '純愛'},
  {'count': 9, 'name': '外国文学', 'title': '外国文学'}],
```


![这里写图片描述](https://img-blog.csdn.net/20180627114313514?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







