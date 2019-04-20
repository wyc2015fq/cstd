# 谷歌云TPU服务正式全面开放：「AlphaGo背后的芯片」进入商用化 - 人工智能学家 - CSDN博客
2018年02月13日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：108
![640?wx_fmt=png&wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9PM1aSRQx6IicbEkhboc3qofdDicpLzfOVGR0ADv6LZMWmE3CQJkXNv7D4ia8ksEiaibOTJoUfr480sdQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&retryload=1)
*来源：机器之心*
*概要：作为科技巨头的谷歌早已把这种高度定制化产品应用在了自己的服务器中，而本周一，谷歌宣布其他公司马上也将可以享受新型芯片带来的计算服务了。*
2016 年 5 月，谷歌向世人发布了一款特别的机器学习专属芯片：张量处理器（Tensor Processing Unit，TPU），去年又推出了它的第二代产品（Cloud TPU）。这是一种被认为比 CPU 、甚至 GPU 更加高效的机器学习专用芯片。作为科技巨头的谷歌早已把这种高度定制化产品应用在了自己的服务器中，而本周一，谷歌宣布其他公司马上也将可以享受新型芯片带来的计算服务了。
虽然新一代 TPU 的适应性如何还有待观察，但是较之于单纯的机器学习任务加速的功能而言，TPU 确实颇具实力。据谷歌称，第一代 TPU 仅能够处理推理任务，而第二代 TPU 还可以用于机器学习模型的训练，这个机器学习过程中重要的一部分完全可在单块、强大的芯片上进行。在 2017 年 4 月，谷歌曾通过一篇论文《In-Datacenter Performance Analysis of a Tensor Processing Unit》介绍了 TPU 研究的相关技术以及第二代芯片与其它类似硬件的性能比较结果（参见：谷歌发布 TPU 研究论文，神经网络专用处理器是怎样炼成的？）。
TPU 可以帮助谷歌的各类机器学习应用进行快速预测，并使产品迅速对用户需求做出回应。谷歌称，TPU 已运行在每一次搜索中；TPU 支持作为谷歌图像搜索（Google Image Search）、谷歌照片（Google Photo）和谷歌云视觉 API（Google Cloud Vision API）等产品的基础的精确视觉模型；TPU 也帮助了谷歌神经翻译质量的提升；而其强大的计算能力也在 DeepMind AlphaGo 对阵李世乭的重要胜利中发挥了作用——这是计算机首次在古老的围棋比赛中战胜人类世界冠军。
谷歌本周一的「Beta 测试」公告也宣示着这家现代科技企业正在改变自己的运营理念——它已开始涉及人工智能专属芯片的解决方案，这是一个包含数十家创业公司，以及英特尔、高通和英伟达这样的传统硬件厂商的重要市场。随着时代的发展，谷歌、亚马逊和微软已不再是纯粹的互联网企业，它们都已或多或少地开始扮演起硬件制造者的角色。
在此之前，谷歌其实也并不是 TPU 的唯一使用者，美国出行服务公司 Lyft 在去年底开始参与了谷歌新型芯片的测试。Lyft 希望通过使用 TPU 加速自动驾驶汽车系统的开发速度：TPU 在计算机视觉模型的训练速度上具有优势，可将原先耗时数日的任务缩短至几小时内完成。
谷歌在其云平台博客上宣布了 TPU 服务开放的消息：
通过谷歌云平台（GCP）提供的 Cloud TPU beta 版自 2018 年 2 月 12 日起可用，其旨在帮助机器学习专家更快地训练和运行 ML 模型。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9PM1aSRQx6IicbEkhboc3qoxcReJxqWxRJIk0ZicgTrBS16cib7897yjKSib2XFa6bmQDicTu90PcibCIg/640?wx_fmt=png)
Cloud TPU 是谷歌设计的一种硬件加速器，旨在优化以加速和扩大使用 TensorFlow 编程的机器学习工作负载。Cloud TPU 使用四个定制化 ASIC 构建，单个 Cloud TPU 的计算能力达到 180 万亿次浮点运算，具备 64 GB 的高带宽内存。这些板卡可单独使用也可通过超快的专门网络联合使用以构建数千万亿次级别的机器学习超级计算机，我们称之为「TPU pod」。今年稍后，我们将在 GCP 上提供更大型的超级计算机。
我们设计 Cloud TPU 的目的是为 TensorFlow 工作负载提供差异化的性能，使 ML 工程师和研究者实现更快迭代。例如：
- 
你们无需费力等待调度共享计算机集群，通过谷歌计算引擎 VM，就可以独立获取交互式的网络联结 Cloud TPU。
- 
无需花费数日或数周等待商用级机器学习模型，你可以在一系列 Cloud TPU 上训练同样模型的不同变体，而且第二天就可以将准确率最高的训练模型部署到生产过程。
- 
使用单个 Cloud TPU，并遵循该教程（https://cloud.google.com/tpu/docs/tutorials/resnet），你可以在不到一天的时间内，训练 ResNet-50 使其在 ImageNet 基准挑战上达到期望的准确率。
**让机器学习模型训练更容易**
传统上，编写自定义 ASIC 和超级计算机的程序需要极高的专业度。而对于 Cloud TPU 而言，你可以使用高级 TensorFlow API 进行编程，我们开源了一系列参考高性能 Cloud TPU 模型实现，帮助大家立刻上手：
- 
ResNet-50（https://cloud.google.com/tpu/docs/tutorials/resnet）和其他常用的图像分类模型（https://github.com/tensorflow/tpu/tree/master/models/official）。
- 
用于机器翻译和语言建模的 Transformer（https://cloud.google.com/tpu/docs/tutorials/transformer (https://research.googleblog.com/2017/08/transformer-novel-neural-network.html)）
- 
用于目标检测的 RetinaNet（https://github.com/tensorflow/tpu/blob/master/models/official/retinanet/README.md）
为了节约大家的时间和精力，我们持续测试这些模型实现的性能和在标准数据集上收敛至期望准确率的情况。
以后，我们还将开源其他模型实现。喜爱冒险的机器学习专家可以使用我们提供的文档（https://cloud.google.com/tpu/docs/）和工具（https://cloud.google.com/tpu/docs/cloud-tpu-tools）利用 Cloud TPU 自己优化其他 TensorFlow 模型。
现在开始使用 Cloud TPU，今年稍后我们宣布 TPU pod 时，你们将从时间-准确率的大幅改进中受益良多。正如我们在 NIPS 2017 上宣布的那样，在完整的 TPU pod 上，ResNet-50 和 Transformer 的训练时间从大半天降到不到 30 分钟，而且无需改变任何代码。
**可扩展的机器学习平台**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9PM1aSRQx6IicbEkhboc3qoYAWmKQEZwkKcSy5fA0JjR6PxApBhPRz8R49NuDUoNYmGibqbVFUG09w/640?wx_fmt=png)
云 TPU 同样简化了对机器学习计算资源的规划和管理：
- 
你可以为自己的团队提供顶尖的机器学习加速，随着需求的变化动态调整自己的容量；
- 
相比于花费资金、时间和专业人才来设计、安装、维护一个实地的机器学习计算群（它还需要专门化的供能、冷却、网络和存储），你可以从谷歌多年以来优化过的大规模、高集成的机器学习基础设施受益；
- 
不需要花费努力保证驱动对各种工作站和服务器保持更新升级，云 TPU 做了不需要驱动安装的预配置；
- 
受谷歌云服务提供的同样复杂的安全机制和实践的保护。在谷歌云，我们还想为客户的每个机器学习负载提供最好的云。伴随谷歌云 TPU，我们也会提供大量的高性能 CPU（包括英特尔 Skylake）和 GPU（包括英伟达的 Tesla V100）。
**开始使用云 TPU**
云 TPU 如今在数量受限的情况下可用，价格以秒计费，大约为每云 TPU 每小时 6.50 美元。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
