
# 一句话总结LDA - 喜欢打酱油的老鸟 - CSDN博客


2018年09月19日 12:52:58[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：67


一句话总结LDA
核心：向最大化类间差异、最小化类内差异的方向线性投影。
线性鉴别分析的基本思想是通过线性投影来最小化同类样本间的差异，最大化不同类样本间的差异。具体做法是寻找一个向低维空间的投影矩阵W，样本的特征向量x经过投影之后得到的新向量：
y = Wx
同一类样投影后的结果向量差异尽可能小，不同类的样本差异尽可能大。
直观来看，就是经过这个投影之后同一类的样本进来聚集在一起，不同类的样本尽可能离得远。下图是这种投影的示意图：
![](https://img-blog.csdn.net/20180919125158696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上图中特征向量是二维的，我们向一维空间即直线投影，投影后这些点位于直线上。在上面的图中有两类样本，通过向右上方的直线投影，两类样本被有效的分开了。绿色的样本投影之后位于直线的下半部分，红色的样本投影之后位于直线的上半部分。
训练时的优化目标是类间差异与类内差异的比值：
![](https://img-blog.csdn.net/20180919125213745?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后归结于求解矩阵的特征值与特征向量：
![](https://img-blog.csdn.net/20180919125235540?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
LDA是有监督的机器学习算法，在计算过程中利用了样本标签值。这是一种判别模型，也是线性模型。LDA也不能直接用于分类和回归问题，要对降维后的向量进行分类还需要借助其他算法，如kNN。


