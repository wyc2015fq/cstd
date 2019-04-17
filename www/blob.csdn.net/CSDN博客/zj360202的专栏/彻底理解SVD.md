# 彻底理解SVD - zj360202的专栏 - CSDN博客





2014年07月15日 17:33:24[zj360202](https://me.csdn.net/zj360202)阅读数：705标签：[SVD																[奇异值分解																[特征值																[特征向量](https://so.csdn.net/so/search/s.do?q=特征向量&t=blog)
个人分类：[数学](https://blog.csdn.net/zj360202/article/category/1452819)





引用维基百科的例子[http://zh.wikipedia.org/wiki/奇异值分解](http://zh.wikipedia.org/wiki/%E5%A5%87%E5%BC%82%E5%80%BC%E5%88%86%E8%A7%A3)




![](https://img-blog.csdn.net/20140715155901765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**1.特征值的求法**

![](https://img-blog.csdn.net/20140715164923750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140715164935406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





[http://sxyd.sdut.edu.cn/zhanshi/shuzhifenxi/shuzhifenxi/4.1/szfx041.htm](http://sxyd.sdut.edu.cn/zhanshi/shuzhifenxi/shuzhifenxi/4.1/szfx041.htm)





![](https://img-blog.csdn.net/20140715170702875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

H的特征可以通过求特征值的方程求得


![](https://img-blog.csdn.net/20140715171033130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


所以上面的B可以转换成

![](https://img-blog.csdn.net/20140715170835296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这里补充一些3阶方阵算特征值的方法

![](https://img-blog.csdn.net/20140715170928203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140715170945625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


后面就是得到多项式乘积就知道特征值了，这里就不计算了




到此，A和B的特征值就算出来了，不过这里要说一些特性，如果A和B都是由M自相乘得来，他们的非零特征值一定数量和值是一样的

这样我们就得到了M的非零奇异特征值![](https://img-blog.csdn.net/20140715171735407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




由此我们就得到了sigma矩阵

![](https://img-blog.csdn.net/20140715172033435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**2.通过特征向量求U和V**



![](https://img-blog.csdn.net/20140715172344176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)的矩阵

先取A的特征值16，算特征向量

![](https://img-blog.csdn.net/20140715173004115?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


得到结果是

-11*x1=0

-5*x2=0

-16*x3=0

0*x4=0

因此x1=0,x2=0,x3=0,x4取任意值（默认是1）

所以得到P1=[0,0,0,1]

同理可以算出其它U的列，还有用B和特征值可以算出V](https://so.csdn.net/so/search/s.do?q=特征值&t=blog)](https://so.csdn.net/so/search/s.do?q=奇异值分解&t=blog)](https://so.csdn.net/so/search/s.do?q=SVD&t=blog)




