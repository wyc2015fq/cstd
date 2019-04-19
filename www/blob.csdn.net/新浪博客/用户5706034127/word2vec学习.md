# word2vec学习_用户5706034127_新浪博客
||分类：[自然语言处理](http://blog.sina.com.cn/s/articlelist_5706034127_3_1.html)|
[http://blog.csdn.net/itplus/article/details/37969519](http://blog.csdn.net/itplus/article/details/37969519)
[http://www.cnblogs.com/neopenx/p/4571996.html](http://www.cnblogs.com/neopenx/p/4571996.html)
[http://blog.csdn.net/mytestmy/article/details/26969149](http://blog.csdn.net/mytestmy/article/details/26969149)
总结下思路：
目标是求单词的向量的表示。
相对于以往的是根据统计信息直接定义每个分量的含义，
这里是通过构造评价函数，也就是NNLM来评价单词的向量表示，来学习得到单词的向量表示。
1，需要注意的是，普通的神经网络模型，输入和输出标记都是已知的，模型需要学习的是模型的参数。
这里模型的输入和参数都是未知的，只有输出标记是已知的。
或者说输入也是已知的，只不过单词到单词向量映射的矩阵是未知的，也就是映射层的参数是未知的。
2，普通的神经网络至少有一层隐含节点，而这里的,却没有，直接输入到输出，所以个人认为都谈不上是NNLM了。
3，输入输出，有分为两种，一种是，根据上下文预测单词CBOW，另一种是根据单词预测上下文Skip-gram，
4，目标评价函数有两种，一种最大化正样本的概率([](http://blog.csdn.net/itplus/article/details/37969979)Hierarchical
Softmax)，另一种是最大化正样本概率同时最小化负样本概率（Negative Sampling）。
5，参数估计，采用的是类似EM的算法，因为有两部分参数，一部分是单词的向量，另一部分是模型的参数。固定一部分，计算另一部分梯度进行更新，在反过来计算另一部分的梯度进行更新。
6，两点疑惑，一个是Hierarchical
Softmax中树的构建，随意对单词进行分类合适么？会不会从某种程度上来说，两个单词是不兼容的，然后被分到同一个子树里面了，分堆就不需要一定的标准么。另一个是，为什么Hierarchical
Softmax中用softmax来做归一化，保证概率和为1，而Negative
Sampling不用保证概率和唯一，仅仅使用了简单的logistic？或者说，为什么之前为什么要用softmax，而不是直接用logistic？
7，之后在看看源码，对源码进行注释解析。
![word2vec学习](http://s7.sinaimg.cn/mw690/006e9W8vzy7a4pJOJ1k26&690)
