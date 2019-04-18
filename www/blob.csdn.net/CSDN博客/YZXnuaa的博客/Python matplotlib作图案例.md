# Python matplotlib作图案例 - YZXnuaa的博客 - CSDN博客
2018年02月01日 15:06:10[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：268
使用matplotlib自带的几种美化样式，就可以很轻松的对生成的图形进行美化。
可以使用matplotlib.pyplot.style.available获取所有的美化样式
**[python]**[view plain](http://blog.csdn.net/you_are_my_dream/article/details/53464662#)[copy](http://blog.csdn.net/you_are_my_dream/article/details/53464662#)
- #!/usr/bin/python
- #coding: utf-8
- 
- import numpy as np  
- import matplotlib.pyplot as plt  
- 
- # 获取所有的自带样式
- print plt.style.available  
- 
- # 使用自带的样式进行美化
- plt.style.use("ggplot")  
- 
- fig, axes = plt.subplots(ncols = 2, nrows = 2)  
- 
- # 四个子图的坐标轴赋予四个对象
- ax1, ax2, ax3, ax4 = axes.ravel()  
- 
- x, y = np.random.normal(size = (2, 100))  
- ax1.plot(x, y, "o")  
- 
- x = np.arange(1, 10)  
- y = np.arange(1, 10)  
- 
- # plt.rcParams['axes.prop_cycle']获取颜色的字典
- # 会在这个范围内依次循环
- ncolors = len(plt.rcParams['axes.prop_cycle'])  
- # print ncolors
- # print plt.rcParams['axes.prop_cycle']
- 
- shift = np.linspace(1, 20, ncolors)  
- for s in shift:  
- # print s
-     ax2.plot(x, y + s, "-")  
- 
- x = np.arange(5)  
- y1, y2, y3 = np.random.randint(1, 25, size = (3, 5))  
- width = 0.25
- 
- # 柱状图中要显式的指定颜色
- ax3.bar(x, y1, width, color = "r")  
- ax3.bar(x + width, y2, width, color = "g")  
- ax3.bar(x + 2 * width, y3, width, color = "y")  
- 
- for i, color in enumerate(plt.rcParams['axes.prop_cycle']):  
-     xy = np.random.normal(size= 2)  
- for c in color.values():  
-         ax4.add_patch(plt.Circle(xy, radius = 0.3, color= c))  
- 
- ax4.axis("equal")  
- 
- plt.show()  
使用ggplot进行美化后的结果
![](https://img-blog.csdn.net/20161205135929081)
