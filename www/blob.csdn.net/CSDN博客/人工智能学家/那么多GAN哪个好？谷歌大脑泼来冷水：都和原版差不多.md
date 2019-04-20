# 那么多GAN哪个好？谷歌大脑泼来冷水：都和原版差不多 - 人工智能学家 - CSDN博客
2017年12月01日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：1358
*![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWPVdgWjJDEQk75HJlla0v3N8xcTGdIw51GVhmIYoC65tlwJibbGe8G1qOeCT1yyc8Qhv7V9LTA4Bg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)*
*来源：量子位*
*概要：从2014年诞生至今，生成对抗网络（GAN）热度只增不减，各种各样的变体层出不穷。*
从2014年诞生至今，生成对抗网络（GAN）热度只增不减，各种各样的变体层出不穷。有位名叫Avinash Hindupur的国际友人建立了一个GAN Zoo，他的“动物园”里目前已经收集了多达214种有名有姓的GAN。
DeepMind研究员们甚至将自己提出的一种变体命名为α-GAN，然后在论文中吐槽说，之所以用希腊字母做前缀，是因为拉丁字母几乎都被占了……
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWPVdgWjJDEQk75HJlla0v3Fl4icZj3OwDYv8U2l3afEEsYmgcqwGYGbRnUc2FfDxG8A65ZlsJ8p5Q/0?wx_fmt=png)
这还不是最匪夷所思的名字，在即将召开的NIPS 2017上，杜克大学还有个Δ-GAN要发表。
就是这么火爆！
那么问题来了：这么多变体，有什么区别？哪个好用？
于是，Google Brain的几位研究员（不包括原版GAN的爸爸Ian Goodfellow）对各种GAN做一次“中立、多方面、大规模的”评测，得出了一个有点丧的结论：
No evidence that any of the tested algorithms consistently outperforms the original one.
非常不严谨地翻译一下：
都差不多……都跟原版差不多……
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWPVdgWjJDEQk75HJlla0v3Jmt29ILaIeZqbkXILibEowq2eKhyAFekEBW1BHdAsZjj8sw0v6QyoLQ/0?wx_fmt=png)
# **比什么？**
这篇论文集中探讨的是无条件生成对抗网络，也就是说，只有无标签数据可用于学习。选取了如下GAN变体：
MM GAN
NS GAN
WGAN
WGAN GP
LS GAN
DRAGAN
BEGAN
其中MM GAN和NS GAN分别表示用minimax损失函数和用non-saturating损失函数的原版GAN。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWPVdgWjJDEQk75HJlla0v3yH71eibLIdxpse5h9SwoGpmCaQfwwRQyaSrQYFvpwWiaDC56vZ9TEUAA/0?wx_fmt=png)
除此之外，他们还在比较中加入了另一个热门生成模型VAE（Variational Autoencoder，变分自编码器）。
**对于各种GAN的性能，Google Brain团队选了两组维度来进行比较。**
一是FID（Fréchet Inception Distance），FID的值和生成图像的质量负相关。
测试FID时用了4个数据集：MNIST、Fashion MNIST、CIFAR-10和CELEBA。这几个数据集的复杂程度从简单到中等，能快速进行多次实验，是测试生成模型的常见选择。
二是精度（precision、）、查全率（recall）和F1得分，用来衡量判别式模型的质量。其中F1是精度和查全率的调和平均数。
这项测试所用的，是Google Brain研究员们自创的一个数据集，由各种角度的三角形灰度图像组成。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWPVdgWjJDEQk75HJlla0v3pHicwJ4N3nbQODaO3iabEtJWnUft3iazFTYkJibjTRREzpBEReqTMaKFFw/0?wx_fmt=jpeg)
###### *△ 精度和查全率都高、高精度低查全率、低精度高查全率、精度和查全率都低的模型的样本*
# **对比结果**
Google Brain团队从FID和F1两个方面对上面提到的模型进行比较，得出了以下结果。
## **FID**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWPVdgWjJDEQk75HJlla0v39UfNFjnRtTBauib1wD1SmHjUJGh0T4wUSPXXLq8vcps5eRFwibolqRYA/0?wx_fmt=png)
通过对每个模型100组超参数的大范围搜索，得出的结论是GAN在训练中都对于超参数设置非常敏感，没有哪个变体能够幸免，也就说，哪个GAN也没能比竞品们更稳定。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWPVdgWjJDEQk75HJlla0v3BYyekqciarMUZXJ3L5BGwUpaZlAvLqxGjFH1bfmMCDXbIKLU13aDHVw/0?wx_fmt=png)
从结果来看，每个模型的性能擅长处理的数据集不太一样，没有在所有数据集上都明显优于同类的。不过，VAE相比之下是最弱的，它所生成出的图像最模糊。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWPVdgWjJDEQk75HJlla0v3P1Hvkkpfw7UBNl5I3ZOoFQoU4uJsKjhfCpIgicf0gXLAcc3bueFdFKA/0?wx_fmt=png)
测试还显示，随着计算资源配置的提高，最小FID有降低的趋势。
如果设定一个FID范围，用比较多计算资源训练的“坏”模型，可能表现得比用较少计算资源训练的“好”模型要更好。
另外，当计算资源配置相对比较低的时候，所有模型的最小FID都差不多，也就是说，如果严格限制预算，就比较不出这些模型之间具有统计意义的显著区别。
他们经过比较得出的结论是，用能达到的最小FID来对模型进行比较是没有意义的，要比较固定计算资源配置下的FID分布。
FID之间的比较也表明，随着计算力的增加，最先进的GAN模型之间体现不出算法上的优劣差别。
## **精度、查全率和F1**
Google Brain团队还用他们的三角形数据集，测试了样本量为1024时，大范围搜索超参数来进行计算的精度和查全率。
对于特定的模型和超参数设置，最高F1得分会随着计算资源配置的不同而不同，如下图所示：
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWPVdgWjJDEQk75HJlla0v3DibcKja3RsmXMvymYqn4gP5IgqPtHPiaphEeONgAs1U62CZKFmT1qGjw/0?wx_fmt=png)
###### *△ 不同计算资源配置下各模型的F1、精度和查全率*
论文作者们说，即使是一个这么简单的任务，很多模型的F1也并不高。当针对F1进行优化时，NS GAN和WGAN的精度和查全率都比较高。
# **和原版GAN相比**
Google Brain团队还将这些变体和原版GAN做了对比。他们得出的结论是，没有实证证据能证明这些GAN变体在所有数据集上明显优于原版。
实际上，NS GAN水平和其他模型持平，在MNIST上的FID总体水平最好，F1也比其他模型要高。
# **相关链接**
要详细了解这项研究，还是得读论文：
Are GANs Created Equal? A Large-Scale Study
Mario Lucic, Karol Kurach, Marcin Michalski, Sylvain Gelly, Olivier Bousquet
https://arxiv.org/abs/1711.10337
查找某种GAN变体，可以去文章开头提到的GAN Zoo：
https://github.com/hindupuravinash/the-gan-zoo
未来智能实验室致力于研究互联网与人工智能未来发展趋势，观察评估人工智能发展水平，由中国科学院虚拟经济与数据科学研究中心刘锋、石勇、和刘颖创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；构建互联网（城市）云脑架构，形成科技趋势标杆企业库并应用与行业与智慧城市的智能提升。
*如果您对实验室的研究感兴趣，欢迎支持和加入我们。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXoz49H9w8lfGXmBjcGga9W15bmYAvZDqbbI8p0liaLcxEh1iao7YXIgJvp6MLHpskd7jHq8ibX0QzDQ/640?wx_fmt=png)

