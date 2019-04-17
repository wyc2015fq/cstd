# 早发arXiv可多获得65%的引用，但…… - 知乎
# 



> 原作 Sergey Feldman
Root 编译自 Semanticscholar
量子位 出品 | 公众号 QbitAI

现在都流行投顶会或杂志之前，先把文章放arXiv上了。

对研究人员来说，这么做主要有两个好处。一方面尽早占坑，另一方面可以绕过漫长的同行评议时间，加速圈内人交流工作进展。

但是今天有个圈内的人，Sergey Feldman，认为这样不好。
![](https://pic1.zhimg.com/v2-1d0ed5708e98192631352eca7a6c6da0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='361' height='361'></svg>)Sergey Feldman，大数据和机器学习顾问
 Feldman发了一篇文章The Association Between Early ArXiv Posting and Citations在arXiv，在文章里他用模型论证了，论文如果在会议接收之前发，会比接受后发多获得65%的引用次数。

他观察到，在arXiv上早发这个现象会促使大家急于立Flag，进而把文章的质量放在第二位。另外，当作者的h指数很高的话（42~49），作者本身影响力会进一步助推文章的引用次数。这样大家只是被名人效应吸引才关注这篇文章所做的工作，而不是因为文章质量本身。

那学术圈里的马太效应要怎么破？
![](https://pic2.zhimg.com/v2-01fe1380676194bbd481bfdd8949a68d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='322'></svg>)
Feldman建议，**以后arXiv文章提交时设置一个匿名时限。**这样可以让学者们只以工作的质量论英雄。

简单介绍一下Feldman的统计条件和评估标准。

Feldman研究的论文对象得符合两个条件：

1）最后被会议或杂志接收的论文；

2）有投放在arXiv上的，接收前接收后都行。

另外，会议必须要是重量级的，只考虑被AAAI、ACL、CVPR、ECCV、EMNLP、FOCS、HLTNAACL、ICCV、ICML、ICRA、IJCAI、INFOCOM、KDD、NIPS、SODA、WWW接收的论文。

还有论文发表时间是限定在2007年到2016年之间，引用次数只考虑会议中稿当年的数据。

符合这些条件的论文，一共有4392篇。
![](https://pic2.zhimg.com/v2-d30074d59279032f8574894675516b15_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='946' height='531'></svg>) 16个顶会分别符合统计标准的论文数
单一的评价尺度可能会有失偏颇。Feldman特定制定了两个不同的评估标准。

一个是看这4千多篇论文在中稿当年的总引用次数。另外一个是排除了作者在以后工作的自引用情况，而且引用的重要程度要很高，在在同一篇文章里，该篇被统计的文章被引用次数要达到3次及以上，才算数。

Feldman建模分析后发现，按第一个标准，接收前发比接收后发的引用次数要多65%。按第二个标准，早发比晚发的引用次数要多75%。

这个数字可以说是相当震惊了。
![](https://pic3.zhimg.com/v2-482e8c40779d81c3663e7a7f54ae3b46_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='600'></svg>)
分析完之后，Feldman解释了一下他做这个研究的目的。

他主要是想看，早发晚发对引用量的影响，也一带研究上作者知名度、文章被接受的会议类型、提前多早发这些变量对引用次数的影响。


他也坦承，这次统计也还有可以改善的部分。比如应该补充变量，像是文章作者所属的研究机构或学校，以及文章质量本身。

最后，为了学术圈更纯粹一些，他建议arXiv开设作者匿名时限就像ICLR 2018的OpenReview平台那样。

想详细地了解统计的模型，可前往：
[https://arxiv.org/abs/1805.05238](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1805.05238)

以及，附上原文地址：
[https://blog.semanticscholar.org/the-association-between-early-arxiv-posting-and-citations-72034f0914b2](https://link.zhihu.com/?target=https%3A//blog.semanticscholar.org/the-association-between-early-arxiv-posting-and-citations-72034f0914b2)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


