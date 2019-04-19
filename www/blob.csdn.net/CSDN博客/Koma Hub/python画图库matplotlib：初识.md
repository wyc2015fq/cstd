# python画图库matplotlib：初识 - Koma Hub - CSDN博客
2018年04月06日 22:05:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：321
虽然不懂python，但是无不钦佩它功能的强大，各种丰富的库。。。
用pip安装matplotlib很简单（我的环境win10+python3.6）：
`python -m pip install matplotlib`
**画个一条曲线**
```python
import matplotlib.pyplot as plt
#program name： main.py
x1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]
y1 = [0]*13
for i in range(1,13):
	y1[i] = x1[i]**2
label2 = ['1','2','3','4','5','6','7','8','9','10','11','11','12','13']
plt.title('y = x*x')
plt.rcParams['font.sans-serif']=['SimHei']#防止中文乱码
plt.xlabel('横向坐标')
plt.ylabel('平方')
plt.plot(x1, y1,'r', label='you guess')
plt.xticks(x1, label2, rotation=1)#设置轴坐标值
plt.legend(bbox_to_anchor=[0.5, 1])
plt.grid()
plt.show()
```
运行与结果（本文所有运行方式相同）：
`python main.py`
![](https://img-blog.csdn.net/20180406211552216)
**画散点图**
```python
import numpy as np
import matplotlib.pyplot as plt
t = np.arange(0., 3., 0.3)
plt.plot(t, t, 'bs', t, t**1.2, 'ro', t, t**1.3, 'g^')
plt.grid()
plt.show()
```
![](https://img-blog.csdn.net/20180406212906386)
**画多个figure（像matlab一样）**
```python
#matplotlib + numpy
import numpy as np
import matplotlib.pyplot as plt
from pylab import *
x = np.arange(-5.0, 5.0, 0.02)
y1 = np.cos(x)
plt.figure(1)
plt.subplot(211)
plt.plot(x, y1)
plt.grid()
plt.subplot(223)
xlim(-2.5, 2.5)
ylim(-1, 1)
plt.plot(x, y1)
plt.grid()
plt.subplot(224)
xlim(0, 5)
ylim(-1, 1)
plt.plot(x, y1)
plt.grid()
plt.show()
```
![](https://img-blog.csdn.net/20180406211741274)
**画饼状图**
```python
import matplotlib.pyplot as plt
labels='中国','美国','日本','俄罗斯'
sizes=51,40,25,35
colors='gold','lightskyblue','yellowgreen','lightcoral'
explode=0.1,0.0,0.0,0.0
plt.rcParams['font.sans-serif']=['SimHei']
plt.pie(sizes,explode=explode,labels=labels,
		colors=colors,autopct='%1.1f%%',shadow=True,startangle=0)
plt.axis('equal')
plt.show()
```
![](https://img-blog.csdn.net/20180406212443837)
**画一个直方图**
```python
import numpy as np
import matplotlib.pyplot as plt
mu, sigma = 100, 15
x = mu + sigma * np.random.randn(100)
plt.subplot(211)
#直方图
plt.hist(x, 50, density=1, facecolor='b', alpha=0.75)
plt.rcParams['font.sans-serif']=['SimHei']
plt.xlabel('横坐标')
plt.ylabel('纵坐标')
plt.title('标题')
plt.text(80, .025, r'这是一个文本')#插入文本
#plt.axis([0, 10, 0, 0.03])
plt.grid(False)
plt.subplot(212)
plt.plot(abs(x))
plt.show()
```
![](https://img-blog.csdn.net/20180406214030321)
**标记某一点**
```python
import numpy as np
import matplotlib.pyplot as plt
 
t = np.arange(0.0, 3.14/4, 0.002)
s = np.tan(2*np.pi*t)
line, = plt.plot(t, s, 'b', lw=3)
 
plt.annotate('<0,0>', xy=(0, 0), xytext=(0.1, 8),
            arrowprops=dict(facecolor='red', shrink=0.0),)
plt.ylim(-20,20)
plt.show()
```
![](https://img-blog.csdn.net/20180406214822280)
**坐标轴刻度**
```python
from pylab import *
X = np.linspace(-np.pi, np.pi, 256,endpoint=True)
C,S = np.cos(X), np.sin(X)
plot(X, C, color="blue", lw=1.0, ls="-")
plot(X, S, color="r", lw=2.0, ls="-")
 
xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi],
       [r'$-pi$', r'$-pi/2$', r'$0$', r'$+pi/2$', r'$+pi$'])
yticks([-1, 0, +1], [r'$-1$', r'$0$', r'$+1$'])
show()
```
![](https://img-blog.csdn.net/20180406215453261)
**图例legend**
```python
#from pylab import * #注释了与上面不变的代码，值显示添加图例修改部分
#X = np.linspace(-np.pi, np.pi, 256,endpoint=True)
#C,S = np.cos(X), np.sin(X)
plot(X, C, color="blue", lw=1.0, ls="-", label='cos')
plot(X, S, color="r", lw=2.0, ls="-",label='sin')
#xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi],
       [r'$-pi$', r'$-pi/2$', r'$0$', r'$+pi/2$', r'$+pi$'])
#yticks([-1, 0, +1], [r'$-1$', r'$0$', r'$+1$'])
legend(loc='upper left')
#show()
```
![](https://img-blog.csdn.net/20180406215851692)
详细介绍请访问：[http://python.jobbole.com/85106/](http://python.jobbole.com/85106/)
