
# 利用条件GANs的pix2pix进化版：高分辨率图像合成和语义操作 | PaperDaily \#23 - Paper weekly - CSDN博客


2017年12月07日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：5155



![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**23**篇文章
本期推荐的论文笔记来自 PaperWeekly 社区用户**@Aidon**。这个名为**pix2pixHD**的项目来自英伟达和 UC Berkeley，**利****用条件 GAN 进行 2048 x 1024 分辨率的图像合成和处理。**
本文从三个方面对 pix2pix 方法做了改进，还将他们的方法扩展到交互式语义操作，这对于传统的图像逼真渲染是一个颠覆性的工作。
如果你对本文工作感兴趣，点击底部的**阅读原文**即可查看原论文。
# 关于作者：郑琪，华中科技大学硕士生，研究方向为计算机视觉和自然语言处理。

■ 论文 | High-Resolution Image Synthesis and Semantic Manipulation with Conditional GANs
■ 链接 | https://www.paperweekly.site/papers/1278
■ 作者 | Aidon

# 论文导读

现有的用于图像逼真渲染的图形学技术，在构建和编辑虚拟环境时往往非常复杂并且耗时，因为刻画真实的世界要考虑的方面太多。

如果我们可以从数据中学习出一个模型，将图形渲染的问题变成模型学习和推理的问题，那么当我们需要创造新的虚拟环境时，只需要在新的数据上训练我们的模型即可。
**之前的一些利用语义标签合成图像的工作存在两个主要问题**：1.
 难以用 GANs 生成高分辨率图像（比如 pix2pix 方法）；2. 相比于真实图像，生成的图像往往缺少一些细节和逼真的纹理。
**本文从三个方面对 pix2pix 方法做了改进**：一个 coarse-to-fine 生成器，一个 multi-scale
 判别器和一个鲁棒的 loss，从而成功合成出 2048 x 1024 的逼真图像。**此外，本文还将他们的方法扩展到交互式语义操作，这对于传统的图像逼真渲染是一个颠覆性的工作。**

# 模型介绍

**1. The pix2pix Baseline**
给定语义标签图和对应的真实照片集*(si,xi)*，该模型中的生成器用于从语义标签图生成出真实图像，而判别器用于区分真实图像和生成的图像，该条件GANs对应的优化问题如下：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPhMyMw0xia64TCib8XgTqeticmXIXawfJhCunmxSuujmUIrfMtPyMgiaAIw/?)
其中：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPMguxD3dWM8Sfe2OP4ibzCvCWV34pmD6mdgBHTjiaDAcictJctJNyscnow/?)

pix2pix 采用 U-Net 作为生成器，在 Cityscapes 数据集上生成的图像分辨率最高只有 256 x 256。
**2. Coarse-to-fine 生成器**
这里一个基本的想法是将生成器拆分成两个子网络*G={G1,G2}*：全局生成器网络*G1*和局部增强网络*G2*，前者输入和输出的分辨率保持一致（如 1024 x 512），后者输出尺寸（2048 x 1024）是输入尺寸（1024 x 512）的 4 倍（长宽各两倍）。
以此类推，如果想要得到更高分辨率的图像，只需要增加更多的局部增强网络即可（如*G={G1,G2,G3}*），具体的网络结构如图所示：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPuxCic8rFV9hSiabZ8Bcf5LHWeCaESnUkLq48CpsGEmNmHg4NE3HWbn5g/?)

其中*G1*由卷积前端![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPPCZWicnOCO4ge9W3u1pSatfd1qORy2fbj6iaLtlGI4fsdl5PB7B0uiaxA/?)，一系列残差块![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxP7hIkN1mODFbDiao25ty18htMDkv3wyzDyeeCj6EZhQvBnRD13qfAyTQ/?)和一个转置卷积后端![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPdeia5cghmwVYT9gopVl8gBMRIBny2fIFXKAx3zz1PrfTmMkjciamLAPQ/?)组成，*G2*也由卷积前端![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPYTt5UMshsCWu8IhOrCBVVrUWc9Kia0vAiaQdmprQbfkPBfpnyAPoNfVA/?)，一系列残差块![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPpLscWIrX3PxbPrFbd8Q35XwmfYofOg5PwewxJ2mw3Y7WMq5Kvice2qw/?)和一个转置卷积（有时也叫反卷积）后端![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPA2I0v0PeyoXW5RfWQl2AmYSRxqaJrw2uibXhVAaY6fGEFx1FAy2ct1A/?)组成。从图中可以看到，![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPpLscWIrX3PxbPrFbd8Q35XwmfYofOg5PwewxJ2mw3Y7WMq5Kvice2qw/?)的输入是由![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPYTt5UMshsCWu8IhOrCBVVrUWc9Kia0vAiaQdmprQbfkPBfpnyAPoNfVA/?)的输出和![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPdeia5cghmwVYT9gopVl8gBMRIBny2fIFXKAx3zz1PrfTmMkjciamLAPQ/?)最后一层的
 feature map 相加得到，这样就使得全局信息可以传递到*G2**。*
**3. Multi-scale 判别器**
要在高分辨率下区分真实的与合成的图像，就要求判别器有很大的感受野，这需要更深的网络或者更大的卷积核才能实现，而这两种选择都会增加网络容量从而使网络更容易产生过拟合问题，并且训练所需的存储空间也会增大。
这里用 3 个判别器 {*D1,D2,D3*} 来处理不同尺度的图像，它们具有相同的网络结构：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxP46srrgIPZmr1zjEZWRHUibwK74rsfDZbWM8bQdHDeyH1hwQqtOiaASqw/?)
**4. 改进的 adversarial loss**
由于生成器要产生不同尺度的图像，为使训练更加稳定，这里引入特征匹配损失：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPmDbL2DRMdm30dbM3gZZzCiaGFGdmVqkEptuSFCz0u6oL6xKAFKVNdIA/?)
其中![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPPGgtCT432YeUfpSEBoCHt1EWhYhkic9NLS5bC5CkIhKibyicnzFOO3cFQ/?)表示判别器*Dk*提取的第*i*层特征，*T*为总的层数，*Ni*为该层总元素的个数。于是，总的目标函数如下：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPjKRu38oaZMr17gGQalJ0uoYhmCtic0RzPppdribeZawXsWdyULWDX06w/?)
**5. 学习 instance-level 的特征嵌入**
当前的图像合成方法都只利用了 pixel-level 的语义标签图，这样无法区分同类物体，而 instance-level 的语义标签为每一个单独的物体提供了唯一的标签。
文章指出，示例图（instance map）所提供的最重要的信息其实是物体的边缘。所以本文首先计算出示例边缘图（instance boundary map），如图所示：

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPupjUleB70ylBSq0paZKoqDiaOO5aVY5ZEU0gJsxTxNvds43246eCbgg/?)

然后将语义标签图和示例边缘图连接起来，输入到生成器网络中。
考虑到一个理想的图像合成算法应该能够从同一个语义标签图产生出多种逼真的图像，而现有的方法无法让用户直观地控制产生什么样的图像，并且不允许 object-level 的控制，于是本文提出将额外的低维特征也输入到生成器网络中。
为此，需要训练一个编码器网络*E*，用于确定与真实图像中每个目标示例的低维特征向量，以*G(s,E(x))*代替之前的*G(s)*，如图所示：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPxnUp17cwfp25NAcRJsOk4PzfvYPiciaeTInia79DRGeRItErnop9szy8Q/?)


在编码器训练好之后，输入训练图像，找出图像中的所有示例，并记录对应的特征。然后利用 KK-means 聚类得到每一个语义类别的特征编码。推断时，随机选取一个聚类中心作为编码特征，与之前的标签图连接输入到生成器网络中。

# 实验结果

实验中设置*λ=10,K=10*，用 3 维向量对示例特征进行编码，采样 LSGANs 用于稳定训练。实验比较了不同的图像合成算法，包括 pix2pix 和 CRN，还比较了加入感知损失（w/o VGG）的结果，其中*F(i)*表示 VGG 网络的第*i*层。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPbc6icw9JNgXldSfIxYiczvG1DLQ36EUnJr1ZHib79CRWTJ8GSwWphrk1A/?)
在 Cityscapes 数据集上的实验结果如下：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPEgursr6IgJXkY9cP4m1ojMlGVbvWCyzP5E9DVoHZe6eHsfEFI68ZUw/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPyTJT8FibOWNb3vhLZqVK9Twwj9stagf1gGrHbKC7ic2pHgBbvPvUXFcQ/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPplUvYDRboLD984EE8dKIAKTIsG6zicyN8qViazDYH1Bia4joAa6I64nhw/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPkSPXXHyuOTtQ0DtgCZGBtbaIAHAogDJtb5Ko6iaXkibZW5EiaVskwNt2Q/?)
在
 NYU Indoor RGBD 数据集上的实验结果如下：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxP0ic1h8uvHFQPH7ZKwq2FJzVZyd3BAo49VyhqwND09v04IOrpdyOWA6A/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPoNR0UzDusTXf0atm8NmbRACicysicicKU4xPXQC5jPMeWSNx0mcicNJtRQ/?)
更多关于的实验结果可以阅读原文或者访问 project 网页：
https://tcwang0509.github.io/pix2pixHD/

# 总结

本文提出了一个有通用性的基于条件 GANs 的网络框架，用于高分辨率图像合成和语义操作。相比于 pix2pix，本文在语义分割结果和图像合成的清晰度以及细节上都有了很大的提升。

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
我是彩蛋

**PaperWeekly x 百度**

深度学习有奖调研

**你最喜欢的深度学习框架是****...**

*TensorFlow?**PyTorch? Caffe?*
**奖品福利**

我们将从认真作答的同学中抽取50名
赠送限量版礼品一份作为答谢

**长按识别二维码，参与调研**
*本次活动奖品由百度提供

![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgn5IactN9nZxEQvic1siawcxPK320HoNUOlotLia5RAOuEV0DTnhicz8qE7DrmvTIkdJibCibGjH4rdOuMw/?)

**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)
▽ 点击 |阅读原文|
 查看原论文


