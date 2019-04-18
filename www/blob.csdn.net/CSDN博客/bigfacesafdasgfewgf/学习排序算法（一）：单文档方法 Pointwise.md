# 学习排序算法（一）：单文档方法 Pointwise - bigfacesafdasgfewgf - CSDN博客





2014年12月24日 10:15:11[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：3796








**学习排序算法（一）：单文档方法 Pointwise**



# 1. 基本思想




这种方法主要是将搜索结果的文档变为特征向量，然后将排序问题转化成了机器学习中的常规的分类问题，而且是个多类分类问题。




# 2. 方法流程




    Pointwise方法的主要流程如下：




![](https://img-blog.csdn.net/20141224101126651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





我们可以看出，整个流程分为三部分：提取特征做训练；分类算法学习；测试query排序。


    我们可以把ranking量化成多个标记，比如{perfect，Excellent，good，fair，bad}；



    然后每个文档对应一个分类标记，这样就成了标准的分类问题形式了（样本特征-类别标记）。网上给出了一个案例，如下：





![](https://img-blog.csdn.net/20141224101306912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    此时，我们就可以用机器学习中的分类算法来学习。



    如何对测试query的搜索结果排序呢？很简单，我们提取这些搜索结果文档的特征，然后对学习到的模型分类，分类之后按照标记进行排序~






# 3. 优缺点分析





- 属于同一类的文档之间无法排序；
- 完全从单文档的分类角度计算，没有考虑文档之间的相对顺序。



























