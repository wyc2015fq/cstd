# 损失函数：Hinge Loss（max margin） - 数据之美的博客 - CSDN博客
2017年09月19日 20:08:15[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2284
目录[(?)](http://blog.csdn.net/luo123n/article/details/48878759#)[[-]](http://blog.csdn.net/luo123n/article/details/48878759#)
- [Hinge Loss简介](http://blog.csdn.net/luo123n/article/details/48878759#t0)
- [变种](http://blog.csdn.net/luo123n/article/details/48878759#t1)
# Hinge Loss简介
Hinge Loss是一种目标函数（或者说损失函数）的名称，有的时候又叫做max-margin objective。其最著名的应用是作为SVM的目标函数。
其二分类情况下，公式如下： 
其中，y是预测值（-1到1之间），t为目标值（1）。
其含义为，y的值在-1到1之间就可以了，并不鼓励，即并不鼓励分类器过度自信，让某个可以正确分类的样本距离分割线的距离超过1并不会有任何奖励。从而使得分类器可以更专注整体的分类误差。
# 变种
实际应用中，一方面很多时候我们的y的值域并不是[-1,1]，比如我们可能更希望y更接近于一个概率，即其值域最好是[0,1]。另一方面，很多时候我们希望训练的是两个样本之间的相似关系，而非样本的整体分类，所以很多时候我们会用下面的公式： 
[Math Processing Error]
其中，y是正样本的得分，y’是负样本的得分，m是margin（自己选一个数）
即我们希望正样本分数越高越好，负样本分数越低越好，但二者得分之差最多到m就足够了，差距增大并不会有任何奖励。
比如，我们想训练词向量，我们希望经常同时出现的词，他们的向量内积越大越好；不经常同时出现的词，他们的向量内积越小越好。则我们的hinge loss function可以是： 
其中，w是当前正在处理的词，是w在文中前3个词和后3个词中的某一个词，是随机选的一个词。
[](http://blog.csdn.net/luo123n/article/details/48878759#)[](http://blog.csdn.net/luo123n/article/details/48878759#)[](http://blog.csdn.net/luo123n/article/details/48878759#)[](http://blog.csdn.net/luo123n/article/details/48878759#)[](http://blog.csdn.net/luo123n/article/details/48878759#)[](http://blog.csdn.net/luo123n/article/details/48878759#)
- 
顶
4
