# 来势汹汹PyTorch！ICLR论文提及频率直追TensorFlow（附对比） - 知乎
# 



机器之心报道，作者：思源、李亚洲。

> 昨日，Facebook 在首届 PyTorch 开发者大会发布了 [PyTorch1.0 预览版](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749442%26idx%3D1%26sn%3Dca773e5a904e4a024e52ae6b998cf97f%26chksm%3D871afe7cb06d776a52a4f8ace7f39cf3130bb2f7a4e256e7acf18bba8b06f9ab02ead7fb085c%26scene%3D21%23wechat_redirect)，标志着这一框架更为稳定可用。从[去年年初发布](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650722553%26idx%3D1%26sn%3Dce635e60fa8f1cc16982c5d6a9a6931b%26chksm%3D871b1487b06c9d9180d7f881784e68d4b9785481c38aa86eccc183aed8254b2a452e073a0c9b%26scene%3D21%23wechat_redirect)以来，PyTorch 已经成为明星框架，发展速度甚至有力压 TensorFlow 的趋势。据网友统计，在最新的 ICLR 2019 提交论文中，提及 TensorFlow 的论文数量从 228 升到了 266，而提及 PyTorch 的论文数量从 2018 年的 87 激增到了 252，这是否也是 PyTorch 即将赶超 TensorFlow 的又一证明？




**ICLR 提交论文提及频率**

今日，Reddit 上的一条帖子吸引了大家的关注：有网友统计，相比于 2018 年，在 ICLR 2019 提交论文中，提及不同框架的论文数量发生了极大变化。

首先，说下 2018 年和 2019 年论文提交数量。ICLR 2019 将于明年 5 月 6 日-9 日在美国新奥尔良举行，今年 9 月 27 日下午 18 时，大会论文提交截止。据统计，ICLR 2019 共收到 1591 篇论文投稿，相比去年的 1000 余篇增长了 60%。

其次，介绍下统计方法，相当简单。在 Frankensteinian search 搜索框下分别搜索提及不同框架的论文结果，如下：
![](https://pic3.zhimg.com/v2-94c964de46b96d20a05b3a222b8c2b22_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='276'></svg>)
网友发现，提及 TensorFlow 的论文数量从 2018 年的 228 篇略微提升到了 266 篇，Keras 从 42 升到 56，但 Pytorch 的数量从 87 篇提升到了 252 篇。从数据上可以明显看出，采用 PyTorch 的 ICLR 论文在这一年内几乎要超越 TensorFlow。
- TensorFlow：228→266
- Keras: 42→56
- Pytorch：87→252

在 PyTorch 1.0 推出之际，这样的数据统计让我们不得不联想到：TensorFlow 的深度学习框架霸主地位是否还保得住？既然 PyTorch 1.0 预览版已经发布，那么让我们再把两个框架放在一起对比下，看哪一款才是适合你的深度学习框架。




**TensorFlow VS PyTorch**

自 2015 年开源以来，深度学习框架的天下就属于 TensorFlow。不论是 GitHub 的收藏量或 Fork 量，还是业界使用量都无可比拟地位列第一。
![](https://pic2.zhimg.com/v2-0442453e5924bf88d8d9aabab58c3109_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='598'></svg>)TensorFlow 的版本迭代
但是 TensorFlow 有一个令人诟病的劣势，即它和 Theano 一样采用的是静态计算图，这令神经网络的搭建和入门学习都变得更加困难。因此在 2017 年 1 月，Torch7 团队开源了 PyTorch，它的宗旨是尽可能令深度学习建模更加简单。
![](https://pic4.zhimg.com/v2-86acd4c725aca54f89fd974971ef2943_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='553'></svg>)PyTorch 的版本迭代
其实机器之心在很多对比文章中都发现 TensorFlow 的使用在目前来说还是最为广泛的，但是 PyTorch 的发展势头非常迅猛，尤其是在学术研究领域的应用上。那么为什么新近开源的 PyTorch 会那么受欢迎呢，首先我们需要了解深度学习框架的关键点：
- 易于构建大型计算图
- 易于在计算图中进行梯度运算
- 能在 GPU 上高效运行（cuDNN、cuBLA 等）

在后两项中，基本上 TensorFlow 和 PyTorch 都能实现高效的自动微分机制和并行运算机制。但是在第一项中，PyTorch 的哲学是解决当务之急，也就是说即时构建和运行计算图，这与 TensorFLow 先建立静态计算图再发生实际运算相比要简单地多。因此在第一项上，PyTorch 具备很大优势，但是 TensorFlow 的静态计算图更有利于部署模型，且现在同样也非常关注动态计算图。




**工业化的缺陷**

PyTorch 最开始发布以来，大家都偏向于使用它做学术研究，而不是用于实际生产。主要的原因可能有两点：首先它比较新，还不太成熟，因此很多 API 接口和结构也都不太稳定；其次是动态计算图在部署上不太方便，而像 TensorFlow 这样的静态图可以在不同的环境下调用计算图和对应参数，因此很容易部署到各种产品中。

由于 PyTorch 与 Python 有着紧密的结合，因此将这种动态计算图部署到其它产品会比较困难。不论是训练脚本还是预训练模型，我们经常需要将研究代码转换为 Caffe2 中的计算图表征，从而实现生产规模上的高效使用。其中 Caffe2 项目是两年前提出的，其目的是标准化 AI 模型的生产工具，目前该框架在 Facebook 服务器以及超过 10 亿台手机上运行，横跨了八代 iPhone 和六代安卓 CPU 架构。

之前，从 PyTorch 到 Caffe2 的迁移过程是手动的，耗时间且容易出错。为了解决这个问题，Facebook 与主要的硬件和软件公司合作创建了 ONNX（开放神经网络交换格式），这是一种用于表示深度学习模型的开放格式。通过 ONNX，开发者能在不同的框架间共享模型，例如我们可以导出由 PyTorch 构建的模型，并将它们导入到 Caffe2。

通过 ONNX 和 Caffe2，使用 PyTorch 构建的研究结果可以快速地转化到生产中。而且昨日发布的 PyTorch 1.0 预览版也标志着 PyTorch 开始走向成熟，很多 API 接口和框架结构也都会变得更加稳定，这些都非常有利于将 PyTorch 应用于实际生产中。




**性能对比**

这两种深度学习框架都有各自的特点，那么它们在相同硬件（GPU）上运行相同神经网络的性能又怎么样？Ilia Karmanov 在 GitHub 上开源了一项测试，他在相同的环境下测试由不同框架编写的相同模型，并借此讨论不同框架的性能。从这些数据中，我们可以了解到在性能上，TensorFlow 和 PyTorch 并不会有显著的差别，不过在特定的任务上还是有一些不同。

项目地址：[https://github.com/ilkarman/DeepLearningFrameworks](https://link.zhihu.com/?target=https%3A//github.com/ilkarman/DeepLearningFrameworks)

以下展示了使用 VGG 在 CIFAR-10 上实现图像分类的速度：
![](https://pic1.zhimg.com/v2-0d17e4eaca44585a02e3e4cdda9db7cc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='868' height='789'></svg>)
以下展示了 DenseNet-121 在 ChestXRay 数据集上的训练速度，在这个图像识别任务中，PyTorch 要比 TensorFlow 表现得更好一些：
![](https://pic3.zhimg.com/v2-53857aae7044e48387dcd4af6eef039e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='998' height='404'></svg>)
如下展示了在 IMDB 数据集上训练门控循环单元（GRU）的速度，它们实现的是情感分析任务。对于循环神经网络，PyTorch 和 TensorFlow 的性能差不多，不过 PyTorch 在 P100 芯片上普遍表现得比 TensorFlow 好。
![](https://pic4.zhimg.com/v2-f2b844b3b396e6725c3eb24a0afc4e6b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='624'></svg>)
该项目还有更多的对比与分析，感兴趣的读者可查看原 GitHub 项目。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*



