# 时空上下文视觉跟踪（STC） - wishchinYang的专栏 - CSDN博客
2014年06月26日 10:45:13[wishchin](https://me.csdn.net/wishchin)阅读数：1598
       论文的关键点是对时空上下文（Spatio-Temporal Context）信息的利用。主要思想是通过贝叶斯框架对要跟踪的目标和它的局部上下文区域的时空关系进行建模，得到目标和其周围区域低级特征的统计相关性。然后综合这一时空关系和生物视觉系统上的focus of attention特性来评估新的一帧中目标出现位置的置信图，置信最大的位置就是我们得到的新的一帧的目标位置。另外，时空模型的学习和目标的检测都是通过FFT（傅里叶变换）来实现，所以学习和检测的速度都比较快。      
       原文链接：[http://blog.csdn.net/zouxy09/article/details/16889905#comments](http://blog.csdn.net/zouxy09/article/details/16889905#comments)
       代码下载：[http://www4.comp.polyu.edu.hk/~cslzhang/STC/STC.htm](http://www4.comp.polyu.edu.hk/~cslzhang/STC/STC.htm)
