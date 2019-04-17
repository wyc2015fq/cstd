# 首届TensorFlow开发者大会：值得关注的亮点都在这里 - 知乎
# 



当地时间 2 月 15 日，谷歌在加州山景城召开了第一届年度 TensorFlow 开发者大会（TensorFlow Developer Summit 2017），这可算得上是 TensorFlow 开发者、支持者与爱好者的第一次盛会，谷歌也在此次会议上发布了开发者期待已久的 [TensorFlow 1.0](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2291)。


经过一年多的发展，TensorFlow 得到了越来越多开发者的认可，也成为了 GitHub 上最受欢迎的框架之一。从发布以来，TensorFlow 一直在不断完善和增加新功能，比如分布式 TensorFlow、Windows 系统支持等，直到最近 TensorFlow 1.0 正式版的诞生。
![](https://pic3.zhimg.com/v2-cbb7318b32300c79ae715bd680c828a6_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2844' height='1576'></svg>)

*TensorFlow 的版本更迭*


但除了这个我们早就已经预计到的重磅消息之外，这个首届 TensorFlow 开发者大会上还有什么值得开发者关注的亮点呢？机器之心在此根据本次大会上的演讲对会上值得关注的内容进行了梳理，并按框架对比、产品和应用、移动端与嵌入式 TensorFlow、资源分别进行了总结，希望能对 TensorFlow 开发者能有所帮助。


**框架对比**


自开源以来，TensorFlow 经过一年多的发展已经成为了 GitHub 上最流行的框架。如同 Jeff Dean 下图中演示那样，短短一年时间，TensorFlow 已经超越 scikit-learn、Caffe 等框架，已在 GitHub 获得了最多的 Star 量。
![](https://pic1.zhimg.com/v2-6cf6353cd58688727577122fb4b2f7b8_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2738' height='1496'></svg>)

*有大量的人在为 TensorFlow 作出贡献，并用其进行各种有趣的尝试。*


此外，Jeff Dean 在 Keynote 中介绍说，TensorFlow 现在已经支持 Python、C++、Java、R、Haskell、Go 在内的多种语言。


框架之间的对比也成为了机器学习社区所关注的一个话题。在一篇文章中，数据科学公司 Silicon Valley Data Science 的数据工程师 Matt Rubashkin（UC Berkeley 博士）对深度学习的 7 种流行框架[进行了横向对比](https://zhuanlan.zhihu.com/p/25285133)，其中包括语言支持、速度、兼容 Keras 在内的 8 项衡量标准，结果如下：

![](https://pic1.zhimg.com/v2-67c36f046eb38d4a130fbf68f7611368_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='563'></svg>)
在本次开发者大会上，谷歌专门也设置了两场演讲凸显 TensorFlow 的优势：XLA 以及 Keras 与 TensorFlow 的融合。

- 
速度是高效的机器学习框架的一切。在这次大会中，Chris Leary 与 Todd Wang 讲解了通过 XLA（加速线性代数）方法减少训练和推断时间的方式。他们介绍了 TensorFlow 如何使用 XLA、JIT、AOT 以及其它编译技术来最小化执行时间和最大化计算资源。![](https://pic4.zhimg.com/v2-6e5fbcf07c3fe58bd5c5f2b414918b43_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1880' height='1056'></svg>)


- 
兼容 Keras：Keras 是成长最快的深度学习框架之一。在此次大会上，Keras 的主要作者 Francois Chollet 用视频 QA 案例演示了如何在 TensorFlow 中使用 Keras。![](https://pic1.zhimg.com/v2-53375d83c30573a4c4567efa294a7f38_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1888' height='1058'></svg>)



**产品与应用**


经过一年飞速的发展，TensorFlow 也逐渐得到了业界的认可，许多企业、公司都在基于 TensorFlow 开发自己的产品或将 TensorFlow 整合到自己的产品中去。其中包括 Airbnb、Uber、Twitter、英特尔和高通等等，当然也还有去年宣布从 [Torch 转向 TensorFlow 的 DeepMind](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650715087%26idx%3D3%26sn%3Da736c842914fc58f4789219a85a66206%26scene%3D21%23wechat_redirect)。

![](https://pic2.zhimg.com/v2-b1f9f8bf65228bee67079fa08264b261_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1884' height='1050'></svg>)
而谷歌自己当然更是在产品开发上给予了 TensorFlow 所有可以提供的支持。据谷歌工程开发主管 Megan Kacholia 介绍，TensorFlow 目前已经在以下十几种产品中得到了应用，其中包括谷歌翻译、Google Play、YouTube 和 Gmail 等。

![](https://pic4.zhimg.com/v2-d6b96e79ec59dd82cdbbd8ce7cf0cf17_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2730' height='1474'></svg>)
谷歌也不吝于分享自己在 TensorFlow 产品应用方面的经验。Google Research 的软件工程师 Jonathan Hseu 在一个演讲中介绍了 TensorFlow 生态系统（参考：[《资源 | TensorFlow 生态系统：与多种开源框架的融合》](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1770)），谈到了 TensorFlow 和产品基础设施的整合方式，并介绍了从数据准备到模型训练到产品应用整个过程。

![](https://pic3.zhimg.com/v2-8bf55f150da1ab2c24931e1a2d5e6a96_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='574' height='266'></svg>)
在数据准备阶段，主要处理过程是：从各个数据源获取数据→执行预处理→导出一个 TensorFLow 支持的文件格式。在这个阶段用的比较多的工具是 Apache Spark、Hadoop MapReduce 和 Apache Beam。


在模型训练阶段，可以选择本地训练（自己的本地机器或远程虚拟机）或分布式训练（速度更快，但需要合适的基础设施）。


然后就是将训练好的模型导出投入到产品中，Hseu 在这里推荐了 TensorFlow Serving 和 In-Process TensorFlow。


而在这些阶段的语言支持上，Python 的支持当然是最好的。

![](https://pic1.zhimg.com/v2-9a16c61902435a4cead044a5711339a0_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='898' height='507'></svg>)
接下来，Google Research 的软件工程师 Noah Fiedel 就做了关于 TensorFlow Serving（serving 是指将训练好的模型应用到生产中的过程）的演讲。Fiedel 介绍说，serving 的目标是实现在线的、低延迟的应用，能将多个模型应用到单一一个流程中，可以随时间加载一个模型的多个版本，可以实时计算成本变化以满足产品需求（通过 CloudML、Docker & K8s 自动扩展），在训练时间通过 mini-batching 提高效率（除非有异步的要求）。而 TensorFlow Serving 就是一个专为生产环境设计的，用于机器学习模型的灵活高性能 serving 平台：[https://tensorflow.github.io/serving](https://link.zhihu.com/?target=https%3A//tensorflow.github.io/serving)

![](https://pic2.zhimg.com/v2-864f94e303c4bb821e910e120a2cb019_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='313'></svg>)
还在演讲后面介绍一种新技术 SavedModel，这是一种用于 TensorFlow 模型的通用的序列化格式（universal serialization format），已经包含在了 TensorFlow 1.0 中，其有两个重要功能：支持多个 MetaGraph（同时共享变量和 asset）和 SignatureDef。

![](https://pic2.zhimg.com/v2-a6d21dfc9d11e11bff00e38e118d9eed_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='355' height='214'></svg>)
其中 SignatureDef 定义了由 TensorFlow graph 所支持的计算的签名。

![](https://pic2.zhimg.com/v2-0252dcd19b84e3709ee3cd81f87bd3d1_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='857' height='475'></svg>)
此外还有 Multi-headed Inference 和 Sequence Models 技术：

![](https://pic1.zhimg.com/v2-c7b0997cadbc3c65ccf4f91b4c05b54c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='852' height='474'></svg>)
分布式 TensorFlow 也是一个值得关注的亮点，Google Research 的软件工程师 Derek Murray 带来了一个自底向上的关于分布式 TensorFlow 的介绍，并展示了所有可以用来利用这种力量的工具。我们为什么要使用分布式 TensorFlow 呢？随着技术和方法的不断发展，深度学习系统的规模也变得越来越大，对计算资源的要求也随之增长。为了应对这个问题，我们可以将计算分配给不同的 GPU 集群而并行地进行计算，从而减少计算时间。

![](https://pic4.zhimg.com/v2-0b387d97fb7dd3b71ab9e4f2fb5cd933_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='576' height='320'></svg>)
我们需要分布式 TensorFlow 的情况是模型非常大的时候，比如谷歌的「[宽度&深度](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650716578%26idx%3D1%26sn%3Daae84df4e4e218afcd9f2d7cc88c96eb%26scene%3D21%23wechat_redirect)」模型 和超大规模模型，现有的单个硬件可能无法将它装进去进行计算。

![](https://pic2.zhimg.com/v2-946851cdd8429731e653262ba0293915_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='876' height='495'></svg>)
*超大规模有多达 680 亿个参数*


在具体的应用方面，Google Research 的软件工程师 Heng-Tze Cheng 介绍了用 TensorFlow 实现「宽度&深度学习」——将记忆（memorization）和归纳（generalization）结合到一起。参阅《[深度 | 谷歌新开源「宽度&深度学习」框架：结合记忆和归纳实现更优推荐](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650716578%26idx%3D1%26sn%3Daae84df4e4e218afcd9f2d7cc88c96eb%26scene%3D21%23wechat_redirect)》，该网络已经在 Google Play 上得到了应用。![](https://pic1.zhimg.com/v2-2a38a6438585f8f80491ccd4387c327c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='912' height='508'></svg>)


TensorFlow 的高级 API，你需要 10 行代码就能实现一个这种类型的网络模型。

![](https://pic2.zhimg.com/v2-3cd37ebd285b47af4cad085ab6fcaea9_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='912' height='501'></svg>)
Google Research 的产品经理 Lily Peng 以视网膜成像为例介绍了 TensorFlow 在医疗领域的应用。机器之心之前已经有过介绍了《[重磅 | 谷歌研发人工智能眼科医生：用深度学习诊断预防失明](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1913)》。在谈到 TensorFlow 所发挥的作用时，Peng 介绍说 TensorFlow 的优点包括：快速的原型构建、支持大规模实验并且可以根据实际的应用所收集到的数据和标签重新训练模型。

![](https://pic4.zhimg.com/v2-0b89f25bb7f1e322ed04b7327fc3d8a7_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='927' height='515'></svg>)
此外，不久之前得以上 Nature 封面的癌症方面的研究也应用到了 TensorFlow。斯坦福大学的研究人员训练了一个可以诊断皮肤癌的算法。在论文中，他们展示了使用一个单一的深度卷积神经网络进行皮肤病变分类的过程，该网络仅使用像素和疾病标签作为输入，直接从图像中端到端地训练出来。测试结果显示深度卷积神经网络在这两个任务上的表现都达到了所有测试的专家的水平，证明了该人工智能的皮肤癌鉴定水平达到了媲美皮肤科医生的水平。配备该深度神经网络的移动设备可以让皮肤科医生的诊断拓展到临床之外。


在此次开发者大会上，论文的合作者之一 Brett Kuprel 讲解了如何使用 TensorFlow 进行癌症图像分类，这是受到学界、业界极大关注的应用之一。

![](://shp.qpic.cn/qqvideo_ori/0/k03756n3r9k_228_128/0)癌症图像分类 - 腾讯视频https://v.qq.com/x/page/k03756n3r9k.html

此外，谷歌研究科学家 Doug Eck 介绍了基于 TensorFlow 的音乐和艺术生成项目 Project Magenta。机器之心之前也曾深度介绍过该项目《[深度 | 人工智能改变 MIDI 创作：谷歌 Magenta 项目是如何教神经网络编写音乐的？](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1620)》。Eck 在演讲中谈到了选择 TensorFlow 的原因：可以使用能操作一切（MIDI、音频）的 Python，灵活且高速的图像、音频、视频 I/O、有很好用的 TensorBoard 和非常好的开发者社区。![](https://pic4.zhimg.com/v2-4a8dc392cd2a90f0ab78fb94497e0467_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='930' height='507'></svg>)


最后要提及的是谷歌收购的 DeepMind 团队在去年从 Torch 转向 TensorFlow 之后，也在积极地将其用到各种应用上。在大会的一场演讲中，来自 DeepMind 应用团队的 Daniel Visentin 就提到了 DeepMind 将它们的人工智能技术应用到谷歌的数据中心上，从而寻找帮助谷歌降低能源费用的方法。而这种方法的开发就得益于围绕 TensorFlow 开发的一些更高水平的库。

![](https://pic3.zhimg.com/v2-7bf0d18c1536b604ecad5d0dff26ca12_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1886' height='1056'></svg>)
**移动端与嵌入式 TensorFlow**


服务器端的大规模机器学习应用，Google 当仁不让（当然，Facebook、Twitter、Linkedin、Netflix、Amazon 等也有自己的看家本领）。但是，移动计算市场对机器学习的需求极其强劲，谷歌自然不会放弃这块巨大的蛋糕。


 Pete Warden 带来了主题为「移动端与嵌入式 TensorFlow」的演讲。首先，对 TensorFlow 生态系统做了基本介绍，接着就移动端实现 TensorFlow 以及一些问题解决（当然，也是 TensorFlow 的优点）做了简单讲解。


目前，TensorFlow 支持的平台包括安卓、iOS 以及树莓派。

![](https://pic1.zhimg.com/v2-04898f6451709c79e0b4ea3af3469288_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1162' height='631'></svg>)
TensorFlow 还与许多芯片制造商，比如 英特尔、ARM 、Movidius 等密切合作，确保 TensorFlow 在一大堆不同硬件上运行更快更流畅。

![](https://pic1.zhimg.com/v2-74530963942daaac72759d72317ba290_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1198' height='677'></svg>)
简单介绍 TensorFlow 的生态环境后，Pete Warden 利用介绍了安卓系统、iOS 以及树莓派的 TensorFlow 实现，还给出了应用实例。![](https://pic2.zhimg.com/v2-e3c72b98cb0ef7053fb3bb934d05b8b1_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1159' height='639'></svg>)


*安卓应用程序用的是 Java，怎么办？答案在上面。*


在 TF 实现中，通常会遇到一些问题，比如

![](https://pic1.zhimg.com/v2-7dc5f349fd1fe162f661d26c7eceace0_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='883' height='506'></svg>)
为此，你可能需要知道 TensorFlow 的打造原理，比如，TensorFlow 的组件巨多，根本不存在一个把这些内容都列出来的单一文件。这时，你需尝试有效操作办法： 

![](https://pic2.zhimg.com/v2-4e0178c84eb576d29afea6bbbeaab211_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1089' height='585'></svg>)
接下来，管理模型大小和速度问题，TensorFlow 有不少办法压缩模型大小。其中，最关键的步骤就是量子化权重。

![](https://pic3.zhimg.com/v2-0a18c660c853956e9fdeca99e9a68162_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1642' height='840'></svg>)
最后介绍了管理二进制文件大小。

![](https://pic1.zhimg.com/v2-0e703337758bcbf74ebbfa3942c64550_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1643' height='796'></svg>)
**TensorFlow 资源汇集**


Ashish Agarwal 在本次开发者大会上介绍了机器学习工具包，他谈到 TensorFlow 虽然是一个非常强大的框架，然而也一直以来都缺乏可以即时使用的解决方案。常用的机器学习工具包括：

![](https://pic1.zhimg.com/v2-8f0f20b1b4196af7baad963df0ce13e0_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='586' height='335'></svg>)

Agarwal  介绍了了一个旨在解决这一问题的算法工具包，并表示这个工具包是 TensorFlow 中的高性能、分布式、可扩展的机器学习算法实现，可以直接拿来使用，比如下面这个联合实现 k-均值和 DNN 的案例：![](https://pic4.zhimg.com/v2-6a9a04016669de5a990860fc7e77e78b_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='879' height='504'></svg>)


最后是我们有关 TensorFlow 的报道：


**基本概述和新闻**


- 
[TensorFlow开源一周年：这可能是一份最完整的盘点](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1794)

- 
[TensorFlow：最棒的深度学习加速器](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/209)

- 
[终于来了，TensorFlow 新增官方 Windows 支持](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1912)

- 
[谷歌开源平台TensorFlow向iOS开放](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1279)

- 
[想揭开深度学习隐藏层的神秘面纱？试试Tensor Flow的神经网络游乐场](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1348)

- 
[TensorFlow版本号升至1.0，正式版即将到来](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2126)

- 
[谷歌召开首届TensorFlow开发者大会，正式发布TensorFlow 1.0](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2291)

- 
[TensorFlow 生态系统：与多种开源框架的融合](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1770)

- 
[谷歌TensorFlow的一份全面评估报告：好的坏的及令人讨厌的](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1207)

- 
[对比深度学习十大框架：TensorFlow最流行但并不是最好](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2082)

- 
[从TensorFlow到Theano：横向对比七大深度学习框架](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2303)



**前沿研究**


- 
[Google Brain论文：TensorFlow，一个大规模机器学习系统](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1242)

- 
[DeepMind最新生成模型WaveNet，将机器合成语音水平与人类差距缩小50%（附论文）](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1509)

- 
[谷歌翻译整合神经网络：机器翻译实现颠覆性突破（附论文）](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1568)

- 
[谷歌增强型风格迁移新算法：实现基于单个网络的多种风格实时迁移（附论文）](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1719)



**应用实现与开源**


- 
[Show and Tell：谷歌在 TensorFlow 上开源图像描述系统](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1555)

- 
[谷歌开源新的 TensorFlow 代码，如何进行文本自动摘要](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1449)

- 
[谷歌开放 TF-Slim：在 TensorFlow 中定义复杂模型的高层库](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1474)

- 
[谷歌开放Inception-ResNet-v2：一种新的图像分类卷积神经网络模型](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1481)

- 
[谷歌开源最精确自然语言解析器SyntaxNet的深度解读：一次关键进步以及一个重要工具](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1206)

- 
[雅虎BigML团队开源大数据分布式深度学习框架TensorFlowOnSpark](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2279)

- 
[谷歌发布深度学习库TensorFlow Fold，支持动态计算图](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2253)

- 
[Wasserstein GAN 的 TensorFlow 实现](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2278)

- 
[TensorFlow 生态系统：与多种开源框架的融合](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1770)

- 
[DeepMind语音生成模型WaveNet的TensorFlow实现](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1614)

- 
[OpenAI 的 PixelCNN++实现：基于 Python3 和 TensorFlow](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2018)

- 
[DeepMind提出的可微神经计算机架构的TensorFlow实现](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1809)



**教程**

- 
[谷歌官方指南：如何通过玩TensorFlow Playground来理解神经网络](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1348)

- 
[没有博士学位，照样玩转TensorFlow深度学习](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2196)

- 
[如何在 TensorFlow 中用深度学习修复图像？（附论文）](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1417)

- 
[资源 | 数十种TensorFlow实现案例汇集：代码+笔记](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1648)

- 
[10种深度学习算法的TensorFlow实现](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1617)

- 
[机器学习敲门砖：任何人都能看懂的TensorFlow介绍](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/1440)


**机器之心原创**


