# 入门python中的matplotlib - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:26:50[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1703








调用matplotlib画图的流程是：调用figure()得到fig对象 -> 调用fig.add_subplot(111)得到axis对象 -> 调用plt.plot绘制 -> plt.show()显示出figure






上面的程序中，figure()返回一个figure对象，add_subplot()返回一个axes对象，里面的参数abc表示在一个figure窗口中，有a行b列个小窗口，然后本次plot在第c个窗口中。







```python
import matplotlib.pyplot as plt
import numpy as np
x=np.arange(0,10,0.1)
y=np.random.randn(len(x))
fig=plt.figure()
ax=fig.add_subplot(111)
l,=plt.plot(x,y)
t=ax.set_title('random numbers')
plt.show()
```






add_subplot(111)效果如下：






![](https://img-blog.csdn.net/20140927172932015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





add_subplot(121)效果如下：






![](https://img-blog.csdn.net/20140927173000719?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





add_subplot(212)效果如下：



![](https://img-blog.csdn.net/20140927172850421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





其他的一些用法参见：

[http://www.cnblogs.com/bovine/archive/2012/11/09/2763374.html](http://www.cnblogs.com/bovine/archive/2012/11/09/2763374.html)










