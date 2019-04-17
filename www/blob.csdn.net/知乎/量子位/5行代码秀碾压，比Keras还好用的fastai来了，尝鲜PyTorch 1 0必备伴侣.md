# 5行代码秀碾压，比Keras还好用的fastai来了，尝鲜PyTorch 1.0必备伴侣 - 知乎
# 



> 休假栗 问耕 假装发自 凹非寺
量子位 出品 | 公众号 QbitAI

PyTorch 1.0来了~

在今天的F8（Facebook开发者大会）上，深度学习框架PyTorch 1.0 rc1版如期发布。然而在海外的论坛上，另一个开源库的关注度不遑多让。

它就是**fastai 1.0**。

简单来说，fastai只要**一个API**，就包含了所有常见的深度学习应用。堪称实用版的“要你命3000”。而且许多性能指标，已经超越了Keras。

比如，**5行**代码完成Keras用**31行**才能解决的事情。同时，误差表现也更好。难怪在Hacker News上，fastai的关注度比PyTorch 1.0还高……

而且fastai基于PyTorch 1.0框架，也被Facebook官方重点宣传。这两个全新的框架同时下载配合使用，疗效更佳。

这个悠长假期，你不试试么？

## **fastai 1.0**

fastai的出品方是fast.ai，机构规模不大，但因为提供大量免费机器学习课程，而名声在外。还有，fast.ai也是美国军方的合作伙伴，研究如何加速人工智能技术的发展。

## **只要一个API**

据创始人Jeremy Howard介绍，fastai只要**一个API**，就包含了几乎所有常见的深度学习应用。

用从前的库，都需要给不同的应用调取相应的API，fastai是**第一个**用单个API就能支持各类应用的库。

这样一来，用户就不需要记得五花八门的API调用方法了。用fastai搭建过**视觉模型**之后，可以用同一个的API再去搭建**自然语言处理**模型了。

在1.0发布之前，fastai最初的版本在9月上线，一波早期用户已经先行探索过这个年幼的库，并有多位大佬表示好用
![](https://pic1.zhimg.com/v2-80fb0c17b2f5e6a7f2dc7f5eb844d14c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='913' height='469'></svg>)
这是Kaggle资深玩家Miguel Pérez Michaus，用fastai做的**风格迁移**成果。

作为一个“**没有正式计算机背景**”的程序猿，他毫不吝惜自己对fastai**灵活性**的赞美：连我都能用。
![](https://pic4.zhimg.com/v2-e6e94908bfabb9829a4069b5a2c038e3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1074' height='637'></svg>)
**GitHub**的机器学习科学家Hamel Husain也用这个库，做了一个GitHub语义代码的搜索工具，还是半路从TensorFlow Hub换成fastai的。现在，搜索工具已经上线了：

[https://experiments.github.com/semantic-code-search](https://link.zhihu.com/?target=https%3A//experiments.github.com/semantic-code-search)

还有，**OpenAI**小姐姐Christine Payne，最近做的一个项目叫Clara，是用神经网络生成音乐的AI。小姐姐说，如果没有fastai的话，**速度**就得不到保障了。

听一下Clara的作品，很美。

更多音乐：

[http://christinemcleavey.com/clara-a-neural-net-music-generator/](https://link.zhihu.com/?target=http%3A//christinemcleavey.com/clara-a-neural-net-music-generator/)

**5行代码就够了**

Howard说，团队做过的研究，都嵌在这个框架里了。所以，大家可以充分享受这些成果。

他用**猫狗识别**的来举栗，那是迁移学习的经典案例，也是fast.ai的课程里最受欢迎的章节之一。

在新的库诞生之前，这样的问题大多是在TensorFlow上面用Keras解决的。如今，Keras依然是fast.ai团队仰慕的对象。

所以，团队也想在猫狗大战里感受一下，自家的框架和Keras对比起来，竞争力究竟如何。
![](https://pic2.zhimg.com/v2-004ae0bbe486ae62c4fb5c438200d0a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1126' height='238'></svg>)
这5行代码，就是在fastai框架里做ResNet50的**two-stage微调**，需要的全部操作了。

而同样的任务，Keras要用**31行**才能完成。
![](https://pic4.zhimg.com/v2-64e688f57fd99bd6dd431d105c05ec73_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1002' height='720'></svg>)
除了代码行数，fastai在两个阶段的**误差也比Keras更小**。

虽然成绩表十分骄人，Howard依然三番五次地表示，Keras始终是大神，整个团队都是Keras的粉丝。

## **先去用用看？**

现在，fastai框架已经在GitHub上线了。下载请至：

[https://github.com/fastai/fastai](https://link.zhihu.com/?target=https%3A//github.com/fastai/fastai)

也可以在**谷歌云**平台上用啊。先打开这个页面：

[https://cloud.google.com/deep-learning-vm/docs/images](https://link.zhihu.com/?target=https%3A//cloud.google.com/deep-learning-vm/docs/images)

然后，给你的示例安装配置，再把fastai部署在PyTorch 1.0上。有了。

如果要在GPU驱动的Jupyter Notebook上运行fastai，也没问题：

[https://salamander.ai/](https://link.zhihu.com/?target=https%3A//salamander.ai/)

除此之外，还有一个好消息。不久就会支持AWS了，可以期待一下。
![](https://pic3.zhimg.com/v2-176a1df73775db419d45277c8cd32efa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='567'></svg>)
## **PyTorch 1.0**

接下来，说说也是最新发布的PyTorch 1.0。

去年1月，PyTorch首次发布，目前下载已经超过100万次。今年五月，Facebook副总裁贾扬清和CTO Mike Schroepfer透露，PyTorch 1.0即将推出。当时，Caffe2代码还并入了PyTorch。

**PyTorch 1.0 rc1版传送门：**

[https://github.com/pytorch/pytorch/releases](https://link.zhihu.com/?target=https%3A//github.com/pytorch/pytorch/releases)

Facebook在官方博客中，还对PyTorch 1.0 rc1版进行了详细的介绍。

新框架有了一个全新的混合模式前端，支持tracing和scripting模型，从eager模式进入graph模式，从而弥合研发与生产部署之间的差距；一个经过改进的torch.distributed库，能让Python和C++环境中的训练更快。

以及这个版本正式融合了PyTorch框架的灵活性与Caffe2的生产力。贾扬清此前给的总结就是：Caffe2 + PyTorch = PyTorch 1.0。

## **三大新功能**

**torch.jit**

JIT是一组编译工具，用于弥合PyTorch研究和生产之前的差距。其中包含一种名为Torch Script的语言（仍然使用Python编写），还有两种方法可以使现有的代码与JIT兼容。

Torch Script代码可以积极优化，并且在序列化之后可以在新的C++ API中使用，这就完全不依赖Python了。

**c10d库**

torch.distributed软件包以及torch.nn.parallel.DistributedDataParallel模块，由新的c10d库提供支持。主要亮点包括：
- c10d是性能驱动的，并且对所有后端完全异步操作
- 分布式数据并行性能显著改进
- 在Gloomy后端增加send和recv支持

**C++前端API**

这个前端API被明确标记为还不稳定（Unstable）。官方预计将在未来一两个月稳定下来，同时不排除发生其中某些部分发生重大变化。

这个C++前端是一个纯C++接口后端是PyTorch，与此前的Python前后端体系结构一样。这个API只在实现高性能、低延迟和裸机C++应用程序的研究。

以下是两种语言前端的简单比较。
![](https://pic3.zhimg.com/v2-4fe702e55d9528bde2a970bdf1c00d76_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='974' height='500'></svg>)


![](https://pic1.zhimg.com/v2-a65e107f0f70127954922bbf1495e6f4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='954' height='496'></svg>)
## **其他**

**生态系统**

Google云、亚马逊AWS、微软Azure Machine Learning等国外的流行云服务，现在与PyTorch更加兼容。

Arm、Nvidia、高通、英特尔、IBM等厂商的，也增加了对PyTorch 1.0的支持。确保PyTorch可以在数据中心、边缘硬件等不同领域得到应用。

根据官方消息，目前Google和Facebook正在合作，为PyTorch用户构建TPU。Google云的Deep Learning VM服务，也将提供PyTorch 1.0映像。

**教育培训**

Facebook透露正与Udacity合作，为开发者提供免费的深度学习入门课程，这些课程都基于PyTorch。此外，Facebook还将赞助300名开发者，完成Udacity的中级课程，这些课程也已经改为基于PyTorch 1.0。

当然，fast.ai的[新课程](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247504925%26idx%3D4%26sn%3Da147a115e3dab4a85cfbaf6db365acd3%26scene%3D21%23wechat_redirect)和深度学习库，也是PyTorch在教育领域的合作成果。




