
# 开源SVM库libSVM介绍 - forever1dreamsxx--NLP - CSDN博客


2012年11月30日 11:08:39[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：906个人分类：[自然语言处理																](https://blog.csdn.net/forever1dreamsxx/article/category/1180977)[数学模型																](https://blog.csdn.net/forever1dreamsxx/article/category/1240651)[
							](https://blog.csdn.net/forever1dreamsxx/article/category/1180977)


原文地址：[http://blog.csdn.net/carson2005/article/details/6527055](http://blog.csdn.net/carson2005/article/details/6527055)
[计算机视觉小菜鸟的专栏](/carson2005)
libSVM是台湾大学林智仁教授等研究人员开发的一个用于支持向量机分类，回归分析及分布估计的c/c++开源库。另外，它也可以用于解决多类分类问题。 libSVM最新的版本是2011年4月发布的3.1版。林智仁教授设计开发该SVM库的目的是为了让其它非专业人士可以更加方便快捷的使用SVM这个统计学习工具。libSVM提供了一些简单易用的接口，从而使得用户可以方便的使用，而不必关心其内部复杂的数学模型和运算过程。libSVM的主要特点有：
（1）各种SVM的表达公式；
（2）有效的多类分类能力；
（3）交叉验证功能；
（4）各种核函数，包括预先计算得到的核矩阵；
（5）用于非平衡数据的加权svm；
（6）提供c++和java源代码；
（7）用于演示SVM分类与回归能力的GUI界面；
.....
很多初学者往往按照以下的步骤使用libSVM：
（1）将数据转换到libSVM指定的格式；
（2）随机选择一个核函数和一些参数；
（3）测试；
这种方法虽然可行，但却不一定能很快达到好的效果。为此，林智仁教授推荐按照以下的步骤来使用libSVM：
（1）将数据转换到libSVM指定的格式；
（2）对数据进行尺度操作（一般指数据的归一化）；
（3）考虑RBF（径向基）核函数；
（4）利用交叉验证来得到最好的参数C和r;
（5）用最好的C和r来训练所有训练集合；
（6）测试；
之所以推荐首选径向基核函数，是由于该核可以将数据非线性地映射到高维空间，而且，它还能处理那种特征（数据）及其属性之间呈现非线性关系的情况，而线性核函数只是径向基核函数的一个特例。另外，相比而言，多项式核函数在高维空间有着更多的参数，从而使得模型更加复杂。同时，需要提醒的是，径向基核函数并非万能的，尤其当特征数据的数值本身比较大的时候，线性核函数要更实用一些。
任何人可以在http://www.csie.ntu.edu.tw/~cjlin/libsvm 来下载libSVM开源库。不过，按照开发者的要求，在使用之前，请务必阅读其copyright，并按照其要求进行相应的引用和说明。另外，在使用之前，强烈推荐大家阅读libSVM.zip里面的readme文件。该文件详细描述了libSVM的使用方法及注意事项。

