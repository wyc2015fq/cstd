# GAN 的发展对于研究通用人工智能有什么意义？ - 人工智能学家 - CSDN博客
2018年04月21日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：342
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWQxQ4uKLYxxuUmxdfYypkC3IkDj0aGhzp8qdDNuAxJtDZYrorUrskWQJq5s7UcWuNaHUibMDzlyuw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
作者：Lyken    来源：知乎
GAN对于人工智能的意义，可以从它名字的三部分说起：Generative Adversarial Networks。为了方便讲述，也缅怀过去两周在某论坛上水掉的时间，我先从Networks讲起。
**Networks：(深度)神经网络**
自从12年AlexNet横空出世后，神经网络俨然已成为现在learning的主流。比起贝叶斯学派的强先验假设(priori)，SVM在核函数(kernel)上的反复钻研，神经网络不需要科研者过多关注细节，只需要提供好海量的数据和设置好超参数，便能达到不错的效果。用武侠小说的方式来说，便是各大门派高手潜心十余载修炼一阳指/九阴真经/麒麟臂等神功，比试时却发现有一无名小卒内力浩瀚如海，出手虽毫无章法可言，但在内功的加持下，轻松打得众人抬不起头。
Deep系列的算法不仅在众多benchmark上霸据榜首，其衍生应用也给人工智能带来了一股新的浪潮，例如创作艺术品(Gatys 的 Neural Alorightm for Artistic Style)，AlphaGo(CNN估值 + 蒙特卡洛剪枝)，高质量的机器翻译(Attention + seq2seq)等等。这些衍生应用在部分任务上，已经能媲美人类中的专家，让人不禁浮想强人工智能(strong AI)的到来。然而，纵使深度网络(Deep Neural Networks)再强大，它也有自己的局限，生成模型上的不尽人意便是其中之一。
**Generative(Model)：生成模型**
机器学习的模型可大体分为两类，生成模型(Generative Model)和判别模型(Discriminative Model)。判别模型需要输入变量![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2TJicg5Cm2aCsu8aUGX8xfMzEibI3ibOR6uZdZCQUOeibKkpXU6XhL99iaItCI8rhibuicIrnsINQRu9vnTQ/640?wx_fmt=jpeg)，通过某种模型来预测![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2TJicg5Cm2aCsu8aUGX8xfMzEgeaw5KwLsGeHhbOCdoAXTcfDJn8b2Te7YQ9xZEg3oW99AAQwjrlrg/640?wx_fmt=jpeg)。生成模型是给定某种隐含信息，来随机产生观测数据。举个简单的例子，
**判别模型：**给定一张图，判断这张图里的动物是猫还是狗
**生成模型：**给一系列猫的图片，生成一张新的猫咪(不在数据集里)
众所周知的imagenet-1000图像分类，自动驾驶的图片语义分割，人体骨架点的预测都属于判别模型，即给定输入预测某种特征。实际上12～14年的大部分工作都属于判别模型，为什么呢，原因之一便是判别模型的损失函数(loss)方便定义。
回到根源，什么是机器学习?一句话来概括就是，在训练过程中给予回馈，使得结果接近我们的期望。对于分类问题(classification)，我们希望loss在接近bound以后，就不要再有变化，所以我们选择交叉熵(Cross Entropy)作为回馈;在回归问题(regression)中，我们则希望loss只有在两者一摸一样时才保持不变，所以选择点之间的欧式距离(MSE)作为回馈。损失函数(回馈)的选择，会明显影响到训练结果的质量，是设计模型的重中之重。这五年来，神经网络的变种已有不下几百种，但损失函数却寥寥无几。例如caffe的官方文档中，只提供了八种标准损失函数 Caffe | Layer Catalogue。
对于判别模型，损失函数是容易定义的，因为输出的目标相对简单。但对于生成模型，损失函数的定义就不是那么容易。例如对于NLP方面的生成语句，虽然有BLEU这一优秀的衡量指标，但由于难以求导，以至于无法放进模型训练;对于生成猫咪图片的任务，如果简单地将损失函数定义为“和已有图片的欧式距离”，那么结果将是数据库里图片的诡异混合，效果惨不忍睹。当我们希望神经网络画一只猫的时候，显然是希望这张图有一个动物的轮廓、带质感的毛发、和一个霸气的眼神，而不是冷冰冰的欧式距离最优解。如何将我们对于猫的期望放到模型中训练呢?这就是GAN的Adversarial部分解决的问题。
**Adversarial：对抗(互怼 )**
在generative部分提到了，我们对于猫(生成结果)的期望，往往是一个暧昧不清，难以数学公理化定义的范式。但等一下，说到处理暧昧不清、难以公理化的问题，之前提到的判别任务不也是吗?比如图像分类，一堆RGB像素点和最后N类别的概率分布模型，显然是无法从传统数学角度定义的。那为何，不把生成模型的回馈部分，交给判别模型呢?这就是Goodfellow天才般的创意--他将机器学习中的两大类模型，Generative和Discrimitive给紧密地联合在了一起。
**模型一览**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2TJicg5Cm2aCsu8aUGX8xfMztfCLKzicI8UDM8sBibYefOxDThkJhiaARSMjPWLTiacoH7DoKqPrS9jKqg/640?wx_fmt=jpeg)
对抗生成网络主要由生成部分G，和判别部分D组成。训练过程描述如下
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2TJicg5Cm2aCsu8aUGX8xfMz07HcYZuONILgduibv20bUsx0juqwsibKOXgXhNqo3j2WBSx6JmRnkGicQ/640?wx_fmt=jpeg)
在整个过程中，![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2TJicg5Cm2aCsu8aUGX8xfMzI6ECE6UkaHUSwyYcVsCZZEeKQvwoBaPkBBGNHwgibnkDvBjPAyZDibJg/640?wx_fmt=jpeg)(火眼晶晶，不错杀也不漏杀)。而![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2TJicg5Cm2aCsu8aUGX8xfMz5mvorWtpsPU57ejKhRBdYeC0731CdIicCywicfw709gpFxhTEAwSKicqQ/640?wx_fmt=jpeg)则要使得![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2TJicg5Cm2aCsu8aUGX8xfMzTB44Zck0G35nYiaKdsnURtOb9icDUYjgwJ97icfVYSv6pRiasqvgsWeiaNA/640?wx_fmt=jpeg)，即让生成的图片尽可能以假乱真。整个训练过程就像是两个玩家在相互对抗，也正是这个名字Adversarial的来源。在论文中[1406.2661] Generative Adversarial Networks ，Goodfellow从理论上证明了该算法的收敛性，以及在模型收敛时，生成数据具有和真实数据相同的分布(保证了模型效果)。
从研究角度，GAN给众多生成模型提供了一种新的训练思路，催生了许多后续作品。例如根据自己喜好定制二次元妹子(逃)，根据文字生成对应描述图片(Newmu/dcgan_code, hanzhanggit/StackGAN)，甚至利用标签生成3D宜家家居模型(zck119/3dgan-release)，这些作品的效果无一不令人惊叹。同时，难人可贵的是这篇论文有很强的数学论证，不同于前几年的套模型的结果说话，而是从理论上保证了模型的可靠性。虽然目前训练还时常碰到困难，后续已有更新工作改善该问题(WGAN, Loss Sensetive GAN, Least Square GAN)，相信终有一日能克服。
从通用人工智能高层次来看，这个模型率先使用神经网络来指导神经网络，颇有一种奇妙的美感：仿佛是在辩日的两小儿一样，一开始两者都是懵懂的幼儿，但通过观察周围，相互讨论，逐渐进化出了对外界的认知。 这不正是吾等所期望的终极智能么 -- 机器的知识来源不再局限于人类，而是可以彼此之间相互交流相互学习。也难怪Yann Lecun赞叹GAN是机器学习近十年来最有意思的想法
链接：https://www.zhihu.com/question/57668112/answer/155367561
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
