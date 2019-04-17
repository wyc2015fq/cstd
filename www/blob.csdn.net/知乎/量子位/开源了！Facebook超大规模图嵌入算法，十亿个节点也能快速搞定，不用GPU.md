# 开源了！Facebook超大规模图嵌入算法，十亿个节点也能快速搞定，不用GPU - 知乎
# 



> 方栗子 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

**图**，是很有用的数据结构，用**节点** (Node) 和**边** (Edge) 织成一张网。**知识图谱**就是这样的网。
![](https://pic1.zhimg.com/v2-70e22803f5c38a6ccb1008a787aaf1f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='770' height='308'></svg>)
处理这类数据，要用到**图嵌入** (Graph Embedding) ：把高维的网络，转换成低维的向量。加工之后之后，**机器学习模型**才能轻易食用。

如果像上图这样，只有几个节点，十几条边，图嵌入没什么难度。

如果有**几十亿**个节点，**几万亿**条边呢？传统的图嵌入方法就捉急了。

但现在，Facebook**开源**了叫做**PyTorch-BigGraph** (简称**PBG**) 的新工具。
![](https://pic4.zhimg.com/v2-80c786f92be1be7ca5e1020674fd46ff_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='708' height='140'></svg>)
有了它，再大的图 (原文是arbitrarily large，任意大) 都能快速生成图嵌入。而且，**完全不需要GPU**。

开源模型除了可以拿去训练之外，还有拿**7,800万节点**的Wikidata图数据**预训练**过的模型，可以直接下载来用。

PBG发表之后，小伙伴们纷纷奔走相告：
![](https://pic1.zhimg.com/v2-5add3c31e92d01b08b01e25ca3f0e5d4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1146' height='562'></svg>)
LeCun老师还转发了两次。

## **如何养成**

PBG是一个**分布式**系统，用**1.2亿个节点**的完整**FreeBase**知识图谱来训练的。
![](https://pic3.zhimg.com/v2-44f571164038d33cffb814c87262834e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='384'></svg>)
训练过程中，PBG会吃进图中所有**边** (Edge) 的大列表，每条边都是用它两端的节点来定义，一个是源 (Source) ，一个是目标 (Target) 。定义中也有两点之间的**关系** (Relation Type) 。

然后，PBG给每一个节点，输出一个特征向量 (就是嵌入) ，让两个相邻的节点在向量空间中离得近一些，让不相邻节点的离远一些。

这样一来，那些周围环境分布相近的节点，在向量空间里的位置也会彼此靠近，图原本要表达的意思就保留下来了。

另外，针对每种不同的关系，“近似度得分 (Proximity Score) ”都可以定制不同的计算方法。这样，一个节点的嵌入，就可以在不同种类的关系里共享了。
![](https://pic4.zhimg.com/v2-0ce8bec2543d81ba65eefd309f0f0c5b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)
### **快一点，再快一点**

要快速处理大规模的图数据，PBG用了几个法术：

> 一是**图分区** (Graph Partitioning) ，这样就不需要把整个模型加载到内存里了。在图嵌入质量不损失的情况下，比不分区时节省了88%的内存占用。

二是一台机器进行**多线程计算**。

三是在**多台机器**上同时跑，在图上各自跑一个不相邻的区域。

四是**批次负采样** (Batched Negative Sampling) ，能让一台CPU每秒处理100万条边，每条边100次负采样。

训练完成之后，在FB15k、Youtube、LiveJournal等等图谱上，都测试过。

团队说，PBG和大前辈们的图嵌入质量相当，但需要的时间明显缩短了。

## **你也试一试**

你也去训练一只PBG吧。

反正，也不用GPU。

不想训练的话，还有用完整WikiData预训练的模型。

你需要的传送门，都在这里了。

代码传送门：
[https://github.com/facebookresearch/PyTorch-BigGraph](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/PyTorch-BigGraph)

食用索引：
[https://torchbiggraph.readthedocs.io/en/latest/](https://link.zhihu.com/?target=https%3A//torchbiggraph.readthedocs.io/en/latest/)

论文传送门：
[https://arxiv.org/abs/1903.12287](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1903.12287)

博客传送门：
[https://ai.facebook.com/blog/open-sourcing-pytorch-biggraph-for-faster-embeddings-of-extremely-large-graphs/](https://link.zhihu.com/?target=https%3A//ai.facebook.com/blog/open-sourcing-pytorch-biggraph-for-faster-embeddings-of-extremely-large-graphs/)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


