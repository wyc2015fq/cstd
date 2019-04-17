# 谁是真凶？《CSI：犯罪现场调查》正帮助AI提高断案能力 - 知乎
# 



> 问耕 编译整理
量子位 出品 | 公众号 QbitAI




曾经的王牌美剧《CSI：犯罪现场调查》，现在成了AI用来提高断案推理能力的试验场。

据介绍，这部剧集已成为美国警方的必备学习教材，连英国苏格兰场、日本警卫厅以及法国警局都视之为反恐教材。

爱丁堡大学的一个研究团队，把《CSI：犯罪现场调查》剧集脚本变成自然语言的训练数据集，输入一个LSTM模型。他们的目标是帮助机器更好的进行自然语言理解，以及训练与之相关的复杂推理能力。

之所以选定这部剧集，原因很简单。《CSI：犯罪现场调查》有着严格的公式化剧本，完全可以被预测。



![](https://pic4.zhimg.com/v2-d8b27f84037d5a0befa2b760108da003_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='173'></svg>)



“每一集都提出了同样的基本问题（即谁是凶手），而罪犯被抓住时自然就给出了答案”，这在研究人员看来，剧集中的调查人员，会对犯罪现场进行研究，找到无可辩驳的证据，抽丝剥茧的把真凶揭露出来。

所以，“谁是真凶”就变成了一个简单的序列标签问题。

研究人员假设这个AI模型和人类一样，从剧集中获得一系列的信息输入，包括文本、视频或者音频，并据此推测凶手。而且这个过程是增量发展的。

结果表现

《CSI：犯罪现场调查》数据集上的实验表明，多模态表示对于自然语言理解非常重要。另外，增量推理策略是准确找到真凶的关键。

研究人员希望模型的预测能力，最终可以超越人类。



![](https://pic2.zhimg.com/v2-242929ed1575bd3c66c69102c9be78a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='248'></svg>)



上图是目前这套系统的评估表现。人类的平均预测精度接近85%，而AI的准确率超过60%。这是一个让研究人员欢欣鼓舞的成绩。

不过作为对比的人类样本还很小（只有三个）。另外与LSTM模型相比，人类的预测精度更高，但通常更为谨慎。AI看剧本会在大约第190句话时猜测真凶，而人类通常在第300句话时才第一次作出判断。

目前还有一些情景，会让AI有点摸不清头脑。例如在数据集中包括一些自杀案件，对这类情况AI还不能很好的处理。与之相比，在三分之二的情况下，人类最终能够意识到案件其实没有其他凶手参与。

研究人员会继续研究如何改善这方面的情况。

不知道以后会不会有人用“狄仁杰”系列训练AI呢？“元芳……”

模型架构

推理任务的顺序特性，适用于循环网络建模。研究人员采用的架构，是把单向的LSTM网络与一个softmax输出层相结合。

模型被喂给一系列（可能是多模态）的输入，每个输入对应于脚本中的一个句子，并且指定一个标签l，直来表示句子中提到了罪犯（l=1）或者没有（l=0）。这是个增量模型，每个标签的决策仅与之前的输入信息有关。



![](https://pic1.zhimg.com/v2-2b546d1b42f9373ffefa4bcb1b855880_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='316'></svg>)



上图概述了罪犯预测任务。图像、音频和文本等输入模型中，每个模态都映射成一个特征表示，融合之后传递给LSTM。然后LSTM来判断其中是否提及罪犯，并给l赋予1或者0的数值。



![](https://pic3.zhimg.com/v2-96834db68aebfb4540998d86483231fa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='436'></svg>)



这张图显示的就是两个时间步长的LSTM模型输入/输出结构。

这个模型的核心，是一个单项LSTM网络。LSTM对于一系列多模态输入的计算，采用了如下的方式：



![](https://pic2.zhimg.com/v2-40153ad6608fe1bf8e140bf6980dec29_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='422'></svg>)



另外，多模态融合采用了如下的方式：



![](https://pic1.zhimg.com/v2-998c7b0c004a21d3326feffdab2b3710_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='98'></svg>)



研究人员还比较了几种不同的模型架构。

## 相关下载

论文

摘要：《CSI：犯罪现场调查》是近似真实世界自然年语言理解和与之相关复杂推理的理想试验台。我们把犯罪剧集作为一个新的推理任务，利用每个事件提出相同的基本问题（即凶手）这一事实，最后找到真凶时自然就能获得答案。我们基于《CSI：犯罪现场调查》开发了一个新的数据集，将寻找真凶变成一个序列标签问题，并开发了一个从多模态数据中学习的LSTM模型。实验结果表明，增量推理策略是进行准确猜测以及从文本、视觉和声音输入融合表示中学习的关键。



![](https://pic3.zhimg.com/v2-53e57197cfc8be998626509f51838332_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='202'></svg>)



论文地址：

[https://arxiv.org/pdf/1710.11601.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1710.11601.pdf)

素材

研究人员把部分研究素材也在网上公开了。

GitHub地址：

[https://github.com/EdinburghNLP/csi-corpus](https://link.zhihu.com/?target=https%3A//github.com/EdinburghNLP/csi-corpus)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)

վ'ᴗ' ի 追踪AI技术和产品新动态


