# 史上最佳GAN现身，超真实AI假照片，行家们都沸腾了 - 知乎
# 



> 夏乙 舒石 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-6d845c2f100cac20432329c3c7022f5c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='259'></svg>)
AMAZING、Incredible、Very impressive、A huge fan……

大周末的，各路人工智能、机器学习的大神，一反常态，纷纷在推特上花样夸奖、交口称赞一篇还在双盲评审中的ICLR 2019论文（BigGAN）。

因为，效果实在是，令！人！震！惊！

DeepMind负责星际项目的Oriol Vinyals，说这篇论文带来了史上最佳的GAN生成图片。陈天奇也说对这个研究及后续充满好奇。

效果有多好？

先看数字。经过ImageNet上进行128×128分辨率的训练后，BigGAN的Inception Score（IS）得分是166.3，一下子比前人52.52的最佳得分提升了100多分，离真实图像的233分更近了。而Frechet Inception Distance（FID）得分，也从之前的18.65优化到了9.6。

再看实例。你能分辨出以下哪张图片是AI生成的假图片，哪张是真实的图片么？
![](https://pic2.zhimg.com/v2-b5d02820f00990107bdd1e9395386269_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1076'></svg>)
再来一个。以下八张，哪个是假的？
![](https://pic2.zhimg.com/v2-b3c0b6c3791cfce8e34513abb6a0c529_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='2168'></svg>)
现在公布答案，以上12张，全都是生成的假图片。现在你能理解为什么大家都震惊并且齐声称赞了吧。

## **512×512轻松搞定**

其实，这些例子中还隐藏着一个更厉害的事实：上边的八拼图像，分辨率是512×512。放两张大图，来共同鉴赏一下：
![](https://pic4.zhimg.com/v2-d957b288ecaa33b8ec6b2b1d0df2485b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='894' height='892'></svg>)![](https://pic1.zhimg.com/v2-2b277015d36eeeea1829ae33d0a24a38_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='892' height='892'></svg>)
是不是依然真假难辨，非常Amazing？

这就对了，除了搞定128×128小图之外，BigGAN还能直接在256×256、512×512的ImageNet数据上训练，生成更让人信服的样本。

上边展示的512×512样本，IS和FID分数分别为241.4和10.9；而256×256样本得到了233.0分的IS、9.3分的FID。

这是BigGAN在各种分辨率下的全部得分情况：
![](https://pic1.zhimg.com/v2-60daa80040ff71e86943cdae5818cf40_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='340'></svg>)
## **原理**

“大”GAN为什么这么厉害？

BigGAN的“大”，不止是模型参数多，训练规模也是有GAN以来最大的。它的参数是前人的2-4倍，批次大小是前人的8倍。

对于图像生成这个任务来说，训练规模大真的很管用。

作者（们）为了做大规模训练，研究了如何克服这种大规模带来的特有的不稳定性。

为了适应大规模训练，他们对GAN架构做了两处简单的改动。BigGAN的生成器和鉴别器架构如下图所示：
![](https://pic2.zhimg.com/v2-c469ac5592b9dacb6aca1575c420f6a1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='709'></svg>)
BigGAN用了ResNet架构，和Takeru Miyato等人在去年ICLR 2017上发表的cGANs with Projection Discriminator里差不多，但是对判别器的通道类型做了一些改动，让每个模块第一个卷积层里的滤波器数量和输出滤波器相等。

他们的生成器G，用了单个共享类嵌入，它具有线性投影性质，能为BatchNorm层生成每个样本的增益和偏差。

这是BigGAN的生成器架构：
![](https://pic1.zhimg.com/v2-2d41856ba8f38a021981a1425ebefb58_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='490' height='790'></svg>)
生成器中的一个残差模块如下图所示：
![](https://pic1.zhimg.com/v2-5d3adb8e3e5b15767e16fe7baf711540_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='744' height='858'></svg>)
另外，他们还发现，将正交正则化应用到生成器上，能让它适用于简单的“截断技巧”，这样就可以通过截断潜在空间，来精细控制样本保真度和多样性之间的权衡。

## **樱桃时刻**

再来赏析一下BigGAN的功力。

其中有一部分，以“像，真像”而令人震惊。
![](https://pic1.zhimg.com/v2-1aca0910c36dfe695e8ba2726f732344_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='759'></svg>)![](https://pic1.zhimg.com/v2-ecf02fe2ac3249fb8b1d75800c428b2c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='743'></svg>)![](https://pic1.zhimg.com/v2-9f5f4a41a35796ff4753064d410f8bbc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='752'></svg>)
有个意大利小哥说，BigGAN生成的食物满分。看起来很好吃的样子。

还有一部分，以“想象力惊人”而令人印象深刻。

比如论文里列的“网球狗”失败案例：
![](https://pic3.zhimg.com/v2-1dd90fbc6b0c95c799a4090c129f0db6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1071'></svg>)
不过一大堆人都说这个网球狗狗，挺！萌！的！

还有这种神奇的大象。
![](https://pic1.zhimg.com/v2-af3639c25ca0fb821bdb71cbd43f358c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='513' height='512'></svg>)
当然，BigGAN也有确实生成特别不好的图片类型，比方说有人的场景。

这里挑选几个例子看看。

这个是骑自行车的人？
![](https://pic4.zhimg.com/v2-606f7b73a68f303384fe4b11ed1fdd47_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='513' height='513'></svg>)
这个是穿貂的人？
![](https://pic3.zhimg.com/v2-b967c243b5641099c79f1ce00b46f9c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='513' height='513'></svg>)
这个是女人？
![](https://pic3.zhimg.com/v2-9665f92f242cbb496b7b818ecd6d8242_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='513' height='513'></svg>)
看来，最难把握的还是人……


最后强调一下，陈天奇说，这项研究是都在分类条件下生成的图像，很好奇它捕捉到了多少分布，非条件版本会是什么样。
![](https://pic4.zhimg.com/v2-9841d37a9be829117778c560b45d5e63_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='751'></svg>)
## **论文地址**

对BigGAN论文感兴趣的朋友，可以直接前往如下地址访问：

[https://openreview.net/pdf?id=B1xsqj09Fm](https://link.zhihu.com/?target=https%3A//openreview.net/pdf%3Fid%3DB1xsqj09Fm)
![](https://pic4.zhimg.com/v2-62e5ecde48d69ddc507ac9d48ee7ae2b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='850'></svg>)
这是一篇良心论文，附录中还包含了“NG镜头集锦”环节（NEGATIVE RESULTS），讲述了研究中遇到的各种坑，非常值得一读。Google AI研究员James Bradbury专门在Twitter上说，真是太喜欢这部分了。

如果你想反cherry pick，作者还放出了生成图片的图集。图集在Google Drive上。

至于代码？好多人在找，不过目前似乎没放出来。GitHub上貌似也搜不到相关的内容。

这篇论文如此火爆，同时也引发了一些人的担心：毕竟这篇论文还在双盲评审中，现在闹得动静这么大，有可能会影响评审结果。

以及，这篇论文的作者到底是谁？目前还不得而知。




— **完 **—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


