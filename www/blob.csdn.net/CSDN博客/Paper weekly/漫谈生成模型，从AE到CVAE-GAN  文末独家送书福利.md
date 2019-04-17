# 漫谈生成模型，从AE到CVAE-GAN | 文末独家送书福利 - Paper weekly - CSDN博客





2018年06月12日 12:25:56[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：969









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




**关于作者：**彭博，人工智能、量化交易、区块链领域的资深技术专家，有 20 年以上的研发经验。在人工智能与信息科技方面，对深度学习、机器学习、计算机图形学、智能硬件等有较为深入的研究；在量化交易方面，曾在全球最大的外汇对冲基金负责程序化交易，对市场的微观和宏观行为有较为深入的理解；在区块链方面，对智能合约、DApp 开发和自动交易有较为深入的实践。知乎上科技领域的大 V，在专栏撰有大量技术文章。




本文内容节选自《深度卷积网络：原理与实践》第 8.5 节。




大家对于 GAN 都已经很熟悉了，但 GAN 的训练目前仍然存在模式坍塌等等难题。目前还有其他采用深度网络的生成模型方法，例如 AE，它们的思想可互补，提高生成图像的质量和稳定性，典型的例子是 CVAE-GAN。在此我们对这些方法做简介。




# 自编码器：从AE到VAE



自编码器（auto-encoder，AE）是经典的生成模型方法，其架构如下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWCFrckrAOgcLKmsicNqSMB3siaV84ZGMfX9z6JosXAz2tQicsAChuzmiaow/640)




AE 可分为两个网络： 



- 
编码（encoder）网络，负责从 x 到 z，可称为 E；



- 
解码（decoder）网络，负责从 z 到 x，可称为 G，因为它和 GAN 中的生成网络都是从 z 到 x。 





可将从 x 到 z 看成是压缩信息的过程，从 z 到 x 看成是解压缩的过程。例如，如果 x 是 64*64 的彩色图像，那么它有 3*64*64=12288 维。而 z 往往只有 50 到 200 维。 




AE 和 GAN 的区别在于，AE 中没有更先进的判别网络（D 网络），AE 的优化目标只是让 x 和 G(E(x)) 尽量在像素上接近。如本书的前文所述，这并不是个好目标，因此 AE 生成的图像往往很模糊，例如 AE 和 GAN 在 Fashion-MNIST 数据集的效果对比如下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzW4kskuERhf1hmeibEMCPDrob2XHTJBMLlhZOLlJWkUfoTT71viaQKOiaFw/640)




不过，AE 相对于 GAN 也有长处，就是生成的图像更均匀，光滑，训练过程更稳定。因此研究人员提出了多种将 AE 和 GAN 结合的方法，后文会介绍的 CVAE-GAN 是效果出色的例子。 




AE 的重要发展是 VAE（Variational Auto-Encoder，变分自编码器，https://arxiv.org/pdf/1312.6114）。它能解决 AE 的一个缺点：AE 的 G 只能保证将由 x 生成的 z 还原为 x。如果我们随机生成 1 个 z，经过 AE 的 G 后往往不会得到有效的图像。 




而 VAE 可让 E 生成的 z 尽量符合某个指定分布，例如标准差为 1 的多维正态分布。那么此时只需从这个分布采样出 z，就能通过 G 得到有效的图像。具体而言，这是通过一个参数化技巧（reparameterization trick）实现，可参阅 VAE 的原始论文。 




举例，对于 MNIST 数据集，如果要求 z 是 2 维的，最终效果如下图所示。





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWNibIXx8ejhYPQ4BqBJ0REdHeLmKADhhevDggGfSZ3yGXkK30wfib7mLA/640)




可见，无论选取怎样的 z 坐标，都能得到较为合理的数字图像。




# 逐点生成：PixelRNN和PixelCNN系列




由 Google 提出的 PixelRNN（https://arxiv.org/abs/1601.06759）和 PixelCNN（https://arxiv.org/abs/1606.05328）是生成模型的另一种思路。




它的方法非常直接：从左到右，从上到下，逐步生成一个个像素，最终生成整张图像。如果读者对于循环神经网络（RNN）熟悉，会意识到这是一个很适合 RNN 的问题。




基本原理如下图所示，以之前生成的像素作为输入，输出对于下一个像素的值的统计分布的预测，然后从分布采样出下一个像素。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWO5QdPKjy02wNW2cibIiaws2ujXajl3kJLhRHoEMIRFAg7CZCdHloeBfg/640)




可以想象，它会很适合生成小图。例如下图中是它生成的珊瑚礁图像，色彩很鲜艳。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWYxiaJ0k4aYUwo9xS0Ur7DVJibPExnR76UA3EaM8SibhOWzIrlnPGaXibSg/640)




而它的缺点无疑就是速度，以及目前仍然难以生成大图。于是读者可能会问，是否可构建出 PixelGAN？答案是肯定的（https://arxiv.org/abs/1706.00531）。




最后，Pixel 系列的思想尤其适合生成音频和文字，例如 WaveNet（https://deepmind.com/blog/wavenet-generative-model-raw-audio/），它用此前生成的音频采样作为输入，生成下一个采样，不断重复此过程，最终可生成高质量的语音和音乐，如下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWjIyDV54kgkIyGibKY8rnDsCBQzwJcL1kLWSeZv8JdWfhdDdKJodwXuA/640)




# 将VAE和GAN结合：CVAE-GAN




CVAE-GAN 架构的论文是《 CVAE-GAN: Fine-Grained Image Generation through Asymmetric Training》（https://arxiv.org/pdf/1703.10155v1.pdf），其中 C 代表能用分类作为输入，生成指定分类的图像。它在各个分类上生成的图像效果都相当好，如下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWNCe1ODUbicympGGVbIY9psibAGibKN2whJneTicSy2roibJvEib5CTHz04aQ/640)




上述图像都是由 CVAE-GAN 生成。它有 4 大组件，对应到 4 个神经网络，互为补充，互相促进： 



- 
E：编码器（Encoder），输入图像 x，输出编码 z。



如果还给定了类别 c，那么生成的 z 就会质量更高，即更随机，因为可移除c中已包含的信息。



- 
G：生成器（Generator）。输入编码 z，输出图像 x。



如果还给定了类别 c，那么就会生成属于类别 c 的图像。



- 
C：分类器（Classifier）。输入图像 x，输出所属类别 c。这是我们的老朋友。



- 
D：辨别器（Discriminator）。输入图像 x，判断它的真实度。





我们先看如果只使用部分组件会是怎样。首先是 CVAE，如下图所示：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWokAY1jsL7bvdJFhsibsmTiaSYbrXw50LEhWXuBTQ6D3udnFWt8Jb708g/640)




然后是 CGAN，其中 y 代表对于真实度的判别，如下图所示： 





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzW0QIgp4X5yPia9uOQcvPQB4bhSRn4f9awRQ2sug08QkKU6eHMDyiaR3iag/640)




它们的效果如下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWbvAWwGOLiagXqzqmPVhlLibClv3n1APLSgcJoLdNWh91hnEsE4CwKzGQ/640)




可见： 



- 
CVAE 生成的图像中规中矩，但是模糊。



- 
CGAN 生成的图像清晰，但有时会有明显错误。 





所以 AE 和 GAN 的方法刚好是互补。 




CVAE-GAN 的架构如下图所示：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWwgpHkxiagH0rLuH5z7IalkUQc75RVqEOOmGnbmicrqbrszG6IZac7XtQ/640)





其中的 G 有 3 个主要目标： 



- 
对于从 x 生成的 z，G 应能还原出接近 x 的 x'（像素上的接近）。这来自 AE 的思想。



- 
G 生成的图像应可由 D 鉴别为属于真实图像。这来自 GAN 的思想。



- 
G 生成的图像应可由 C 鉴别为属于 c 类别。这与 InfoGAN 的思想有些相似。 





最终得到的 z 可相当好地刻画图像。例如，同样的 z 在不同 c 下的效果如下图所示。





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWyWUvaaIvxTo19WutQ4pljnicaPbMnicXDTauiaVD41oya88dr29szYAOw/640)




这里的不同 c，代表不同的明星。相同的 z，代表其他的一切语义特征（如表情，角度，光照等等）都一模一样。




于是，通过保持 z，改变 c，可轻松实现真实的换脸效果，如下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWH6TydIoEVvh89GwAqYaKI72P830L90kSVSzq4NPib35EYWylXALexPg/640)




CVAE-GAN 在语义插值上的效果也很出色，如下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmK3GYP6E9BYXVPGzeeRYzWj6iazMgzYNzu2EA4536Ilh7xcwakutkYfb8JohWdY0c7lbI6ndr5Riag/640)




由于 CVAE-GAN 生成的样本质量很高，还可用于增强训练样本集，使其他模型（如图像分类网络）得到更好的效果。







**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****福 利 时 间#**




**以下是简单粗暴送书环节**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm5Wb1iaUHxx8mBh1Km3dWjfPlgYsxpxlV44icJWDVwuPorALMxCQglAC8Dx8JMeic5wHeNw29gJV8SA/640)



PaperWeekly × 机械工业出版社





**深度卷积网络**

**原理与实践**

<3本>





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgniay3ZYlnz3MCAzPrPkM1rT5Mjia0meqrJnc5MR0QxkbsgGImwb7cUPvKiaXXTyLdrqqIsdDFvyJrPQ/640)




**技术理论√工作原理√**实践方法****√****




**作者：**彭博




20 年开发经验资深专家/知乎大 V，从技术理论、工作原理、实践方法、架构技巧、训练策略和技术前瞻 6 个维度系统、深入讲解 DCNN 和 GAN。




深度卷积网络（DCNN）是目前十分流行的深度神经网络架构，它的构造清晰直观，效果引人入胜，在图像、视频、语音、语言领域都有广泛应用。




本书以 AI 领域最新的技术研究和和实践为基础，从技术理论、工作原理、实践方法、架构技巧、训练方法、技术前瞻等 6 个维度对深度卷积网络进行了系统、深入、详细地讲解。




以实战为导向，深入分析 AlphaGo 和 GAN 的实现过程、技术原理、训练方法和应用细节，为读者依次揭开神经网络、卷积网络和深度卷积网络的神秘面纱，让读者了解 AI 的“思考过程”，以及与人类思维的相同和不同之处。



** 参与方式 **




请在文末留言分享

**关于GAN的训练，你有什么心得体会？**



小编将随机抽取3位同学

送出机械工业出版社新书




**截止时间：**6月15日（周五）20:00











**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 订购《深度卷积网络》




