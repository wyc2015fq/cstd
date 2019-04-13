
# 文本adversarial examples - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[文本adversarial examples](https://www.cnblogs.com/eniac1946/p/8615636.html)
|Posted on|2018-03-21 10:45|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8615636)|[收藏](#)
|对文本对抗性样本的研究极少，近期论文归纳如下：
|文本对抗三个难点：
|text data是离散数据，multimedia data是连续数据，样本空间不一样；
|对text data的改动可能导致数据不合法；
|基于word的改动（替换、增、删）会有语义兼容性的问题；
|论文：
|[Deep Text Classification Can be Fooled](http://link.zhihu.com/?target=https%3A//arxiv.org/abs/1704.08006)|和|[Towards Crafting Text Adversarial Samples：](http://link.zhihu.com/?target=https%3A//arxiv.org/abs/1707.02812)
|针对文本分类生成对抗样本——对输入文本进行增删改处理，使得文本分类出现分类错误
|两篇文章都提出：用梯度来度量word对分类的影响程度；
![](https://images2018.cnblogs.com/blog/1181483/201803/1181483-20180321103051727-84712242.png)
|第二篇文章还提出可以用后验概率来计算对分类的影响，不过这种方法计算每个word会很耗时；![](https://images2018.cnblogs.com/blog/1181483/201803/1181483-20180321102626278-141248338.png)
|产生同义词候选集用到的词向量，需要经过后处理，在这篇文章中提出：![](https://images2018.cnblogs.com/blog/1181483/201803/1181483-20180321104247823-1571066778.png)
|可以在后处理过的词向量空间中采用KNN等算法，找到N个最接近的同义词
![](https://images2018.cnblogs.com/blog/1181483/201803/1181483-20180321104007960-1252745961.png)
|[Adversarial Examples for Evaluating Reading Comprehension Systems](http://link.zhihu.com/?target=https%3A//arxiv.org/abs/1707.07328)|：
|针对QA系统生成对抗样本——对原文paragraph增添句子，让QA系统回答错误







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
