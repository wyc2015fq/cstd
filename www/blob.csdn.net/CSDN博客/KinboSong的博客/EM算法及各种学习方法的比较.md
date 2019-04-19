# EM算法及各种学习方法的比较 - KinboSong的博客 - CSDN博客
2017年03月10日 09:34:49[songjinbo3](https://me.csdn.net/KinboSong)阅读数：329
**1、判别学习模型、生成学习模型、贝叶斯统计与规则化、EM算法的区别与联系**
针对这几种学习方法经常混淆的情况，现汇总一下这4中方法。
![](https://img-blog.csdn.net/20170310104628962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**判别学习模型、生成学习模型、EM模型共同点**：theta为固定值，采用最大似然估计。
**贝叶斯统计与正则化：**theta为随机值，最终演化为MAP。
**判别学习模型和生成学习模型：**判别学习模型根本上是假设采样点xi的分布与参数theta相互独立；而生成学习模型假设yi的分布于theta有关；同时，我认为生成学习模型不一定 局限在![](https://img-blog.csdn.net/20170310144551480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)上，还可以变形为：![](https://img-blog.csdn.net/20170310144715589?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。
**注意：EM方法实际上求的是联合概率，属于生成学习算法。**
**2、EM算法：**
参考：[http://blog.sina.com.cn/s/blog_6f3325fa01019j5e.html](http://blog.sina.com.cn/s/blog_6f3325fa01019j5e.html)  （讲的非常好）
[http://http://blog.csdn.net/zouxy09/article/details/8537620/](http://http//blog.csdn.net/zouxy09/article/details/8537620/)
![](https://img-blog.csdn.net/20170310150304112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
EM算法由两步组成：E步和M步。E步估计隐藏变量的分布，M步对参数做最大似然估计。
     EM算法的具体实现分为三种：高斯混合模型（Gaussian Mixture Model）、混合朴素贝叶斯模型、因子分析模型（Factor analysis）。
