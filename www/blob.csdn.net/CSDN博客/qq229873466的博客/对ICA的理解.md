# 对ICA的理解 - qq229873466的博客 - CSDN博客

2016年03月24日 22:09:47[qq229873466](https://me.csdn.net/qq229873466)阅读数：624标签：[机器学习																[ICA](https://so.csdn.net/so/search/s.do?q=ICA&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[机器学习](https://blog.csdn.net/qq229873466/article/category/6151050)


参考斯坦福大学Ng教授机器学习讲义note11。

问题：有2个不同的声源发出声音，有4个录音机从不同的地方记录声音，已知4个录音机录到的声音数据，怎么得到2个声源的声音？

![](https://img-blog.csdn.net/20160324220415936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

问题建模：求一个映射矩阵W，使得S=WX（其中S, W未知）

难点：

![](https://img-blog.csdn.net/20160324221240535)

算法求解：

![](https://img-blog.csdn.net/20160324220648877)

![](https://img-blog.csdn.net/20160324220654205)



