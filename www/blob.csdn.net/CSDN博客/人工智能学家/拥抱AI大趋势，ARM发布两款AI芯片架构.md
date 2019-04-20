# 拥抱AI大趋势，ARM发布两款AI芯片架构 - 人工智能学家 - CSDN博客
2018年02月14日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：1763
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBW8HfKzYtzdrMuiac5xAefRUlkjPS1icMHB1o8WBDyYxtfUlHn2ziaetibF6iajFqBiaCQp1nNQSdUnaTibg/640?wx_fmt=png)
*来源：量子位*
*概要：ARM发布了两款针对移动终端的AI芯片架构，物体检测（Object Detection，简称OD）处理器和机器学习（Machine Learning，简称ML）处理器。*
今天，ARM发布了两款针对移动终端的AI芯片架构，物体检测（Object Detection，简称OD）处理器和机器学习（Machine Learning，简称ML）处理器。
以往，ARM都是架构准备好了，才发公告。这次一反常态，没货却先发公告：OD处理器，计划在第一季度才能提供给合作伙伴；ML处理器得等到年中。
这也看出了ARM很焦急。
毕竟在过去的几个月中，尤其是在移动端圈子里，机器学习在半导体行业中很热。
好几家提供芯片架构的公司都宣布了提供消费者解决方案，连华为都开始自主研发架构了。那么多玩家入场，ARM却没啥动作。
直到现在，ARM才把重点放在了Armv8.2的CPU ISA扩展上，该扩展借助半精度浮点和整数点产品来简化和加速神经网络的专用指令。
除了CPU的改进之外，还看到了G72中机器学习的GPU改进。虽然这两项改进都有所帮助，但想要最大性能和效率，这些改进还不够。
在测试Kirin 970的NPU和Qualcomm的DSP时，可以看出，专用架构上运行推理的效率，比在CPU上运行的效率高出一个数量级以上。
正如ARM官方解释的那样，Armv8.2和GPU的改进只是建立机器学习解决方案的第一步，还必须研究对专用解决方案的需求。
ARM也从合作的小伙伴那里感受到了行业的压力，才熬出来ML处理器。
下面简单介绍一下这次发布的两个新的架构：机器学习ML处理器以及OD处理器。
ML处理器，是专门为加速神经网络模型推理所设计的。这种架构比传统的CPU和GPU架构有明显的优势。
在执行机器学习任务时，这款ML处理器可为数据优化内存管理。
这款处理器具有数据可高度重复使用的特点，能最大限度地减少数据的输入和输出，从而实现高性能和高效率。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBW8HfKzYtzdrMuiac5xAefRUVScfApM5ibwsMlia6QppmeKouUnNJq7dFdZHl9OE8ZEojUvZ8Gtib8q3g/640?wx_fmt=png)
ML处理器，理论上可在1.5W功率下，有超过4.6TOPs（8位整数）的理论吞吐量，最高可达3TOPs / W。
虽然TOPs值并不能完全体现处理器的性能，不过它对于行业标准化仍然有用。
作为一个完全独立的独立IP（电路功能）模块，ML处理器具有自己的ACE-Lite接口，可集成到SoC中，也可以集成到DynamiQ中。
此外，ARM没有透露ML处理器更多的架构信息。
OD处理器，是针对物体检测的任务进行了优化。尽管ML处理器也能完成相同的任务，但OD处理器可以更快。给单项任务提供专用架构，才能够获得最大效率。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBW8HfKzYtzdrMuiac5xAefRUW7KKxV26WIs13OThBR6B5k3q0O6Fy8LNpyhat5HOqcFUib29FvOiaOjA/640?wx_fmt=png)
ARM也考虑到了可能会出现OD和ML处理器集成在一起用的情况：OD处理器负责把图像中的目标处理区分割出来，然后把它们传递给ML处理器，进行更细颗粒度的处理。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBW8HfKzYtzdrMuiac5xAefRUKbnhj83hObZVNW4Tc1tmIxSAz4YlQf7Au6iaWS5rZwy0faoDjOXNgRQ/640?wx_fmt=png)
ARM还提供大量软件，帮助开发人员将他们的神经网络模型应用到不同的NN框架中。从今天开始，这些软件大家可以在ARM开发者网站找到，同时也在Github上提供。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBW8HfKzYtzdrMuiac5xAefRUBBIhIWbBryfwzqEicwsAVFmghrTOf518YWOGLTzYibsx9gr44lHoKwKA/640?wx_fmt=png)
考虑到SoC开发的周期，基于新架构的芯片大概得在2019年年中或年末才能发布。ARM这次，可以说半导体及架构供应商中响应AI趋势比较慢的企业了。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
