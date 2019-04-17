# Hinton反思新作：我说反向传播不好，但还是没谁能颠覆它。 - 知乎
# 



> 栗子 问耕 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-399b73f2d370d7e3e4132f2f4d9a76b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
32年前，人工智能、机器学习界的泰斗Hinton提出反向传播理念，如今反向传播已经成为推动深度学习爆发的核心技术。

然而反向传播自诞生起，也受到了无数质疑。这些质疑来自各路科学家，也来自Hinton自己。

主要是因为，反向传播机制实在是不像大脑。

去年九月，Hinton站在众人面前，亲口说出他对反向传播“深感怀疑”，并且振臂一呼的号召：“我的观点是把它**全部抛掉，重头再来。**”

他不止这么说，也在亲自践行。
![](https://pic4.zhimg.com/v2-d0b586b3129c3e3b93e697655866bbff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='549'></svg>)
这些年来，科学家们也为反向传播寻找了不少“生物学上更合理”、也就是更像大脑工作机制的替代品。但这些是通往未来的道路么？

Hinton决定亲自尝试一下。他集结了来自DeepMind和多伦多大学的强大力量，对这些替代品进行了一次评估。结论是：

在比较复杂的分类任务 (如ImageNet) 里，那些更像大脑机制的算法，都**远不及**反向传播。

## **对比测试**

在Hinton参与的新论文Assessing the Scalability of Biologically-Motivated Deep Learning Algorithms and Architectures里，反向传播的挑战者包括：

1号选手，**目标传播** (Target-Propagation，TP) 。

2号选手，**反馈对比** (Feedback Alignment，FA) 。

3号选手，**目标差传播** (Difference Target Propagation，DTP) 。
![](https://pic4.zhimg.com/v2-06f47838033dc529c78203c8124577a3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='584'></svg>)
三位选手，还各自拥有几种变体。

加上守擂方反向传播，四者挑战的有MNIST、CIFAR以及最难的ImageNet这几个**分类**数据集。

谁的学习能力，能更好地推广到复杂的数据集里，就代表它更有潜力，去解释大脑的运作。

先来看MNIST和CIFAR两项比赛的成绩。
![](https://pic3.zhimg.com/v2-c308ddc0de71f0ea679114536eb38cee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='403'></svg>)
## **△**下划线加粗为最佳

无论是在全连接 (Fully-Connected) 还是局部连接 (Locally-Connected) 的神经网络中，反向传播的表现都是最好的。

除此之外，用BP训练的CNN，拥有**共享权重** (Shared Weights) ，也能有效提升模型的表现。

这一点值得注意，是因为CNN在生物学意义上，有一个“不太可取”的特性，就是权重共享。
![](https://pic4.zhimg.com/v2-97a4aa022055991a7e3a848350dfa9c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='392' height='251'></svg>)
因为，每个神经元的权值，都需要非常精确地传递开来，这个操作在自然界里太不现实。

不过，数据证明，权重共享并不是“不可取”。它的存在，大大减少了自由参数，让模型的学习能力更容易向复杂任务中推广。于是，BP ConvNet击败了亲近自然的方法，和它们更加自然的变体。

说到更加复杂的任务，下一个比赛场地，就是ImageNet数据集了。
![](https://pic1.zhimg.com/v2-1e34be78e597692b1825bee7e2ae7c1c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='902' height='340'></svg>)
遗憾的是，在ImageNet这座大山面前，所有的方法成绩都不理想。

但在所有的不理想中间，反向传播的表现依然优于其他选手，且以**卷积网络**的版本为最优。

也就是说，即便无法像人类一样轻取复杂问题，反向传播依然离这个目标更近，加上共享权重就更近。

研究团队在论文中说，如果想从生物学上，找到学习效果更好的算法，反向传播的挑战者们还有很长的路要走。
![](https://pic1.zhimg.com/v2-e2d4e55fe3a0d9401efe08a324e05930_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='550' height='400'></svg>)
不论是现有的“生物学合理”的算法，还是大家要找的新方法。

是啊，路还长，这次的比赛成绩也只是阶段性结果。

未来，翘首以待。

## **论文**

为了完成这个研究，来自多家著名机构的学者，组成了一个特混战队。

其中Hinton来自多伦多大学和Google Brain，Sergey Bartunov（一作）和Adam Santoro来自DeepMind，Blake A. Richards来自多伦多大学。Timothy P. Lillicrap来自DeepMind和UCL。

外界对这篇论文也有一些很强烈的赞誉之声。

譬如，有人说这是一个伟大的研究，随着时间的推移，这些讨论会变得越来越有意思；也有人说被这个研究惊到了。

当然，也有人持保留意见。

不知道你会怎么认为，去读Paper吧。
![](https://pic4.zhimg.com/v2-e57501b0123cb4c3cd402d6a8a5464fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='260'></svg>)
这篇评估了反向传播各路替代品的论文是：

**Assessing the Scalability of Biologically-Motivated Deep Learning Algorithms and Architectures**

作者：Sergey Bartunov, Adam Santoro, Blake A. Richards, Geoffrey E. Hinton, Timothy Lillicrap

最近这篇论文也发到了arXiv上，大家有空可以看一看。

传送门：

[https://arxiv.org/pdf/1807.04587.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1807.04587.pdf)

## **特立独行**

Hinton从来都不是一个跟随主流的人。他声名煊赫的整个家族都弥漫着这样的一种气质。

1972年，25岁的Hinton在爱丁堡大学攻读博士学位，并把神经网络作为研究重点。导师几乎每周都会提醒他在浪费时间。然而Hinton不为所动。

实际上几十年来，Hinton一直徘徊在人工智能研究的边缘地带。他像一个局外人一样坚守着一个简单的观点：计算机可以像人类一样思考，依靠直觉而不是规则。



![](https://pic2.zhimg.com/v2-75290489f70e49f98e42fafbce094e7d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='412'></svg>)



一直到大约2009年前后，神经网络才又引发更多人的关注。谈到那些灰暗的日子，Hinton给出了这样的回答：

“是什么支持着你不放弃？”

“其他人都错了。”

“我们本来在体制之外，力图证明传统路线是错的，然而有趣的是，转眼间我们成了正统。”Hinton的学生、OpenAI创始人Ilya Sutskever说。

作为机器学习的先锋，Hinton从中开辟了“深度学习”这个子领域，这让计算机可以自动建立起一层层的智慧。

得益于近年来计算力的猛增，深度学习成为主流方法，从我们智能手机里的语音识别、图像探测到亚马逊为你推荐的图书，都离不开它。

因为患有腰间盘突出，坐下变成一种痛苦，从2005年开始，Hinton就不再坐着了，工作时就一直站着。而现在站立工作，似乎也是一种风潮。

“我领先于潮流，”Hinton说。
![](https://pic4.zhimg.com/v2-71f8aa4e932989b5ea89745395d9a773_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='334'></svg>)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


