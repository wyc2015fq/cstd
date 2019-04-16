# 论文笔记：代码语义的表示（Neural Code Comprehension: A Learnable Representation of Code Semantics） - 彩虹糖的博客 - CSDN博客





2019年03月18日 20:09:28[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：29标签：[word2vec																[NLP																[代码语义](https://so.csdn.net/so/search/s.do?q=代码语义&t=blog)
个人分类：[NLP																[机器学习](https://blog.csdn.net/caozixuan98724/article/category/7173933)](https://blog.csdn.net/caozixuan98724/article/category/7560387)





# 论文概况

论文链接：[Neural Code Comprehension: A Learnable Representation of Code Semantics](http://papers.nips.cc/paper/7617-neural-code-comprehension-a-learnable-representation-of-code-semantics.pdf)

论文作者：Tal Ben-Nun, Alice Shoshana Jakobovits, Torsten Hoefler

论文来源： 32nd Conference on Neural Information Processing Systems (NeurIPS 2018), Montréal, Canada

# 论文摘要

word embedding方法在自然语言处理领域取得了巨大的成功，因此研究人员也考虑将这种方法用于代码分析。目前的研究大都直接把代码当成自然语言处理，但是因为代码本身具有的一些结构化的特性（函数声明，分支，不可更改的代码顺序等），使得目前的方法都不够健壮。在这篇论文中，作者提出了一种新的基于中间语言的代码表示方法，并用于多种任务中（代码分类，性能预测等），在不经过微调，只是用RNN的情况下都得到了很好的效果。

# 论文详情



### 一. 基本假设

在自然语言处理模型中，我们常常基于该假设：出现在相同上下文的不同词汇应该具有相似的含义。我们把这个假设推广到代码中，就得到了如下的假设：

出现在相同**上下文**的**代码语句**应该具有**相似的含义**。

接下来，我们对这个假设中的加粗部分再做进一步的说明。

**代码语句**：定义语句即所谓Statement，其实并不容易。我们希望我们选取的语句能在代码表达方面具有**普遍性**和**一致性**。对于普遍性，我们希望它不依赖于某一种语言，而是各种程序语言都可以使用的。对于一致性，我们希望每一个语句表达的东西应当是一致的，举一个不一致的例子：如果我们拿代码的一行作为语句的划分，一行代码可以只完成一个运算，但也可以定义一个类，甚至可以完成一个排序算法，这就造成了不一致。

**相似的含义**：我们认为，计算机的程序的作用可以分为两类-使用系统资源和以某种特定的方式改变系统状态。因此，我们对相似的定义即为两个代码语句使用相同的资源，或者用同样的方式改变系统状态。

**上下文**：对于代码语句的上下文，我们不能像自然语言一样，直接取前面几个单词，后面几个单词，要根据代码的特点来定义上下文，在这一方面已经有了一下工作，比如使用抽象语法树，数据流动图等方式来定义代码的上下文，具体可以看论文中的Related Work部分，在作者看来，代码语句的上下文必须体现数据的依赖性和执行的依赖性。

![](https://img-blog.csdnimg.cn/20190318200121895.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

### 二. 代码语义表达算法inst2vec

1. 使用LLVM，把源语言程序转化为LLVM中间语言的代码，LLVM的Statement格式如下：

![](https://img-blog.csdnimg.cn/20190318200138660.png)

2. 构建上下文流动图

在前文的分析中，作者指出了之前使用的流动图的方法确定代码上下文方面存在的不足，因此作者使用LLVM生成的中间语言，提出了一套生成上下文流动图的方法。

![](https://img-blog.csdnimg.cn/20190318200151784.png)

3. 将代码语句映射到向量空间

首先，作者将中间语言代码进行预处理，去除了注释等，将变量和常量用统一的符号替换。

![](https://img-blog.csdnimg.cn/20190318200209906.png)

之后，作者选取了有代表性的开源项目作为训练集。

![](https://img-blog.csdnimg.cn/2019031820022298.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

最后，作者使用了Word2vec使用的skip-gram model算法进行训练，得到了类似word2vec的inst2vec。

关于skip-gram的内容，可以参考这篇文章：[Skip-Gram模型理解](https://www.jianshu.com/p/da235893e4a5)。

### 对论文的理解

我认为，这篇论文的创新点主要集中在两点：1.使用LLVM生成的中间语言，使得代码转化成向量的过程不依赖特定的语言。2. 上下文的处理充分考虑到了代码在数据和执行方面的相关性。

比较疑惑的一点是作者完全排除了注释和变量名称对代码语义的理解的帮助性。可以说，根据变量名去理解语义正是作者批判的观点，但是我觉得把代码变成中间语言一个很大的问题就是丢失了这一部分信息。可以说，这种程序人理解起来依然很困难，不理解算法是如何理解这些代码中蕴含的语义的。

### 实验部分

待续](https://so.csdn.net/so/search/s.do?q=NLP&t=blog)](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)




