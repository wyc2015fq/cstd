# 学习世界模型，通向AI的下一步：Yann LeCun在IJCAI 2018上的演讲 - 人工智能学家 - CSDN博客
2018年07月17日 22:50:25[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：141
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXwbHPcOswdYdDH97WX31GDHssyITqrB2siaWgMIMP9WF8WDiajIDAPF2El1WlLm9GynkBe7hZ09cicg/640?wx_fmt=png)
来源：机器之心
摘要：人工智能顶会 IJCAI 2018 的主要议程于昨日在瑞典首都斯德哥尔摩开始。昨天上午，Facebook 首席人工智能科学家、纽约大学教授 Yann LeCun 在会上发表了近一个小时，以《Learning World Models: the Next Step towards AI》为主题的演讲，引起了人们的广泛关注。本文将对 LeCun 的演讲进行简要介绍。
完整演讲视频：

Yann LeCun 开场介绍说，当前几乎所有的机器学习从业者在实践中使用的都是监督式学习：向机器展示大量的样本，然后告诉机器正确的答案与内部参数，然后就能做图像识别之类的任务。而过去几年，监督式学习有了极大的成功，应用也非常广泛。下图对比了传统机器学习与深度学习各自的特点。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJicbCM9QaguIE9DwYBicE4neLJ7QRTkfoRDFJ02BD4Ga4yiaXhOcgtXEaA/640?wx_fmt=png)
深度学习的演讲回溯到 20 世纪 50 年代，当时 AI 社区尝试构建神经网络。建立这样略为复杂的系统，你需要两个基础的运算：线性的加权和与非线性的激活函数。这里，Yann LeCun 回顾了他们在 80 年代末期提出的用来识别数字的卷积神经网络 LeNet5，从最初的识别单个目标发展到了识别多个目标。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJutKhN13GAyRmNPnAB35N3tQfxFndmibc3wFow3lExzI1QyYNkvqQrgA/640?wx_fmt=png)
很快，他们就建立了一个识别手写文件的系统。在 1995 年，他们在 AT&T 完成了一个商业系统来读取支票账号。经历了如下图展示的过程。Yann LeCun 介绍了，这是上一波人工智能浪潮下的极大成功示例。之后，人们就对神经网络的研究失去了兴趣。一方面是准确率的原因，还因为很多领域当时没有足够多的数据来训练学习系统。
接着，Yann LeCun 介绍说，实际上在 1996-2001 年这段时间内，他并没有研究机器学习，而是在研究其它东西，尤其是图像压缩。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJebGNw9VQxiaBYcCFRHJdwkFuS9icjGfOibsL0bACHqibGT7s31TT8XIZAA/640?wx_fmt=png)
下图是在 21 世纪初加入 NYU 之后用模仿学习做的研究。这个研究激发了 DARPA 的 LAGR 项目。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJj3WnmpCx6q8W11A8Bze6aB2vQMIBR2xmWYsMowJCUGwSr4icvCkeZyg/640?wx_fmt=png)
Yann LeCun 随后回顾了卷积神经网络在不同任务中的应用，包括用于自动驾驶汽车的目标检测与语义分割等。这些基于视觉的任务绝大部分都需要卷积神经网络的支持，当然也离不开并行计算设备的支持。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJgEPvwILsBd6N3cYh0GK5zcEuNrTOPWa3zmIgkELWqaVzpMbXMkSEHg/640?wx_fmt=png)
第一个得到广泛关注与应用的卷积神经网络是 2012 年提出来的 AlexNet，它相比于 LeNet-5 最大的特点是使用更深的卷积网络和 GPU 进行并行运算。AlexNet 还应用了非常多的方法来提升模型性能，包括第一次使用 ReLU 非线性激活函数、第一次使用 Dropout 以及大量数据增强而实现网络的正则化。除此之外，AlexNet 还使用了带动量的随机梯度下降、L2 权重衰减以及 CNN 的集成方法，这些方法现在都成为了卷积网络不可或缺的模块。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJjVC7AicQtFoD3tB6IzNl561cxVdvQwRUyYAf5OOt5OicnT0faFFW7jfA/640?wx_fmt=png)
随后在 ImageNet 挑战赛中，卷积网络的深度与性能都逐年提升。从 12 年到 16 年，参赛者使用的卷积神经网络不断加深，错误率也逐年下降。
如下所示，牛津大学 2014 年提出了另一种深度卷积网络 VGG-Net，与 AlexNet 相比，它的卷积核更小，层级更深。谷歌同年提出了 GoogLeNet（或 Inception-v1），该网络共有 22 层，且包含了非常高效的 Inception 模块。后来到了 15 年，何恺明等人提出的深度残差网络骤然将网络深度由十几二十层提升到 152 层，且性能大幅提高。
此外，去年提出的 DenseNet 进一步解决了 ResNet 遗留下的梯度问题，并获得了 CVPR 2017 的最佳论文。DenseNet 的目标是提升网络层级间信息流与梯度流的效率，并提高参数效率。它也如同 ResNet 那样连接前层特征图与后层特征图，但 DenseNet 并不会像 ResNet 那样对两个特征图求和，而是直接将特征图按深度相互拼接在一起。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJicibbsLnRicwEXgHpgmWx20Cz2R2m67y3hqtHbZEJfBPIW8Y4dicCx53tw/640?wx_fmt=png)
那么为什么卷积神经网络在计算机视觉任务上如此高效？Yann LeCun 随后就对深度卷积网络的表征方式做了介绍。他表明对于图像数据来说，数据的信息与结构在语义层面上都是组合性的，整体图像的语义是由局部抽象特征组合而成。因此深度网络这种层级表征结构能依次从简单特征组合成复杂的抽象特征，如下我们可以用线段等简单特征组合成简单形状，再进一步组合成图像各部位的特征。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJ7yYYYztJia8axdok3uzkkWHLBRSCWNAw1OOvUEybHYHWV36NaoRT9lw/640?wx_fmt=png)
卷积神经网络在目标识别、目标检测、语义分割和图像描述等领域都有非常多的应用，而这些实现很多都依赖于深度学习框架。LeCun 随后重点介绍了 PyTorch 与 Detectron，其中 PyTorch 因为采用了动态计算图而受到了广泛的关注，它也是当前发展最快的框架之一。
如下所示，Facebook AI 研究院开源的 Detectron 基本上是业内最佳水平的目标检测平台。据 LeCun 介绍，该项目自 2016 年 7 月启动，构建于 Caffe2 之上，目前支持目标检测与语义分割算法，其中包括 Mask R-CNN（何恺明的研究，ICCV 2017 最佳论文）和 Focal Loss for Dense Object Detection（ICCV 2017 最佳学生论文）等优秀的模型。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJu9Ml3sCMiaLgjX2kgLczoeVCOdTlEiaXE0YWQy5BbvjH33EGxfG0fD3w/640?wx_fmt=png)
最后，作为对卷积神经网络的总结，LeCun 带我们回顾了卷积神经网络的应用，包括医疗影像分析、自动驾驶、机器翻译、文本理解、视频游戏和其它学科研究。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJMujVicYbTQ5Tl9W07zd4ic59j9PnmnUAzKfeEo9PFE74aPRtO6yXtnMw/640?wx_fmt=png)
Yann LeCun 谈到当前深度学习缺乏推理能力，因此未来的一个重点发展方向就是深度学习和推理的结合。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJj1SqicvGT3IpFJlKZVMGWcqkH0nq7C4fKqbHQiarfePTia34KfSZgia0Yw/640?wx_fmt=png)
人们已经在多个方向上进行尝试。例如，在网络中增加记忆增强模块，典型的工作在下图中列出，这是实现推理的第一步。在对话模型中，由于对话的轮换和非连续的特点，通过增强记忆，有助于预测能力的提高，进而能实现长期而有效的对话。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJeNib5elMtUVCWNia0bCUal0tZ9TFP66LlmhdevlzD0UdFkZ0boq4iaPrA/640?wx_fmt=png)
为什么要强调记忆建模的重要性呢？在强化学习中，无模型的强化学习训练需要大量的尝试才能学会一项任务。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJPf9NXshCzMZmCAl5lRDbD1BsqyicEjvGUZ3OqsvIN6H7198SkesPr7Q/640?wx_fmt=png)
因此此类方法在游戏中表现良好，如 FAIR、DeepMind、OpenAI 等之前都已在许多游戏上实现接近甚至超越人类的 AI 系统，但这些系统并没有达到现实应用的水平。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJbxLy2wvibG98Ia3CNkO8oic0CYjwSwHhPJH8V2SiaQJ4HEMWqdc95JZMA/640?wx_fmt=png)
因为现实环境远远比游戏中的环境要复杂得多，无论是变量复杂度还是不确定性方面，对此，无模型的强化学习系统面对的探索空间是相当巨大的。而且，不像 AlphaGo 那样可以在计算机上模拟成千上万次比赛，现实世界环境是无法被「加速」的，有些试验还涉及很大的风险，这也大大限制了系统的训练资源。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJWVIEQ2xGPeP0BXYibHn0LrLRaWeAYjo97UDYiciaq3Fl7TTdUKtZhgyJA/640?wx_fmt=png)
那么目前来看，人工智能到底缺少了什么？监督学习需要太多的样本，强化学习需要太多的尝试，AI 系统缺乏常识。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJ0vY2bQVayRX9Dx1xw5MF9JpvgaZ9MX7gFCtCPj7CeRDW9jE7FYfOyw/640?wx_fmt=png)
Yann LeCun 总结了一下这两类系统的缺点：缺乏独立于任务的背景知识；缺乏常识；缺乏预测行为后果的能力；缺乏长期规划和推理的能力。简言之就是：没有世界模型；没有关于世界运行的通用背景知识。用更一般的语言来讲就是，目前的机器无法在心里想象（表征）世界，而只是像僵尸一样被气味驱使着（无意识地）行动。记忆建模只是一方面，建立完整的世界表征才是我们真正需要的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJkJavmaQrQrLZ95IDfBuv6ZZDA23JyVshLbGZBtS6xC8PwhFCupPZzw/640?wx_fmt=png)
在现实应用层面，Yann LeCun 总结道，利用现有的监督学习和强化学习技术，我们可以在自驾汽车、医疗图像分析、个性化医疗、语言翻译、聊天机器人（有用但还很蠢）、信息搜索、信息检索、信息过滤以及其它领域中取得不错的进展，但仍然无法实现常识推理、智能个人助理、智能聊天机器人、家庭机器人以及通用人工智能等。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJMZJT7HTR4QdndtIibR2LicTnNSric7PcywuyYQl4NaG2rb1ka2nibE7fRA/640?wx_fmt=png)
然后，Yann LeCun 指出我们可以从婴儿的学习方式上获得启发。婴儿对外部世界的概念学习大部分是通过观察，仅有小部分是通过交互，而视觉比触觉、体感等能捕获更多、更完整的外部信息。那么机器如何做到这一点？其实，这种情况不止发生在人身上，动物也是如此。LeCun 随后展示了一幅婴儿和大猩猩观看魔术的图，并解释说，当违反世界模型，也就是当我们观察到一些不寻常、与世界模型不匹配的东西时，我们的注意力就会被调动（如婴儿和大猩猩看到魔术表演会大笑就是因为世界模型被违反了）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJiaEQ0dDQSuMF0IhMg7SbcHUBkql2JOoeUPzbRsFiajibfBx7icib8Pr6zsw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJQ8ZnHR0zSIVaicCibxJd5pncTq7gsibQeKhqkHAuXbdedRXbbGKCdWkAw/640?wx_fmt=png)
接下来，LeCun 解释了突破强化学习现状的解决方法：自监督学习，它能通过输入的某一部分预测其它部分。在空间层面上包括图像补全、图像变换等，在时间层面上包括时序数据预测、视频帧预测等。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJM7QNcY33JRvXKhKD3GTNvSLx4I4SGz9olnYWhtXhAfvwbmhXgY1NGw/640?wx_fmt=png)
Yann LeCun 总结了三类学习范式，分别是强化学习、监督学习和自监督学习，相比于强化学习和监督学习，自监督学习将输入和输出当成完整的整体。它们的区别和联系在于反馈信息的逐渐增多，模型表征复杂度、适用任务类型也大幅增加，同时任务中涉及的人类工程比重也大大减少，意味着自动化程度的增加。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJINDkb6y6TzJKHYgQgiaQgQ6nFamVtq5vIS2pVscZONxPXvsNib5wyjLw/640?wx_fmt=png)
LeCun 还用之前经常使用的蛋糕比喻来说明三者的关系，之前蛋糕胚代表的是无监督学习，现在则被换成自监督学习。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJw6z6eFEnrAhhaAOTrAB2nacVsIib4rAFrnVicuOtSWQia9BCUnpwpQdkA/640?wx_fmt=png)
Hinton 在自监督学习领域探索了多年，LeCun 之前一直持怀疑态度，现在终于认可了这个方向。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJ8eic8AMrQwH85Iz2QJKicYY2NlPjeD1QoKTPXib9YGavZsHdViaT602yoQ/640?wx_fmt=png)
机器学习的未来不会是监督学习，当然也不会纯粹是强化学习，它应该是包含了深度模块的自监督学习。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJiajQtDapL5d2VJ9XkQPa38bpEaT2xdUicARdCCygrUDNSA039bqTKPmg/640?wx_fmt=png)
那么下一个问题是，自监督学习能够产生一般的背景知识吗？这里重点是模型需要推断出背景知识，它需要从真实世界收集的背景知识推理出当前它希望预测的任务。如下 LeCun 举了一个例子，如果接收到一个自然语句，模型应该推断出当前场景的各种背景知识。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJNsFsvO0UbGUhVMkwQCIDvRTCG2PmeJb06icKk55ew01FGyULpgbIGIA/640?wx_fmt=png)
对于基于模型的经典最优控制，我们可能需要初始化一个序列来模拟世界，并通过梯度下降调整控制序列来最优化目标函数。而目前我们可以通过强化学习的方式模拟世界，这些模型不仅需要预测下一个可能的动作，同时还需要预测一系列可能的未来。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJU4b9behANDiaCFk9G0TOGeja475iaQIDWetQS4jb1sJTA78nv3sKQXEw/640?wx_fmt=png)
接下来 Yann LeCun 介绍了使用对抗训练的视频预测。他首先展示了预测无监督学习。人类是很擅长预测的，然而机器很难预测未来会发生什么。近年来的研究中出现了一些使用「对抗训练」的成功案例，但是仍有很长的路要走。Yann LeCun 用一个纽约公寓的视频示例进行举例说明。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJUZhNsv4nLSBZHXialgF2JmpnSmCePpaVOfia4HTsBH16ic00KK4tUhsiaw/640?wx_fmt=png)
语义分割空间的视频预测
这部分 Yann LeCun 介绍了当前语义分割预测的现状。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJKGHbQCBwHrzDAR20pDJ4Bpb7vbY0fQqnoibv9PowoiafSD8FK5eDmjibw/640?wx_fmt=png)
Latent-Variable Forward Models for Planning and Learning Policies
下图展示了用于推断动作和潜在变量的模型架构。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJ6TsUxZVkT1sp0PiaYrEadqSmuLDcU2Ep74qQ5tr01bk87djdYEVaKRw/640?wx_fmt=png)
然后 Yann Lecun 展示了一个现实世界的真实案例。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJHtEib4umhYDqOQWplFgbsb3lu4ZnY1oKn9grvQt0nQKDorfJHMaicvMg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJiaDqv2lXKHgibTknse0ta9VyUQOZ9b7iaf92cBLq8sicn1d1cicXBUD2jrQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJ9ZGjZyG5Hw89QEnzpwvskmseBnZhuRBicKEJjkRX8VKPrQkyjSURZwg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJaE20sLoriaxMLEoibjCuACsVQiaxjc3CS1YVnvHh6Nz03SJkdp9WhMtJQ/640?wx_fmt=png)
最后，Yann Lecun 总结了技术和科学之间的互相驱动和促进，如望远镜和光学、蒸汽机和热力学、计算机和计算机科学等。并提出了疑问：什么相当于智能的「热力学」？
- 
人工智能和自然智能背后是否存在底层原则？
- 
学习背后是否存在简单的准则？
- 
大脑是否是进化产生的大量「hack」的集合？![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8Zfpicd40EribGuaFicDBCRH6IOu1Rnc4T3W3J1wE0j6kQ6GorRSgicib0fmNrj3yzlokup2jia9Z0YVeA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW82dcXDb4wTaOSMxrqO6xLJ2XET7gUrAPOiaZNc7NsBgdlOBI6XNtwfCjgm3w8agZHoNkaj6MswQLA/640?wx_fmt=png)
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
