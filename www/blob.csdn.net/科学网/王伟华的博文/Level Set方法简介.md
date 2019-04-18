# 科学网—Level Set方法简介 - 王伟华的博文




# Level Set方法简介                           

已有 13042 次阅读2009-9-3 15:51|个人分类:[图像处理](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11843&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|关键词:水平集方法|[水平集方法](misc.php?mod=tag&id=35051)


　　Level Set方法是由Sethian和Osher于1988年提出，最近十几年得到广泛的推广与应用。简单的说来，Level Set方法把低维的一些计算上升到更高一维，把N维的描述看成是N＋1维的一个水平。举个例子来说，一个二维平面的圆，如x^2+y^2=1可以看成是二元函数f(x,y)=x^2+y^2的1水平，因此，计算这个圆的变化时就可以先求f(x,y)的变化，再求其1水平集。这样做的好处是，第一，低维时的拓扑变化在高维中不再是一个难题；第二，低维需要不时的重新参数化，高维中不需要；第三，高维的计算更精确，更鲁棒；第四，Level Set方法可以非常容易的向更高维推广；最后，也是非常重要的一点就是，上升到高维空间中后，许多已经成熟的算法可以拿过了直接用，并且在这方面有非常成熟的分析工具，譬如偏微分方程的理论及其数值化等。当然，这种方法最为诟病的就是他增加了计算量，但新的快速算法不断出现，使得这也不是个大问题。



Level Set的适用范围： 


　　这儿只是列举一些经典的领域，但并不完全，如果你能在自己的领域找到新的应用，祝贺你。 Level Set最初始的应用领域就是隐含曲线（曲面）的运动，现在Level Set已经广泛应用于图像恢复、图像增强、图像分割、物体跟踪、形状检测与识别、曲面重建、最小曲面、最优化以及流体力学中的一些东西。 



Level Set需要掌握的知识： 


　　学习和应用Level Set需要掌握偏微分方程理论及其数值化方法，其中又应该着重掌握偏微分方程中的Conversation Law，The Theory of Viscosity Solution(粘性溶液 ) and Hamilton-Jacobi Equation( 哈密尔顿-雅可比方程 )及其数值化方法。同时，在学习Level Set的时候也会经常遇到变分法和测度论的一些内容，但对这两方面的要求不高，了解一下就行了。 



Level Set的推荐读物： 


（1） Stanley Osher and Ronald Fedkiw. Level Set Methods and Dynamic Implicit Surfaces. Springer-Verlag (2002). 评点：这本书是创始人之一Osher写的，这本书是论述Level Set的最完整的书籍之一，更偏重于数值化的高精度解，应用领域涉及图像处理以及计算物理。
（2） James A. Sethian. Level Set Methods and Fast Marching Methods. Cambridge University Press (1999). 评点：这是另外一个创始人Sethian的作品，与Osher的书互有侧重，互相补充，这本书更偏重于Fast Marching Methods, 非结构化网格，涉及的应用领域更广泛。
（3） Guillermo Sapiro, Geometric Partial Differential Equations and Image Analysis, Cambridge University. 评点：这本书对理解Level Set也非常有帮助，它更偏重于图像中的几何特征，如曲率等，对几何偏微分方程介绍的比较详细。
（4） Gilles Aubert and Pierre Kornprobst，
Mathematical problems in image processing: Partial Differential Equations and the Calculus of Variation, Springer, Applied Mathematical Sciences, Vol 147, 2002。这本书数学味太浓，一般人没兴趣读下去，但如果你确实想对你的方法奠定更好的理论基础，这本书就非常有用了，它可以指导你应该在哪方面下功夫。另外，这边书的前言和第一章写的非常好，非常值得一读。
总评：（1）和（2）是学习Level Set常备案头的手册， 如果你想深入，（3）和（4）也应该看一看。 



Level Set推荐文章 


（1） Osher, S., and Sethian, J.A., Fronts Propagating with Curvature-Dependent Speed: Algorithms Based on Hamilton–Jacobi Formulations, Journal of Computational Physics, 79, pp. 12–49, 1988. 评点：这是开创Level Set的一篇文章，必读。
http://math.berkeley.edu/~sethian/Publications/publications.html
这儿可以下载，但是这儿下载的文章只有文字没有图，要想看真正原版的，到图书馆复印吧。 

（2） Osher, S. and Fedkiw, R., “Level Set Methods: An Overview and Some Recent Results”, J. Comput. Phys. 169, 463-502 (2001). 评点：这是一篇比较早的综述，UCLA CAM Report 00-08。
http://www.math.ucla.edu/%7Eimagers/htmls/reports.html可以下载。 

（3） Richard Tsai and Stanly Osher，level set methods and their applications in image science，COMM. MATH. SCI. Vol. 1, No. 4, pp. 623-656 评点：这篇综述内容更丰富些，结果也比较新。intlpress.com/CMS/issue4/levelset_imaging_chapter.pdf 可以下载。
总评：关于Level set的文章太多，无法一一列举，强烈建议到下面的网址逛一逛，那儿有最新的文章。http://www.math.ucla.edu/~imagers/reports.htm 



Level Set推荐网站： 


（1）http://math.berkeley.edu/~sethian/level_set.html
评点：这是Sethian的网站，上面关于Level Set的论述非常多，分门别类，非常清晰。
（2）http://www.math.ucla.edu/~imagers/
评点：这是UCLA的研究组，由Osher创办，关于Level Set的新进展几乎都跟他们相关，这个网站是关注Level Set的最新新闻的最好的地方。 

Level Set的工具包：
http://www.cs.ubc.ca/~mitchell/ToolboxLS/index.html评点：这是Michell开发的工具包，通用性比较好，缺点是自己修改起来非常麻烦。建议自己重新写这些函数，可以把这个工具包拿来验证自己写的对否。 


转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-253159.html](http://blog.sciencenet.cn/blog-81613-253159.html)

上一篇：[图像处理中的数学问题（连载１）](blog-81613-253111.html)
下一篇：[最全的在线查询](blog-81613-255791.html)


