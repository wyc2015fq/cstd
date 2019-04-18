# Probability Latent Semantic Analysis (PLSA) 模型 学习笔记 - bigfacesafdasgfewgf - CSDN博客





2014年12月04日 10:00:09[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：6932标签：[PLSA																[主题模型																[EM算法																[NLP																[语义](https://so.csdn.net/so/search/s.do?q=语义&t=blog)
个人分类：[NLP](https://blog.csdn.net/puqutogether/article/category/2595129)





**Probability Latent Semantic Analysis (PLSA) 模型 学习笔记**



    PLSA是前面LSA的兄弟版，相比于LSA而言，PLSA定义了概率模型，而且每个变量以及相应的概率分布和条件概率分布都有明确的物理解释了。这篇博文我们分三部分来说PLSA：基本思想，EM算法推导，以及优缺点分析。




# 1.  PLSA的基本思想




    PLSA是一种主题模型topic model，是针对文本中隐含的主题来建模的方法。PLSA就是给定了文档d之后，需要以一定的概率选择与文档相对应的主题z，然后再从主题z中以一定的概率选择单词w。我们用下图来形象说明：

![](https://img-blog.csdn.net/20141204100833129?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





中间的那一层就是PLSA引入的“主题层”。其实从上面的介绍中你就可以发现，PLSA是一种混合模型，需要使用两层概率（上面中两处红色标记）对整个样本空间建模。下面的图更加抽象地描述了PLSA的模型：

![](https://img-blog.csdn.net/20141204101250374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    我们继续使用一个例子来通俗理解PLSA的基本思想以及它的应用：

    想象某个人要写N篇文档，他需要确定每篇文档里每个位置上的词。假定他一共有K个可选的主题，有V个可选的词项，所以，他制作了K个V面的 “主题-词项” 骰子，每个骰子对应一个主题，骰子每一面对应要选择的词项。然后，每写一篇文档会再制作一颗K面的 ”文档-主题“ 骰子；每写一个词，先扔该骰子选择主题；得到主题的结果后，使用和主题结果对应的那颗”主题-词项“骰子，扔该骰子选择要写的词。他不停的重复如上两个扔骰子步骤，最终完成了这篇文档。重复该方法N次，则写完所有的文档。在这个过程中，我们并未关注词和词之间的出现顺序，所以PLSA也是一种词袋方法；并且我们使用两层概率分布对整个样本空间建模，所以PLSA也是一种混合模型。


而真实的PLSA方法两个“骰子”可能就不是均匀的，因为每个主题的概率不一定一样，主题下每个词的概率也不一定一样。在PLSA模型中，我们需要做的就是如何求出这两个概率。




# 2. EM算法推导PLSA




    这部分网上讲解的太多了，我选择一个比较好懂，参考过来。（powered by Xinyan Lu）




![](https://img-blog.csdn.net/20141204104212312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








![](https://img-blog.csdn.net/20141204104309633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








# 3. PLSA的优缺点



**  优点：**PLSA可以解决了同义词和多义词的问题，利用了强化的期望最大化算法（EM）来训练隐含类（潜在类）。而且相对了LSA，有了坚实的统计学基础。




**    缺点：**随着document和term 个数的增加，pLSA模型也线性增加，变得越来越庞大，也就是说PLSA中训练参数的值会随着文档的数目线性递增。还有，PLSA可以生成其所在数据集的的文档的模型，但却不能生成新文档的模型。](https://so.csdn.net/so/search/s.do?q=NLP&t=blog)](https://so.csdn.net/so/search/s.do?q=EM算法&t=blog)](https://so.csdn.net/so/search/s.do?q=主题模型&t=blog)](https://so.csdn.net/so/search/s.do?q=PLSA&t=blog)




