# 17种GAN变体的Keras实现请收好 | GitHub热门开源代码 - 知乎
# 



> 夏乙 编译整理
量子位 出品 | 公众号 QbitAI
题图来自Kaggle blog

从2014年诞生至今，生成对抗网络（GAN）始终广受关注，已经出现了200多种有名有姓的变体。

这项“造假神技”的创作范围，已经从最初的手写数字和几百像素小渣图，拓展到了[壁纸级高清照片](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247491675%26idx%3D1%26sn%3D52f62f181507ea3fee9b63b592f5037f%26chksm%3De8d05729dfa7de3f52f7ecd0b9fb6aa97175771246b945b60cf98cb27176e6745f6a37252947%26scene%3D21%23wechat_redirect)、[明星脸](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247492785%26idx%3D2%26sn%3Dc3df45b77458e8c1094375d4cf231ae2%26chksm%3De8d053c3dfa7dad55b865612054be9049ff0fd3b896ee14120f952656ac902f800dbf609d40a%26scene%3D21%23wechat_redirect)，甚至[艺术画作](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247489414%26idx%3D1%26sn%3Dfb6ed561e87f1e80694edfa8824be57f%26chksm%3De8d3a0f4dfa429e27a74a7cfd8c4ae2e99f5cf153ef95153dcfeef7015648101e2ffd0be5168%26scene%3D21%23wechat_redirect)。

心痒难耐想赶快入门？

通过自己动手、探索模型代码来学习，当然是坠吼的~如果用简单易上手的Keras框架，那就更赞了。

一位GitHub群众eriklindernoren就发布了**17种GAN的Keras实现**，得到Keras亲爸爸François Chollet在Twitter上的热情推荐。

干货往下看：

[eriklindernoren/Keras-GAN](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN)

## **AC-GAN**

带辅助分类器的GAN，全称Auxiliary Classifier GAN。
![](https://pic3.zhimg.com/v2-4088845458080c9c91a635bad813a38e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='335'></svg>)
在这类GAN变体中，生成器生成的每张图像，都带有一个类别标签，鉴别器也会同时针对来源和类别标签给出两个概率分布。

论文中描述的模型，可以生成符合1000个ImageNet类别的128×128图像。

**code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/acgan/acgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/acgan/acgan.py)

**paper**：

Conditional Image Synthesis With Auxiliary Classifier GANs

Augustus Odena, Christopher Olah, Jonathon Shlens

[[1610.09585] Conditional Image Synthesis With Auxiliary Classifier GANs](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1610.09585)

## **Adversarial Autoencoder**

这种模型简称AAE，是一种概率性自编码器，运用GAN，通过将自编码器的隐藏编码向量和任意先验分布进行匹配来进行变分推断，可以用于半监督分类、分离图像的风格和内容、无监督聚类、降维、数据可视化等方面。

在论文中，研究人员给出了用MNIST和多伦多人脸数据集 (TFD)训练的模型所生成的样本。
![](https://pic2.zhimg.com/v2-c56ce4de7ff963a44a3541ecae531601_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='639'></svg>)
**code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/aae/adversarial_autoencoder.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/aae/adversarial_autoencoder.py)

**paper**：

Adversarial Autoencoders

Alireza Makhzani, Jonathon Shlens, Navdeep Jaitly, Ian Goodfellow, Brendan Frey

[[1511.05644] Adversarial Autoencoders](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1511.05644)

## **BiGAN**

全称Bidirectional GAN，也就是双向GAN。这种变体能学习反向的映射，也就是将数据投射回隐藏空间。
![](https://pic3.zhimg.com/v2-1dc99a0aef0bc6bd885022cf96a1a482_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='533'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/bigan/bigan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/bigan/bigan.py)

**Paper**：

Adversarial Feature Learning

Jeff Donahue, Philipp Krähenbühl, Trevor Darrell

[[1605.09782] Adversarial Feature Learning](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1605.09782)

## **BGAN**

虽然简称和上一类变体只差个i，但这两种GAN完全不同。BGAN的全称是boundary-seeking GAN。

原版GAN不适用于离散数据，而BGAN用来自鉴别器的估计差异度量来计算生成样本的重要性权重，为训练生成器来提供策略梯度，因此可以用离散数据进行训练。

BGAN里生成样本的重要性权重和鉴别器的判定边界紧密相关，因此叫做“寻找边界的GAN”。
![](https://pic3.zhimg.com/v2-73fd2de0076b1408865919511c6dfe86_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='848'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/bgan/bgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/bgan/bgan.py)

**Paper**：

Boundary-Seeking Generative Adversarial Networks

R Devon Hjelm, Athul Paul Jacob, Tong Che, Adam Trischler, Kyunghyun Cho, Yoshua Bengio

[[1702.08431] Boundary-Seeking Generative Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1702.08431)

## **CC-GAN**

这种模型能用半监督学习的方法，修补图像上缺失的部分。
![](https://pic1.zhimg.com/v2-b841a52c7962e0c57fa13dea750f9430_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1040'></svg>)![](https://pic2.zhimg.com/v2-fc69e6d06cafab20642f7a89433e9b85_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='587'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/ccgan/ccgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/ccgan/ccgan.py)

**Paper**：

Semi-Supervised Learning with Context-Conditional Generative Adversarial Networks

Emily Denton, Sam Gross, Rob Fergus

[[1611.06430] Semi-Supervised Learning with Context-Conditional Generative Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1611.06430)

## **CGAN**

条件式生成对抗网络，也就是conditional GAN，其中的生成器和鉴别器都以某种外部信息为条件，比如类别标签或者其他形式的数据。
![](https://pic3.zhimg.com/v2-4c8ec31193bd55539eebc0c743b173e6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1030' height='918'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/cgan/cgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/cgan/cgan.py)

**Paper**：

Conditional Generative Adversarial Nets

Mehdi Mirza, Simon Osindero

[[1411.1784] Conditional Generative Adversarial Nets](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1411.1784)

## **Context Encoder**

这是一个修补图像的卷积神经网络（CNN），能根据周围像素来生成图像上任意区域的内容。
![](https://pic1.zhimg.com/v2-a5eb5f09f71887b457fa6c5aeec6a9fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='690' height='948'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/context_encoder/context_encoder.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/context_encoder/context_encoder.py)

**Paper**：

Context Encoders: Feature Learning by Inpainting

Deepak Pathak, Philipp Krahenbuhl, Jeff Donahue, Trevor Darrell, Alexei A. Efros

[Feature Learning by Inpainting](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1604.07379)

## **CoGAN**

这类变体全名叫coupled GANs，也就是耦合对抗生成网络，其中包含一对GAN，将两个生成模型前几层、两个辨别模型最后几层的权重分别绑定起来，能学习多个域的图像的联合分布。

**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/cogan/cogan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/cogan/cogan.py)

**Paper**：

Coupled Generative Adversarial Networks

Ming-Yu Liu, Oncel Tuzel

[[1606.07536] Coupled Generative Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1606.07536)

## **CycleGAN**

这个模型是[加州大学伯克利分校的一项研究成果](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247484928%26idx%3D2%26sn%3D4442def06e466086dd7002c70152bb63%26chksm%3De8d3b172dfa438647fa14ad3527167ebe9848c9c948c49d41e6f7372dd9ca8438971cecdcab0%26scene%3D21%23wechat_redirect)，可以在没有成对训练数据的情况下，实现图像风格的转换。



![](https://pic3.zhimg.com/v2-d5d9fc801c7876374cb379562bc9f08e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='531'></svg>)
这些例子，你大概不陌生：
![](https://pic1.zhimg.com/v2-aadb4d252cf60f4f30f23248c12c74c0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='519'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/cyclegan/cyclegan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/cyclegan/cyclegan.py)

**Paper**：

Unpaired Image-to-Image Translation using Cycle-Consistent Adversarial Networks

Jun-Yan Zhu, Taesung Park, Phillip Isola, Alexei A. Efros

[[1703.10593] Unpaired Image-to-Image Translation using Cycle-Consistent Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.10593)

论文原作者开源了Torch和PyTorch的实现代码，详情见项目主页：

[Unpaired Image-to-Image Translation using Cycle-Consistent Adversarial Networks](https://link.zhihu.com/?target=https%3A//junyanz.github.io/CycleGAN/)

## **DCGAN**

深度卷积生成对抗网络模型是作为无监督学习的一种方法而提出的，GAN在其中是最大似然率技术的一种替代。
![](https://pic1.zhimg.com/v2-7c869418709ea4f278322f01223b5220_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='691'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/dcgan/dcgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/dcgan/dcgan.py)

**Paper**：

Unsupervised Representation Learning with Deep Convolutional Generative Adversarial Networks

Alec Radford, Luke Metz, Soumith Chintala

[[1511.06434] Unsupervised Representation Learning with Deep Convolutional Generative Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1511.06434)

## **DualGAN**

这种变体能够用两组不同域的无标签图像来训练图像翻译器，架构中的主要GAN学习将图像从域U翻译到域V，而它的对偶GAN学习一个相反的过程，形成一个闭环。
![](https://pic4.zhimg.com/v2-b55d0d34e5b750c1544f13259edd7123_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='473'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/dualgan/dualgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/dualgan/dualgan.py)

**Paper**：

DualGAN: Unsupervised Dual Learning for Image-to-Image Translation

Zili Yi, Hao Zhang, Ping Tan, Minglun Gong

[Unsupervised Dual Learning for Image-to-Image Translation](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1704.02510)

## **GAN**

对，就是Ian Goodfellow那个原版GAN。
![](https://pic2.zhimg.com/v2-ac66035245530738e17baa4d1574286d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1068' height='796'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/gan/gan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/gan/gan.py)

**Paper**：

Generative Adversarial Networks

Ian J. Goodfellow, Jean Pouget-Abadie, Mehdi Mirza, Bing Xu, David Warde-Farley, Sherjil Ozair, Aaron Courville, Yoshua Bengio

[[1406.2661] Generative Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1406.2661)

## **InfoGAN**

这个变体是GAN的信息论扩展（information-theoretic extension），能完全无监督地分别学会不同表示。比如在MNIST数据集上，InfoGAN成功地分别学会了书写风格和数字的形状。

**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/infogan/infogan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/infogan/infogan.py)

**Paper**：

InfoGAN: Interpretable Representation Learning by Information Maximizing Generative Adversarial Nets

Xi Chen, Yan Duan, Rein Houthooft, John Schulman, Ilya Sutskever, Pieter Abbeel

[Interpretable Representation Learning by Information Maximizing Generative Adversarial Nets](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1606.03657)

## **LSGAN**

最小平方GAN（Least Squares GAN）的提出，是为了解决GAN无监督学习训练中梯度消失的问题，在鉴别器上使用了最小平方损失函数。

**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/lsgan/lsgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/lsgan/lsgan.py)

**Paper**：

Least Squares Generative Adversarial Networks

Xudong Mao, Qing Li, Haoran Xie, Raymond Y.K. Lau, Zhen Wang, Stephen Paul Smolley

[[1611.04076] Least Squares Generative Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1611.04076)

## **Pix2Pix**

这个模型大家应该相当熟悉了。它和CycleGAN出自同一个伯克利团队，是CGAN的一个应用案例，以整张图像作为CGAN中的条件。
![](https://pic2.zhimg.com/v2-dfe1fe09c1bec6e1bcbcee497be4ad59_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='684'></svg>)
在它基础上，衍生出了各种上色Demo，波及[猫](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247484309%26idx%3D3%26sn%3D8c9884e899b590bb675c7773e0b6f250%26chksm%3De8d3b4e7dfa43df1d656380d8f2059bea2a14a9f97990a05a2ea111a8f4eda2a44e4d6dfd23c%26scene%3D21%23wechat_redirect)、[人脸](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247486048%26idx%3D2%26sn%3Dbe762df28877eb59a1f75bfea10c0e73%26chksm%3De8d3bd12dfa43404f2cbf790454145593c1117c6ea607c5ff2e0974bc7974771da78ba91c33f%26scene%3D21%23wechat_redirect)、房子、包包、[漫画](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247492663%26idx%3D1%26sn%3D3f9df390e6eea05159510ab1601895d6%26chksm%3De8d05345dfa7da5331d10cabc6bd65cbd91d2e102f6e05ea9db179dd894b61d3ec2321a04307%26scene%3D21%23wechat_redirect)等各类物品，甚至还有人用它来[去除（爱情动作片中的）马赛克](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247486669%26idx%3D6%26sn%3Df74e6b03f9c066d77d42b034f001e050%26chksm%3De8d3bbbfdfa432a92f034a7b6333604a42b747b957ba49d4ec47568d73b51b49a16dc21cb5a3%26scene%3D21%23wechat_redirect)。
![](https://pic2.zhimg.com/v2-a5624d9527dc474e65eaadd803080099_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='397'></svg>)
**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/pix2pix/pix2pix.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/pix2pix/pix2pix.py)

**Paper**:

Image-to-Image Translation with Conditional Adversarial Networks

Phillip Isola, Jun-Yan Zhu, Tinghui Zhou, Alexei A. Efros

[[1611.07004] Image-to-Image Translation with Conditional Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1611.07004)

Pix2Pix目前有开源的Torch、PyTorch、TensorFlow、Chainer、Keras模型，详情见项目主页：

[Image-to-Image Translation with Conditional Adversarial Networks](https://link.zhihu.com/?target=https%3A//phillipi.github.io/pix2pix/)

## **SGAN**

这个变体的全称非常直白：半监督（Semi-Supervised）生成对抗网络。它通过强制让辨别器输出类别标签，实现了GAN在半监督环境下的训练。

**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/sgan/sgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/sgan/sgan.py)

**Paper**:

Semi-Supervised Learning with Generative Adversarial Networks

Augustus Odena

[[1606.01583] Semi-Supervised Learning with Generative Adversarial Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1606.01583)

## **WGAN**

这种变体全称Wasserstein GAN，在学习分布上使用了Wasserstein距离，也叫Earth-Mover距离。新模型提高了学习的稳定性，消除了模型崩溃等问题，并给出了在debug或搜索超参数时有参考意义的学习曲线。

本文所介绍repo中的WGAN实现，使用了DCGAN的生成器和辨别器。

**Code**：

[https://github.com/eriklindernoren/Keras-GAN/blob/master/wgan/wgan.py](https://link.zhihu.com/?target=https%3A//github.com/eriklindernoren/Keras-GAN/blob/master/wgan/wgan.py)

**Paper**：

Wasserstein GAN

Martin Arjovsky, Soumith Chintala, Léon Bottou

[[1701.07875] Wasserstein GAN](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1701.07875)

**最后补充一点**，作者为了让没有GPU的人也能测试这些实现，比较倾向于使用密集层（dense layer），只要在模型中能得出合理的结果，就不会去用卷积层。

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


