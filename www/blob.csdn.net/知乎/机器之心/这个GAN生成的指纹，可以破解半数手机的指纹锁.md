# 这个GAN生成的指纹，可以破解半数手机的指纹锁 - 知乎
# 



选自 arXiv，作者：Philip Bontrager 等，机器之心编译。

> 2017 年，来自纽约大学坦登工程学院和密歇根州立大学的 Roy 等人提出，智能手机与其他电子产品所使用的安全辨识系统只需要一部份的指纹相似性就足以开机解锁，这种以指纹为基础的系统安全性比想象中更脆弱。这篇论文提出了 MasterPrints 的概念（万能指纹，可怕！）。然而，你以为这就足够了吗？近日，Philip Bontrager、Aditi Roy 等人再次提出破坏性更大、能够匹配更多指纹的 DeepMasterPrints，该研究基于真实指纹图像，利用潜变量进化和 GAN，生成可比 MasterPrints 匹配更多指纹的 DeepMasterPrints。研究指纹识别系统的漏洞有利于进一步提升这些系统的安全性，以便更好地保护用户。该论文已被 IEEE BTAS 2018 大会接收。

GAN 生成的指纹究竟可以多逼真？研究人员尝试使用 DeepMasterPrints 去攻击手机的指纹解锁，破解成功率为 22-78%（成功率取决于手机指纹传感器的安全等级）。目前看来，它是最具「攻击性」的对抗样本了。

指纹被越来越多地用于核实个人身份，其应用范围非常广泛，包括门锁解锁、手机解锁、授权付款等。出于人体工程学方面的考量，手机解锁等应用中的指纹传感器非常小，因此，这些传感器只获取用户指纹的部分图像。由于一小部分指纹的辨识度低于完整指纹，因此（一个手指上的）部分指纹与（另一个手指上的）部分指纹错误匹配的几率更高。Roy 等人在论文《MasterPrint: Exploring the Vulnerability of Partial Fingerprint-Based Authentication Systems》中利用该观察引入了 MasterPrints 的概念。MasterPrints 是一组真实或合成的指纹，可能和大量其他指纹意外重合。因此，对手可以利用它们启动一个针对特定主体（subject）的字典攻击，从而破坏指纹识别系统的安全性。也就是说，在不获取任何主体指纹相关信息的情况下「伪造」该主体指纹是可以实现的。

Roy 等人 [25] 表明，MasterPrints 既可以从真实指纹图像中获取，也可以利用爬山算法合成。合成的 MasterPrints 是通过在指纹模板中修改指纹特征点（minutiae point）生成的「模板级」MasterPrints。[25, 24] 中的方法不生成图像。然而，要想在实践中发起伪造-攻击（spoof-attack），必须在构建「图像级」MasterPrints，之后将其转变为人造指纹。这一观察驱使我们去寻找一种生成 DeepMasterPrints（视觉上与自然指纹图像相似的图像）的方法。

为了设计 DeepMasterPrints，我们需要一种搜索指纹图像空间的可行方法。由于并非所有指纹系统都使用指纹特征点 [20, 21]，因此如果没有在设计过程中充分利用指纹特征点信息（与 [25, 24] 不同），那么这种做法是有利的。现在，神经网络可以用于生成合成指纹图像。具体来说，生成对抗网络（GAN）已经证明了其在生成与特定风格或领域一致的图像方面的广阔前景 [5, 8, 7]。然而，它们的标准设计是不可控的。也就是说，它们不允许生成器指定额外的约束和目标，只能复现训练数据的风格。对于 DeepMasterPrints 来说，我们需要创建一个能骗过指纹匹配器的合成指纹图像。指纹匹配器不仅需要意识到图像是合成的（视觉真实感），还需要将许多不同的身份与该指纹图像匹配。因此，生成器网络必须与 DeepMasterPrints 的搜索方法相结合。

该论文展示了一种创建 DeepMasterPrints 的方法，该方法使用神经网络学习生成指纹图像。然后使用进化优化来搜索该神经网络的潜在变量空间。协方差矩阵自适应进化策略（Covariance Matrix Adaptation Evolution Strategy，CMA-ES）用于搜索完美指纹图像的已训练神经网络的输入空间。进化优化和生成神经网络的独特结合使得该神经网络可以约束搜索空间，同时进化算法可以处理离散适应度函数。

该研究首次创建了图像级合成 Masterprint，进一步强化了在指纹应用中利用分辨率低的小型传感器的风险。该研究直接展示了如何利用它以 0.1% 的错误匹配率伪造 23% 的主体指纹。生成的 DeepMasterPrints 在 1% 的错误匹配率下能够伪造 77% 的主体指纹。




**论文：DeepMasterPrints: Generating MasterPrints for Dictionary Attacks via Latent Variable Evolution**
![](https://pic3.zhimg.com/v2-48def06eb8907f2ad146c44c96ac313a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='172'></svg>)
论文链接：[https://arxiv.org/pdf/1705.07386.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1705.07386.pdf)

**摘要**：最近的研究证明指纹识别系统在面对基于 MasterPrints 的字典攻击时的脆弱性。MasterPrints 是真实或合成的指纹，可能和大量真实指纹意外重合，从而破坏指纹系统的安全性。Roy 等人之前的研究生成了特征级别的合成 MasterPrints。本研究则生成了完全图像级别的 MasterPrints，并将其命名为 DeepMasterPrints，其攻击准确率优于之前的方法。我们提出的方法叫作潜变量进化（Latent Variable Evolution，LVE），以在真实指纹图像数据集上训练生成对抗网络为基础。使用协方差矩阵自适应进化策略形式的随机搜索被用于搜索生成器网络的潜在输入变量，它可以最大化指纹识别器评估的合成指纹匹配数。实验表明了该方法在生成 DeepMasterPrints 时的有效性。这一方法可能在指纹安全和指纹合成方面有广泛的应用。




**提出的方法**

生成 DeepMasterPrint 的理想系统应该能够 (a) 生成每一张可能的图像；(b) 在现有的所有指纹匹配器上测试每张图像；(c) 选择成功匹配最多指纹的图像，不同指纹意味着不同的身份。由于不可能获取每一个指纹匹配器，因此必须基于身份和匹配器样本得到 DeepMasterPrint，然后使之泛化。将图像范围限制在指纹图像范围内有助于泛化。该研究提出的理想系统能够生成任意指纹图像，并搜索身份和匹配器样本来找到完美的解决方案。该方法不仅能够生成图像，还可以找到比之前方法更高效的解决方案。为实现该方法，研究者开发了一种新方法——潜变量进化（Latent Variable Evolution，LVE）。

LVE 包括两个部分：1. 训练一个神经网络，用来生成指纹图像；2. 搜索该网络的潜在变量（生成器网络的输入向量），找到能够带来最优 DeepMasterPrint 的指纹，即该指纹图像能够与其他指纹图像匹配。为了训练图像生成器，该研究使用 WGAN 方法，然后使用 CMA-ES 来演化指纹。该方法在两个指纹数据集和多个不同匹配器上进行了测试。
![](https://pic4.zhimg.com/v2-8422476ceff351c1fa9539a68defc363_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='557'></svg>)图 1：生成器网络架构。判别器与该模型相反，判别器使用的是子采样而不是上采样，使用的激活函数是 LeakyReLu![](https://pic3.zhimg.com/v2-e542023054797f47a6853133c2ac6312_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='253'></svg>)图 2：使用已训练网络的潜变量进化。左图是 CMA-ES 的高级概览，右图展示了潜变量的评估过程
**实验**

实验使用了两种类型的指纹图像，分别是根据手指蘸墨水后在纸上按压的指纹扫描而成，以及从电容传感器收集的指纹。前者是 NIST Special Database 9 指纹数据集，后者是 FingerPass DB7 数据集。以下是实验结果：




**生成的指纹**
![](https://pic2.zhimg.com/v2-e2bd3542d0ab62916b4f9a3d254fb389_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='342'></svg>)
**DeepMasterPrints**
![](https://pic4.zhimg.com/v2-0d89caaba26b8ccd12e9be9ec7044783_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='349'></svg>)图 4：按压指纹的进化 DeepMasterPrints（上），电容式指纹的进化 DeepMasterPrints（下）。从左至右，每个指纹分别为 0.01%、0.1% 和 1% 的 FMR 进行优化![](https://pic4.zhimg.com/v2-0bf9bb00b41f55636e90cefcab3e8b73_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='78'></svg>)表 1：在电容式指纹数据集上的成功匹配结果。图 4 中的 DeepMasterPrints 是为 VeriFinger 软件而优化的，有三种安全级别，训练数据集是电容式指纹数据集。每种 DeepMasterPrints 在测试集上的有效性见上表
**泛化**
![](https://pic3.zhimg.com/v2-4fa04081d1b985d89f542e88a5affa5e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='295'></svg>)表 2：为最高安全级别优化的 DeepMasterPrints 泛化效果最好
**对比结果**
![](https://pic3.zhimg.com/v2-f130ecc7d40afb23f06a1efd7fc97ebe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='123'></svg>)表 3：使用 DeepMasterPrint 匹配的主体在生成 MasterPrints 方面的提升。该结果是在电容式指纹数据集上训练得到的，使用的是 VeriFinger 匹配器*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*



