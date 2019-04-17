# 谷歌带来GAN入门神器：浏览器上运行可视化工具GAN Lab - 知乎
# 



**选自GitHub，机器之心编译，机器之心编辑部。**

> 佐治亚理工学院和谷歌大脑的研究者开发了 GAN 的可视化工具 GAN Lab，无需安装也无需专门硬件，通过网页浏览器（推荐 Chrome）就可以打开。用户可以利用 GAN Lab 交互地训练生成模型并可视化动态训练过程的中间结果，用动画来理解 GAN 训练过程中的每一个细节。

用浏览器打开这个网页，就可以开始耍了：[https://poloclub.github.io/ganlab/](https://link.zhihu.com/?target=https%3A//poloclub.github.io/ganlab/)




**什么是 GAN？**

许多机器学习系统希望将一些复杂的输入（如图像）生成简单的输出（比如标签「猫」）。相比之下，生成模型的目标则相反：取一些简单的输入，可能是一些随机数字，然后生成复杂的输出，如一张逼真的人脸图像。而生成对抗网络（GAN）是一种特别有效的生成模型，它仅在几年前才由 Ian Goodfellow 提出（[https://arxiv.org/pdf/1406.2661.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1406.2661.pdf)），一直是机器学习界非常感兴趣的主题。

你也许会好奇我们为什么想要一个生成逼真图像的系统，或者任何其它类型数据的逼真模拟。除了是我们智力的挑战，这种模型还是一种非常便捷的工具，应用范围包括艺术、增强模糊图像等。




**GAN 的工作原理**

机器从零开始「创建」逼真图像看起来很神奇，但 GAN 用了两个关键技巧来将这个看似不可能实现的模糊目标变成现实。

第一个技巧对 GAN 来说并不新鲜，就是纳入随机性。基本上，这种做法不无道理：如果你构建的系统每次运行都生成同样的人脸图像，那就没什么意思了。并且，从概率角度来看，这种做法有助于我们将图像生成问题转化为自然的数学框架。我们当然不想随机均匀地挑选图像，因为那样只会产生噪声。相反，我们希望该系统能够了解哪些图像可能是人脸，哪些不是。从数学角度来看，这涉及对图像的概率分布建模，即一个能够分辨人脸图像的函数。这类问题——在高维空间中建模函数——正是神经网络所要解决的问题。

定义 GAN 的一个重要概念是将这个建模问题设定为一种竞赛。这也是「生成对抗网络」这个名字中「对抗（adversarial）」的来源。该网络的核心观念是建立两个相互竞争的网络：一个生成器和一个判别器。生成器尝试创建随机合成的输出（如人脸图像），而判别器则试着将这些输出与真实样本（如名人数据库）区分开来。其期望是随着这两个网络的对抗，输出会变得越来越好，最终生成一个能够产生逼真输出的生成器网络。

总之，生成对抗网络是学习从特殊分布中选择样本的神经网络（「生成」），它们通过建立竞争关系来实现这一目的（「对抗」）。




**可视化训练过程**

GAN 的结构非常复杂，关于 GAN 的可视化研究还有很多工作要做。以下是 GAN Lab 的基础思想。

首先，我们并不是可视化像生成逼真图像那样复杂的过程。我们仅在两个维度上展示 GAN 学习点分布的过程。如此简单的可视化并没有什么实际应用，但能相当简单地展示 GAN 的运行机制。此外，可视化 2D (x,y) 空间的概率分布比可视化高维空间的概率分布要简单得多。




**选择一个数据分布**

在界面顶部，你可以选择一个概率分布让 GAN 来学习，我们将其可视化为一组数据样本。一旦选择了一个分布，我们将在两个位置展示它：左侧的「model overview graph」视图展示了尺寸较小的版本；右侧的「layered distributions」视图展示了尺寸较大的版本。
![](https://pic2.zhimg.com/v2-a4b932f22c729c780f099a975f6836cd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='520' height='230'></svg>)图 1：选择的数据分布被展示在两个位置
我们设计了两个视图来帮助用户更好地理解 GAN 生成逼真样本的过程：

（1）「model overview graph」展示了 GAN 的架构、主要组件及其连接方式，另外还可视化了这些组件生成的结果。

（2）「layered distributions」覆盖了「model overview graph」视图中的组件的可视化，因此你可以在分析模型时更容易地对比组件的输出。




**开始训练**

要开始训练 GAN 模型，请单击工具栏上的开始键（
![](https://pic1.zhimg.com/v2-27f5528c28f5924704cb6ad264b0c9ac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='28' height='24'></svg>)
）。除了所选分布中的真实样本，你还会看到模型生成的假样本。随着训练的进行，假样本的位置会不断更新。完美的 GAN 将会生成与真实样本的分布无法区分的假样本。当这种情况发生时，你将在「layered distributions」视图中看到这两种分布很好地重叠。
![](https://pic1.zhimg.com/v2-737f30b43123a087b612de388bf58ec4_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='246' height='293'></svg>)图 2：随着训练的进行，假样本的位置不断更新。然后，真实样本和假样本的分布很好地重叠
**可视化生成器和判别器**

GAN 中的生成器和判别器有竞争关系，它们彼此竞争，迭代地更新假样本，使其与真实样本更加相似。GAN Lab 可以对二者之间的相互作用进行可视化。

生成器。如前所述，生成器是将随机输入转换成合成输出的函数。在 GAN Lab 中，随机输入是一个 2D 样本，值为 (x, y)（从均匀分布或高斯分布中采样得到），输出也是一个 2D 样本，但是映射到不同的位置，是一个假样本。可视化该映射的一种方式是使用流形（manifold）[Olah, 2014]。输入空间被表示为均匀方形网格。随着该函数将输入空间的位置映射到新的位置，如果我们能够看到输出，则整个网格（现在包含了不规则四边形）看起来像原始网格的变形版。每个（变形）单元的区域（或密度）已经改变了，我们将密度编码为不透明度，这样高不透明度就表示在较小的空间内有更多样本。非常精细的流形看起来集合和该假样本的可视化图一样。该可视化图展示了生成器如何学习映射函数以使其输出与真实样本的分布更加相似。
![](https://pic2.zhimg.com/v2-1df94a368f9164b0c88e229bbf4c5639_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='580' height='193'></svg>)图 3：生成器的数据变换被可视化为流形，将输入噪声（最左侧）转换成假样本（最右侧）
判别器。生成器创建了假样本，判别器（二分类器）尝试区分假样本和真实样本。GAN Lab 将其决策边界可视化为 2D 热图（类似 TensorFlow Playground）。每个网格单元的背景色编码了分类器结果的置信度值。绿色越深表示该区域的样本更有可能是真实的；紫色越深的区域中的样本更有可能是假的。随着 GAN 趋近最优，整个热图变得越来越灰，这表示判别器无法轻松区分假样本和真实样本了。
*![](https://pic4.zhimg.com/v2-66e1f9b104587c3256f95dfc71680317_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='230' height='230'></svg>)*
图 4：生成器的性能可以通过 2D 热图读出。图中所示的生成器表现良好，因为多数真实样本分布在其分类表面的绿色区域（假样本在紫色区域）。




**理解生成器与判别器之间的相互作用**

GAN 的两个网络在其迭代地更新自身时相互影响。GAN Lab 的一大用途是利用其可视化来了解生成器如何增量更新，从而改进自身，生成越来越逼真的假样本。生成器通过愚弄判别器来实现这一点。在判别器将假样本分类为真实样本时，生成器的损失值下降（对判别器不利，对生成器有利）。GAN Lab 将假样本的梯度可视化（粉色线），以促成生成器的成功。
![](https://pic2.zhimg.com/v2-a86971cc50a7cf5ffd102f14eefedbf1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='395' height='200'></svg>)
图 5：基于这些样本的当前位置和判别器的当前分类表面（通过背景颜色可视化），假样本的移动方向由生成器的梯度（粉色线条）表示。




通过这种方式，生成器逐步改善，生成更加真实的样本。一旦假样本得以更新，判别器也将进行相应更新，微调其决策边界，等待下一批试图蒙蔽它的假样本。这一交互式更新过程不断进行，直到判别器无法分辨真假样本为止。




**使用交互功能**

GAN Lab 有很多支持交互实验的非常棒的功能。
- 交互式超参数调整

点击「编辑」图标（
![](https://pic2.zhimg.com/v2-ee3bbea6d6b1c04caa8f4f0f2a60e8d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='57' height='24'></svg>)
）来展示独立的超参数，然后在训练期间实时地编辑它们。
- 用户定义数据分布

如果你不喜欢我们提供的数据分布，可以通过点击数据分布列表最后的图标（
![](https://pic3.zhimg.com/v2-4ce45833e946df565ef4813ed2156cda_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='25' height='24'></svg>)
），编辑你想要的数据分布。
- 慢镜头模式

动画太快看花眼？别担心，你可以点击「慢动作」图标（
![](https://pic4.zhimg.com/v2-7aafa13c9ef6b0b85144516ca32ce65b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='26' height='24'></svg>)
）进入慢动作模式。
- 手动逐步执行




如果你想进行更多的控制，可以点击该图标（
![](https://pic4.zhimg.com/v2-becde1a84d7d6f04e02c66a102a3eeaf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='26' height='24'></svg>)
）一步步手动训练单个迭代。

查看以下视频，快速了解 GAN Lab 功能：
- GAN Lab 介绍（0:00-0:38）
- 通过超参数调整训练简单分布（0:38-1:05）
- 训练用户定义的数据分布（1:05-1:51）
- 慢动作模式（1:51-2:19）
- 手动逐步执行（2:19-3:10）




**如何实现？**

GAN Lab 使用浏览器内 GPU 加速的深度学习库 TensorFlow.js 来实现。从模型训练到可视化，所有的一切都通过 JavaScript 实现。你只需要一个网页浏览器（如 Chrome），即可运行 GAN Lab。这一实现方法极大地拓宽了人们使用深度学习交互工具的渠道。

源代码链接：[https://github.com/poloclub/ganlab](https://link.zhihu.com/?target=https%3A//github.com/poloclub/ganlab)




**论文：GAN Lab: Understanding Complex Deep Generative Models using Interactive Visual Experimentation**
![](https://pic3.zhimg.com/v2-df79462b1a2728dcc00775ad0f3b15f2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='168'></svg>)
论文地址：[https://arxiv.org/abs/1809.01587v1](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1809.01587v1)

**摘要：**深度学习领域的最新进展吸引了众多从业者及学生的目光，激发他们当中的很多人去学习这项新技术。尽管视觉和交互的方法已经让深度学习变得更易掌握，但多数现有的工具聚焦于较简单的模型。本文提出了 GAN Lab，这是第一个为非专业人士学习、试验生成对抗网络（一种流行的复杂深度学习模型）而设计的交互式视觉工具。用户可以利用 GAN Lab 交互地训练生成模型，并可视化动态训练过程的中间结果。GAN Lab 紧密集成了总结 GAN 结构的模型概述图（model overview graph）和帮助用户解释子模型之间相互作用的分层分布视图（layered distributions view）。GAN Lab 引入了新的交互试验特征，用于学习复杂的深度学习模型，如在多个抽象层次上进行逐步训练以理解复杂的训练动态。GAN Lab 使用 TensorFlow.js 实现，任何人都可以通过网页浏览器进行访问，无需安装，也无需专门硬件，克服了部署深度学习交互式工具的一个重大实际挑战。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[https://poloclub.github.io/ganlab/](https://link.zhihu.com/?target=https%3A//poloclub.github.io/ganlab/)


