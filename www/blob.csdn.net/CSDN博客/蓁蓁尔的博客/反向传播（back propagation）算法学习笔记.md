# 反向传播（back propagation）算法学习笔记 - 蓁蓁尔的博客 - CSDN博客





2016年12月28日 11:15:47[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：4269
个人分类：[机器学习相关](https://blog.csdn.net/u013527419/article/category/6202203)









反向传播算法最早于上世纪70年代被提出，但是直到1986年，由David Rumelhart, Geoffrey Hinton, 和Ronald Williams联合发表了一篇著名论文（Learning representations by back-propagating errors）之后，人们才完全认识到这个算法的重要性。
- 
最最基础入门： 

通过一个简单的实例理解反向传播的基本计算过程： 

英文版：[https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/](https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/)

中文版：[http://www.cnblogs.com/charlotte77/p/5629865.html](http://www.cnblogs.com/charlotte77/p/5629865.html)

注：中文版后面附了详细的代码实现，python版。- 
关于反向传播算法的详细解释，很好的材料 
[http://www.offconvex.org/2016/12/20/backprop/](http://www.offconvex.org/2016/12/20/backprop/)

- [https://github.com/rasbt/python-machine-learning-book/blob/master/faq/visual-backpropagation.md](https://github.com/rasbt/python-machine-learning-book/blob/master/faq/visual-backpropagation.md)
- 扩展阅读 

英文版：[https://medium.com/@karpathy/yes-you-should-understand-backprop-e2f06eab496b#.i6d27yds7](https://medium.com/@karpathy/yes-you-should-understand-backprop-e2f06eab496b#.i6d27yds7)

中文版： 
[https://mp.weixin.qq.com/s__biz=MzA3MzI4MjgzMw==&mid=2650721602&idx=2&sn=f18e2d3a23dec485350611651e571031](https://mp.weixin.qq.com/s__biz=MzA3MzI4MjgzMw==&mid=2650721602&idx=2&sn=f18e2d3a23dec485350611651e571031)

注：中文版中有个小错误，当 z=0.5 时，它的局部梯度 z*(1-z) 的会达到最大值 0.25，应该是最大值为0.25，毕竟是开口向下的二次函数。也就是说每经过一次S形函数，梯度将减少1/4，最终使某个网络中较低层的训练比较高层慢得多.





