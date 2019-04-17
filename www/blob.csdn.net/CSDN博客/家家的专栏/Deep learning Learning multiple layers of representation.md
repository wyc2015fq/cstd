# Deep learning: Learning multiple layers of representation - 家家的专栏 - CSDN博客





2012年12月06日 10:49:49[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2813
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)









Geoffrey E. Hinton 的Learning multiple layers of representation可作为初学deep learning的paper之一吧~

 结合以前所学习的paper资料，随便记录一二，没有参考价值~

Deep learning (以下简称DL)，这种深度架构，还是对人的视觉理解的一种模拟~

compute vision这个领域中，重大的突破往往能从人的视觉原理来解释它~

这或许正是machine learning的依据~  机器只能在最大程度上模拟人脑，难于超越之~



正因为人的视觉理解中的，存在着多层表示的深度架构，所以神经网络NN, CNN, RMB, DBN等一步步发展~

Back-propagation 后向传输，来有效计算神经网络~

但是它要求是有标签的训练数据，并在深度网络中不能够很好的work~

学习这种多层生成模型，比较难~ 但是每次只学习一层非线性的、分布式表示，将会容易很多~

（多层RMB，层叠起来~）

1、学习特征描述子





