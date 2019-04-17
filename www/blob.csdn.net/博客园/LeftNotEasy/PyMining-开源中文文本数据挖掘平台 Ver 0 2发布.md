# PyMining-开源中文文本数据挖掘平台 Ver 0.2发布 - LeftNotEasy - 博客园







# [PyMining-开源中文文本数据挖掘平台 Ver 0.2发布](https://www.cnblogs.com/LeftNotEasy/archive/2011/05/29/2062293.html)





**项目首页：**

[http://code.google.com/p/python-data-mining-platform/](http://code.google.com/p/python-data-mining-platform/) （可能需FQ）

目前比如tutorial, install, release等内容，已经更新到项目首页的wiki中



**项目介绍：**

这是一个能够根据源数据（比如说用csv格式表示的矩阵，或者中文文档）使用多种多样的算法去得到结果的一个平台。

算法能够通过xml配置文件去一个一个的运行，比如在开始的时候，我们可以先运行一下主成分分析算法去做特种选择，然后我们再运行随机森林算法来做分类。

目前算法主要是针对那些单机能够完成的任务，该架构良好的扩展性能够让你在很短的时间内完成自己想要的算法，并且用于工程之中（相信我，肯定比Weka更快更好）。**该项目的另一个特色是能够很好的支持中文文本的分类、聚类**等操作。

**下图是使用PyMining主成分分析（PCA）算法将一个数千维的数据投影到2维平面的结果**，也是PyMining中的一个例子，位于example/pca_matplot_example.py。原始数据为文本，每种颜色代表不同分类的文本，可以看出，虽然维度变成了2维，但是不同分类的文本投影后的结果还是有一定的区分度的。

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/201105/201105292104532267.png)**重要：**目前项目已经集成了Scipy与Matplotlib，目前PCA调用了Scipy，上面的例子调用了Matplotlib。在Ubuntu下，Scipy的安装可以参考我的另一篇文章：[Scipy在Ubuntu上的安装](http://www.cnblogs.com/LeftNotEasy/archive/2011/05/29/2062324.html)，Matplotlib的可以使用sudo apt-get install python-matplotlib。**对于其他平台，只有自己去搜索配置一下**，可以见项目主页中wiki下面的installing页面。



**ChangeLog：**

**     重大改变：**
- 更新项目的组织方式，让package的组织、example的组织等等更合理一点

**     新增Feature：**
- 加入了K-Means算法，能够对文本进行聚类
- 加入了基于补集的朴素贝叶斯算法，大大提升了分类的准确率，目前该算法在搜狗实验室文本分类数据中，对20000篇、8分类左右的数据的预测准确率在90%左右
- 加入了Sogou实验室文本分类数据的导入器，可以进行更多的实验

**     Fix Bugs：**
- 修正了chi-square在example调用中的一个错误



**获取PyMining：**

     于[此处](http://code.google.com/p/python-data-mining-platform/downloads/detail?name=py_mining_0_2_D.zip&can=2&q=#makechanges)，可获取目前最新的0.2版












