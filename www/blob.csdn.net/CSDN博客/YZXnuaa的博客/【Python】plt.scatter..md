# 【Python】plt.scatter. - YZXnuaa的博客 - CSDN博客
2018年03月14日 10:52:43[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：407
1、scatter函数原型
![](https://img-blog.csdn.net/20151104231449817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、其中散点的形状参数marker如下：
![](https://img-blog.csdn.net/20151104234024896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、其中颜色参数c如下:
![](https://img-blog.csdn.net/20151104234431749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、基本的使用方法如下：
**[python]**[view plain](http://blog.csdn.net/u013634684/article/details/49646311#)[copy](http://blog.csdn.net/u013634684/article/details/49646311#)
- #导入必要的模块
- import numpy as np  
- import matplotlib.pyplot as plt  
- #产生测试数据
- x = np.arange(1,10)  
- y = x  
- fig = plt.figure()  
- ax1 = fig.add_subplot(111)  
- #设置标题
- ax1.set_title('Scatter Plot')  
- #设置X轴标签
- plt.xlabel('X')  
- #设置Y轴标签
- plt.ylabel('Y')  
- #画散点图
- ax1.scatter(x,y,c = 'r',marker = 'o')  
- #设置图标
- plt.legend('x1')  
- #显示所画的图
- plt.show()  
结果如下：
![](https://img-blog.csdn.net/20151104234850697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5、当scatter后面参数中数组的使用方法，如s，当s是同x大小的数组，表示x中的每个点对应s中一个大小，其他如c，等用法一样，如下：
（1）、不同大小
**[python]**[view plain](http://blog.csdn.net/u013634684/article/details/49646311#)[copy](http://blog.csdn.net/u013634684/article/details/49646311#)
- #导入必要的模块
- import numpy as np  
- import matplotlib.pyplot as plt  
- #产生测试数据
- x = np.arange(1,10)  
- y = x  
- fig = plt.figure()  
- ax1 = fig.add_subplot(111)  
- #设置标题
- ax1.set_title('Scatter Plot')  
- #设置X轴标签
- plt.xlabel('X')  
- #设置Y轴标签
- plt.ylabel('Y')  
- #画散点图
- sValue = x*10
- ax1.scatter(x,y,s=sValue,c='r',marker='x')  
- #设置图标
- plt.legend('x1')  
- #显示所画的图
- plt.show()  
![](https://img-blog.csdn.net/20151104235655651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（2）、不同颜色
**[python]**[view plain](http://blog.csdn.net/u013634684/article/details/49646311#)[copy](http://blog.csdn.net/u013634684/article/details/49646311#)
- #导入必要的模块
- import numpy as np  
- import matplotlib.pyplot as plt  
- #产生测试数据
- x = np.arange(1,10)  
- y = x  
- fig = plt.figure()  
- ax1 = fig.add_subplot(111)  
- #设置标题
- ax1.set_title('Scatter Plot')  
- #设置X轴标签
- plt.xlabel('X')  
- #设置Y轴标签
- plt.ylabel('Y')  
- #画散点图
- cValue = ['r','y','g','b','r','y','g','b','r']  
- ax1.scatter(x,y,c=cValue,marker='s')  
- #设置图标
- plt.legend('x1')  
- #显示所画的图
- plt.show()  
结果：
![](https://img-blog.csdn.net/20151104235959852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（3）、线宽linewidths
**[python]**[view plain](http://blog.csdn.net/u013634684/article/details/49646311#)[copy](http://blog.csdn.net/u013634684/article/details/49646311#)
- #导入必要的模块
- import numpy as np  
- import matplotlib.pyplot as plt  
- #产生测试数据
- x = np.arange(1,10)  
- y = x  
- fig = plt.figure()  
- ax1 = fig.add_subplot(111)  
- #设置标题
- ax1.set_title('Scatter Plot')  
- #设置X轴标签
- plt.xlabel('X')  
- #设置Y轴标签
- plt.ylabel('Y')  
- #画散点图
- lValue = x  
- ax1.scatter(x,y,c='r',s= 100,linewidths=lValue,marker='o')  
- #设置图标
- plt.legend('x1')  
- #显示所画的图
- plt.show()  
![](https://img-blog.csdn.net/20151105000655254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
【Python】plt.scatter.            
