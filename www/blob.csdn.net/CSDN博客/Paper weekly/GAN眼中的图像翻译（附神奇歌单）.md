
# GAN眼中的图像翻译（附神奇歌单） - Paper weekly - CSDN博客


2017年10月09日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：2006


这是一篇总结文，总结我看过的几篇用GAN做图像翻译的文章的“套路”。
首先，什么是图像翻译？
为了说清楚这个问题，下面我给出一个不严谨的形式化定义。我们先来看两个概念。第一个概念是图像内容(content)![640.png?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8mjoF7X9zB3ozv0jxyCo7oFBQ6FuD8TLcwOrAjHibNWaW71iawTY1jtVg/640.png?wxfrom=5&wx_lazy=1)，它是图像的固有内容，是区分不同图像的依据。第二个概念是图像域(domain)，域内的图像可以认为是图像内容被赋予了某些相同的属性。举个例子，我们看到一张猫的图片，图像内容就是那只特定的喵，如果我们给图像赋予彩色，就得到了现实中看到的喵；如果给那张图像赋予铅笔画属性，就得到了一只“铅笔喵”。喵~
![640.jpeg?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8yGmFc5sW1RNTdwZEnmSNW6hSibicLb6aCGYrI8Q2OoC3E4kBnJp5FKqQ/640.jpeg?wxfrom=5&wx_lazy=1)
图像翻译是指图像内容从一个域![640.png?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8jlnGpt0wia5pLsBibjcInCBBQ9ibArzmOfIqmyJjPAriaUEBMmhRHibzU5Q/640.png?wxfrom=5&wx_lazy=1)迁移到另一个域![640.png?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8n5D90dkFv5xb2ice5lHWVN4dWdadRrrA7koB4BZa33pUwoIaKKhQQ5g/640.png?wxfrom=5&wx_lazy=1)，可以看成是图像移除一个域的属性![640.png?wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8uicPXuZKmKqPpNPUlic0KCbRIlkdB5qB4sRU0zoQ0BbYicLWdFfYvrYUw/640.png?wxfrom=5&wx_lazy=1&retryload=1)，然后赋予另一个域的属性![640.png?wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8xLNugpZe2G7hummbEKgh2RM2zklarXVxcPMicM3Hs4IxX5S9Vne368A/640.png?wxfrom=5&wx_lazy=1&retryload=1)。我们用![640.png?wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8mDZIz3bmOtS4HvXQK3J33Hn3xENIMp8VQiaqzg97MSCdOIIE0kbCnUg/640.png?wxfrom=5&wx_lazy=1&retryload=1)和![640.png?wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8U9tAZYyjuoEiaH4DdLx4HEzjibo5ibUnu1abqmechkz05bw43JIkAia6oQ/640.png?wxfrom=5&wx_lazy=1&retryload=1)来表示域![640.png?wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8WlLHp6cN5iaz4JmbguWXlkXDyXvqVXBlz8IxfsFl12Q7Gl7djqOPX2g/640.png?wxfrom=5&wx_lazy=1&retryload=1)和域![640.png?wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx84zBqBY6J3mw2BKzCzKqbCicNadKmSrzDcAKppQxyAsozfesqFh7FevA/640.png?wxfrom=5&wx_lazy=1&retryload=1)的图像，图像翻译任务即可以定义为，寻找一个合适的变换![640.png?wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8CiaSHx7TfMN31V9otKh4ibtUc7g79MlKEgXq1kVyyYY4iahD4QWTpcMicg/640.png?wxfrom=5&wx_lazy=1&retryload=1)使得
![640.png?wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8usmZv1kSlLibYGJTciaVY5LBibo0DDNib0rVV2ZvhJr6FF5ZbytCDnzCVQ/640.png?wxfrom=5&wx_lazy=1&retryload=1)
当然，还有一种图像翻译，在翻译的时候会把图像内容也换掉，下面介绍的方法也适用于这种翻译，这种翻译除了研究图像属性的变化，还可以研究图像内容的变化，在这里就不做讨论了。

## 常见的GAN图像翻译方法
下面简单总结几种GAN的图像翻译方法。

pix2pix


简单来说，它就是跟cGAN。Generator的输入不再是noise，而是图像。
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8wibg0BdJS0IpwK6UOib0RBJJBZqXYrGM28GF3L7lzSuEzjrEm6mbuia5Q/0.png)

CycleGAN/DualGAN/DiscoGAN

要求图像翻译以后翻回来还是它自己，实现两个域图像的互转。
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8ibiaqKIBNvdDhkTE7O1CJkXgVSRt75gCzCX0A7kYI7yQ07FNtKE2tiaoA/0.png)

DTN


用一个encoder实现两个域的共性编码，通过特定域的decoder解码，实现图像翻译。
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8g5yQdn7zp02yciaduEtNxbLwvSk03uIC1Qu3X6HYZNUl8jhJrYIoDog/0.png)

FaderNets


用encoder编码图像的内容，通过喂给它不同的属性，得到内容的不同表达。
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx82icREGHKQicWq7JGoBarZLkFPevkB7GmPxIXibAalzMwLLkRNJ9CiaXlSA/0.png)

IcGAN


依靠cGAN喂给它不同属性得到不同表达的能力，学一个可逆的cGAN以实现图想到图像的翻译（传统的cGAN是编码+属性到图像的翻译）。
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8Zo8HVhHBiapzyzTm2omKTyPHka9yiaPVcZrVicm5m0L0cYS2Lt1wdjbXg/0.png)

GeneGAN


将图像编码成内容和属性，通过交换两张图的属性，实现属性的互转。
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnqddg3WDY5z5ssDo5xsmZnC8SzbK0Drp0em8qKJsskUP5qtbpROtMppBtKeI4vXEIVZsA8J0rRKQ/0.png)

Face Age-cGAN


这篇是做同个人不同年龄的翻译。依靠cGAN喂给它不同属性（年龄）得到不同年龄的图像的能力，学cGAN的逆变换以得到图像内容的编码，再通过人脸识别系统纠正编码，实现保id。
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8icI2ZOdDmlY021Vh4GlrxjZVX8ic1jziazYuAkib9DdYg3yLQF5gO3WLxw/0.png)
## 图像翻译方法的完备性
我认为一个图像翻译方法要取得成功，需要能够保证下面两个一致性（必要性）：
Content consistency（内容一致性）
Domain consistency（论域一致性）
此外，我们也似乎也可以认为，满足这两点的图像翻译方法是能work的（充分性）。
我把上述两点称为图像翻译方法的完备性，换句话说，只要一个方法具备了上述两个要求，它就应该能work。关于这个完备性的详细论述，我会在以后给出。

下面，我们来看一下上述几种方法是如何达成这两个一致性的。

> 内容一致性

我把它们实现内容一致性的手段列在下面的表格里了。

![0.png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8a6hte6bLRcgByibz1ZbrgcK8RC6kqdDWK0qzns3LX5sOx2QYE30xxMQ/0.png)

这里有两点需要指出。

其一，有两个方法（IcGAN和Face Age-cGAN）依靠cGAN的能力，学cGAN的逆映射来实现图像换属性，它们会有多个训练阶段，不是端到端训练的方法。而cGAN训练的好坏，以及逆映射的好坏对实验结果影响会比较大，经过几个阶段的训练，图像的内容损失会比较严重，实际中我们也可以观察到
 IcGAN 的实验效果比较差。Face Age-cGAN通过引入人脸识别系统识别结果相同的约束，能够对内容的编码进行优化，可以起到一些缓解作用。

其二，DTN主要依靠TID loss来实现内容的一致性，而编码一般来说是有损的，编码相同只能在较大程度上保证内容相同。从DTN的emoji和人脸互转的实验我们也可以看出，emoji保id问题堪忧，参看下图。

![0.jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8RJAZ0yuCOELYP5KSiadPH65kNNZgfGK6f35UuhbUfW9icttyR7tyzR2A/0.jpeg)

> 论域一致性

论域一致性是指，翻译后的图像得是论域内的图像，也就是说，得有目标论域的共有属性。用GAN实现的方法，很自然的一个实现论域一致性的方法就是，通过discriminator判断图像是否属于目标论域。

上述几种图像翻译的方法，它们实现论域一致性的手段可以分为两种，参见下表。

![0.png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx8Ox6MIHIeiaqOo7uibR5JOCUWbdj7O0oiciaQJDvYn3zhpYgjXYunvcHIgA/0.png)

此外，可以看到，FaderNets实现两个一致性的方法都是剥离属性和内容，而实现剥离手段则是对抗训练。编码层面的对抗训练我认为博弈双方不是势均力敌，一方太容易赢得博弈，不难预料到它的训练会比较tricky，训练有效果应该不难达成，要想得到好的结果是比较难的。目前还没有看到能够完美复现的代码。文章的效果太好，好得甚至让人怀疑。

最后的最后，放一个歌单，听说听这个歌单炼丹会更快哦。

![0.jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/yAnhaHNJib1bbqMSOXXs5bRSrh22cmRx81sepxnqqcuXSnFWpXkFdkEcibNg7drZGsS0W7OA4moibTJmfSFoowGXQ/0.jpeg)

## 参考文献
1. Isola P, Zhu J Y, Zhou T, et al.Image-to-image translation with conditional adversarial networks[J]. arXiv preprint arXiv:1611.07004,
 2016.
2. Zhu J Y, Park T, Isola P, et al.Unpaired image-to-image translation using cycle-consistent adversarial networks[J]. arXiv
 preprint arXiv:1703.10593, 2017.
3. Yi Z, Zhang H, Gong P T.DualGAN: Unsupervised Dual Learning for Image-to-Image Translation[J]. arXiv preprint arXiv:1704.02510,
 2017.
4. Kim T, Cha M, Kim H, et al.Learning to discover cross-domain relations with generative adversarial networks[J]. arXiv preprint
 arXiv:1703.05192, 2017.
5. Taigman Y, Polyak A, Wolf L.Unsupervised cross-domain image generation[J]. arXiv preprint arXiv:1611.02200, 2016.
6. Zhou S, Xiao T, Yang Y, et al.GeneGAN: Learning Object Transfiguration and Attribute Subspace from Unpaired Data[J]. arXiv
 preprint arXiv:1705.04932, 2017.
7. Lample G, Zeghidour N, Usunier N, et al.Fader Networks: Manipulating Images by Sliding Attributes[J]. arXiv preprint arXiv:1706.00409,
 2017.
8. Brock A, Lim T, Ritchie J M, et al.Neural photo editing with introspective adversarial networks[J]. arXiv preprint arXiv:1609.07093,
 2016.
9. Antipov G, Baccouche M, Dugelay J L.Face Aging With Conditional Generative Adversarial Networks[J]. arXiv preprint arXiv:1702.01983,
 2017.
10. Perarnau G, van de Weijer J, Raducanu B, et al.Invertible Conditional GANs for image editing[J]. arXiv preprint arXiv:1611.06355,
 2016.

**相关活动**

**不得不读的GAN**

GAN 是现在最热门的研究方向之一，那么到底有哪些 paper 是值得一读的？

为此，我们将在 PaperWeekly 社区发起一次**GAN专题论文推荐**，欢迎大家一起参与推荐和 GAN 相关的论文。针对活动中出现的优质论文，我们将组织发起**论文共读**，由一名学术大咖为活动参与者进行在线论文解读。如果你推荐的论文被大家认可，获得了全场最高点赞数，我们将为你送出一份**PaperWeekly
 神秘大礼包**。

**活动时间**

**10 月 9 日-10 月 20 日**

**参与方式**

**1**点击本文底部的**阅读原文**进入活动页面
**2**点击**我要参加**，页面右上角将会收到活动消息通知（需注册登录）
**3**点击网站右上方的**推荐论文**进行推荐
**4**或从**今日arXiv**右侧下载arXiv
 Paper推荐**插件**，即可进行一键推荐
**5**为了更好的使用体验，建议**通过PC端访问网站**

*尚未注册PaperWeekly社区的用户请先申请注册。网站目前采用审核制注册，请如实填写个人信息，我们将在**12小时内**完成审核。

**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmUEtia3RCJ5eZHIskGRIYc1Uen4885tjUqeiaS963f1BQ5PdHHibppPMuHbfbpVsbicYsU00j6RwOA2w/640?)


