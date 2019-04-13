
# Distributional Vector VS. Distributed Vector - 我和我追逐的梦~~~ - CSDN博客


置顶2016年08月15日 12:03:15[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：8401标签：[自然语言处理																](https://so.csdn.net/so/search/s.do?q=自然语言处理&t=blog)[embedding																](https://so.csdn.net/so/search/s.do?q=embedding&t=blog)[distribute																](https://so.csdn.net/so/search/s.do?q=distribute&t=blog)[词向量																](https://so.csdn.net/so/search/s.do?q=词向量&t=blog)[网络节点向量																](https://so.csdn.net/so/search/s.do?q=网络节点向量&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=词向量&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=distribute&t=blog)个人分类：[数据挖掘与机器学习																](https://blog.csdn.net/heyongluoyao8/article/category/2220409)
[
																								](https://so.csdn.net/so/search/s.do?q=distribute&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=embedding&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=embedding&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=自然语言处理&t=blog)

### Distributional Vector VS. Distributed Vector
异同点
对于自然语言
相同点
都是基于distribution思想
使用相同的语料库
Distributional models(BOW,LSI,LDA)
共现在同一文本区域中词(如同一语句)相关,在语料中的共现语句越多,越相关
使用共现语句个数构建词与词(上下文)的PMI/PPMI矩阵(高维稀疏矩阵)，然后进行SVD得到每个词的低维稠密向量(隐向量)
Distributed models(NPLM,LBL,Word2vec,Glove)
在相同上下文中出现的词具有相关性,相同的上下文在语料库中越多,越相关,不要求同时出现
思想来源于深度学习(Inspired by deep learning), 使用预测代替共现计数
Example
A dog is in the room.
A cat is in the room.
dog 与 room 是 Distributional
dog 与 cat 是 Distributed
思想区别
Distributional思想
同一个语境中(上下文)出现的词相关。
它是一种横向思想，如句子“今晚的天空有很多星星”，“天空”与“星星”横向相关。
Distributed思想
相似的语境下(上下文)的词相关。
这里相似的语境可以是同一个句子，也可以是不同的语句(纵向)。
它包含一种纵向思想，如句子“今晚的天空有很多星星”，“今晚的天空有月亮”，“星星”与“月亮”纵向相关。
方法区别
Distributional使用隐矩阵分解(Implicit Matrix Factorization)
Distributed使用神经网络词嵌入(Neural word embedding)
Distributional使用共现计数来构建原始矩阵
Distributed通过神经网络来进行上下文词预测

对于关系网络
Distributional
在同一的路径中的点相关(取决于路径长度，最简单的便是只考虑1邻居)
如果使用词的共现构建网络，那么有边连接的点相关
或者使用图的邻接矩阵构建原始矩阵，然后进行矩阵分解
Distributed
不仅考虑邻接的点相关性
还考虑了拥有共同邻居的不直接相连的点的相关
即如果某两个点，其虽然不直接相连，但是它们拥有相同的邻居，或者相同的邻居点很多，那么它们也具有相似性
可以使用deepwalk来对图中的点来仿造语句(词序列)来构建图节点序列



