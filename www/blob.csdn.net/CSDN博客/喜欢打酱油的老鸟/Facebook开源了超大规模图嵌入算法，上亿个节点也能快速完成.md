
# Facebook开源了超大规模图嵌入算法，上亿个节点也能快速完成 - 喜欢打酱油的老鸟 - CSDN博客


2019年04月04日 08:44:04[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：150标签：[Facebook																](https://so.csdn.net/so/search/s.do?q=Facebook&t=blog)[图嵌入算法																](https://so.csdn.net/so/search/s.do?q=图嵌入算法&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Facebook&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6675562914365047300/](https://www.toutiao.com/a6675562914365047300/)
**图**，是很有用的数据结构，用**节点**(Node) 和**边**(Edge) 织成一张网。比如，知识图谱就是这样的网。
![Facebook开源了超大规模图嵌入算法，上亿个节点也能快速完成](http://p1.pstatp.com/large/pgc-image/4f46775c4796451aab5672247b9d530e)

处理这样的数据，要用到**图嵌入**(Graph Embedding) ：把高维的网络，转换成低维的向量。处理之后，**机器学习模型**才能轻易食用。
如果像上图这样，只有几个节点，十几条边，图嵌入没什么难度。
如果有**几十亿**个节点，**几万亿**条边呢？传统的图嵌入方法就捉急了。
但现在，Facebook**开源**了叫做**PyTorch-BigGraph**(简称**PBG**) 的新工具。
![Facebook开源了超大规模图嵌入算法，上亿个节点也能快速完成](http://p1.pstatp.com/large/pgc-image/c2f165febfac47228f0a773cf25d070a)

有了它，再大的图 (原文是arbitrarily large，任意大) 都能快速生成图嵌入。而且，**完全不需要GPU**。
开源模型除了可以自己训练之外，还有拿7,800万节点的Wikidata数据预训练过的模型，可以直接下载来用。
PBG发表之后，小伙伴们纷纷奔走相告：
![Facebook开源了超大规模图嵌入算法，上亿个节点也能快速完成](http://p1.pstatp.com/large/pgc-image/7e83e0dee3c4427486ad9f1cf409dc77)
LeCun老师还亲自转发了两次。
**如何养成**
PBG是一个**分布式**系统，用**1.2亿个节点**的完整**FreeBase**知识图谱来训练的。
![Facebook开源了超大规模图嵌入算法，上亿个节点也能快速完成](http://p3.pstatp.com/large/pgc-image/0c4556765f2144dd8c5e471bcd957908)

训练过程中，PBG会吃进图上所有**边**(Edge) 的大列表，每条边都是用它两端的节点来定义，一个是源 (Source) ，一个是目标 (Target) 。定义中也有两点之间的**关系**(Relation Type) 。
然后，PBG给每一个节点，输出一个特征向量 (就是嵌入) ，让两个相邻的节点在向量空间中离得近一些，让不相邻节点的离远一些。
这样一来，那些周围环境分布相近的节点，在向量空间里的位置也会彼此靠近，图原本要表达的意思就保留下来了。
另外，针对每种不同的关系，“近似度得分 (Proximity Score) ”都可以定制不同的计算方法。这样，一个节点的嵌入，就可以在不同种类的关系里共享了。
**快一点，再快一点**
要快速处理大规模的图数据，PBG用了这几个法术：
> 一是
> 图分区
> (Graph Partitioning) ，这样就不需要把整个模型加载到内存里了。在图嵌入质量不损失的情况下，比不分区时节省了88%的内存占用。二是一台机器进行
> 多线程计算
> 。三是在
> 多台机器
> 上同时跑，在图上各自跑一个不相邻的区域。四是
> 批次负采样
> (Batched Negative Sampling) ，能让一台CPU每秒处理100万条边，每条边100次负采样。
训练完成之后，在FB15k、Youtube、LiveJournal等等图谱上，都测试过。
团队说，PBG和大前辈们的图嵌入质量相当，但需要的时间明显缩短了。
**你也试一试吧**
你也去训练一个PBG吧。
反正，也不用GPU。
不想训练的话，还有用完整WikiData预训练的模型。
你需要的传送门，都在这里了。
代码传送门：
https://github.com/facebookresearch/PyTorch-BigGraph
食用索引：
https://torchbiggraph.readthedocs.io/en/latest/
论文传送门：
https://arxiv.org/abs/1903.12287
博客传送门：
https://ai.facebook.com/blog/open-sourcing-pytorch-biggraph-for-faster-embeddings-of-extremely-large-graphs/

