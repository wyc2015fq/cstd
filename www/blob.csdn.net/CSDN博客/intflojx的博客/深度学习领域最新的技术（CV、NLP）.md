
# 深度学习领域最新的技术（CV、NLP） - intflojx的博客 - CSDN博客


2019年02月27日 23:50:36[paulpanwang](https://me.csdn.net/intflojx)阅读数：211标签：[深度学习进展																](https://so.csdn.net/so/search/s.do?q=深度学习进展&t=blog)[深度学习最新论文																](https://so.csdn.net/so/search/s.do?q=深度学习最新论文&t=blog)[计算机视觉最新论文																](https://so.csdn.net/so/search/s.do?q=计算机视觉最新论文&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=深度学习最新论文&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=深度学习进展&t=blog)个人分类：[学习笔记																](https://blog.csdn.net/intflojx/article/category/7252345)[读书笔记																](https://blog.csdn.net/intflojx/article/category/7252346)[
							](https://blog.csdn.net/intflojx/article/category/7252345)
[
				](https://so.csdn.net/so/search/s.do?q=深度学习进展&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=深度学习进展&t=blog)

人工智能正在日益渗透到所有的技术领域。而深度学习(DL)是目前最活跃的分支。最近几年，DL取得了许多重要进展。其中一些因为事件跟大众关系密切而引人瞩目，而有的虽然低调但意义重大。深度学习从业人员应该保持足够的嗅觉，这个领域正在发生很多事情，你必须要跑的足够的快才能跟上时代步伐。
**一. 计算机视觉（CV）**
这是现在深度学习中最受欢迎的领域，我觉得我们已经完全获取了计算机视觉中容易实现的目标。无论是图像还是视频，我们都看到了大量的框架和库，这使得计算机视觉任务变得轻而易举。
**1.BigGAN****的发布**
在2014年，Ian Goodfellow设计了GAN，这个概念产生了多种多样的应用程序。年复一年，我们看到原始概念为了适应实际用例正在慢慢调整，直到今年，仍然存在一个共识：机器生成的图像相当容易被发现。但最近几个月，这个现象已经开始改变。或许随着[BigGAN](https://arxiv.org/pdf/1809.11096.pdf)的创建，该现象或许可以彻底消失，以下是用此方法生成的图像：
![](https://img-blog.csdnimg.cn/20190227233059466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190227232823911.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
除非你拿显微镜看，否则你将看不出来上面的图片有任何问题。毫无疑问GAN正在改变我们对数字图像（和视频）的感知方式。
摘要：尽管近期由于生成图像建模的研究进展，从复杂数据集例如 ImageNet 中生成高分辨率、多样性的样本仍然是很大的挑战。为此，研究者尝试在最大规模的数据集中训练生成对抗网络，并研究在这种规模的训练下的不稳定性。研究者发现应用垂直正则化（orthogonal regularization）到生成器可以使其服从简单的「截断技巧」（truncation trick），从而允许通过截断隐空间来精调样本保真度和多样性的权衡。这种修改方法可以让模型在类条件的图像合成中达到当前最佳性能。当在 128x128 分辨率的 ImageNet 上训练时，本文提出的模型—BigGAN—可以达到 166.3 的 Inception 分数（IS），以及 9.6 的 Frechet Inception 距离（FID），而之前的最佳 IS 和 FID 仅为 52.52 和 18.65。
论文地址：
https://openreview.net/pdf?id=B1xsqj09Fm
更多样本地址：
https://drive.google.com/drive/folders/1lWC6XEPD0LT5KUnPXeve_kWeY-FxH002
**2. Fast.ai****的模型18分钟内在ImageNet上被训练**
有人统计了历年ICLR论文录用者使用的深度学习框架，发现虽然TensorFlow还高居榜首，但PyTorch近一年来的使用数据已经翻了3倍，可以和TF比肩。这是个令人惊讶的消息，也让不少从业者开始正视这一发展趋势，筹备“双修”事宜。在下文中，论智给读者带来的是fast.ai发布的一个简便、好用的PyTorch库——对PyTorch感兴趣的读者不妨先从这个库开始试手。
这是一个非常酷的方向：大家普遍认为需要大量数据以及大量计算资源来执行适当的深度学习任务，包括在ImageNet数据集上从头开始训练模型。我理解这种看法，大多数人都认为在之前也是如此，但我想我们之前都可能理解错了。
![](https://img-blog.csdnimg.cn/20190227233729573.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
Fast.ai的模型在18分钟内达到了93％的准确率，他们使用的硬件48个NVIDIA V100 GPU，他们使用fastai和PyTorch库构建了算法。所有的这些放在一起的总成本仅为40美元！ 在[这里](https://www.fast.ai/2018/08/10/fastai-diu-imagenet/)更详细地描述了他们的方法，包括技术。这是属于每个人的胜利！
**3.vid2vid技术**
在过去的4-5年里，图像处理已经实现了跨越式发展，但视频呢？事实证明，将方法从静态框架转换为动态框架比大多数人想象的要困难一些。你能拍摄视频序列并预测下一帧会发生什么吗？答案是不能！
NVIDIA决定在今年之前开源他们的方法，他们的vid2vid方法的目标是从给定的输入视频学习映射函数，以产生输出视频，该视频以令人难以置信的精度预测输入视频的内容。Pytorch实现了我们的高分辨率（例如，2048x1024）逼真的视频到视频转换方法。它可用于将语义标签贴图转换为照片般逼真的视频，合成人们从边缘地图谈话，或从姿势生成人体。
![](https://upload-images.jianshu.io/upload_images/910914-83bf69ac7412ed0e.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/571/format/webp)
![](https://upload-images.jianshu.io/upload_images/910914-0a88a316cce3337d.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/592/format/webp)
你可以在这里的[GitHub](https://github.com/NVIDIA/vid2vid)上试用他们的PyTorch实现。
论文：[Vid2Vid](https://tcwang0509.github.io/vid2vid/paper_vid2vid.pdf)
代码：[项目主页](https://tcwang0509.github.io/vid2vid/)
5. styleGAN
是NVIDIA的一篇新论文，一个对于GAN（[StyleGAN](https://arxiv.org/abs/1812.04948)）的基于样式的生成器体系结构，提出了一个新的模型来应对这个挑战。StyleGAN是一步一步地生成人工图像的，从非常低的分辨率开始，一直到高分辨率（1024×1024）。通过分别地修改网络中每个级别的输入，它可以控制在该级别中所表示的视觉特征，从粗糙的特征（姿势、面部形状）到精细的细节（头发颜色），而不会影响其它的级别。
StyleGAN是一篇突破性的论文，它不仅可以生成高质量的和逼真的图像，而且还可以对生成的图像进行较好的控制和理解，甚至使生成可信度较高的假图像变得比以前更加的容易。在StyleGAN中提出的一些技术，特别是映射网络和自适应实例标准化（AdaIN），可能是未来许多在GAN方面创新的基础。
![](https://upload-images.jianshu.io/upload_images/2509688-d038fab9b2ef35f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/800/format/webp)
作者：阿里云云栖社区
链接：https://www.jianshu.com/p/5dc2486c70cf
6.**autoML**
深度学习模型部署到很多应用之中，如图像识别、语音识别及机器翻译。通常情况下，深度学习模型都是由工程师和科学家团队精心设计出来的。这种手动设计的过程是非常困难的，因为模型组件的搜索空间可以非常巨大——一个典型的10层网络模型能有1010种可能的网络。因此，设计网络的过程通常需要花费优秀的机器学习专家大量的时间和经验。.为了让设计机器学习模型的过程变得更加简单，谷歌一直在探索自动设计机器学习模型的方法。研究发现，演化学习和强化学习有非常大的潜力。本文将探讨强化学习方法所取得的成果。
谷歌将这种方法命名为[auto](https://cloud.tencent.com/developer/information/auto)ML，一个控制[神经网络](https://cloud.tencent.com/developer/information/%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C)提出一个可以在特定任务上训练和评测性能的子模型架构，评测的反馈可以让控制器知道下一轮如何改进它自己提出的模型。谷歌将这个过程重复了无数遍——生成新的架构、进行测试、提供反馈让控制器生成新的架构。最终控制器学习到更大可能地使用能够在验证集上取得更优结果的架构。整个过程如下图：
![](https://ask.qcloudimg.com/http-save/developer-news/2xz0nwzsuc.jpeg)
谷歌将这个方法应用到两个重量级的深度学习数据集——CIFAR-10图像识别数据集以及Penn Treebank语言模型数据集。在这两个数据集上，谷歌的方法设计出的模型取得的正确率可以与由机器学习专家设计出的最前沿的模型相媲美。
**2019趋势展望**
Analytics Vidhya预计，明年在计算机视觉领域，对现有方法的改进和增强的研究可能多于创造新方法。
在美国，政府对无人机的限令可能会稍微“松绑”，开放程度可能增加。而今年大火的自监督学习明年可能会应用到更多研究中。
Analytics Vidhya对视觉领域也有一些期待，目前来看，在CVPR和ICML等国际顶会上公布最新研究成果，在工业界的应用情况还不乐观。他希望在2019年，能看到更多的研究在实际场景中落地。
Analytics Vidhya预计，视觉问答（Visual Question Answering，VQA）技术和视觉对话系统可能会在各种实际应用中首次亮相。
**二. 自然语言处理（NLP）**
让机器分析单词和句子似乎是一个梦想，就算我们人类有时候也很难掌握语言的细微差别，但2018年确实是NLP的分水岭。
我们看到了一个又一个显著的突破：ULMFiT、ELMO、OpenAI的Transformer和Google的BERT等等。迁移学习（能够将预训练模型应用于数据的艺术）成功应用于NLP任务，为无限可能的应用打开了大门。让我们更详细地看一下这些关键技术的发展。
**1.ULMFiT**
ULMFiT由Sebastian Ruder和fast.ai的Jeremy Howard设计，它是第一个在今年启动的NLP迁移学习框架。对于没有经验的人来说，它代表通用语言的微调模型。Jeremy和Sebastian让ULMFiT真正配得上Universal这个词，该框架几乎可以应用于任何NLP任务！
想知道对于ULMFiT的最佳部分以及即将看到的后续框架吗？事实上你不需要从头开始训练模型！研究人员在这方面做了很多努力，以至于你可以学习并将其应用到自己的项目中。ULMFiT可以应用六个文本分类任务中，而且结果要比现在最先进的方法要好。
你可以阅读Prateek Joshi关于如何开始使用ULMFiT以解决任何文本分类问题的[优秀教程](https://www.analyticsvidhya.com/blog/2018/11/tutorial-text-classification-ulmfit-fastai-library/)。
**2.ELMO**
猜一下ELMo代表着什么吗？它是语言模型嵌入的简称，是不是很有创意？ ELMo一发布就引起了ML社区的关注。
ELMo使用语言模型来获取每个单词的嵌入，同时还考虑其中单词是否适合句子或段落的上下文。上下文是NLP的一个重要领域，大多数人以前对上下文都没有很好的处理方法。ELMo使用双向LSTM来创建嵌入，如果你听不懂-请参考[这篇文章](https://www.analyticsvidhya.com/blog/2017/12/fundamentals-of-deep-learning-introduction-to-lstm/)，它可以让你很要的了解LSTM是什么以及它们是如何工作的。
与ULMFiT一样，ELMo显着提高了各种NLP任务的性能，如情绪分析和问答，在[这里](https://allennlp.org/elmo)了解更多相关信息。
**3.BERT**
不少专家声称BERT的发布标志着NLP的新时代。继ULMFiT和ELMo之后，BERT凭借其性能真正击败了竞争对手。正如[原论文](https://arxiv.org/abs/1810.04805)所述，“BERT在概念上更简单且更强大”。BERT在11个NLP任务中获得了最先进的结果，在SQuAD基准测试中查看他们的结果：
有兴趣入门吗？你可以使用[PyTorch实现](https://github.com/huggingface/pytorch-pretrained-BERT)或Google的[TensorFlow代码](https://github.com/google-research/bert)尝试在自己的计算机上得出结果。
我很确定你想知道BERT代表什么，它实际上是Transformers的双向编码器表示，如果你能够领悟到这些，那很不错了。
**4.PyText**
Facebook开源了深度学习NLP框架PyText，它在不久之前发布，但我仍然要测试它，但就早期的评论来说非常有希望。根据FB发表的研究，PyText使会话模型的准确性提高了10％，并且缩短了训练时间。
PyText实际上落后于Facebook其他一些产品，如FB Messenger。如果你对此有兴趣。你可以通过[GitHub](https://github.com/facebookresearch/pytext)下载代码来自行尝试。
**2019****年NLP趋势：**
塞巴斯蒂安·罗德讲述了NLP在2019年的发展方向，以下是他的想法：
预训练的语言模型嵌入将无处不在，不使用它们的模型将是罕见的。
我们将看到可以编码专门信息的预训练模型，这些信息是对语言模型嵌入的补充。
我们将看到有关多语言应用程序和跨语言模型的成果。特别是，在跨语言嵌入的基础上，我们将看到深度预训练的跨语言表示的出现。


