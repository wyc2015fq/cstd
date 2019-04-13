
# 阿里舆情︱舆情热词分析架构简述（Demo学习） - 素质云笔记-Recorder... - CSDN博客

2017年02月03日 17:14:31[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5510


本节来源于阿里云栖社区，同时正在开发一个舆情平台，其中他们发布了一篇他们所做的分析流程，感觉可以作为案例来学习。文章来源：[觉民cloud/云栖社区](http://mp.weixin.qq.com/s?__biz=MzI0NTE4NjA0OQ==&mid=2658354242&idx=2&sn=5272d12d6c5393003a2b0450d7f7ee2d&chksm=f2d58f2cc5a2063ab87fe27f3a541d2b18d3b52f8dad063a140cf3ea2ba6848ae31c39a2ed70&mpshare=1&scene=1&srcid=0124PGKjA7EhHj2EnM2R3sHI#rd)
平台试用链接：[https://prophet.data.aliyun.com/hotword?spm=5176.100239.blogcont68813.6.QB2AAc&isDemo=true](https://prophet.data.aliyun.com/hotword?spm=5176.100239.blogcont68813.6.QB2AAc&isDemo=true)
![这里写图片描述](https://img-blog.csdn.net/20170203170339804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一般热词分析历经：分词、关键词提取、词关联计算、热度计算
# 一、分词
主要是词包大法，你懂得，阿里词包都不用自己写，本身一大堆啊！！！厉害了word哥：公众趋势分析背后有百万级的人名、品牌、地址、组织机构名、商品、品牌词库等做支撑。
万能词包啊！！！
.
.
# 二、关键词提取
> 如何在一篇长文本中挑出关键词呢？ 在一定程度也就是等于找
> 词权重
> ，一种衡量一个句子里面词语重要性指标，其他方法可见博客：

> [NLP︱句子级、词语级以及句子-词语之间相似性（相关名称：文档特征、词特征、词权重）](http://blog.csdn.net/sinat_26917383/article/details/52174672)

> 那么，该舆情平台使用的是：TextRank算法为文本生成关键词。

> TextRank的算法思想来源于PageRank，旨在通过文本中句子、词之间的相互投票，为句子、词进行权重的排序。PageRank假设一个网页的入链越多，则其权重越高。随机地为每个网页分配一个初始权重，在每一轮投票中，每个网页将其权重均匀地分配给其出链，收敛后（平稳马尔科夫过程）每个网页得到的权重值反映了其重要性，每轮投票的数学表述为：
![这里写图片描述](https://img-blog.csdn.net/20170203170920062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170203170920062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 其中d为阻尼系数，（1-d）/N表示每次页面转移时有一定的概率会从全网随机选择url，这样可以避免没有外链的悬挂网页让所有权重收敛到0。

> [
](https://img-blog.csdn.net/20170203170920062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> PageRank通过页面之间的链接关系建立投票机制，TextRank以此为启发，通过词之间的邻近关系建立词权重投票机制，即假如两个词出现在同一个窗口中，则它们之间产生一次权重投票，这样可以通过PageRank的求解方法，计算每个词在文本中的权重。得到权重的排序之后，就可以挑选topN词作为关键词了。
![这里写图片描述](https://img-blog.csdn.net/20170203170925781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> .

> 三、词关联计算

> 词关联计算也可以使用社交网络的方式（点出度、点入度等，参考：
> [R语言︱SNA-社会关系网络 ](http://blog.csdn.net/sinat_26917383/article/details/51436643)

> R语言实现专题（基础篇）（一）
> [），也可以使用一些统计学的方式（互信息， ](http://blog.csdn.net/sinat_26917383/article/details/51436643)

> [NLP︱句子级、词语级以及句子-词语之间相似性（相关名称：文档特征、词特征、词权重）](http://blog.csdn.net/sinat_26917383/article/details/52174672)
> ）。

> [
](http://blog.csdn.net/sinat_26917383/article/details/51436643)
> 该平台使用的是互信息.

> 词关联使用点互信息PMI（pointwise mutual information）来表示，用信息论的语言来表述，点互信息衡量的是“给定一个随机变量后，另一个随机变量不确定性的减少程度”。假设有两个词x和y，则x和y之间的点互信息由下述公式表示：
![这里写图片描述](https://img-blog.csdn.net/20170203171221686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170203171221686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 其中p(x,y)表示x和y同时出现的概率，p(x)和p(y)分别表示x和y单独出现的概率。简单粗暴地理解，就是说相对于单独出现，某两个词更喜欢一起出现，则它们之间的关联程度越高。

> [
](https://img-blog.csdn.net/20170203171221686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> .

> 四、词热度计算

> 词的热度计算不能仅仅统计这个词在所有文章中的出现次数，因为每篇文章的热门程度不一样，汪峰上了头条时的报道，对于热度的计算不能跟一般的小道消息同日而语。热词分析在计算热度时，会用文章的热度对词进行加权，而文章的热度会综合考虑以下因素：

> 文章的转发量

> 浏览量

> 评论量

> 文章发布的时间，如果发布时间越长，则热度衰减地越高

> 具体的计算公式就不在这公开了,那是我们经过无数次的调试之后的结果，有兴趣的读者也可以了解一下这篇文章:
> [http://www.smartinsights.com/social-media-marketing/facebook-marketing/how-to-use-facebook-edgerank/?spm=5176.100239.blogcont68813.7.0MuQOh](http://www.smartinsights.com/social-media-marketing/facebook-marketing/how-to-use-facebook-edgerank/?spm=5176.100239.blogcont68813.7.0MuQOh)


