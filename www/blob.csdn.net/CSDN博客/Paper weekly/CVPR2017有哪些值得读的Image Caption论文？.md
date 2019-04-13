
# CVPR2017有哪些值得读的Image Caption论文？ - Paper weekly - CSDN博客


2017年11月29日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：3270


作者丨崔权
学校丨早稻田大学硕士生
研究方向丨深度学习，计算机视觉
知乎专栏丨サイ桑的炼丹炉

# 前言

由于最近在做一个 Image Caption 的任务，读了一些最新的论文，把 CVPR2017 里比较中规中矩的 Image Caption 论文给拿出来总结一下。

说中规中矩是因为有一些 Caption 的论文做的方向比较偏，比如有一篇叫做 StyleNet，生成一些具有特定风格的描述，比如幽默风趣的 caption。而这篇总结里挑出来的论文都是在 MSCOCO 的 caption 任务上取得了不错效果的。

没有接触过 Image Caption 的同学这里有之前我在给研究生新生介绍 Image Caption 时用的 slides，Introduction of Image Caption[1]。没有什么文字，都是直观上的图片解释。

一共四篇论文，列表如下：

1. SCA-CNN: Spatial and Channel-wise Attention in Convolutional Networks for Image Captioning
2. Knowing When to Look: Adaptive Attention via A Visual Sentinel for Image Captioning
3. Skeleton Key: Image Captioning by Skeleton-Attribute Decomposition
4. Deep Reinforcement Learning-based Image Captioning with Embedding Reward

# SCA-CNN: Spatial and Channel-wise Attention in Convolutional Networks for Image Captioning

**由于这篇文章应该是效果最好的，所以放在最前面，如果对 encoder-decoder 模型不熟悉可以先看第二篇，第二篇基于的是传统的 visual
 attention 方法。**

这篇论文出自于腾讯 AI Lab，文章一开始就肯定了 visual attention 在 image caption 中起到的重要作用，并且指出其原因主要是人类的视觉系统在进行相关任务时并不是处理一整张图片而是根据需要每次处理图片的
 selective part，这和 attention机制想要模拟的过程是相同的。

这篇文章使用的还是传统的 CNN-RNN 模型，也称为 Encoder-Decoder 模型，但是文章指出，在之前别人的研究中使用的 attention 模型仅仅通过**空间 attention 权重**对上下文语义和
 conv layer 输出的最后的 feature map 进行了映射，在 CNN 环节中并没有进行 attention 的相关操作。

在这篇文章中，作者们充分利用了 CNN 的三个特性，来提出一种新的 attention 机制。

具体来说，这是一种 spatial and channel-wise 的 attention 机制，这种 attention 机制学习的是多层 3D-feature map 中的每一个 feature 与 hidden state
 之间的联系，也就是在 CNN 中引入 attention，而不是单单使用 CNN 部分的输出。

结合下图阐述两点 SCA 的 motivation：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decicODaX3uK8MnlcPP7DHsdY1TSMkEsYltqLvmJFSDm2QvAcODaIKmpQw/?)

因为一张 channel-wise的feature map 本质上就是一个对应 filter 产生的 detector response map（feature map 中 element 较大的地方说明 response 大，也就是该处含有某种 semantic attribute，比如眼睛、鼻子等），那么基于 channel-wise 的 attention 机制就可以被视为是一个根据上下文语义选取相关语义特征的过程。

比如图中的例子，当我想预测 cake 这个单词时，channel-wise attention 就会给含有蛋糕、火焰、灯光和蜡烛形状物体的 feature map 分配更大的权重。

由于一张 feature map 依赖于前层的 feature map 们，一个很自然的想法就是对多层 feature map 使用 attention 机制，以便能够获得多层语义抽象概念。

比如，在前层中对 feature map 中含有的较低级的 semantic attributes，例如圆柱体（蛋糕的形状）、阵列（蜡烛的排放），赋予更大的权重，对于 cake 这个单词的预测是很有益的。 SCA-CNN 的大概运作方式如图：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decGGL08iaYAmCk9s1yxbJT5DziajFDpf7IKsmtPlMGpGe4ibicLbicc9p6cAw/?)

对第 l 层来说，未进行 attention 的 feature map![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decE9yrp3t9oRDvic1KhrgffsqqOEicQEYGBicjzoIdqGGn9H4O1iab6z425g/?)是第(l-1)卷积层的输出，网络首先通过
 channel-wise attention 函数![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9dec1a43aMw2iatrZDM7RcMicZaIg5WtZE9TeiaSxyyj7woC1nj2DKrAtlHibg/?)来计算
 channel-wise attention 权重![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9deck5hicnSkjPhhzWWpzJIaAKIRPtcgiaQZtQAzqMiaQ1ywatHkBJo3dIl9w/?)。

attention 权重![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9deck5hicnSkjPhhzWWpzJIaAKIRPtcgiaQZtQAzqMiaQ1ywatHkBJo3dIl9w/?)将用来与![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decE9yrp3t9oRDvic1KhrgffsqqOEicQEYGBicjzoIdqGGn9H4O1iab6z425g/?)进行
 channel-wise 乘积（![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decE9yrp3t9oRDvic1KhrgffsqqOEicQEYGBicjzoIdqGGn9H4O1iab6z425g/?)的
 channel 维度和![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9deck5hicnSkjPhhzWWpzJIaAKIRPtcgiaQZtQAzqMiaQ1ywatHkBJo3dIl9w/?)向量的长度是相等的，将![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decE9yrp3t9oRDvic1KhrgffsqqOEicQEYGBicjzoIdqGGn9H4O1iab6z425g/?)中第
 i 个 channel 维度的 feature map 与![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9deck5hicnSkjPhhzWWpzJIaAKIRPtcgiaQZtQAzqMiaQ1ywatHkBJo3dIl9w/?)中第
 i 个标量相乘![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decubWYEPCCdznicmLo8Hwb8HH2v6zicCbTNlia0CPjTbB6UVPy5XHwKEDVw/?)，乘积返回的结果是一个和![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decE9yrp3t9oRDvic1KhrgffsqqOEicQEYGBicjzoIdqGGn9H4O1iab6z425g/?)形状相同的张量，这样的操作称为 channel-wise 乘积）。

然后使用 spatial attention 函数![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decpKV3pj4TWvuWPjD5UWVnbJphhzvt89JruQS8t6DXxqtTIo6XvtcQrg/?)来计算
 spatial attention 权重![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decotYDuic3CL0m5mia1ddxvQvKCpc4NzIuYkRTheEm6xNUF5J2vvqXwEEA/?)，最后该层的输出由![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9deciaeDpVP0wpVmElVjviccq5ANbIEia60kx8Tiap6ezzzNQr4UfZHg9N207Q/?)共同决定该层的输出![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decmic8g0FzEuXrvZZ1xovbFjzPxjkZ1XiaicSnPdPvVqQ6jHtUeyRTNEHxA/?)。

在进行预测第 t 个单词的时候，通过将 t-1 时刻的 hidden layer 状态![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decINthsMB8NsR8FPdOLOYS5nNtNicqeu2Ribq0xLsazpJQT7az1ianaBshA/?)、最后一层convlayer 经过 C-S attention 的输出![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decmic8g0FzEuXrvZZ1xovbFjzPxjkZ1XiaicSnPdPvVqQ6jHtUeyRTNEHxA/?)和
 t-1 时刻的 word embedding![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9deckwGu31ibVSicfeIiaJGVloich3Jy1NNfuiaJJmA5icmISiaWhmWaIvRUM0ib6g/?)共同输入进
 LSTM，对 t 时刻的 hidden layer 状态![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decOUUbibAymdaGdFqO6sKf9a3IgiaCucm1hIFC0ibrp0IYPVoceyRvY5RXA/?)进行预测：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decZFqpNqaGBNnzGygzLAqU3z1bQVfx0zCIh3ZgctyK4OSxmN5ia9fu3sQ/?)

再将 t 时刻的 hidden state![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decOUUbibAymdaGdFqO6sKf9a3IgiaCucm1hIFC0ibrp0IYPVoceyRvY5RXA/?)与
 t-1 时刻的word embedding![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9deckwGu31ibVSicfeIiaJGVloich3Jy1NNfuiaJJmA5icmISiaWhmWaIvRUM0ib6g/?)结合起来进行
 MLP，输出为一个字典大小*D x 1*的向量，最后进行 softmax 选出最大概率对应的 word：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decqheIxlfb9K4JicpM034L2dvOVFNgCSva8cbagaxoKpt2s7RV00FI5tQ/?)

据我看过的代码，attention 的实现基本都是通过 MLP+activation 来完成，论文的具体公式推导不作分析，我已经全部推导过，各个变量的维度也很清晰，如果有问题可以联系我。

有的人可能会说，为什么一定要先进行 channel-wise attention 再进行 spatial attention 呢？

文章中给出了两种结构，先 channel 后 spatial 的称为 C-S 模型，另一种称为 S-C 模型，对这两种模型都进行了实验，结果是，总的来说，C-S 模型比 S-C 模型表现要好一点。

# Knowing when to look: Adaptive Attention via A Visual Sentinel for Image Captioning

**这篇文章的主要工作是对 visual attention 进行的改进。**

传统的 Encoder-Decoder 模型在生成每个单词时都要求网络 attend to 图片，但是 decoder 在生成一些 non-visual 的单词比如"the"、"and"和"of"时似乎不需要 visual attention，还有一些单词是一些管用词组，可以通过语言模型来可靠预测，也不需要 visual attention。

这篇文章提出的是基于 visual sentinel（哨兵）的 adaptive attention 模型，在生成每个单词时，模型决定是 attend to 图片还是视觉哨兵，当语言模型不能提供有效信息的时候，模型才 attend to 图片。

举个例子：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decDvobsiaWTTyR3mVNR3dvtqptVZ2Ce2pre6OgxKvSfPic6IypkMXwHIuA/?)

图片的描述为"A white bird perched on top of a red stop sign"，一只白色的鸟栖息在一个红色 stop 标志上。

像"A"和"of"这种单词是没有像"red"和"bird"这种易于理解的视觉信号的，并且在生成"on top of"这种固定搭配词组的时候，语言模型就可以胜任，并不需要visual attention的帮助。

事实上，如果在生成这种不应该依靠 visual attention 的词时，使用了 visual attention 的话，生成的梯度可能会误导模型，并且可能会毁了之前 visual attention 在更新参数时做出的努力。

对 visual sentinel 的解释：

visual sentinel![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decTtPHibK45lCh3wd1T070NibibcKtiasiaHaGDX0aAzrCwH3YfLFo49u3j3g/?)是一个隐变量（latent
 variable），主要用来表示 decoder 已知的东西（what the decoder already knows）。decoder 记忆的内容有长期的也有短期的内容，有图片的内容也有语言的内容，visual sentinel 就是拿来帮助 decoder 决定是否需要 attend to 图片，通过一个 sentinel 门![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decSHEu1kd4kSwv8OSObOGZWCrdic8xzCpneBBRqSSmiaJJ2YVX1sKpyRdA/?)实现，通过线性插值的方式决定是否需要依赖图片，表示为：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decsV4OmJYVZR3RIhJCJyk0nCsuLaqkSkKtN2yoFDfYoks109fYWS95yQ/?)

这里![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decZ5QIMAXwsefoNGNbhHPBd15EnQdRKuoYbxlzHA9AU9F8kp3rTZoeeQ/?)表示
 visual attention，总的 attention 结果由 visual sentinel 和 visual attention 共同决定。

该模型的 attention module 如图所示：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decfTRqxYQPMMoL3wVZOb3GHCh3mNqxlrWNMrtG1Z3oZrYyITCMdzBZug/?)

整个 attention 计算过程和预测词的过程如下：

1. 首先是普通 visual attention 的计算：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decawaeIQ2BoTnBDfhv6TBMfV9p7gxEf05MmtlOl9Cuxur6FHHLAZLM6g/?)

这里最后一个特殊符号是一个元素全部为 1 的向量，其主要目的是为了将与其相乘的矩阵变得与加号前面的矩阵大小相同。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decyj0mhfzIib7lGAc9ZrsSt7IOmAjELWbfjr5VB0ib6JEcGfhmIt5J8X4g/?)

2. 然后是 visual sentinel 的计算：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decEMNnSbhEXrOS91V3H40GwsCLRG1ibHZ8P1pgWzMfjCTU4Oo2ianjwURA/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decMAIFdgzbutsRbYtuVS6gZE2XichGsPJibSHiacsBRwFlsQsPRjj2pqH5w/?)

中间的乘号是 element-wise multiplication。

3. 接着是 sentinel gate 的计算：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9deckK8FPMhtPbgZLpBJiahJEiaA3MZsuwo96hahu9wdmS0ibvWDpep2s61SQ/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9dec7YbQPtwmAic12wicdI0s1BY2gu6IDEVZdE0kUalqPyibNrGhscCM4Uibmw/?)

![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decsibviaQp31QXjiaIxvKMqzHrIXlPUXnjxK4tnGIYSZVftszT8ba3CucPw/?)是一个
 1xk 的向量，上面式子中后面一项算出来的是一个标量，sentinel gate就是![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9dechn3tEj3Zf0x5vmDriaLvGQqJbU6GJjMKDu0wp8picA4DBCQlYILZDllw/?)的最后一项，也就是第
 k+1 项。
4. 总的attention计算：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decQ0mB0bIbibCKUicSzlH72KoWs96Ncf2CZO78tMJZhZkknicCJLLJsVTLQ/?)

5. 最后是预测词：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decuptquzCX3WsG06cwbbjOIiahCw2gOV0oCpCaVLWakU6ELTjYEicEVH5g/?)

p 是一个 1xD 的向量，D 表示词库的大小，最后得到的是概率最大词对应的 index，再从词库中返回这个 word。


# Skeleton Key: Image Captioning by Skeleton-Attribute Decomposition

这篇文章指出，目前大多数的 image caption 模型中使用的 language model 都是逐个逐个生成词的，按照 label 中 caption的顺序，但是对于人来说，一种更自然的方式是，先确定图中有哪些物体，物体之间的关系是什么，然后再使用物体的瞩目特征来仔细描述每一个物体。

文章提出了一种 coarse-to-fine（我将其理解为椎轮大辂）的方法，将原始的图片描述分解为两个部分，一个主干句（skeleton sentence）和各种物体的特征（attributes），再生成语句的时候，也分为两个部分进行生成。通过这种分解，该文章的模型达到了 state-of-the-art 的效果。

举个例子：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decHFkAsXrXQ5GNc7w0h8JMVp2jPwSzNLns8libCZzyYPpXoic2eL0SzWkg/?)

为了生成上图中的描述，首先生成一句 skeleton sentence: horse on pasture（马儿在牧场上），用来表示物体中的主要物体和物体之间的关系，horses on pasture；然后，这些通过 visual attention 重新访问这些物体，当物体的 attention response 很大时，就生成对这些物体特征的描述，Three 和
 a green。将两个句子结合起来生成最终的描述，Three horses on a green pasture。

方法的结构如图：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm0sJNP0RozZ0jvwfGS9decWThj96RpRRicW97ALkF1yzc7bgI446cV4891IZtTqGQjWiaj9BrY5iakw/?)

1. Skeleton-Attribute decomposition for captions：第一步需要对已有的训练集 caption 作语法分析，并且提取出主干句和特征描述。NP/VP/PP/AP 分别是名词/动词/副词/形容词。

2. 使用经过了 CNN 提取的特征作为语言模型的输入，decoder 部分由两个 LSTM 组成，一个是 Skel-LSTM，用来生成主干句，另一个是 Attr-LSTM 用来生成描述特征。

3. 在 Skel-LSTM 中使用的是 visual attention，在每个 time step 生成一张 attention map，用来预测这一时刻的词。用来描述 skeleton sentence 中的 skeleton object 时生成的 attention map 也可以用在生成对该 object 的特征描述上，所以在 Skel-LSTM 中获得的
 visual attention 也可以在 Attr-LSTM 中再次使用。

在 Attr-LSTM 中对特征的生成不是逐个 object 生成的，是一次性生成的，因为学习过程中由于 Skel-LSTM 中物体是有顺序的，所以 Attr-LSTM 也学习了特征的顺序。

Attr-LSTM 中不仅使用了 attention，还使用了 current time step t 对应 LSTM 的 hidden state 和对应 skeleton word 的 word embedding。具体方法不细说请参考论文，欢迎讨论。

4. 最后将 skeleton sentence 和 attributes 结合起来，作为最终的 caption。

顺便提及一下，文章也指出了 encoder-decoder 模型的两个问题，很值得我们思考解决方法：

鹦鹉学舌式地复述在训练过程中出现的语句，并且在生成语句时，描述和句式两个方面缺乏变化；

由于是逐字生成描述，物体的特征需要在物体之前生成，因为不管是英语还是中文，形容词都是在名词之前出现的。但是从人类的角度来说，对一个物体的描述是需要 refer to 物体的。


# Deep Reinforcement Learning-based Image Captioning with Embedding Reward

这是将强化学习与图像描述结合起来的论文，因为我的强化学习学得不多，只停留在 GAN 和 GAN 的一些变体，如果有错误请指正。

文章提出了一种新的 decision-making 框架来进行图像描述，用了一个"policy network"和一个"value network"协同生成描述。

policy network 扮演一个局部 guidance 的角色，根据这一时刻的状态，提供对下一个词预测的置信度。

value network 扮演一个全局、前瞻的 guidance，评估现状态所有可能的扩展状态。该网络本质上是在调整学习的方向，从预测正确的词汇到描述与 ground truth 相似的描述（这句话我不是很理解）。

使用的是 actor-critic 方法来训练网络，也就是先行动后打分的方法，是一种平衡了 value-based methods 和 policy-based methods 的方法。

作者说，他们的这种方法只通过 policy network 就可以提取出正确但是概率却很低的单词，具体例子如下图：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm0sJNP0RozZ0jvwfGS9dec8dNjAicNccIWtCGAVqvlp3iaFlspJqiaZtO9tnap8UTE6hCvF5UzdHyDg/?)
Next Action 表示下一个生成单词的 preference ranking，显然在现在时刻最合适的词 holding 并不是最优选择，但是 value network 却会 go forward 到假设生成 holding 这个词的 state，然后评估这个 state，对于生成一个合适描述这一目标，分数有多高。

作者首先使用基于交叉熵 loss 的监督学习 pretrain 了一个 policy network，使用均方误差训练了一个 value network，然后使用深度强化学习来改进两个网络。深度强化学习常用的领域是游戏和控制，对于 caption 这个任务来说，很难定义一个明确的目标，也就没法明确优化的目标。

这篇论文中提出了一种 actor-critic 模型，使用的 reward 由 visual semantic embedding 生成（visual semantic embedding 计算的是图片和描述的相似度，也就间接说明了 caption 的正确性，可以作为强化学习中合适的 target 来进行优化）。

文中具体的实现方法我也只能直观上地理解，公式推导我还不行，当我补上了足够的强化学习知识再来进行补充。

最近看新闻，腾讯 AI lab 在 MSCOCO 的 caption 任务上夺冠的模型也使用的是强化学习模型，我觉得强化模型在 caption 方向上可能会大放异彩。

# 相关链接
[1] slides：http://slides.com/walkingdead526/deck\#/

# 论文链接
[1] https://arxiv.org/abs/1611.05594
[2] https://arxiv.org/abs/1612.01887
[3] https://arxiv.org/abs/1704.06972
[4] https://arxiv.org/abs/1704.03899


我是彩蛋
**解锁新功能：热门职位推荐！**

PaperWeekly小程序升级啦

**今日arXiv√猜你喜欢√****热门职位****√**

找全职找实习都不是问题
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**职位发布**
请添加小助手微信（**pwbot01**）进行咨询

**长按识别二维码，使用小程序**
账号注册![0](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/b96CibCt70iaajvl7fD4ZCicMcjhXMp1v6UYhfOp924bYQUqYCVgoT9tY9zibIyUB4m4udOibRHibe4ZiboyZXLGXa16A/0)paperweek.ly

![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)



**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)
▽ 点击 |阅读原文|
 访问作者知乎专栏


