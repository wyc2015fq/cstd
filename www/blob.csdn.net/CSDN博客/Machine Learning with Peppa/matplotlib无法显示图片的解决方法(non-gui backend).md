# matplotlib无法显示图片的解决方法(non-gui backend) - Machine Learning with Peppa - CSDN博客





2018年07月03日 13:14:30[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：4961








# 一、解决警告信息

```
... ...
... ...

_tkinter.TclError: no display name and no $DISPLAY environment variable
```

两种解决方法：

## 1、python文件中：

在 from matplotlib import pylot 之前，添加代码：

如下：

```python
import matplotlib as mpl
mpl.use('Agg')

from matplotlib import pylot
```

## 2、在配置文件中：

Linux中，创建文件 ~/.config/matplotlib/matplotlibrc，
（其中，~/.config/matplotlib/是配置文件matplotlibrc的路径）

添加如下一行：
`backend : Agg`
Windows下解决方案应该是一样的（只是配置文件位置不同），还没有使用Windows跑过，请自行Google。

# 二、生成图片

支持的格式（注意不支持jpg格式）： 
eps, pdf, pgf, png, ps, raw, rgba, svg, svgz

在plt.draw() 或者 plt.show() 之后添加生成图片的代码：

```
...
...

plt.show()

plt.savefig('/home/moxiao/code/python/classification_pr.png')
```

# 三、在GUI里打开代码生成的图片即可

# 四、完整示例

picture.py代码：

```
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

plt.figure(1) # 创建图表1
plt.figure(2) # 创建图表2
ax1 = plt.subplot(211) # 在图表2中创建子图1
ax2 = plt.subplot(212) # 在图表2中创建子图2

x = np.linspace(0, 3, 100)
for i in range(5):
    plt.figure(1)  # 选择图表1
    plt.plot(x, np.exp(i*x/3))
    plt.sca(ax1)   # 选择图表2的子图1
    plt.plot(x, np.sin(i*x))
    plt.sca(ax2)  # 选择图表2的子图2
    plt.plot(x, np.cos(i*x))

plt.show()
plt.savefig('table.png')
```

运行结果: 在本地生成 table.png， 在GUI中打开图片： 
![这里写图片描述](https://img-blog.csdn.net/20170817084906887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWR5YW5nMTEyNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



