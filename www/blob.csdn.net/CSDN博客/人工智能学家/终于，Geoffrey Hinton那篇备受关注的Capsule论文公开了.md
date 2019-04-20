# 终于，Geoffrey Hinton那篇备受关注的Capsule论文公开了 - 人工智能学家 - CSDN博客
2017年10月28日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：238
*![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVRic2iaLlszzl0PvDxwzVbqGnVp6Wlkahp0nRcMsVC5jPXq5sfrofklldIdN0VBPaOvibKFMDRLG7HA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)*
*来源：机器之心*
*概要：9 月份，Axios 的一篇报道指出，Geoffrey Hinton 呼吁研究者们对反向传播保持怀疑态度，并准备在深度学习之上重构人工智能的理论体系。*
9 月份，Axios 的一篇报道指出，Geoffrey Hinton 呼吁研究者们对反向传播保持怀疑态度，并准备在深度学习之上重构人工智能的理论体系。报道指出，他和其他两位研究者被 NIPS 2017 接收的论文《Dynamic Routing Between Capsules》正是 Hinton 对于未来人工智能形态的新探索。
在论文未放出之前，业内已经对其有着热烈的讨论。昨天，这篇文章终于被提交到了预印版论文平台上，我们得以一览全貌。
论文：《Dynamic Routing Between Capsules》
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZpccOiasJrcTmPQCvmu0qJ6U7SDWmgJric4EbcCJQibh9YqUwZ7cViavkiaw/640?wx_fmt=png)
论文链接：https://arxiv.org/abs/1710.09829
Capsule 是一组神经元，其活动向量（activity vector）表示特定实体类型的实例化参数，如对象或对象部分。我们使用活动向量的长度表征实体存在的概率，向量方向表示实例化参数。同一水平的活跃 capsule 通过变换矩阵对更高级别的 capsule 的实例化参数进行预测。当多个预测一致时，更高级别的 capsule 变得活跃。我们展示了判别式训练的多层
 capsule 系统在 MNIST 数据集上达到了顶尖的性能，比识别高度重叠数字的卷积网络的性能优越很多。为了达到这些结果，我们使用迭代的路由协议机制（routing-by-agreement mechanism）：较低级别的 capsule 偏向于将输出发送至高级别的 capsule，有了来自低级别 capsule 的预测，高级别 capsule 的活动向量具备较大的标量积。
**1. 前言**
人类视觉可以通过使用一个小心确定的注视点序列来保证仅有极少部分画面需要进行高分辨率处理。我们很难通过内省的思路来理解一个场景中注视点序列能够传递多少信息，我们能从单一注视点中获取多少信息，但在谷歌大脑 Sara Sabour、Nicholas Frosst 与 Geoffrey Hinton 提交的这篇论文中，研究人员假设单一注视点可以提供远多于目标种类及其属性等信息。研究人员假设人类的多层视觉系统可以在每个注视点上创建类似于解析树一样的机制，在研究中，我们将忽略这些单一注视点解析树在多个注视点中如何协调的问题。
解析树通常通过动态分配内存来构建，但根据 Hinton 等人 2000 年的研究，在新的论文里我们假设对于单一注视点而言，解析树是由指定多层神经网络雕刻而出的，就像在岩洞的石壁上雕刻的图画一样。每一层会被分割为很多神经元小组，它们被称为胶囊（「Capsule」，Hinton et al., 2011），解析树上的每个节点对应于一个活动的 Capsule。在迭代地进行路由选择的过程中，每个活动
 Capsule 将在上面的层中选择一个 Capsule 作为其树中的父单元。对于更高级的视觉系统而言，这个迭代过程将解决从部分分配到整体的问题。
在活动 Capsule 中神经元的活动表示了图像中存在的特定实体的各种性质。这些性质可以包含很多种不同的实例化参数，例如姿势（位置，大小，方向）、变形、速度、反射率，色彩、纹理等等。其中一个非常特殊的属性就是图像中实例化实体的存在与否。表示存在的一个简单方法就是使用单独的逻辑单元，让它输出实体存在的概率。在本论文中，研究人员探索了一个有趣的替代方法——使用实例化参数向量的总长度来表示实体的存在，并强制向量的方向来表示实体的属性。研究人员通过应用一个非线性方法，让向量方向不变，但大小可以缩放，以此确保了输出的向量长度不超过
 1。
Capsule 的输出是向量，所以我们可以使用强大的动态路由机制确保输出的信息可以被传送到合适的父 capsule 中（上一层）。最初，输出可以通向所有可能的父节点，但因为耦合系数为 1，这个范围被急剧缩小。对于每个可能的父节点来说，capsule 会通过其自身的输出乘以权重矩阵来计算一个「预测向量」。如果这个预测向量与可能的父节点输出存在一个大的标量积，则存在自上而下的反馈，使得该父节点的耦合系数上升，其他父节点的耦合系数下降。这增加了
 capsule 对该父节点的贡献，进而提高了 capsule 预测和该父节点输出的的标量积。这种「路由协议」（routing-by-agreement）应该比由最大池化实现的原始形式的路由效果好得多，后者允许一层中的神经元忽视其他特征检测器，除了下一层的局部最活跃的特征检测器。我们证明我们的动态路由机制可以有效实现分割高度重叠的物体所需的「解释」（explaining away）。
卷积神经网络（CNN）使用学到的特征检测器副本（translated replicas），这使它们能够在图像中一个位置获得的优秀权重值知识转化到其它位置上。这已经被证明其在图像解释方面非常有帮助。尽管我们用向量输出 capsules 代替 CNN 中标量输出的特征检测器，用协议路由（routing-by-agreement）代替最大池化，但我们仍希望通过复制空间内学到的知识，所以我们令除了最后一层的
 capsules 层级都为卷积层。
和 CNN 一样，我们创建更高级别的 capsule 来覆盖图像的更大区域，但是与最大池化不同，我们无法抛弃区域内实体的确切位置信息。对于低级 capsule 而言，位置信息由活跃的 capsule 进行「位置编码」（place-coded）。随着层级的上升，越来越多的位置信息层级由 capsule 的输出向量中的实数值部分进行「率编码」（rate-coded）。从位置编码到率编码的转变和高级别
 capsule 代表具备更多自由度的更复杂实体共同表明 capsule 的维度应该随着层级的上升而增长。
**2. Capsule 如何计算输入和输出向量**
现在有很多可能的方式能实现 Capsule 的一般性概念。本论文的目的并不是探索整个可能的空间，而是简单地展示了一个非常直接与有效的实现方式，即动态路由选择（dynamic routing）。
我们希望 Capsule 输出向量的长度能表征实体（由 Capsule 表征）存在于当前输入的概率。因此我们能使用非线性「压缩」函数确保短向量的长度能够缩短到几乎等于零，而长向量的长度压缩到 1 的左邻域内。我们可以将它们放入判别式学习（discriminative learning）以充分利用非线性的特性。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZ1FBFXWRQic8icoxSKia45jJbwichU3QkXBG6Uh6YcWiaiciaAZcVNiaEFNxHoQ/640?wx_fmt=png)
其中 v_j 为 Capsule j 的输出向量，s_j 为总输入向量。
对于 Capsule 除了第一层的所有层级，Capsule s_j 的总输入为下面 Capsule 层级中所有「预测向量」![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZAhyRf1It17jFvgqS9IzyOuqXepl65a8nk9ExKAzX6PKq3d90lwicUEw/640?wx_fmt=png)的加权和，而预测向量
 u_hat 可通过计算一个 Capsule 的 u_i 输出与下面层级的权重矩阵 W_ij 的乘积而得出：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZcpcJJKG0UGEDr0fmQQJSf3WSia3gZQjLwO7aE3kuGt4j9sh7aTbFvzA/640?wx_fmt=png)
其中 c_ij 为耦合系数（coupling coefficients），该系数由迭代的动态路由选择过程确定。
Capsule i 的耦合系数和上面层级所有的 Capsule 耦合系数和为 1，并且由「routing softmax」决定，该 softmax 函数中的初始 logits b_ij 为 Capsule i 应该和 Capsule j 耦合的对数先验概率：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZiccTvuo1StUZYxeIgRdPrnDm8ez2yvwicLoNBJstL5TBAzDibvR6l94ww/640?wx_fmt=png)
对数先验（log priors）可以同时和其它权重有区别地进行学习。它们依赖于两个 Capsule 的位置与类型，但不依赖于当前的输入图像。初始化耦合系数可以通过测量前面层级中每一个 Capsule j 的当前输入 v_j 和 Capsule i 的预测![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZs2OsuJMMcnnaB4iawfU5PLPnwTPNnXpKTj2FcoDqCXm7kVB9PFHagGA/640?wx_fmt=png)间一致性（agreement），然后借助该测量的一致性迭代地精炼更新耦合系数。
这个一致性（agreement）可以简单地设置为标量乘积![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZfqCpuUia6PDL3f9yOCSocfOwAIPOK2iaiaepu8tyriaOS6Z7np6OHADZIw/640?wx_fmt=png)。该一致性可以被视为对数似然度，并可加入初始
 logit 中，b_ij 在计算所有耦合系数更新值之前已经将 Capsule i 链接到更高级的 Capsule 中。
在卷积 Capsule 层中，每一个 Capsule 单元都是一个卷积单元，因此每一个 Capsule 将输出一个向量组而不是单个输出向量。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZHWusnzLWKNicTaciam112RaphuJgSmEdRa03g3uLtE1NcEuBc2KH5ia2w/640?wx_fmt=png)
**4. CapsNet 架构**
图 1 展示了一个简单的 CapsNet 架构。该架构只有两个卷积层和一个全连接层。Conv1 有 256 个 9 × 9 卷积核，步幅为 1，带有 ReLU 激活。该层将像素强度（pixel intensity）转换成局部特征检测器的活动，然后作为输入传送至主 capsule 中。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZLye4Df3BEXxxq5KpicEGSJFBzGiaekyhpUBF5Kkib6Bx3nvLFsZ29tneg/640?wx_fmt=png)
*图 1：一个简单的 3 层 CapsNet。该模型的结果与深度卷积网络（如 Chang and Chen [2015]）差不多。DigitCaps 层中每个 capsule 的活动向量（activity vector）的长度代表出现的每个类别的样例，用于计算分类损失。W_ij 是 PrimaryCapsule 中每个 u_i 之间的权重矩阵，i ∈ (1, 32 × 6 × 6)，v_j,j ∈ (1, 10)。*
主 Capsule 是最低级别的多维实体，从逆向图（inverse graphics）的角度来看，激活主 capsule 对应的是逆转渲染过程（rendering process）。这和将多个实例化部分拼合起来组成一个熟悉的整体是一种非常不同的计算类型，而后者正是 capsule 的设计初衷。
第二个层（Primary Capsules）是一个卷积 capsule 层，具有 32 个通道、卷积 8D capsule（即每个主 capsule 包括 8 个卷积单元，每个卷积单元有一个 9 × 9 核，步幅为 2）。每个主 capsule 输出接收到所有 256×81 Conv1 单元的输出，它们的感受野与 capsule 的中心位置重叠。PrimaryCapsules
 一共有 [32, 6, 6] capsule 输出（每个输出都是一个 8D 向量），[6, 6] 网格中的每个 capsule 彼此共享权重。可以把 PrimaryCapsules 看作一个区块非线性（block non-linearity）为 Eq. 1 的卷积层。最后的层（DigitCaps）的每个数字类别都有一个 16D capsule，每个 capsule 接收来自下面一层所有 capsule 的输入。
我们只在两个连续的 capsule 层之间执行路由选择（如 PrimaryCapsules 和 DigitCaps）。Conv1 的输出是 1D 的，无法对其空间进行定位。因此 Conv1 和 PrimaryCapsules 之间无法进行路由选择。所有路由选择 logit（b_ij）都要被初始化为 0。因此，最初 capsule 输出（u_i）以同样的概率（c_ij）被发送至所有父
 capsule（v_0...v_10）。我们在 TensorFlow 中进行实现（Abadi et al. [2016]），使用 Adam 优化器及其 TensorFlow 默认参数，包括指数递减学习率，以最小化 Eq. 4 中的边际损失。
**4.1 重构损失作为正则化方法**
我们使用额外的重构损失（reconstruction loss）来促进数字 capsule 对输入数字的实例化参数进行编码。在训练期间，我们蒙住所有向量，除了正确的数字 capsule 的活动向量。然后，使用该活动向量来重构。数字 capsule 的输出被馈送至包含 3 个全连接层的解码器，该解码器对图 2 描述的像素强度进行建模。我们最小化逻辑单元（logistic
 unit）输出和像素强度之间的平方差的和。我们按 0.0005 的比例缩小重构损失，以使它不会主导训练过程中的边际损失。如图 3 所述，仅保留重要的细节，从 CapsNet 的 16D 输出进行重构是鲁棒的。
**5. MNIST 上的 Capsule**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZzZkaraXcQRXQGcicv1rL5YxtYdXFrVZyiaqlibns5KLLea7dWszdrdCvg/640?wx_fmt=png)
*图 3：进行 3 次路由迭代的 CapsNet 在 MNIST 数据集上的测试重构样例。(l, p, r) 分别代表标签、预测和重构目标。最右两列是两个失败的重构样例，它展示了模型如何混淆该图像中的 5 和 3。其他列来自正确的分类，展示了模型如何挑剔细节，同时使噪声变得平滑。*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZO1q6G1A7Vsek6ibiaSkAf8hkPIJxa0X3F0ibyXnY6e4KaRz7S78lHjr8w/640?wx_fmt=png)
*表 1：CapsNet 分类测试准确度。其中 MNIST 均值和标准差都是根据三个试验计算而出。*
我们发现 capsule 中的一个维度（所有 16 个维度中）几乎总是可以表征数字的宽度。而多个维度可以表征全局变化的组合，还有其它一些维度可以表征数字的局部变化。
*![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZ0313AHoOT4gvCAc9jsTswKpvX4cgxODMVdia39sYZXuw6ice67D8DiaxQ/640?wx_fmt=png)*
*图 4：维度扰动（dimension perturbation）。每一行展示当 DigitCaps 表征的 16 个维度之一在区间 [−0.25, 0.25] 中以 0.05 的间隔（interval）进行变化并重构。*
图 5 中的重构表明 CapsNet 能够将图像分割成两个原始数字。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibu3etiazeNJCY3t0Ovo50icZsg6sJicNdjk9BoGYV8zSviclp7NME24U58XmLYicBKj0Hib8AWuUlhrKTg/640?wx_fmt=png)
*图 5. 一个 CapaNet 的样本重构，其中在 MultiMNIST 测试数据上使用了 3 个路径迭代。*
两个重构数字以绿色和红色重叠位于图下方。图上方展示了输入图像。L:(l1, l2) 表示图中输入的两个数字的标签，R:(r1, r2) 表示重构的两个数字。最右两列展示了两个错误重构的例子，分别从标签和预测（以 P 标记）重构。在（2，8）的例子中，模型混淆了 8 和 7，在（4，9）的例子中，模型混淆了 9 和 0。其它列都给出了正确的分类结果，表明模型能考虑所有的像素并决定每个像素如何分配给两个数字，即使是在相当困难的情景下（第
 1—4 列）。注意数据集生成的时候，像素的值都简化为 1。带*号的两列展示了既不是从标签也不是从预测重构的数字的结果。这些结果表明模型不止是为图像中的所有（包括未出现的）数字寻找最佳拟合。因此，在（5，0）的例子中，它无法重构出一个 7，因为它知道 5 和 0 拟合得最好，并且模型已经考虑了所有的像素。同样，在（8，1）的例子中，数字 8 中的环并不会使模型指向 0，因为它已经考虑了 8。所以，如果其中一个没有任何其它的支持线索，模型不会将一个像素分配给两个数字。
**8. 讨论**
Capsule 做出了一个很强大的具有代表性的假设：在图像的每一个位置，一个 capsule 只表示实体类型的至多一个实例。这个假设受被称为「拥挤」（crowding，Pelli et al. [2004]）的知觉现象的启发，能消除「绑定」（binding，Hinton[1981]）问题，并允许一个 capsule 使用一个分布表示（它的活动向量）在给定的位置编码该类型的实体的实例化参数。这种分布表示的有效性比通过在高维网格上激活一个点（并使用正确的分布表示）编码实例化参数呈指数增长。然后
 capsule 可以充分利用这样的事实，即空间关系可以通过矩阵乘法建模。
Capsule 利用和视角变化一致的神经活动，而不是尽可能地消除活动中的视角变化。这给了它们超越「归一化」方法如空间转换器网络（spatial transformer network）的优势（Jaderberg et al. [2015]）：它们能同时处理多个不同目标（或目标部分）的多种仿射变换。
Capsule 也很擅长分割任务（这是计算机视觉中的另一个难题），因为实例化参数的向量允许它们使用路由协议机制（routing-by-agreement），正如研究者在论文中所证明的。现在，对 capsule 的研究处于和本世纪初用 RNN 研究语音识别类似的阶段。有许多基本的具有代表性的理由使我们相信这是一个更好的方法，但在 capsule 能超过一种高度发展的技术之前，还需要很多的细节洞察。一个简单的
 capsule 系统已经能在分割重叠数字任务中表现出空前强大的性能，这种迹象提醒我们 capsule 是一个值得探索的方向。
