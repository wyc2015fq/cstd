# SRGAN With WGAN：让超分辨率算法训练更稳定 | 附开源代码 - Paper weekly - CSDN博客





2018年05月21日 13:15:51[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：2034














作者丨胡智豪

单位 | 棒谷网络科技图像工程师

经历丨实现过电商大规模以图搜图、超分辨率等项目




# 写在前面




此文挖坑了很久，项目本在 7 个月前已经结束，但一直没心思把代码整理出来，后来发现有相同思路的团队把它写成论文占坑了，也就更没动力写了。昨晚发现我的 Github 项目竟然有星星，感受到了莫大的支持，忽然燃起了写文章的动力，于是就有了现在这篇。

# SRGAN




SRGAN，2017 年 CVPR 中备受瞩目的超分辨率论文，把超分辨率的效果带到了一个新的高度，而 2017 年超分大赛 NTIRE 的冠军 EDSR 也是基于 SRGAN 的变体。






**▲ **图片源自原论文






**▲ **图片源自原论文




**SRGAN 是基于 GAN 方法进行训练的**，有一个生成器和一个判别器，判别器的主体使用 VGG19，生成器是一连串的 Residual block 连接，同时在模型后部也加入了 subpixel 模块，借鉴了 Shi et al 的 **Subpixel Network**[6] 的思想，让图片在最后面的网络层才增加分辨率，提升分辨率的同时减少计算资源消耗。详细的介绍建议大家还是直接看论文 [1]，网上也有一些解读的文章，这里就直接介绍实现的一些创新细节。






**▲ **图片源自原论文




# GAN存在的问题




**传统 GAN 存在一个问题是，你无法知道什么时候该停止训练 Generator，什么时候该停止训练 Discriminator**，如果过度训练 Discriminator，Generator 就无法学习下去，反之也会导致模型效果差。




如果能有一个 loss 指标来反映训练情况的话，训练的难度就会大大降低。而 17 年提出的** WGAN**[3] 正是解决这一问题的重要方法。 




**WGAN 使用 Wasserstein 距离来描述两个数据集分布之间的差异程度**，只要把模型修改成 WGAN 的形式，就能根据一个唯一的 loss 来监控模型训练的程度。有关 WGAN 的解释强烈推荐大家阅读这篇文章：**令人拍案叫绝的 Wasserstein GAN**[4]，作者用非常直白明了的语言介绍 WGAN。




# SRGAN结合WGAN




SRGAN 的一个超赞复现来自 @董豪 之手，他也是 tensorlayer 的作者之一，他的复现项目在 Github 上得到了大量的星星，而笔者的代码就正是在他的基础上进行拓展，首先非常感谢作者的开源。




> 
· 判别器最后一层去掉 sigmoid 

· 生成器和判别器的 loss 不取 log 

· 每次更新判别器的参数之后把它们的绝对值截断到不超过一个固定常数 c 

· 不要用基于动量的优化算法（包括 momentum 和 Adam），推荐 RMSProp，SGD 也行 

--来自《令人拍案叫绝的Wasserstein GAN》





根据这篇文章的介绍，对代码进行以上四项修改，把 GAN 的训练方式转换成 WGAN，可以在 tensorboard 中监控 loss 的下降情况。因此，**笔者对项目原作的进行了一些修改：**




1. 对模型代码进行了上面 WGAN 的改造； 




2. 增加了 Tensorboard，监控 loss 下降情况； 




3. 对作者的 model.py 中，Generator 的最后一层的卷积 kernel 从 1×1 改成 9×9，这是原论文建议的结构。 




**SRGAN With Wasserstein GAN 的完整代码已经开源到笔者的 Github**[5]，里面有详细的改动介绍和使用说明，觉得好的话就给个 Star 呀！







**▲ **修改后的训练Loss下降情况




**下面是一些复现后的超分复原效果：**



















# 一个来自工业界的问题




在实际生产使用中，**遇到的低分辨率图片并不一定都是 PNG 格式的**（无损压缩的图片复原效果最好），**而且会带有不同程度的失真**（有损压缩导致的 artifacts）。笔者尝试过很多算法，例如 SRGAN、EDSR、RAISR、Fast Neural Style 等等，**这类图片目前使用任何一种超分算法都没法在提高分辨率的同时消除失真**。




这个问题我在 @董豪 SRGAN 项目的 issue 中也讨论过，同时在知乎也提出过这个问题：**SRGAN 超分辨率方法对于低清 jpg 格式的图片复原效果是否比不上对低清 png 格式的复原效果？**




可惜没有很好的答案。目前学术界貌似还没有很好的算法，这里欢迎各位在评论区或者 Github 上来讨论。




# 参考文献 & 相关链接




**[1]. Photo-Realistic Single Image Super-Resolution Using a Generative Adversarial Network**

https://www.paperweekly.site/papers/664

**[2]. Is the deconvolution layer the same as a convolutional layer?**

https://arxiv.org/abs/1609.07009

**[3]. Wasserstein GAN**

https://www.paperweekly.site/papers/808

**[4]. 令人拍案叫绝的Wasserstein GAN**

https://zhuanlan.zhihu.com/p/25071913

**[5]. 本项目仓库：SRGAN_Wasserstein**

http://github.com/JustinhoCHN/SRGAN_Wasserstein

**[6]. Real-Time Single Image and Video Super-Resolution Using an Efficient Sub-Pixel Convolutional Neural Network**

https://www.paperweekly.site/papers/386









**点击以下标题查看其他文章：**




- 
[综述：图像风格化算法最全盘点 | 内附扩展应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489172&idx=1&sn=42f567fb57d2886da71a07dd16388022&chksm=96e9c914a19e40025bf88e89514d5c6f575ee94545bd5d854c01de2ca333d4738b433d37d1f5&scene=21#wechat_redirect)

- 
[自适应注意力机制在Image Caption中的应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489065&idx=1&sn=ea3a885b18e07e457169f5e5685266a9&chksm=96e9c9a9a19e40bf8b802f452855b92d59e426d165004510fd99c9ff77fc5859ac712074002c&scene=21#wechat_redirect)

- 
[CVPR 2018值得一看的25篇论文，都在这里了](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488760&idx=1&sn=eda8baba99e3e5dfdcbaf6a2bf26f747&chksm=96e9cb78a19e426e70106410fa8ca1b48b75de5c2c3f94ea8af2603efadb9760f6bb895dd1f1&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[进击的YOLOv3，目标检测网络的巅峰之作](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489099&idx=1&sn=d2aa0e49cc72c9957c411afe7fb72e22&chksm=96e9c9cba19e40dda2281d2dd535ad5b7ad718aada74bd726290a1cbc9b1af36e92d5dfe52f5&scene=21#wechat_redirect)


- 
[Wasserstein距离在生成模型中的应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488555&idx=1&sn=d24b8b306c7e854ec7b2d3225a49aaa0&chksm=96e9cbaba19e42bdc1309a9ecade04d03e41d68935c710b50bbdcfef80632e502258401a3c4e&scene=21#wechat_redirect)








[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)

**▲**戳我查看招募详情




**#****作 者 招 募#**



****[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)****







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。






▽ 点击 | 阅读原文| 进入作者知乎专栏




