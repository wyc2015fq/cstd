# 不同的领域、框架，这是一份超全的深度学习模型GitHub集合 - 知乎
# 



选自Model Zoo，机器之心整理。

> 一直以来，研究者与开发者都经常在 GitHub 宝库中搜索比较有意思的项目与实现，但我们可能需要一个确切的主题，并做一些筛选。近日有开发者发布了一个模型聚合平台，我们可以分领域在上面搜索到当前最为流行的开源模型。作者根据 GitHub 项目收藏量、项目所属领域、实现框架等特点对这些模型分类，因此我们能快速定位合适的模型。

项目地址：[https://modelzoo.co/](https://link.zhihu.com/?target=https%3A//modelzoo.co/)

深度学习近来是机器学习最流行的子领域，因为针对高维数据拥有强大的建模能力，它在很多不同的任务与领域都绽放了夺目的光彩，例如计算机视觉、自然语言处理与智能体学习等。深度学习模型是一种表示学习方法，即根据模型学习表示的本身，而不仅仅如同传统机器学习那样将表示映射到输出。

目前深度学习主要通过不同层级的神经元从原始数据自动抽取特征，它学习到的表示往往比传统 ML 手动设计的表示有更好的性能。目前 GitHub 上受关注的 ML 实现大部分都是深度学习模型，它们以不同的层级结构与网络架构建立起一个个解决具体问题的模型。例如 2D 卷积层以分层的形式堆叠就能由简单到复杂抽取二维数据（如图像）的特征，而循环单元的堆叠就适合处理机器翻译等序列标注问题。再加上残差连接、门控机制和注意力机制等模块就能针对具体任务学习特定知识。

这些层级表征的深度模型在近来有非常多的研究，很多开发者与研究者都分享了他们的代码与创意。ModelZoo 旨在为搜索优秀深度模型提供一个集中式的平台，并希望为复现论文、构建复杂神经网络、学习模型代码与直接使用预训练模型提供资源。

Model Zoo 是新构建的平台，因此它包含的模型库将不断发展。如果读者发现有其它一些优秀的模型还没有收录进去，可以在网站上提交并通知作者。此外，这个网站的开发者目前还只是新加坡的一名本科生，他希望能借这样一个平台帮助更多的学习者。

Model Zoo 所收集的所有模型都能在 GitHub 中找到，作者 Jing Yu 根据 GitHub 项目收藏量、项目所属领域、实现框架等特征对模型做了整理，此外我们也能使用关键词搜索确定需要查找的模型。每一个模型都在页面上展示了对应 GitHub 项目的 README 文档，因此读者可快速判断项目的基本信息与要求。此外，作者还构建了 Newsletter 模块，每月整理一次当月最流行与重要的模型。
![](https://pic2.zhimg.com/v2-16d49e7e31aa3b6420ee21c85b9d4f8d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='580'></svg>)
目前，该项目内的深度学习模型有五大分类：计算机视觉、自然语言处理、生成模型、强化学习和无监督学习。这些类别或领域相互之间是有交叉的，例如很多生成模型都可以归为无监督学习，实际上很多模型确实归为了多个类别。
![](https://pic4.zhimg.com/v2-c3172b37877c50989a84f60b5873bb87_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='501'></svg>)
目前收录的实现框架有 Tensorflow、Caffe、Caffe2、PyTorch、MXNet、Keras。虽然还有很多框架没有收录，例如 CycleGAN 的原实现因为使用的是 Lua 语言与 Torch 框架，即使该项目的收藏量达到了 6453，这个平台同样没有收录。
![](https://pic2.zhimg.com/v2-4eef02a665ff8481d19e8e8864663de1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='329'></svg>)
可以从 Browse Frameworks 和 Browse Categories 分别进入以上的框架分类界面和模型分类界面，并按其分类进行浏览。下方的搜索框支持模型名称的搜索。
![](https://pic4.zhimg.com/v2-daca3bdc506588962dafcff0a2e8d26f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='500'></svg>)
模型基本是按 GitHub 的收藏量（如 Magenta 的 10164 star）排序的，预览界面还包含项目简介、实现框架和分类等信息。点击后可进入项目细节（即对应 GitHub 项目的 README 文档）。后文我们将针对各领域简要介绍其中最流行的项目：




**计算机视觉**
- GitHub star：10164
- 实现框架：TensorFlow

Magenta 是一个用于探索机器学习在艺术、音乐创作中的作用的研究项目，因为应用众多作者将其划分为计算机视觉与强化学习领域。其主要涉及为生成歌曲、图像、素描等开发新的深度学习和强化学习算法。但它也在尝试构建智能工具和接口，以帮助艺术家和音乐家扩展他们使用这些模型的方式。Magenta 由来自谷歌大脑团队的一些研究员和工程师启动，但也有很多其他人为该项目做出了重要的贡献。
![](https://pic4.zhimg.com/v2-cf2c0f8d10c13cdcad97542ea13867db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='506'></svg>)
目前该项目有非常多的应用实现与演示，读者可查看 [https://magenta.tensorflow.org/blog。](https://link.zhihu.com/?target=https%3A//magenta.tensorflow.org/blog%25E3%2580%2582)

**Mask-RCNN**
- GitHub star：6104
- 实现框架：Keras

Mask R-CNN 是一个两阶段的框架，第一个阶段扫描图像并生成候选区域（proposals，即有可能包含一个目标的区域），第二阶段分类候选区域并生成边界框和掩码。Mask R-CNN 扩展自 Faster R-CNN，由同一作者在去年提出。Faster R-CNN 是一个流行的目标检测框架，Mask R-CNN 将其扩展为实例分割框架。
![](https://pic3.zhimg.com/v2-5abcb40789712c2c345918dfa9794d46_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='511'></svg>)
该项目是 Mask R-CNN 的在 Python3、Keras 和 TensorFlow 上的实现。该模型可以为图像中的目标实例生成边框和分割掩码。其架构基于特征金字塔网络（FPN）和 ResNet101 骨干网络。

该 repo 包含：Mask R-CNN 建立在 FPN 和 ResNet101 上的源代码；数据集 MS COCO 的训练代码；MS COCO 的预训练权重；可视化检测流程的每个步骤的 Jupyter notebook；多 GPU 训练的 ParallelModel 类；在 MS COCO 指标（AP）上的评估；训练自定义数据集的示例。
![](https://pic3.zhimg.com/v2-ddb688253904c565df654599fa95c0ba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='858' height='643'></svg>)



**自然语言处理**

**WaveNet**
- GitHub star：3692
- 实现框架：TensorFlow

下图展示了 WaveNet 的主要结构，这是一个完全卷积的神经网络，其中的卷积层有不同的扩张系数（dilation factors），这让其感受野可根据层级深度指数式地增长并可覆盖数千个时间步骤。此外 Wave Net 还是第一个提出因果卷积的模型，从直观上来说，它类似于将卷积运算「劈」去一半，令其只能对过去时间步的输入进行运算。其实后来很多使用卷积网络处理序列标注问题的模型都借鉴了 WaveNet 所提出的这两个结构。
![](https://pic4.zhimg.com/v2-9ab51c968cd8f332c10f4ab2438cbc8f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='406'></svg>)
DeepMind 提出的 WaveNet 神经网络架构可以直接生成新的音频波形，在文本到语音转换和音频生成中有优越的表现。在训练时间，其输入序列是由人类说话者录制的真实波形。训练之后，我们可以对这个网络进行采样以生成合成话语。在采样的每一个时间步骤，都会从该网络所计算出的概率分布中取出一个值。然后这个值会被反馈进入输入，并为下一个步骤生成一个新的预测。像这样一次一步地构建样本具有很高的计算成本，但这对生成复杂的、听起来真实感强的音频而言至关重要。




**Sentence Classification with CNN**
- GitHub star：3416
- 实现框架：TensorFlow

论文《Convolutional Neural Networks for Sentence Classification》的简化版 TensorFlow 实现。
![](https://pic3.zhimg.com/v2-7104ebc703649acd7b6fae878de5d3be_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='820' height='327'></svg>)
**生成模型**

**DCGAN-tensorflow**
- GitHub star：4263
- 实现框架：TensorFlow

Goodfellow 等人提出来的 GAN 是通过对抗过程估计生成模型的新框架。在这种框架下，我们需要同时训练两个模型，即一个能捕获数据分布的生成模型 G 和一个能估计数据来源于真实样本概率的判别模型 D。生成器 G 的训练过程是最大化判别器犯错误的概率，即判别器误以为数据是真实样本而不是生成器生成的假样本。因此，这一框架就对应于两个参与者的极小极大博弈（minimax game）。而 DCGAN 主要使用卷积神经网络作为生成器 G 与判别器 D 的架构，因此能生成比较清晰与优秀的图像。

论文《Deep Convolutional Generative Adversarial Networks》的 TensorFlow 实现，这是一个稳定的生成对抗网络。
![](https://pic3.zhimg.com/v2-41a9b437a018a786904e07f078d46726_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='448'></svg>)
上图展示了生成器 G 的架构，它使用四个转置卷积进行上采样，即将 100 维的随机变量恢复到图像。这个项目为了防止判别器器网络收敛过快，当判别器迭代一次生成器网络会连续迭代两次，这和原论文不太一样。




**Image-to-Image Translation with Conditional Adversarial Networks**
- GitHub star：4242
- 实现框架：PyTorch

因为 CycleGAN 的原实现是用 Lua 写的，因此很多研究者使用该项目复现图像到图像的转换。CycleGAN 的主要想法是训练两对生成器-判别器模型以将图像从一个领域转换为另一个领域。在这过程中我们要求循环一致性，即在对图像应用生成器后，我们应该得到一个相似于原始 L1 损失的图像。因此我们需要一个循环损失函数（cyclic loss），它能确保生成器不会将一个领域的图像转换到另一个和原始图像完全不相关的领域。

该项目是配对、非配对的图像到图像转换的 PyTorch 实现。




**强化学习**

**Deep Reinforcement Learning for Keras**
- GitHub star：2639
- 实现框架：Keras
![](https://pic2.zhimg.com/v2-0c86c93a5b5af267624f41b319f0c9ad_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='398'></svg>)
keras-rl 让人们可以轻松使用当前最佳的深度强化学习算法，使用 Keras 实现，并结合了 OpenAI Gym 来构建项目。

目前该项目实现了以下算法：
- Deep Q Learning (DQN)
- Double DQN
- Deep Deterministic Policy Gradient (DDPG)
- Continuous DQN (CDQN or NAF)
- Cross-Entropy Method (CEM)
- Dueling network DQN (Dueling DQN)
- Deep SARSA




**DQN-tensorflow**
- GitHub star：1610
- 实现框架：TensorFlow

论文《Human-Level Control through Deep Reinforcement Learning》的 TensorFlow 实现。
![](https://pic1.zhimg.com/v2-e160124e399b3e6352b7f335683d521c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='648'></svg>)
该实现包括：Deep Q-network 和 Q-learning
- 经验重放记忆
- 减少持续更新的相关性
- 用于 Q-learning 目标的网络由于区间问题而经过修改
- 减少目标和预测 Q 值之间的关联




**无监督学习**

**MUSE: Multilingual Unsupervised and Supervised Embeddings**
- GitHub star：1298
- 实现框架：PyTorch
![](https://pic1.zhimg.com/v2-810794a13778976352f0db1826227ae4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='193'></svg>)
MUSE 是一个多语言词嵌入的 Python 库，其目的是为社区提供当前最佳的基于 fastText 的多语言词嵌入，以及用于训练和评估的大规模高质量双语词典。

该项目使用了两种方法，一种是使用双语词典或相同字符串的监督方法，另一种是没有使用任何平行数据的无监督方法。




**Domain Transfer Network (DTN)**
- GitHub star：654
- 实现框架：TensorFlow

论文《Unsupervised Cross-Domain Image Generation》的 TensorFlow 实现，下图是其模型架构概览。
![](https://pic4.zhimg.com/v2-7e0dbc0d7844ea3e402c437da14bacb7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='991' height='351'></svg>)
以上是各类别主要流行的模型简介，还有很多优秀的项目并没有展示，读者根据该平台与具体的需求筛选模型。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)





