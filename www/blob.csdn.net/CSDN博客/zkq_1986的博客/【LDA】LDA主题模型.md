# 【LDA】LDA主题模型 - zkq_1986的博客 - CSDN博客





2017年12月23日 18:01:11[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：165








LDA是一个概率生成模型。认为文档是由词袋中的词按一定概率生成。对于语料集中的每篇文档，其生成过程是：

首先，从文档的所有主题分布中选取一个主题，这个过程服从所有主题的多项式分布。同时文档所有主题服从Dirichlet分布。

其次，从选取的主题中选取一个词，这个过程也是服从多项式分布。

重复上面的操作，这样一篇文档就生成了。再重复文档的生成过程，一个文档集也就生成。





LDA(Latent Dirichlet Allocation，隐含狄利克雷分布）是一个生成概率模型。其基本思想就是认为文档是由潜在的一些主题随机组合而成的，而每个主题又由词组成，即一个文档中可能包含多个主题，一个词也可能同时属于多个主题。LDA是一个三层贝叶斯模型，包含词、文档和主题三层结构。



![](https://img-blog.csdn.net/20181023112716509?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181023112855325?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181023112932436?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181023112955300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181023113016500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







