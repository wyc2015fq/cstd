# 大神Hinton的Capsule论文终于公开，神经网络迎来新探索 - 知乎
# 



> 夏乙 问耕 编译整理
量子位 出品 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-91a00ab8d84ab64d023f28d993eaa79a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='400'></svg>)
“我的观点是把它全部抛掉，重头再来。”

今年9月的一次大会上，大神Geoffrey Hinton再次号召展开一次AI革命。

应该向何处去？实际上最近几年来，Hinton的大量精力都投入到了对capsule的研究之上。他想做capsule和想要推翻反向传播，几乎是出于同一个原因：不像大脑。

capsule的提出，正是为了应对神经网络与大脑的一个不相似之处，也是Hinton这3年来一直在讲的“神经网络的一个问题”：结构中层次太少。

Hinton提出，应该在现有神经网络结构的基础上，加一个capsule的层级，每一层网络的神经元组成capsule，在内部做计算，然后输出一个压缩的结果。

到底什么是capsule？

在去年的AMA中，Hinton就谈到过“基于capsule的神经网络”，说他在Google这3年，在这项基础研究上投入了大量精力。

今年，这项研究似乎有了重大突破。

Hinton上个月在多伦多大学讲“卷积神经网络的问题及对策”时，就谈到要在现有神经网络结构的基础上，加一个capsule的层级，每一层网络的神经元组成capsule，在内部做计算，然后输出一个压缩的结果。

NIPS刚刚公布的接收论文列表中，也有Sara Sabour、Nicholas Frosst和Geoffrey E Hinton合作的**Dynamic Routing Between Capsules**。

今年，关心capsule的网友也不少，甚至直接点名Hinton来问：

**capsule怎么样了？**

按照之前的说法，Hinton和谷歌大脑的同事Sara Sabour、Nicholas Frosst合作了一篇论文Dynamic Routing Between Capsules详细谈及，而这篇论文，将作为spotlight在今年12月举办的NIPS 2017上发表。

虽说Hinton曾不止一次讲他的capsule，但这篇论文一直没有亮相。

如今这篇外界期待已久的论文终于正式对外发布，地址在此：

[https://arxiv.org/abs/1710.09829](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1710.09829)
![](https://pic1.zhimg.com/v2-a1f3eeee5db6ddf5bd16007bf3aeaa98_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)



## **论文摘要：**

capsule是一组神经元，其活动向量表示特定类型实体（例如对象或对象部分）的实例化参数。我们使用活动向量的长度来表示实体存在的概率及其表示实例化参数的方向。活动capsule在一个层面上通过变换矩阵对更高层capsule的实例化参数进行预测。当多个预测相符时，较高层的capsule就开始活动。本文显示，经过区别训练的多层capsule系统在MNIST上实现了最先进的性能，在识别高度重叠的数字时，效果明显好于卷积神经网络。为了达到这样的结果，我们用了一个迭代的路由协议机制：一个低层的capsule希望将其输出发送到一个活动向量具有大标量积、预测来自低层capsule的高层capsule。

## **Capsule是什么？**

人类视觉系统中，有一个“注视点”的概念，能让人类在视野范围内只对极小部分的图像进行高分辨率处理。这篇论文假设一个注视点带给我们的信息不仅仅是一个识别对象及其属性，还假设我们的多层视觉系统在每个注视点上都创建了一个类似分析树的东西，并忽略这些分析树在多个注视点之间如何协调。

分析树通常靠动态分配内存来构建，但是这篇论文假设对于单个注视点来说，分析树是从固定多层神经网络中“雕刻”出来的，就像用石头刻出雕像一样。神经网络的每一层会被分成很多组神经元，每一组称为一个capsule，而分析树的每个节点对应着一个活跃的“胶囊”。

## **Capsule的输入和输出**

Capsule是输出是一个向量，这篇论文中，在保持它方向不变的情况下应用非线性缩小了它的量级，确保这个非线性输出不大于1。也正因为capsule的输出是一个向量，确保了能使用强大的动态路由机制，来确保这个输出能够发送到上层合适的parent处。

因为论文作者们希望capsule输出向量的长度能够表示这个capsule所代表的实体存在于当前输入的概率，他们使用了非线性“压缩”函数来确保端向量长度缩短到接近于0，而长向量的长度略小于1。这篇论文用判别学习来充分利用非线性特征。

## **公式**
![](https://pic1.zhimg.com/v2-c7f973a527d2ac0be14be669dac60108_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='116'></svg>)



如上面公式所示，vj表示capsule j的输出，而sj是其输入。

## **架构**
![](https://pic4.zhimg.com/v2-7defccfab9be285fabb43e242e0cbe87_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='510'></svg>)



一个简单的CapsNet共有3层。两个卷积层和一个全连接层。卷积层Conv1有256个9×9军集合，步长为1，激活函数为ReLU。这层把像素强度转换成局部特征检测信息，然后输入给主Capsules。

第二层（主Capsules）是一个卷积capsule层，32通道卷积8D capsule。

## **分割重叠数字**

论文中的测试表明，CapsNet能够把重叠在一起的数字，还原成两个原始数字。



![](https://pic2.zhimg.com/v2-7162a3ea207886385035048eea5b68d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='378'></svg>)
## **讨论**

30年来，最先进的语音识别使用具有高斯混合的隐马尔可夫模型作为输出分布，这些模型具有致命的代表性限制。

Capsules通过将像素强度转换为识别片段的实例化参数向量来避免指数低效。Capsules做出了一个非常强的代表性假设：在图像的每个位置，一个Capsule代表的实体类型至多为一个。

Capsules现在的研究阶段，就像本世纪初将RNN应用于语音识别的阶段。有非常具有代表性的理由相信这是一个更好的方法，但很多细节还需要接续观察。

事实上，一个简单的Capsules系统已经在分割重叠数字时展现出无与伦比的性能，这提醒我们，Capsules是值得探索的方向。

最后再说一次，研读这篇论文，地址在此：

[https://arxiv.org/abs/1710.09829](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1710.09829)
![](https://pic1.zhimg.com/v2-6e3f50f2aec1ecdc744cdd48c2fd4318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='578'></svg>)



— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)

վ'ᴗ' ի 追踪AI技术和产品新动态


