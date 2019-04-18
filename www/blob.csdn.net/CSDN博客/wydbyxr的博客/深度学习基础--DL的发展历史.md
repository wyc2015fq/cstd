# 深度学习基础--DL的发展历史 - wydbyxr的博客 - CSDN博客
2018年11月02日 10:20:54[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：36
个人分类：[深度学习基础](https://blog.csdn.net/wydbyxr/article/category/6829999)
# DL的发展历史
  1）从ML到DL
  在Machine Learning时代，复杂一点的分类问题效果就不好了，Deep Learning深度学习的出现基本上解决了一对一映射的问题，比如说图像分类，一个输入对一个输出，因此出现了AlexNet这样的里程碑式的成果。
  2）从DL到deep reinforcement learning
  但如果输出对下一个输入还有影响呢？也就是sequential decision making的问题（有关序列的问题），单一的深度学习就解决不了了，这个时候Reinforcement Learning增强学习就出来了，Deep Learning + Reinforcement Learning = Deep Reinforcement Learning深度增强学习。有了深度增强学习，序列决策初步取得成效，因此，出现了AlphaGo这样的里程碑式的成果。
  3）到meta learning
  但是，新的问题又出来了，深度增强学习太依赖于巨量的训练，并且需要精确的Reward，对于现实世界的很多问题，比如机器人学习，没有好的reward，也没办法无限量训练，怎么办？这就需要能够快速学习。
  而人类之所以能够快速学习的关键是人类具备学会学习的能力，能够充分的利用以往的知识经验来指导新任务的学习，因此Meta Learning成为新的攻克的方向。
**元学习学习到的是学习能力，而不是知识本身。**
