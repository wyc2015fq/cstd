# 因子图（factor graph） - wydbyxr的博客 - CSDN博客
2018年09月04日 15:25:45[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：5565
个人分类：[机器学习具体算法](https://blog.csdn.net/wydbyxr/article/category/7945743)
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 因子图（factor graph）
　　Factor Graph 是概率图的一种，概率图有很多种，最常见的就是Bayesian Network (贝叶斯网络)和Markov Random Fields(马尔可夫随机场)。 
　　在概率图中，求某个变量的边缘分布是常见的问题。这问题有很多求解方法，其中之一就是可以把Bayesian Network和Markov Random Fields 转换成Facor Graph，然后用sum-product算法求解。基于Factor Graph可以用sum-product算法可以高效的求各个变量的边缘分布。
## 更详细的理解
　　将一个具有多变量的全局函数因子分解，得到几个局部函数的乘积，以此为基础得到的一个双向图叫做因子图。 
　　所谓factor graph(因子图)，就是对函数因子分解的表示图，一般内含两种节点，变量节点和函数节点。我们知道，一个全局函数能够分解为多个局部函数的积，因式分解就行了，这些局部函数和对应的变量就能体现在因子图上。 
　　在概率论及其应用中, 因子图是一个在贝叶斯推理中得到广泛应用的模型。
## sum-product算法
　　在因子图中，所有顶点，要不然就是变量节点不然就是函数节点，边线表示他们之间的函数关系。在讲解朴素贝叶斯和马尔可夫的时候，我们变线上标注的符 号，也就是Psi函数表示符号，就是表示我们模型中x和y的联系函数。Psi函数在不同的环境下有着不同的含义，因此解释这种东西总是比较棘手的。在动态模型里面，或者任何其他的图概率模型，都是可以用因子图表示的，而Psi在这里，表征的通常都是概率或者条件概率。 因子图和Psi函数表示法，在machine learning的paper中是比较常用的。 
　　参考资料：[http://www.cnblogs.com/549294286/archive/2013/06/06/3121454.html](http://www.cnblogs.com/549294286/archive/2013/06/06/3121454.html)
## 图例
　　第一个公式等价于下图： 
![这里写图片描述](https://img-blog.csdn.net/2018090415252161?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180904152512584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
　　下面就是隐马尔可夫模型的因子图： 
![这里写图片描述](https://img-blog.csdn.net/20180904152528946?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
