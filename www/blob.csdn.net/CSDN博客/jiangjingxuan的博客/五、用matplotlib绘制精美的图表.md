# 五、用matplotlib绘制精美的图表 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:19:56[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：745












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_8_default_big.png)



matplotlib是python最著名的绘图库，它提供了一整套和matlab相似的命令API，十分适合交互式地进行制图。本节通过几个样例来介绍下如何使用matplotlib绘制精美的图表

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 安装方法
`pip install matplotlib`


## 绘制一元函数图像y=ax+b

创建single_variable.py，内容如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import matplotlib.pyplot as plt
import numpy as np

plt.figure() # 实例化作图变量
plt.title('single variable') # 图像标题
plt.xlabel('x') # x轴文本
plt.ylabel('y') # y轴文本
plt.axis([0, 5, 0, 10]) # x轴范围0-5，y轴范围0-10
plt.grid(True) # 是否绘制网格线
xx = np.linspace(0, 5, 10) # 在0-5之间生成10个点的向量
plt.plot(xx, 2*xx, 'g-') # 绘制y=2x图像，颜色green，形式为线条
plt.show() # 展示图像
```



执行后图像：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/d1604157156a42e12a1285f071295f6aed1fd18b.png)



## 绘制正弦曲线y=sin(x)

创建sinx.py，内容如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import matplotlib.pyplot as plt
import numpy as np

plt.figure() # 实例化作图变量
plt.title('single variable') # 图像标题
plt.xlabel('x') # x轴文本
plt.ylabel('y') # y轴文本
plt.axis([-12, 12, -1, 1]) # x轴范围-12到12，y轴范围-1到1
plt.grid(True) # 是否绘制网格线
xx = np.linspace(-12, 12, 1000) # 在-12到12之间生成1000个点的向量
plt.plot(xx, np.sin(xx), 'g-', label="$sin(x)$") # 绘制y=sin(x)图像，颜色green，形式为线条
plt.plot(xx, np.cos(xx), 'r--', label="$cos(x)$") # 绘制y=cos(x)图像，颜色red，形式为虚线
plt.legend() # 绘制图例
plt.show() # 展示图像
```

执行后图像：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/21eaa5294afd5e281cbadfda0800273cbcb07e43.png)

## 绘制多轴图

创建multi_axis.py内容如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import matplotlib.pyplot as plt
import numpy as np

def draw(plt):
    plt.axis([-12, 12, -1, 1]) # x轴范围-12到12，y轴范围-1到1
    plt.grid(True) # 是否绘制网格线
    xx = np.linspace(-12, 12, 1000) # 在-12到12之间生成1000个点的向量
    plt.plot(xx, np.sin(xx), 'g-', label="$sin(x)$") # 绘制y=sin(x)图像，颜色green，形式为线条
    plt.plot(xx, np.cos(xx), 'r--', label="$cos(x)$") # 绘制y=cos(x)图像，颜色red，形式为虚线
    plt.legend() # 绘制图例

plt.figure() # 实例化作图变量
plt1 = plt.subplot(2,2,1) # 两行两列中的第1张图
draw(plt1)
plt2 = plt.subplot(2,2,2) # 两行两列中的第2张图
draw(plt2)
plt3 = plt.subplot(2,2,3) # 两行两列中的第3张图
draw(plt3)
plt4 = plt.subplot(2,2,4) # 两行两列中的第4张图
draw(plt4)

plt.show() # 展示图像
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

执行后图像：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/dfa79f85084b2d7f623ab23968674a1b996dbe8e.png)



## 绘制3D图像

创建plot_3d.py，内容如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
ax = fig.add_subplot(1,1,1,projection='3d')
theta = np.linspace(-4 * np.pi, 4 * np.pi, 500) # theta旋转角从-4pi到4pi，相当于两圈
z = np.linspace(0, 2, 500) # z轴从下到上,从-2到2之间画100个点
r = z # 半径设置为z大小
x = r * np.sin(theta) # x和y画圆
y = r * np.cos(theta) # x和y画圆
ax.plot(x, y, z, label='curve')
ax.legend()

plt.show()
```

执行后图像：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/ef7cd85aee7a7783df99fe065ce08b69f605331f.png)



## 3D散点图

创建plot_3d_scatter.py，内容如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
ax = fig.add_subplot(1,1,1,projection='3d')
xx = np.linspace(0, 5, 10) # 在0-5之间生成10个点的向量
yy = np.linspace(0, 5, 10) # 在0-5之间生成10个点的向量
zz1 = xx
zz2 = 2*xx
zz3 = 3*xx
ax.scatter(xx, yy, zz1, c='red', marker='o') # o型符号
ax.scatter(xx, yy, zz2, c='green', marker='^') # 三角型符号
ax.scatter(xx, yy, zz3, c='black', marker='*') # 星型符号
ax.legend()

plt.show()
```

执行后图像：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/d8603f1848125e3fc011838c91364c7063c55839.png)

## 绘制3D表面

创建plot_3d_surface.py，内容如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()
ax = fig.gca(projection='3d')

X = np.arange(-5, 5, 0.25)
Y = np.arange(-5, 5, 0.25)
X, Y = np.meshgrid(X, Y)

Z = X**2+Y**2

ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.coolwarm, linewidth=0, antialiased=False)

plt.show()
```



执行后图像：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/61696589c79558491341908184d0564f8b4b8b36.png)




