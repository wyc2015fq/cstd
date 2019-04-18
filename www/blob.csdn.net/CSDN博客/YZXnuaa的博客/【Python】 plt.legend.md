# 【Python】 plt.legend - YZXnuaa的博客 - CSDN博客
2018年03月14日 10:14:32[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：6115
参数：
loc(设置图例显示的位置)
'best':,(onlyimplementedforaxeslegends)(自适应方式)'upper right':,'upper left':,'lower left':,'lower right':,'right':,'center left':,'center right':,'lower center':,'upper center':,'center':,
ncol(设置列的数量，使显示扁平化，当要表示的线段特别多的时候会有用)
使用pyplot的方式
**[python]**[view plain](http://blog.csdn.net/You_are_my_dream/article/details/53440964#)[copy](http://blog.csdn.net/You_are_my_dream/article/details/53440964#)
- #!/usr/bin/python
- #coding: utf-8
- 
- import numpy as np   
- import matplotlib.pyplot as plt   
- 
- x = np.arange(1, 11, 1)  
- 
- # plt.plot(x, x * 2, label = "First")
- # plt.plot(x, x * 3, label = "Second")
- # plt.plot(x, x * 4, label = "Third")
- 
- # # loc 设置显示的位置，0是自适应
- # # ncol 设置显示的列数
- # plt.legend(loc = 0, ncol = 2)
- 
- # 也可以这样指定label
- label = ["First", "Second", "Third"]  
- plt.plot(x, x * 2)  
- plt.plot(x, x * 3)  
- plt.plot(x, x * 4)  
- plt.legend(label, loc = 0, ncol = 2)  
- plt.show()  
使用面向对象的方式
**[python]**[view plain](http://blog.csdn.net/You_are_my_dream/article/details/53440964#)[copy](http://blog.csdn.net/You_are_my_dream/article/details/53440964#)
- #!/usr/bin/python
- #coding: utf-8
- 
- import numpy as np   
- import matplotlib.pyplot as plt   
- 
- x = np.arange(1, 11, 1)  
- 
- fig = plt.figure()  
- ax = fig.add_subplot(111)  
- 
- # 面向对象的方式中有三种显示label的方法
- 
- # # 第一种
- # # l后面不加逗号会出错，一定要注意
- # l, = ax.plot(x, x * 2)
- # l.set_label("Demo")
- # ax.legend(loc = 0)
- 
- # 第二种
- # ax.plot(x, x * 2, label = "Demo")
- # ax.legend(loc = 0)
- 
- # 第三种
- ax.plot(x, x * 2)  
- ax.legend(['Demo'], loc = 0)  
- 
- 
- plt.show()  
![](https://img-blog.csdn.net/20161203010824500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
