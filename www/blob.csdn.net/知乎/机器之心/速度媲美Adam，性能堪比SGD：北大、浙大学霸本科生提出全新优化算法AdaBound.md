# 速度媲美Adam，性能堪比SGD：北大、浙大学霸本科生提出全新优化算法AdaBound - 知乎
# 



> 为什么 Adam 不够好、SGD 不够快？因为新的 AdaBound 已经提出来了啊。

2018 年 12 月 21 日，[ICLR 2019 论文接收结果揭晓](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650754242%26idx%3D3%26sn%3D774d931a458fb87b6af631aedb288f91%26chksm%3D871a88bcb06d01aa60c152bb9b5e9e42f0f026bc92d1ab5d014b0850ac53532016507794be10%26scene%3D21%23wechat_redirect)。据统计，ICLR 2019 共收到 1591 篇论文投稿，相比去年的 996 篇增长了 60%。ICLR 2019 共接收论文 500 篇，其中 oral 论文 24 篇、poster 论文 476 篇。

今天，我们发现了一篇有趣的论文，该研究提出了一种新型[优化方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757787%26idx%3D1%26sn%3De5be45031edd8a6de06334177544a186%26chksm%3D871a9ee5b06d17f3aab5e8252337f8639306a36423b0b880847396b437f98ef6b6facf7a0a6e%26token%3D1375602381%26lang%3Dzh_CN) AdaBound，「和 [Adam](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757787%26idx%3D1%26sn%3De5be45031edd8a6de06334177544a186%26chksm%3D871a9ee5b06d17f3aab5e8252337f8639306a36423b0b880847396b437f98ef6b6facf7a0a6e%26token%3D1375602381%26lang%3Dzh_CN)一样快、和 [SGD](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757787%26idx%3D1%26sn%3De5be45031edd8a6de06334177544a186%26chksm%3D871a9ee5b06d17f3aab5e8252337f8639306a36423b0b880847396b437f98ef6b6facf7a0a6e%26token%3D1375602381%26lang%3Dzh_CN)一样好」。据了解，四位作者 Liangchen Luo、Yuanhao Xiong、Yan Liu、Xu Sun 均来自国内。共同一作骆梁宸和 Yuanhao Xiong 分别来自北京大学和浙江大学，Yan Liu 来自南加州大学，Xu Sun 来自北京大学。而且一作骆梁宸是北京大学的本科生……（长江后浪推前浪嘤 :)

作者在 reddit 网站发帖介绍了这项研究，并提供了 PyTorch 实现。他们目前在 MNIST 和 CIFAR-10 数据集做了测试，但由于计算资源有限而无法选择 ImageNet 等大数据集。此外，作者还表示 AdaBound 可以直接通过 pip 安装，不过使用 AdaBound 不意味着不用调参，只不过 AdaBound 可以帮助大家减少所需要的时间。
- 项目地址：[https://github.com/Luolc/AdaBound](https://link.zhihu.com/?target=https%3A//github.com/Luolc/AdaBound)

**为什么这篇论文很重要？**

正如作者所言，AdaBound 最重要的属性是「和 Adam 一样快、和 SGD 一样好」。其中「快」表示收敛快，深度学习模型只需要更少的算力就能收敛到最优解；而「好」表示收敛的结果好，收敛后的模型要有更高的准确率的更好的泛化性。

如下所示这张训练图展示了 AdaBound 的主要优势，其中测试准确率曲线越平滑表示最优化算法越稳定，模型的训练会少走「弯路」。而若给定 Epoch，垂直线上的高度表示收敛的快慢，测试准确率越高则收敛得越快。AdaGrad 收敛快是众所周知的，但 AdaBound 也能这么快就非常令人吃惊了。

此外，最右边近乎水平的曲线则表明各最优化算法已经完成收敛，曲线的水平高低则表明了收敛的好坏，当然测试准确率越高则表明收敛的最优解越好。我们可以看到，AdaBound 和 AMSbound 确实是收敛得最好，而 SGDM 甚至都比 Adam 收敛得好。
![](https://pic4.zhimg.com/v2-27b5ef034b761da1025ebefb3f909f5f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='531'></svg>)
在去年 ICLR 2018 的最佳论文《[On the convergence of Adam and Beyond](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650738083%26idx%3D1%26sn%3Db2582e000f7226bea9b42b77b532a53d%26chksm%3D871ac9ddb06d40cbcbb5f10855efaa426de6dfd67557d6b12d9a986cad85996792f79222de74%26scene%3D21%23wechat_redirect)》中，作者首次分析了为什么 Adam 等新提出的方法会收敛得不好，该论文提出的 AMSGrad 尝试解决这个问题。但是在这篇新论文中，北大骆梁宸等研究者表明 AMSGrad 实际上也没有解决问题，因此他们提出了新的解决方案：AdaBound。

**为什么 Adam 不行？**

目前 Adam 可以说是最常用的最优化器，它和传统的随机梯度下降有很多不同。SGD 保持单一的学习率来更新所有权重，学习率在训练过程中并不会改变，Adam 则通过计算梯度的一阶矩估计和二阶矩估计，为不同的参数设计独立的自适应性学习率。

听起来 Adam 好像高级了很多，但在 Reddi 等人的 ICLR 2018 最佳论文中，他们表示 Adam 等算法在经验上常会收敛得比带动量的 SGD 方法差，他们证明了导致该问题的一个原因是这些算法使用了指数滑动平均（exponential moving average）操作。这也是去年的研究重点，很多研究者尝试改良 Adam 以解决收敛差这个问题。

在 AdaBound 这篇论文中，北大等研究者表明当用 Adam 训练的模型接近收敛时，学习率基本上由小于 0.01 的学习率和大于 1000 的学习率组成，这就表明训练的最后阶段确实存在学习率太小或太大的问题。在作者对这个现象进行深入研究后，他们表示极端学习率确实存在潜在不良影响。算法在不解决这个问题的情况下不可能获得很好的泛化性能，因此我们需要以某种方法限制 Adam 在训练后期的学习率。

鉴于这个观察 Adam 也就能改良成 AdaBound 了。

**这篇论文干了什么？**

AdaBound 这篇论文首先对 ADAM 进行了实证研究，并说明在训练结束时，学习率存在极大和极小两种情况。该结果与 Wilson 等人（2017）提出的观点一致，后者认为自适应方法的泛化性能较差要归因于不稳定和极端的学习率。实际上，AMSGRAD 中的关键思想——引入非递增学习率，可能有助于减轻极大学习率造成的不良影响，虽然它忽略了较小学习率的可能影响。AdaBound 论文的作者进一步提供了简单凸优化问题的示例，以阐明自适应方法的极小学习率如何导致无法收敛。在这种情况下，可证明 RMSPROP 和 ADAM 无法收敛到最优解，且不管初始步长 α 有多大，ADAM 都无法与 scale-down term 对抗。

基于以上分析，作者提出了 ADAM 和 AMSGRAD 的新变体，分别是 ADABOUND 和 AMSBOUND，新变体不会受到极端学习率的消极影响。作者对这些自适应方法中的学习率应用了动态边界，其中上下界被初始化为 0 和无穷大，而且它们都能平滑收敛至恒定的最终步长。新变体在训练初期可视为自适应方法，然后逐渐随着步长的增加转化为 SGD（或带动量的 SGD）。使用该框架中可以获得快速的初始训练进程和良好的最终泛化能力。

**论文：Adaptive Gradient Methods with Dynamic Bound of Learning Rate**
![](https://pic2.zhimg.com/v2-382c91d44f7bd2745b217549ce2c68e1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='385'></svg>)
论文地址：[https://openreview.net/pdf?id=Bkg3g2R9FX](https://link.zhihu.com/?target=https%3A//openreview.net/pdf%3Fid%3DBkg3g2R9FX)

摘要：自适应优化方法（如 AdaGrad、RMSprop、Adam）旨在通过调整学习率上的元素级缩放项来获取快速的训练过程。尽管这些方法很普遍，但它们与 SGD 相比泛化性能较差，甚至会由于不稳定和极端的学习率导致无法收敛。近期研究提出了一些新算法（如 AMSGrad）来解决这个问题，但这些算法相比现有算法并没有太大改进。这篇论文展示了极端学习率会导致性能不好。此外，该论文还提供了 Adam 和 AMSGrad 的新变体——AdaBound 和 AMSBound，它们对学习率使用动态边界，以实现从自适应方法到 SGD 的逐步平滑过渡，为收敛提供理论依据。作者在多个流行任务和模型上进行了实验，而这在之前的研究中是匮乏的。实验结果表明这些新变体能够弥合自适应方法和 SGD 之间的泛化差距，同时在训练早期保持更高的学习速度。此外，这些新变体相比原版有巨大改进，尤其是对于复杂深度网络。

**极端学习率导致的无法收敛**

在这部分中，研究者通过初步的实验和严格的证明阐述了当前自适应方法的主要缺陷。如上所述，根据观察结果 ADAM 这样的自适应方法表现比 SGD 要差。Reddi 等人（2018）提出了 AMSGRAD 来解决这个问题，但最近的研究指出 AMSGRAD 并没有明显的改进。Reddi 等人表示与 ADAM 相比，AMSGRAD 的学习率更小，但他们仅认为较大的学习率是造成 ADAM 表现糟糕的原因。然而，较小的学习率可能也是陷阱。因此，本研究的作者认为极大和极小学习率都是导致 ADAM 泛化能力平平的原因。

**动态边界的自适应矩估计**

这部分介绍了该研究开发的新优化方法，并提供了收敛性分析。该研究旨在设计一个最优化策略，它能够结合自适应优化方法的优势（即快速的初始化进度）和 SGD 良好的泛化性能。直观地说，研究者希望构建一种算法，能够在训练早期表现得像自适应方法，在训练后期表现得像 SGD。
![](https://pic2.zhimg.com/v2-c0144e4498c66a921d9a4f75a723a16d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='351'></svg>)
受梯度裁剪（一种实践中常用的技术，用于裁剪大于阈值的梯度以避免梯度爆炸）的启发，本文研究者对 ADAM 中的学习率进行了裁剪，提出了 ADABOUND，如算法 2 所示。考虑在 ADAM 中应用以下操作：
![](https://pic4.zhimg.com/v2-f1511085f7ab9a3b26bf4da0ec442eab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='154'></svg>)
通过逐元素地裁剪学习率，它使得输出限制在 [η_l , η_u] 内。由此可见，α = α* 的 SGD(M) 可视为 η_l = η_u = α*。至于 ADAM，它的范围则相当于 η_l = 0 以及 η_u = ∞。现在我们可以通过以下步骤提供新的策略。我们将 η_l 和 η_u 作为 t 的函数，而不是常数的上下界，其中η_l(t) 是非递减函数，在 t=0 时它从 0 开始，渐近收敛至 α*；η_u(t) 是非递增函数，在 t=0 时它从 ∞ 开始，也渐近收敛至 α ∗。在这种设定下，ADABOUND 在开始表现得像 ADAM，因为上下界对学习率的影响非常小，而后面逐渐表现得像 SGD(M)，因为边界变得越来越受限。研究者证明了 ADABOUND 的以下关键结果：
![](https://pic3.zhimg.com/v2-289afe4a9c1c698031ab6f9ff20285c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='433'></svg>)
不难看出 ADABOUND 的后悔值（regret）上限是 O(sqrt(T))。与 Reddi 等人（2018）的做法类似，研究者可以使用更谨慎的动量衰减率 β_1t = (β_1)/t，仍确保 O( sqrt(T)) 的后悔值。值得一提的是，我们还可以在 AMSGRAD 中加入动态边界。由此产生的算法 AMSBOUND 也保持了 O(sqrt(T)) 的后悔值，而收敛的证明基本与理论 4 一样（详见附录 F）。在下一部分中，我们会看到在一些已知的任务上，AMSBOUND 具有与 ADABOUND 相似的性能。

**实验**

在此部分，研究者对不同的模型进行实证研究，将新方法与常用优化方法进行对比，包括 SGD(M)、ADAGRAD、ADAM 和 AMSGRAD。实验着重于三个任务：MNIST 图像分类任务、CIFAR-10 图像分类任务、在 PTB 上的语言建模任务。
![](https://pic4.zhimg.com/v2-480a18489935e9d0b0bb70e373909df7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='927' height='338'></svg>)表 2：本文实验所用模型概述
图 2 展示了每个优化方法在训练和测试集上的学习曲线。研究者发现在训练上，所有的算法都能达到近 100% 的准确率。而在测试上，SGD 比 ADAM 和 AMSGAD 自适应方法表现略好。
![](https://pic1.zhimg.com/v2-b730af9e1e1d1bb973907bc31cee4f98_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='371'></svg>)图 2：前馈神经网络在 MNIST 上的训练（左）与测试（右）准确率
图 3 中，ADABOUND 和 AMSBOUND 甚至超越了 SGDM 1%。尽管自适应方法的泛化能力相对较差，但该研究提出的方法通过为自适应学习率分配边界克服了这一缺陷，使得 DenseNet 和 ResNet 在 CIFAR-10 测试集上几乎取得了最佳准确率。
![](https://pic3.zhimg.com/v2-17248edef8376234a34a2ce9caa9504a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='718'></svg>)图 3：DenseNet-121 和 ResNet-34 在 CIFAR-10 上的训练与测试准确率
然后作者在 PTB 数据集上训练一些 LSTM，固定预算下运行 200 个 epoch。他们使用困惑度作为性能评估的标准，结果如图 4。
![](https://pic4.zhimg.com/v2-b352dd2c71253eaf0a7fb13a63f2a39b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1060' height='780'></svg>)图 4：在 PTB 上使用不同层的 LSTM 对比 SGD、ADAM、ADABOUND 和 AMSBOUND 困惑度曲线
**匿名评审结果**

这篇论文已被 ICLR 2019 接收，评审得分为：7，4，6。

匿名评审 2 给出了 7 分，Ta 认为作者在多个常用机器学习测试集中验证了所提出 ADAM 和 AMSGrad 新变体的效果，同时对凸优化问题提供了详细的证明，也因此 ADAM 后悔值（regret）不能收敛为零。

但 Ta 同时认为这篇论文中的证明仍然不够有说服力。性能差的情况确实存在，但这无法说明算法的平均性能，而这在实践中很关键。此外，这篇论文的验证数据集较少，数据集规模也不够大，该评审建议作者在较大的数据集上进行实验，以使结果更具说服力。

作者对此回应道，他们在 CIFAR-100 等较大数据集和其它任务上也进行了实验，结果都很不错。之后会把相关结果添加到论文中。

匿名评审 3 打出了 4 分（rejection）。Ta 对论文的技术细节提出了几点质疑，主要涉及 η_u 和 η_l 的公式及结果、研究结果在其它算法上是否有同样的效果、学习率的选择和变化图等方面。

匿名评审 1 给出了 6 分，Ta 认为这篇论文提出了一个简单的方法来避免标准自适应方法中的学习率极端行为，但理论不足，无法说明该方法相比标准自适应方法的优势。

**reddit 讨论**

这篇论文在 reddit 上也引起了大量讨论。

很多人对该研究表示赞赏，还有一些人询问该研究是否针对 GAN、迁移学习进行了实验，以及是否有 TensorFlow 实现。作者表示暂未针对 GAN 和迁移学习进行实验，且由于对 TensorFlow 不太了解，因此暂时没有 TensorFlow 实现。

reddit 用户 Berecursive 就实验结果的持续性（consistent）提问：如果我将同样的实验运行 100 次，那么我会得到比较合理的训练和测试误差的方差分布。通常误差图的形状是一致的，但是如果你要绘制方差图，则其范围会因任务而变得非常宽泛。对于优化方法而言，结果的一致性也是一个重要的度量标准。

作者对此的回应是：我在特定超参数设置下运行了 3-5 次，发现在相同设置下 AdaBound 的学习曲线很接近。之前我并不知道如何绘制方差图，稍后会进行尝试。此外，我在附录 G 中就 AdaBound 的一致性问题进行了一些额外实验研究，使用的是不同的超参数设置。此外，我们还提供了 Jupyter notebook，可以帮助可视化和复现。

此外，与匿名评审 2 类似，reddit 用户 alper111 也对实验用数据集 CIFAR-10 提出了质疑：使用 CIFAR-10 来测试泛化误差有些奇怪吧？这个数据集中的样本与训练数据集中的样本有很大的重复性。

论文作者对此的回应是：CIFAR-10 是一个完备的基准数据集，完全可以使用，近似重复问题并不能改变这个事实。此外，使用 CIFAR-10 数据集方便与其它基于该数据集的结果进行对比。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*







