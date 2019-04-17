# MIT课程全面解读2019深度学习最前沿 | 附视频+PPT - 知乎
# 



> 夏乙 发自 凹非寺
量子位 出品 | 公众号 QbitAI




人类公元纪年2019年伊始，深度学习技术也同样处在一个新的“开端”，宜review、宜展望。

MIT正在进行中的深度学习课程就全面描绘了当下的状态。

最近一节课，Lex Fridman老师展现了深度学习各方向的最领先状态，也回顾了过去两年间通往前沿的一步步进展。涉及自然语言处理、深度强化学习、训练和推断的加速等等。



![](https://pic1.zhimg.com/v2-e02930c8e60838d58998981ec34935a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)



这里的“领先”，只谈想法方向，无关基准跑分。

面对课上展现的大进展，甚至有网友说：
![](https://pic1.zhimg.com/v2-03b9387498da1a6a0600a41798ebdb4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='808' height='244'></svg>)
> 课前喝一杯？机器学习进展如此巨大，简直值得来一两瓶。

视频放出一天多，收获感谢无数。

量子位上完课，结合PPT写了笔记一份，分享给大家。

正式开始前，先来一份**小目录**。

这节课涉及的重大进展，共12个方面，分别是：
- BERT和自然语言处理（NLP）
- 特斯拉Autopilot二代（以上）硬件：规模化神经网络
- AdaNet：可集成学习的AutoML
- AutoAugment：用强化学习做数据增强
- 用合成数据训练深度神经网络
- 用Polygon-RNN++做图像分割自动标注
- DAWNBench：寻找快速便宜的训练方法
- BigGAN：最领先的图像合成研究
- 视频到视频合成
- 语义分割
- AlphaZero和OpenAI Five
- 深度学习框架




我们一个一个细说。

## **BERT和NLP**

2018年是自然语言处理之年。很多圈内人把2012年称为深度学习的ImageNet时刻，因为这一年，AlexNet带来了计算机视觉领域的性能飞跃，激励着人们去探索深度学习在这一领域的更多可能性。

2016年到18年，NLP的发展也类似，特别是BERT的出现。

NLP的发展要从编码器-解码器架构说起。



![](https://pic2.zhimg.com/v2-7509f7be2c23c48b98c93c822dec3a81_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



上图展示了一个用于神经机器翻译的序列到序列（seq2seq）模型。RNN编码器负责将输入序列编码成固定大小的向量，也就是整个句子的“表征”，然后把这个表征传递给RNN解码器。

这个架构在机器翻译等领域很管用。



![](https://pic3.zhimg.com/v2-24bf6d99e79ab4a56813cf1c5ae20bb6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



接下来发展出了注意力（Attention）。这种机制，不强迫网络将所有输入信息编码城一个固定长度的向量，在解码过程中还能重新引入输入序列，还能学习输入序列和输出序列中哪一部分互相关联。

比较直观的动态展示是这样的：



![](https://pic1.zhimg.com/v2-527ef45306355b9a515c8fe7463c76c4_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='211'></svg>)



再进一步，就发展到了自注意力（Self-Attention），和谷歌“Attention is all you need”论文提出的Transformer架构。



![](https://pic3.zhimg.com/v2-bf8a62f103c907bbb728c7223d08b992_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



Transformer架构的编码器用自注意力机制为输入序列生成一组表征，解码过程也使用注意力机制。

说完注意力的发展，我们再看一下NLP的一个关键基础：词嵌入。



![](https://pic1.zhimg.com/v2-f32ff3e752a91e8d2246269042c9d6e0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



词嵌入的过程，就是把构成句子的词表示成向量。传统的词嵌入过程用无监督方式将词映射成抽象的表征，一个词的表征参考了它后边的序列。

这是一个语言建模（language modeling）过程。

后来，有了用双向LSTM来做词嵌入的ELMo。



![](https://pic4.zhimg.com/v2-f2d843ee46725669a3551e811030a63f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



双向LSTM不仅考虑词后边的序列，也考虑它前边的。因此，ELMo词嵌入考虑了上下文，能更好地表征这个词。

接下来出现了OpenAI Transformer。



![](https://pic1.zhimg.com/v2-d1966cb9371176d57c09c914bdcbb260_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



也就是用先预训练一个Transformer解码器来完成语言建模，然后再训练它完成具体任务。



![](https://pic4.zhimg.com/v2-d9662e8c4e5177e807744531ab0d9917_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



经过这些进展的积累，2018年出现了BERT，在NLP的各项任务上都带来了性能的明显提升。



![](https://pic1.zhimg.com/v2-1f20181d2d09de3f484a345fd2a6ef54_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



它可以用在各种各样的NLP任务上，比如说：
- 像ELMo一样创建语境化词嵌入；
- 句子分类；
- 句对分类；
- 计算句对的相似度；
- 完成选择题形式的问答任务；
- 给句子打标签；
- 完成普通的问答任务。

量子位之前有回顾2018年深度学习进展、NLP进展的文章，也都详细谈了BERT。

[一文看尽2018全年AI技术大突破](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247510685%26idx%3D1%26sn%3D215d8b68c6e2c5eb58eb171ee0cea167%26chksm%3De8d01defdfa794f9a8266298cad8db9f4c28fd911860cbd1813f158fd24f45837d6bf14e4d52%26scene%3D21%23wechat_redirect)

[不只有BERT！盘点2018年NLP令人激动的10大想法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247510739%26idx%3D2%26sn%3D3b1d8c968712ba9b5a8f8280dfa86697%26chksm%3De8d01da1dfa794b72fe21b7b2bc1098b73857073601c11779c4d7fc0a2799c99a1bda8be8224%26scene%3D21%23wechat_redirect)

## **特斯拉AutoPilot二代硬件**

特斯拉的出现毫不意外，量子位之前介绍过，Lex老师是[马斯克的小迷弟](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247512678%26idx%3D4%26sn%3D466a43fb4993545e579f0a53851a1136%26chksm%3De8d00514dfa78c02fe81530c7f0b8d84c4360ea033f1e62fc65fc54289da9cea271ce2ddce5d%26scene%3D21%23wechat_redirect)。

作为神经网络大规模应用的代表，也的确值得一提。



![](https://pic1.zhimg.com/v2-bb7abbdd138cad5cae9a5b6828b51a04_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



Autopilot二代硬件使用了英伟达的Drive PX 2，还搭载了8个摄像头获取输入数据。

Drive PX 2上运行着Inception v1神经网络，运用摄像头获取的各种分辨率数据进行各种来计算，比如分割可以前进的区域，物体检测等等。



![](https://pic2.zhimg.com/v2-470ab601d76586c656767b6246ecdfe9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



这套系统，已经把神经网络带进了车主们的日常生活中。

## **AdaNet：可集成学习的AutoML**

AutoML是机器学习界的梦想：



![](https://pic1.zhimg.com/v2-3d38298bf2ada8b1a9d45283f2ed84fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



将机器学习的一方面或者整个流程自动化，扔一个数据集进去，让系统自动决定所有的参数，从层数、模块、结构到超参数……

谷歌的神经架构搜索，无论从效率还是准确率来看成绩都不错。

2018年，AutoML又有了一些新进展，AdaNet就是其中之一。



![](https://pic2.zhimg.com/v2-be7b722e5a8e5a18da6ac1af97722955_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



它是一个基于Tensorflow的框架，AutoML一样，也是用强化学习方法。不过，AdaNet不仅能搜索神经网络架构，还可以学习集合，将最优的架构组合成一个高质量的模型。

要进一步了解AdaNet，可以参考量子位之前的文章：

[谷歌开源集成学习工具AdaNet：2017年提出的算法终于实现了](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247507257%26idx%3D5%26sn%3Dfeef78c3c41c921a5cafff9d1d2949b6%26chksm%3De8d06a4bdfa7e35d53591380b2ea7c49e8363cb2c240b4d69bfa2010b082ebb63264a110f757%26scene%3D21%23wechat_redirect)

## **AutoAugment：用强化学习增强数据**

从2012年的“ImageNet时刻”到现在，图像识别的新算法层出不穷，成绩一直在提升。

而在计算机视觉里，数据量非常重要，数据增强（data augmentation）也是重要的提升识别率的方法。但是，关于数据增强的研究却不多。

2018年，Google继自动搜索神经架构之后，把数据增强这个步骤也自动化了。



![](https://pic1.zhimg.com/v2-7cb2bbeca87b0b396ac3ecbe75491e3c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



AutoAugment能根据数据集学习这类图像的“不变性”，知道哪些特性是最核心的，然后，神经网络在增强数据时应该让核心特性保持不变。

比如说，它自动增强门牌号数据集的时候，会常用剪切、平移、反转颜色等方法，而在ImageNet这种自然照片数据集上，就不会剪切或者翻转颜色，只会微调颜色、旋转等等。

这个过程和计算机视觉里常用的迁移学习差不多，只不过不是迁移模型权重，而是迁移大型数据集体现出的数据增强策略。

进一步了解AutoAugment，推荐阅读：

[利用AutoAugment提升深度学习性能](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU1OTMyNDcxMQ%3D%3D%26mid%3D2247484491%26idx%3D1%26sn%3Debed342f16a9e628f3587b5332d71888%26chksm%3Dfc184f03cb6fc6159cf7027ff770783835eef3cfc98c396e91c93de1a52fcdaa17eeb2106dd4%26scene%3D21%23wechat_redirect)

## **用合成数据训练深度神经网络**



![](https://pic2.zhimg.com/v2-6136c0a9ab47c674cbd35fc0e0ff15dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



很多机构都在研究如何用合成数据来训练神经网络，英伟达是其中的主要势力。

英伟达非常擅长创造接近真实的场景，他们2018年一篇用合成数据训练深度神经网络的论文就充分利用了这一点，合成数据训练的模型，成绩完全不逊于真真实数据。

合成数据还非常灵活多变，比如说汽车、屋子，都能合成出各种各样的数量、类型、颜色、纹理、背景、距离、视角、灯光等等。

关心如何用合成数据训练模型？英伟达的这篇论文值得一读：

Training Deep Networks with Synthetic Data: Bridging the Reality Gap by Domain Randomization

[https://arxiv.org/abs/1804.06516](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.06516)

## **Polygon-RNN++：图像分割自动标注**



![](https://pic2.zhimg.com/v2-360ed2c7b2610af91ffd04815f10cd8d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



在训练深度学习模型过程中，数据标注是非常耗时费力的环节。特别如果要标注出语义分割图，就更加麻烦。

Polygon-RNN++是一种自动标注图像的方法，能自动生成一个多边形，来标注出图像里的对象。

它用CNN提取图像特征，然后用RNN解码出多边形的顶点，提出多个候选多边形。然后，一个评估网络从候选中选出最好的。

这也是让深度学习尽可能自动化的一种努力。

Polygon-RNN++的主页上，能找到这一工具的论文、Demo、代码等各种资料：

[http://www.cs.toronto.edu/polyrnn/](https://link.zhihu.com/?target=http%3A//www.cs.toronto.edu/polyrnn/)

## **DAWNBench：寻找快速便宜的训练方法**

除了让深度学习尽可能自动化之外，还有一个一直受关注的研究方向：让它更易用。也就是快速、便宜。



![](https://pic1.zhimg.com/v2-87362e416364a268256bf8741a682ac8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



斯坦福提出的DAWNBench基准，评估的就是模型的速度和成本。它测试各种模型、框架、硬件在ImageNet上达到93%准确率，CIFAR上达到94%准确率的训练、推断速度和成本。

官网在这里：
[https://dawn.cs.stanford.edu/benchmark/](https://link.zhihu.com/?target=https%3A//dawn.cs.stanford.edu/benchmark/)

在快速廉价这方面，fast.ai有很好的成绩。他们用租来的亚马逊AWS的云计算资源，18分钟在ImageNet上将图像分类模型训练到了93%的准确率。

他们所用的方法，关键在于在训练过程中逐渐提高学习率、降低momentum，这样就可以用非常高的学习率来训练，还能避免过拟合，减少训练周期。

量子位曾经介绍过：

[40美元18分钟训练整个ImageNet！他们说，这个成绩人人可实现](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247502384%26idx%3D2%26sn%3De5f0517baca5e5d63d26de66071d6e98%26chksm%3De8d07d42dfa7f4548c5547bf30bf84020a8f2cb4fd648905b2afeac9fbf67b60ff1ae7c1323e%26scene%3D21%23wechat_redirect)

## **BigGAN：最领先的图像合成研究**

在用GAN合成照片这个方向上，2018年最受人瞩目的成就是DeepMind的BigGAN。



![](https://pic3.zhimg.com/v2-6cb293ac9b959325fc8847597f3666da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



它所用的基本模型没有太大变化，关键在于提升训练规模，增大了模型的容量和训练的批次大小。

BigGAN还提交到ICLR匿名评审时，学界就已经沸腾，因为效果实在是堪称史上最佳。

现在，官方开放了BigGAN的Demo，人人都能上手体验：

[史上最强GAN：训练费10万起，现在免费体验，画风鬼畜又逼真](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247507950%26idx%3D1%26sn%3D5f6ad3faa1a7a9dada286911f549bbd5%26chksm%3De8d0689cdfa7e18a6daa6101172ce6c69c4b97da57f20175e1b2365c4c9f3f08ad4b1a3501ca%26scene%3D21%23wechat_redirect)

## **视频到视频合成**

这个领域有一项非常值得关注的研究，那就是英伟达和MIT的“Video-to-Video Synthesis”。

只要一幅动态的语义地图，你就可以获得和真实世界几乎一模一样的视频。



![](https://pic2.zhimg.com/v2-d008d025f79343ffcb8d3c20898b3349_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='250'></svg>)



通过一个简单的白描草图，也能生成细节丰富、动作流畅的高清人脸：



![](https://pic4.zhimg.com/v2-f5340092b4d4c2047f8cdf3fd085a7ff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='203'></svg>)



甚至整个身子都能搞定，跳舞毫无压力：



![](https://pic2.zhimg.com/v2-9a1cfd497697a03bb31d5d0785202801_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='471' height='293'></svg>)



论文、代码都已公布，量子位也比较详细地介绍过，可以到这里看：

[真实到可怕！英伟达MIT造出马良的神笔](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247502757%26idx%3D1%26sn%3D8603dbc1b19f33b59095c5647d3d2fc3%26chksm%3De8d07cd7dfa7f5c1a01323f0daccd292ddf6d9db8b59bf75c93f1f16dbca8e4233030315fc3d%26scene%3D21%23wechat_redirect)

## **语义分割**

视觉感知的问题可以分为三大层级，目前研究最充分的，是分类，也就是图像识别。



![](https://pic4.zhimg.com/v2-646d7cdce79402dc80cfbe5771635b47_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



然后，是目标检测：在一幅图像里，框出物体，识别出它是什么。



![](https://pic2.zhimg.com/v2-bede8ab9bf676529285c91e03fea25e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)





![](https://pic4.zhimg.com/v2-e2abb05d74a8380c7c52bba9cefe5e83_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



关于目标检测的研究非常多：



![](https://pic3.zhimg.com/v2-aba0767b90dc60831f58fc6d4d0335b6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



其中不少工作很exciting，但Lex老师认为哪一个都称不上“突破”。

而语义分割，是视觉感知问题的最高层级。



![](https://pic4.zhimg.com/v2-c809b577e30136ff9892aac1f9d1a98f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



现在，这一领域的最好成绩属于DeepLab v3+。



![](https://pic4.zhimg.com/v2-c917ac7afb8503f33df751de770e7c47_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



关于语义分割的重要研究，可以说从2014年的FCN开始，经历了2015年的SegNet、Dilated Convolutions的发展，2016年出现了DeepLab v1、v2版本。



![](https://pic2.zhimg.com/v2-9cb119f54b172f281afa7bccdecb167d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)





![](https://pic3.zhimg.com/v2-e0eefda9399ec1ec1ee064bc8bcd641e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)





![](https://pic2.zhimg.com/v2-8da5e905957b9c55233a1662714a8139_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)





![](https://pic1.zhimg.com/v2-9bef067bb10bc0d7f131c2d3fef189a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



DeepLab系列用ResNet来提取图像特征，增加了全连接条件随机场（CRF）作为后续处理步骤，能捕捉图像中的局部和长距离依赖，优化预测图，还引入了空洞卷积。



![](https://pic4.zhimg.com/v2-102e9ddce16b3b8882f8b5b3645cbb2f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



在前两版的基础上，DeepLab v3又增加了多尺度分割物体的能力。



![](https://pic1.zhimg.com/v2-5f2c480caa043c08a7714409973e2414_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



DeepLab的代码，在TensorFlow模型中：
[https://github.com/tensorflow/models/tree/master/research/deeplab](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/tree/master/research/deeplab)

## **AlphaZero和OpenAI Five**

用强化学习打游戏这件事，2018年也有明显进步。

这个领域最初的尝试，是DeepMind用强化学习算法玩了一遍雅达利的小游戏，推出了DQN。



![](https://pic4.zhimg.com/v2-4f42c61ebfbe385ae16943884d13641b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



随后，就是围棋AI AlphaGo广为人知的一系列发展：



![](https://pic3.zhimg.com/v2-6dca89df4547aff7b32cce7a2f591a12_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)





![](https://pic2.zhimg.com/v2-a2500adbe4ff78c4d42b2472a6bfa689_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)





![](https://pic1.zhimg.com/v2-ce7e8f7cd0a1331d0090b62cf8383d28_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



从AlphaGo到AlphaZero的过程，也是一个越来越节约人工、机器自主性越来越高的过程。

AlphaZero掌握围棋、国际象棋、日本将棋三大棋类，但都是完美信息、规则清晰游戏。而OpenAI把目光投向了更混乱的试验场：Dota 2。



![](https://pic3.zhimg.com/v2-8fff61728997a44850378716349cb106_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



对于强化学习算法来说，Dota 2更接近真实世界的情况，它需要解决团队协作、长时间协调、隐藏信息等问题。

OpenAI在Dota 2上取得的进展如下：



![](https://pic1.zhimg.com/v2-896d0c99327114c1665ed92a919a6960_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)


- 2017年，他们的算法在1v1比赛中击败顶尖选手Dendi；
- 2018年，5v5的AI队伍OpenAI Five在Dota比赛TI上对战顶尖职业选手，输了两局。

两场比赛的回顾、技术解读、精彩片段，都在量子位曾经的报道里：
- 第一场回顾 - [AI溃败，被Dota2职业战队打蠢！独家专访OpenAI：我们发现一个Bug](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247502925%26idx%3D1%26sn%3D49945a8d34add5f7636016823226ef61%26chksm%3De8d07b3fdfa7f229b2ee9cf43aa7e3045121c7b330700d06942972809d70710bcf4e9f499395%26scene%3D21%23wechat_redirect)
- 第二场回顾 - [AI又输了！中国传奇Dota2冠军联手，OpenAI快速进化然并卵](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247502972%26idx%3D1%26sn%3D3e068a146864e23e2a72d7710bddd97a%26chksm%3De8d07b0edfa7f2180185dc254df378d1d3110b720ece2840d6cc8e4c61cadee2eff5dbbb6e76%26scene%3D21%23wechat_redirect)
- 赛后解读和精彩片段集锦 - [遇劣势变蠢、发语音嘲讽人类……OpenAI这些奇葩DOTA操作跟谁学的？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247503039%26idx%3D2%26sn%3Db5560fb4a7c8771b45ce43d59db9f02f%26chksm%3De8d07bcddfa7f2db676fe4de7bdeac1795e46dd6f63ca4865ded3fe8e163481924aee63f90b9%26scene%3D21%23wechat_redirect)

未来仍需努力。

## **深度学习框架**

回顾整个2018年，深度学习框架排名是这样的：



![](https://pic2.zhimg.com/v2-b58218e843141c75075621499529e261_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



## **△** 来源：L[https://towardsdatascience.com/deep-learning-framework-power-scores-2018-23607ddf297a](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/deep-learning-framework-power-scores-2018-23607ddf297a)

框架的格局会往什么方向发展？要看这样几个用户关心的因素：
- 学习曲线
- 开发速度
- 社区的规模和热情
- 用这一框架实现的论文数量
- 长期发展和稳定的可能性
- 工具生态

## **2019展望**

要展望2019，我们应该好好思考Hinton谈过的两个观点：



![](https://pic2.zhimg.com/v2-5651f7e3ea5dff38bb0e931e48589755_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)



关于反向传播：“我的观点是把它完全抛弃，从头再来。”

“未来取决于我说什么都深切好奇的那些研究生们。”

最后，附整节课视频（纯生肉，英语、无字幕）：
[深度学习最新前沿进展(2019)-MIT_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/z082938vmu3.html)
以及PPT：

[https://www.dropbox.com/s/v3rq3895r05xick/deep_learning_state_of_the_art.pdf?dl=0](https://link.zhihu.com/?target=https%3A//www.dropbox.com/s/v3rq3895r05xick/deep_learning_state_of_the_art.pdf%3Fdl%3D0)

量子位（公众号QbitAI）还在网盘传了一份PPT，在公众号对话界面回复“最前沿”可得。

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)


