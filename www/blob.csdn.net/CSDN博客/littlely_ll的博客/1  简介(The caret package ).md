# 1. 简介(The caret package ) - littlely_ll的博客 - CSDN博客





2017年02月28日 13:26:16[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：2099
所属专栏：[R语言caret包应用](https://blog.csdn.net/column/details/29369.html)









**译者话：博主正在学习caret包，觉得这个包对于学机器学习的人来说是必不可少的，它具有统一的模式或流程对模型进行处理、训练。caret包参数众多，而且有的函数还依赖于其他的包，这对于学习有一定的困难，不过有一篇技术文档专门介绍了caret包的使用，包括预处理、可视化、模型训练和调参等，但是直到现在我也没见有中文版介绍caret包的。今日突发感想，想把[The caret package](https://topepo.github.io/caret/index.html)的文档翻译出来，这样即能把其中的程序走一遍，熟悉一下caret包的流程，也更能加深对caret包的理解,并且方便以后自己的阅读查看。由于博主是第一次翻译这么大的技术文档，这对于博主来说是一个很大的挑战，翻译之处可能有语句不通或一些错误的地方，欢迎大家批评指正！**

**caret包 作者：*Max Kuhn***

# 1. 简介

caret包（_C_lassification_A_nd_RE_gression_T_raining的缩写）是一系列函数的集合，它试图对创建预测模型的过程进行流程化。caret包包含了解决以下问题的工具： 

 * 数据分割 

 * 预处理 

 * 特征选择 

 * 用重抽样进行模型调参 

 * 变量重要性估计 

等还有其他一些功能。 

在R中还有很多建模函数，一些函数对建模训练和（或）预测有不同的语法。caret包提供了对函数统一的界面，还有对普通任务进行标准化（例如调参和变量重要性）。 

最新的版本能在[CRAN](https://cran.r-project.org/web/packages/caret/)上找到，工程放在了[github](https://github.com/topepo/caret)上。 

一些资源： 

 * 《应用预测建模》突出了caret包和超过40个其他的R包。在亚马逊或出版社的官网上有售。这是这本书的[网站](http://appliedpredictivemodeling.com/)。 

 * 在[Journal of Statistical Soteware](https://www.jstatsoft.org/article/view/v028i05)期刊上有关于caret包的文章。数据可以从这获得。 

 * 这是caret包在Youtube上的[在线会议](https://www.youtube.com/watch?v=7Jbb2ItbTC4)，是有Orange County R用户组的Ray DiGiacomoJr组织并录制。 

 * 在useR!2014,我受访并讨论了caret包和这本书。 

 * DataCamp有一个使用caret包的课程：[beginner’s tutorial on machine learning in R](https://www.datacamp.com/courses/machine-learning-toolbox)。 

你有问题、评论或建议可以email我。 

这些HTML网页由[bookdown](https://bookdown.org/yihui/bookdown/)制作。



