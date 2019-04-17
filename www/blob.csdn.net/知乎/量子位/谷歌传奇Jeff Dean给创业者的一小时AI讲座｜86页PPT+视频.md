# 谷歌传奇Jeff Dean给创业者的一小时AI讲座｜86页PPT+视频 - 知乎
# 



> 李林 若朴 编译整理
量子位 出品 | 公众号 QbitAI

少年，你知道Jeff Dean么？

传奇一般的Jeff Dean现在领导着Google Brain团队，也是Google研发群组的高级研究员（Senior Fellow）。近日，Jeff Dean为YC AI小组的同学讲了一堂时长近1小时的课程，讲述了目前Google目前在人工智能方面的研究和进展。

这节不容错过的AI课程，现在有两个观看方式：

**第一：视频方式。**

科学前往YouTube([https://youtu.be/HcStlHGpjN8）](https://link.zhihu.com/?target=https%3A//youtu.be/HcStlHGpjN8%25EF%25BC%2589%25EF%25BC%258C%25E6%2588%2596%25E8%2580%2585zai),或者在量子位微信公众号（量子位）上查看我们搬运回来的视频。

**第二：图文方式。**

Jeff Dean这节课配有86页PPT，我们也把全部内容搬运过来，空耳听译了Jeff Dean对每一页的讲述，然后整理配发为每一页的要点。

所以，开始吧~



![](https://pic1.zhimg.com/v2-38cfe4134ea64d82fcd5dddfb5871bd4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我会全面地讲一讲我们正在用深度学习处理哪些任务，正在建立怎样的系统来提升深度学习的速度。

这份PPT是很多人合作的成果，包括我领导的Google Brain团队。
![](https://pic4.zhimg.com/v2-5248cb0d7f4a4cf2492475315b863343_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
Google Brain团队的任务是让机器更智能，进而让人们生活得更好。
![](https://pic2.zhimg.com/v2-2155fa8ac9806b02bc38c43b7dec6c91_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
为了达到这个目标，我们：
- 进行长期的研究；
- 建立并开源TensorFlow等系统；
- 与Google和母公司Alphabet的其他部门合作，让几十亿用户能在Alphabet的产品中用上我们的工作成果；
- 通过实习和Google Brain Residency项目培训新研究者。
![](https://pic4.zhimg.com/v2-0d563c63ac9837e831b0902f4264ee27_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
Google Brain的主要研究领域有以上这些。
![](https://pic3.zhimg.com/v2-0e2a4f4e873d407159fcc11811414882_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我今天主要会讲到红色的这些：
- 一般机器学习算法和技巧；
- 机器学习的计算机系统；
- 自然语言理解；
- 健康；
- 机器人。

也会涉及一些感知领域的研究。
![](https://pic4.zhimg.com/v2-c8d04138df499baa5cfab53933e6bf17_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
年初，我写了一篇博客，总结2016年我们团队都做了哪些工作。

[http://research.googleblog.com/2017/01/the-google-brain-team-looking-back-on.html](https://link.zhihu.com/?target=http%3A//research.googleblog.com/2017/01/the-google-brain-team-looking-back-on.html)

博客中的每一条超链接都指向了这项工作的成果展示或论文。
![](https://pic1.zhimg.com/v2-efc403a92899fc15375eab8d3deb2064_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
你们可能都在做AI相关的创业公司，不过我还是要说，深度学习改变了我们解决很多问题的方式。

在80、90年代，对于很多问题来说，神经网络还不是最好的解决方案，当时训练数据的量、计算力都还不够。所以，人们或者用其他方法，或者用比较浅层的机器学习方法，需要人工进行大量的特征工程。
![](https://pic4.zhimg.com/v2-9a722f2dcf3467a787c8133f84342857_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
但是现在，我们有了更强大的计算力。

90年代，我的本科论文是关于神经网络的并行训练的，神经网络模型非常吸引我，我想，如果我们能通过并行计算获取更多计算资源，比如说用一台64个处理器的超立方体结构机器，结果我发现，60倍的计算力远远不够，我们需要数十万倍的计算力。
![](https://pic4.zhimg.com/v2-07aab7b624105515b0cef86fefd718c7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
到今天，我们已经有这样的计算能力了。结果就是神经网络成了很多问题的最佳解决途径。
![](https://pic3.zhimg.com/v2-5a12e0dec1cd63e0d8f4df4a7b14eae2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
最开始，我们的团队只是想证明大量的计算资源能帮助神经网络解决一些有趣的问题，于是我们做了大规模的无监督学习，那时候甚至还没用上GPU，我们用了16000个CPU核心，做了一些有趣的事情。（量子位注：就是机器从YouTube视频中认出了猫的那一次）

后来，我们把深度学习用到了更多领域，上图左侧是包含我们第一代和第二代（TensorFlow）深度学习框架模型描述文件的程序数量，我们和Google的其他团队合作，把深度学习用到各种产品之中。
![](https://pic1.zhimg.com/v2-8727ddccce94d4407752dda7ed9034f4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们在工作中关注的一个重点是如何缩短实验的周转时间，提高研究效率。

实验周期需要1个月和实验周期只有几分钟，会带给研究者截然不同的体验。所以，能尽快取得实验结果对我们来说非常重要。
![](https://pic4.zhimg.com/v2-e8f30733205a8dedda1316830d9b094f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
为此，我们要建造正确的工具。
![](https://pic4.zhimg.com/v2-8bc36e0d0ea92192a583a37a464f57e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
TensorFlow就是我们为解决深度学习问题而建造的第二代系统，它从一开始就是作为一个开源系统而打造的。

量子位注：我们之前发文介绍过Google是怎样管理TensorFlow这么庞大的开源社区的：（在微信公众号中搜索）TensorFlow技术主管详解：Google是怎样管理开源软件的
![](https://pic4.zhimg.com/v2-98099c68eba6545539dd2a9fa2697b4b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
TensorFlow的目标是，建立一个呈现机器学习创意和系统的通用平台，并让这个平台在研究和生产环境中都成为世界最佳，再通过开源，让它成为所有人的平台，而不只是Google的。
![](https://pic2.zhimg.com/v2-d7779b2836e3a3045cff8e8b093db31d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这是GitHub星标数的趋势图，你可以看到TensorFlow与其他机器学习框架的对比。

我想，TensorFlow在研究中足够灵活，又很容易部署到实际产品上，还能在多种环境中运行，这些特性很有吸引力。
![](https://pic4.zhimg.com/v2-72b906fa1d3b1ddf952ed93c3649de83_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们经常会做一些基准测试，这些测试显示TensorFlow的可扩展性还是不错的。

其实我们很重视速度。

在TensorFlow刚发布的时候，我们推出了很多教程，但是当时我们犯了一个错误，我们发布的教程代码追求清晰、解释详细，但忽略了性能，于是很多人模仿那些代码来建立高效的TensorFlow模型。后来我们逐渐更新了那些代码，对代码的清晰程度和性能同样重视。
![](https://pic1.zhimg.com/v2-b3bd1e1928b1e247470b6a54582b44b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
在很多任务上，TensorFlow支持多达64个GPU，还能随着GPU的增加保持性能的线性提升。

如果你听说TensorFlow很慢，不要信
![](https://pic4.zhimg.com/v2-844bb11e19a6c2e034be915629cff957_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们还支持多种平台，包括iOS、Android、CPU、GPU、我们自己的TPU，还有Raspberry Pi等等。
![](https://pic4.zhimg.com/v2-bc58face1e9e72a444399c32872b5bc3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们还支持各种变成语言，目前支持得最好的是Python，C++做得也不错。
![](https://pic3.zhimg.com/v2-11d91f9746c22d41c024bf3437d26f8e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
除此之外，我们还有个不错的用户基础。
![](https://pic3.zhimg.com/v2-8bee7b3984f1ef1a5119bf6e69eff172_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
在GitHub上最受欢迎的程序里，我们排第6。前5名要么是JS，要么是LISP编程书。
![](https://pic2.zhimg.com/v2-814cb537b16f18eb07c19262b35ed79d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这是世界上为TensorFlow标星的人口分布，全世界几乎都在用机器学习。
![](https://pic2.zhimg.com/v2-d00d6ec8eab899a2d9ad53b325092831_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
TensorFlow在全世界有近千人的外部贡献者，为它增加功能、修复bug、改进系统。

各种机器学习课程中也在越来越多地使用TensorFlow。
![](https://pic4.zhimg.com/v2-cc0215fabebeb2908f2389263bb74d03_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
接下来我要谈一谈深度学习在Google具体产品中的应用。Google Photos是个很好的例子，计算机视觉让机器能知道人们的照片中有什么，照片中发生了什么事情。
![](https://pic3.zhimg.com/v2-fe37d2d043653c145e4858a0a7498316_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
作为人工智能领域的创业者，你们需要关注各种机器学习成果，很多时候，你可以使用其中的方法，把它用到另一个数据集上，就可能有意想不到的发现。

给定一张图片，让机器预测其中的关键像素，是一个常用的模型，有很多种实现方式。

不过，我们的做法是用一个能实现这种功能的模型，迁移到各种其他任务上。
![](https://pic3.zhimg.com/v2-bd0b6f5d7bf7ab783219e24441a8065e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
计算机视觉公司Clarifai的创始人Matt Zeiler是我之前的暑期实习生，我们和街景团队合作，识别街景照片中的文字。
![](https://pic2.zhimg.com/v2-8794f486949f183043026712981a05dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
训练数据就是人类手工圈出了文字区域的照片，任务是让机器预测普通街景照片中哪里有文字，选定区域之后，就可以通过OCR来识别这些文字了。
![](https://pic1.zhimg.com/v2-dc268b5efbaa81bf51f8db143ad72344_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
后来地图团队决定开发另一个程序，来识别你家房顶适不适合采集太阳能，装上太阳能板可以生产多少电。

这个程序的第一件事是在图中找出房顶，这和找到文字区域的模型不完全一样，但其实只是换了一种训练数据。
![](https://pic3.zhimg.com/v2-de64c4707480f325123fdfaea3ee9ea6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们还把这个模型用到了医疗领域。
![](https://pic2.zhimg.com/v2-877b614d94ec478f7d0c07b76082d6d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
还是一样的基础模型，我们用它来解决医学图像问题。

具体来说，是用在眼科检查上。我们让算法从上图这样的视网膜图片上，分辨出是否有糖尿病视网膜病变。

在这个问题上，我们需要识别图上可能是病变的区域，然后再进行图像分类，确定这个眼部照片是否真的有问题。
![](https://pic3.zhimg.com/v2-6f929e86fb109fc140177659405753aa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们的研究收集了15万张视网膜照片，每张照片找7名眼科专家来标注，然后训练我们的算法。
![](https://pic4.zhimg.com/v2-78ff44c655b4344244064c879af303bf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
最后，我们获得了一个识别糖尿病视网膜病变的算法，和8名协会认证的眼科专家相比，算法的准确率和他们的中位数持平，有时还要高出一点。

相关论文：

[http://jamanetwork.com/journals/jama/fullarticle/2588763](https://link.zhihu.com/?target=http%3A//jamanetwork.com/journals/jama/fullarticle/2588763)
![](https://pic3.zhimg.com/v2-7865dd359d3a6e7a753771872cfb80ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
另外，我们可以看到，深度学习在机器人领域也非常用用。

我们在实体机器人和模拟环境中都做了很多实验，还试着让机器人通过模仿人类的动作来学习。
![](https://pic2.zhimg.com/v2-abe15e66bb9f40c9ab099262744feb89_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们建了个“机械臂农场”，摆了很多机器人，让它们学习抓取物体。

我们每天还会把传感器获取的数据收集起来，用它们训练一个模型来优化机器人的抓取技能，这些机器人每天都会提高。

我们公开了这个数据集，其中包含80万次抓取尝试。

项目主页：

[https://sites.google.com/site/brainrobotdata/home](https://link.zhihu.com/?target=https%3A//sites.google.com/site/brainrobotdata/home)

论文：

[https://arxiv.org/abs/1603.02199](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1603.02199)

数据集：

[https://sites.google.com/site/brainrobotdata/home/grasping-dataset](https://link.zhihu.com/?target=https%3A//sites.google.com/site/brainrobotdata/home/grasping-dataset)
![](https://pic3.zhimg.com/v2-46abc8be4824ec95f37bcdae51c4e8a6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这张图是我在做动作，试着让机器人模仿。
![](https://pic1.zhimg.com/v2-017ec02e2da69c9f2125ddd15d8f9d3c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们首先让模拟器里的机器人学习人类动作，然后再迁移到实体机器人。效果还不错。
![](https://pic4.zhimg.com/v2-edaf8fa22fe73acb286f56acd6af216f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
还有一个激动人心的领域，是深度学习在科学研究中的应用。

科学研究通常都会用到高性能计算机上运行的模拟器，它们会产生大量数据，但问题是，模拟器的运行消耗大量资源，价格昂贵。
![](https://pic2.zhimg.com/v2-55d74bfcad018e238b303d471a124251_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
举个深度学习应用在这里例子，我们可以用模拟器中的数据作为训练数据，运行一小时模拟器，然后用获取的数据来训练一个神经网络，让它来预测分子的性质。

当然，模拟器给出的结果更精确，但神经网络给出结果的速度要快上几十万倍。

相关研究：

[https://research.googleblog.com/2017/04/predicting-properties-of-molecules-with.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2017/04/predicting-properties-of-molecules-with.html)

[https://arxiv.org/abs/1702.05532](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1702.05532)

[https://arxiv.org/abs/1704.01212](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1704.01212)
![](https://pic1.zhimg.com/v2-a78eedb10fedc5eb5d6ce805c08f6d84_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
图像处理技术也能用在细胞研究上。怎么用呢？
![](https://pic3.zhimg.com/v2-20e6bf98f20306e0d7685d6fc229fa36_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
Google有人做了一个模型，尝试通过一张照片来预测景深。

这是一个pixel to pixel的学习问题，你可以想象，还有很多问题都是类似的。
![](https://pic3.zhimg.com/v2-125f8611d426813ae6bd0873bc5e804e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
在拍照中，景深的预测有很多用途，比如说预测一张人像照片的景深。
![](https://pic4.zhimg.com/v2-1927609ee114743ee14b8153c1b7098f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
然后就可以做出一些神奇的效果，比如说保留人像的颜色，把背景变成黑白的，或者让背景变得模糊、给背景加上特定的风格等等。
![](https://pic3.zhimg.com/v2-d467b19b4e55644aac171eb132f73a1a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这种模型还有更神奇的用途：

用显微镜原始图像做为输入，可以输出一张染色后的显微镜图像。
![](https://pic4.zhimg.com/v2-8f1489457fc0980b8372f27011ce836b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这有助于帮人们分清细胞核和它外层的物质。

给细胞染色的时候，通常会伤害细胞的活性，所以这种虚拟染色的功能非常有用，能让你在染色后继续追踪细胞的后续活动。
![](https://pic3.zhimg.com/v2-b17a60265eefce40dc2cd96a8d8ebcde_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这种技术也能用来观察神经元的活动。
![](https://pic2.zhimg.com/v2-05237d95c5119e2ba76c64fefa8eefad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们在语言理解领域也做了不少工作。
![](https://pic3.zhimg.com/v2-e4128957b75ab6b0c5231b1f1e7535b2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
最开始，我们的团队提出了seq2seq模型。给神经网络输入一个序列，它会输出一个预测序列。

seq2seq能用来处理很多问题。
![](https://pic4.zhimg.com/v2-0eb608c2fe88c49b2dde18cb5cab0b43_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
机器学习就是其中之一。

比如说你有一组成对的英语-法语句子，就可以用seq2seq模型，以法语为输入序列，英语为输出序列，将它们对应起来，训练一个RNN。
![](https://pic3.zhimg.com/v2-ae1cada485b1829a8d74a1e5b0e8f16a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
然后你会通过beam search来找到最合适的输出序列。
![](https://pic2.zhimg.com/v2-dd8677b9948d0ed7fd63aab312fbf0a1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
Gmail中的Smart Reply自动回复功能也是seq2seq的应用之一。

举个例子，这是我发给同事们的一封邮件，问大家要不要参加感恩节聚会。这种邮件只需要简短的回复，有了Smart Reply用户只需要选择“是”或者“否”。
![](https://pic3.zhimg.com/v2-ed7231eb7dffe74aec66b15e54485506_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们的seq2seq模型会在用户选了是/否之后，生成一个简短的回复。
![](https://pic2.zhimg.com/v2-14832515ea70e3b4588db92eb993fbf9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这个功能，是2009年Google开的一个愚人节玩笑：“哈哈哈哈我们要帮你自动回复邮件了！”

2015年11月5日，真正的产品发布了；到2016年2月1日，Inbox手机应用中由超过10%邮件是自动回复的。
![](https://pic4.zhimg.com/v2-bfaf4ada2bb8261f3f28e9020a8338df_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
实际上，seq2seq最具潜力的应用领域还是翻译。

我们把seq2seq用在了在线的谷歌翻译产品中。
![](https://pic3.zhimg.com/v2-ce29c8dd5283730d17a519732ccce08a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
在这篇有很多作者的论文Google’s Neural Machine Translation System: Bridging the Gap between Human and Machine Translation中，我们介绍了机器翻译背后的技术详情。

论文地址：

[https://arxiv.org/abs/1609.08144](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1609.08144)
![](https://pic4.zhimg.com/v2-a8a6cb95f8e829bd523f06901d934af3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这是我们模型的结构，它包含很深的LSTM stack，每一层运行在不同的GPU上；还有一个注意力模块，追踪所有的状态，决定在生成输出序列的各部分时，分别该注意输入数据中的哪一部分。
![](https://pic2.zhimg.com/v2-1d2f82134d8e0895b7320893b2511411_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
每一份这个模型都运行在一台GPU机器上。我们把这个模型复制了很多份，在训练数据上进行平行训练，然后共享学到的参数。
![](https://pic3.zhimg.com/v2-e0ab50f0ec4eaa35fdd9fd13b00b4dba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
上图显示了人类和Google的两种机器翻译模型的翻译质量。蓝色表示我们原来用的基于短语的机器翻译模型，绿色的是基于seq2seq的神经网络机器翻译模型，黄色的表示双语人类（非专业翻译）。

基于神经网络的模型远远超过了基于短语的统计模型，越来越接近人类的水平。
![](https://pic3.zhimg.com/v2-de633fef51b8fc41dde9961b9a5e90fe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们的日语-英语神经网络机翻上线后，很多日本人都发现，翻译质量好了很多，终于真的能用了。

这是一位日本教授把海明威《乞力马扎罗的雪》第一段从英语翻译成日语、再从日语翻译回来的结果。上边是基于短语的统计模型的结果，下边是神经网络模型的结果。
![](https://pic4.zhimg.com/v2-4603e06eef23cd04a88d74efb06f2e7f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们正在进行大量研究的另一个领域，是如何自动化解决机器学习的问题，我们称之为“learning to learn”。
![](https://pic2.zhimg.com/v2-a0f7600313522fed4526eb064a563021_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
目前解决机器学习问题的主要方式，是机器学习专家+数据+计算力。然后用不同的学习率尝试某个模型，希望通过迁移学习的方式从某个数据集得到解决方案。

我们研究的解决方案，是数据+100倍的计算力，这中间不需要人类机器学习专家。如果能做到这一点，世界上会有更多的组织和数据能受益于机器学习。
![](https://pic4.zhimg.com/v2-7abe4690174272e0516e636a8aedd1e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们在这个领域做了很多研究，今天主要讨论其中的两个。一是自动设计神经网络架构的方法，二是自动学习如何优化。
![](https://pic4.zhimg.com/v2-792eca63c9519ed652fa28700af098eb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
架构搜索。这个想法，是想用一个模型生成另一个模型。目前的机器学习中，设计神经网络的架构仍然需要大量的专业知识并且耗费大量时间。在这个研究中，我们生成10个模型架构，分别训练数小时，然后使用生成模型的损失，作为强化学习的信号。

这一基于梯度的方法，可以找到最优的架构，不过目前只适用于小问题。
![](https://pic2.zhimg.com/v2-c0616e0b5a478aa60dc4c58684020761_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这是一个例子。这个模型看起来就不像人类设计的东西，这些连线有点疯狂。这是CIFAR-10数据集上的一个应用，架构搜索的方法已经非常接近目前最先进的技术，这中间并没有人对底层架构进行过干预。
![](https://pic1.zhimg.com/v2-a5b49040ddc16d97bf395255063e5b04_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们还在Penn Treebank数据集上，尝试了一个语言建模任务。我们只是给了架构搜索一个LSTM cell，然后让它寻找处理顺序数据的方法。在这个案例中，架构搜索的方法比此前最先进的方法表现更好。
![](https://pic4.zhimg.com/v2-59d23463158ec8065f6f2f7734622f23_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
常见的优化方法都是类似的。经常使用的方法包括SGD（随机梯度下降）、Momentum、ADAM、RMSProp等。在这一页可以看到，自动化的方法提出了15种完全不同的表达式，而且表现基本都优于人类设计的优化方法。
![](https://pic4.zhimg.com/v2-dbda1e9497ff6fae699c386f545b41cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们把其中最好的一个表达式迁移应用在一个不同的问题上，结果发现这个自动设计的优化器，表现比现有最好的ADAM更出色。

很多机器学习专家想要解决一个问题，需要运行非常多的试验，有可能一个周末就需要试验12000次。人类专家不能操作太多的试验，但是机器可以。当然目前也有很多时候，结果并不尽如人意。
![](https://pic2.zhimg.com/v2-606497b4bca26811ac3066b24805d125_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
另一个有趣的事情是，我们之所以能解决很多问题，是因为我们有了非常多的数据，而且能够在不同的问题上应用大规模计算能力。

深度学习改变了我们设计计算机的方式。
![](https://pic3.zhimg.com/v2-b5b2770ce1e27f0f7a21496a9b108e4e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
深度学习有两个非常好的特性。一个是完美的宽容了非常低的计算精度，另一个是能够处理多种不同的运算，所以我们有机会建造一个定制化的机器学习硬件系统，能够以极大降的低精度运算线性代数。
![](https://pic4.zhimg.com/v2-9ca34d7271ba0e639ab08dcb616e534b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这是我们一直在搞的定制化机器学习加速器（TPU），目标是加速推理以及训练，这个硬件的第一代版本已经在我们的数据中心部署了两年半，在五月的Google I/O大会上，我们展示了第二代TPU。

重要的不是设计一个芯片，而是整个系统。
![](https://pic1.zhimg.com/v2-360cd36921ca7b2bd921748e1483829c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
单个芯片不可能提供足够大的计算量，因此我们把TPU芯片连接在一起。我们称其为：TPU Pod。连接在一起的TPU能够提供更大的计算力。
![](https://pic3.zhimg.com/v2-8af2358d62b0013c12a02c308f1e98f6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
通常给一台超级计算机编程有点烦人。所以我们决定使用TensorFlow，基本上可以实现用一个新的接口表达一个模型，而且我们在TensorFlow 1.2中添加了估计量，相同的程序在CPU、GPU、TPU上只需要微调就能运行。

这些都会引用在TPU云服务上，届时运行TensorFlow应该超快。
![](https://pic3.zhimg.com/v2-0a032c23379dab0d64c868a608e091d2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
我们还把上千台这样的设备，免费提供给全球的研发人员使用，只要你致力于有趣的研究，需要更多的计算力，而且承诺会公开研究成果。我们想要构建一个科学和机器学习研究的生态系统，我们也希望得到使用TPU的反馈。
![](https://pic4.zhimg.com/v2-4e8cf3bb3af62f58bf13f0f423f24d0f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这里，实力广告一波Google Cloud。
![](https://pic3.zhimg.com/v2-a6457d1367efa702ec487238c57e312a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
最后讲一下，我们也在尝试用机器学习的方式得到更高性能的机器学习模型。
![](https://pic2.zhimg.com/v2-52366f2f82d8f35d8cd5a8082b4e3099_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
使用强化学习来配置设备。在这个案例中，我们使用强化学习的方式，使用序列到序列模型来预测TensorFlow数据流图的子集应该运行在哪些设备上。例如，如何分配两个GPU的计算力，让模型能够尽快的完成运算。

应用这个方法，我们在神经机器翻译模型、Inception-V3模型上，都获得了近20%的速度提升。此前，计算资源的分配智能通过人类专家手工摸索设置。
![](https://pic4.zhimg.com/v2-07aab7b624105515b0cef86fefd718c7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)


![](https://pic3.zhimg.com/v2-b6f5e99af5962473201ea396c685e042_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
所以，我们认为更多的计算力带来更多的机会和收益。
![](https://pic2.zhimg.com/v2-202dc11db40b7f11eb382eb2bcdd9f21_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这是一些未来会实现的需求。例如通过图像来发现糖尿病引发的视网膜病变；用西班牙语描述一段视频；让机器人从厨房沏一杯茶来；帮我找到与机器人强化学习有关的文档，并用德语总结出来。
![](https://pic4.zhimg.com/v2-4b2ce9a8af425681304c5061ebe60b27_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
结论。你们肯定已经知道深度神经网络正在带来巨大的变革，大家应该更关注这件事。
![](https://pic2.zhimg.com/v2-76b0c2ed3f8dc807770b4ec9573e2af5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='1688'></svg>)
这是我们正在研究的一些领域，访问[http://g.co/brain](https://link.zhihu.com/?target=http%3A//g.co/brain)可以查看更多信息。你们应该可以加入我们的团队，不过鉴于你们已经开始创业了~（呵呵哒）

## PPT下载

想要下载Jeff Dean此次讲座的86页PPT全文，请在量子位微信公众号（QbitAI）对话界面，回复：“**jeff**”四个字母。

—— **完** ——

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

**量子位** QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态


