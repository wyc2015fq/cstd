# 666元！英伟达史上最便宜AI硬件发布：可运行所有AI模型，算力472 GFLOPS，功耗5瓦 - 知乎
# 



> 李根 乾明 郭一璞 发自 圣何塞 
量子位 报道 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-aed47d973a87dac075df2740d3de3bcd_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
中国古语有言：否极泰来。

AI芯片巨头英伟达的2018，再糟糕不过，所以2019年GTC大会，也比以往更受关注。

而且颇有意思的是，教主黄仁勋把这次的主题演讲地点选在了圣何塞州立大学礼堂。
![](https://pic4.zhimg.com/v2-7b59825ebc94d240767d06aa4c881f7b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='737'></svg>)
这所公立大学在硅谷小有名气，且在计算机领域，最有名的校友莫过于原Intel董事长高登·摩尔——“摩尔定律”就是以他之名命名。

而老黄就是那个全世界最热衷宣扬“摩尔定律已经失效”的人。

但这一次，老黄一句“摩尔定律”都没提，默默扔出99美元AI开发套件Jetson Nano。
![](https://pic4.zhimg.com/v2-dfb8aff91d4f709a3dbf8830c90c6907_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
具体情况怎么样，我们完整回顾下今年GTC发布情况。

## **CUDA-X：50倍提速**

首先，直接看核心重头戏：CUDA-X。

按照英伟达的说法，这是全球唯一的一个，端到端的数据科学加速库。帮助从事数据分析、机器学习、深度学习的数据科学家们，更快的完成工作。

CUDA-X由十几个专用加速库组成。整个系统中，包含几个部分：用cuDF加速数据分析，用cuDNN深度学习原语，用cuML进行机器学习算法，和DALI等数据处理。
![](https://pic3.zhimg.com/v2-b54540c9122c77db2225c34aa6f696ae_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
简单来说，所有英伟达的库，都被整合成一个库：CUDA-X。

包括TensorFlow、PyTorch和MXNet在内的所有深度学习训练框架，随着CUDA-X的发布也会自动为英伟达Tensor Core GPU进行优化。

英伟达表示，CUDA-X解锁了Tensor Core GPU的灵活性，能够将机器学习和数据科学工作负载加速多达**50倍**。CUDA-X可以加速典型AI工作流程的每一步，无论是用深度学习训练语音和图像识别系统，还是数据分析评估抵押贷款组合的风险。

而且据说，“只需要点击几下”。
![](https://pic1.zhimg.com/v2-8e3091e27e394c6a0bafe3a61ddc480c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
微软的Azure Machine Learning（AML）是第一个集成RAPIDS的云服务，RAPIDS是CUDA-X的关键组件。通过访问RAPIDS开源库，数据科学家可以让AML上的英伟达GPU以“前所未有”的速度进行预测分析。

英伟达官方披露的数据称，在AML上使用RAPIDS，可以帮助企业把训练AI模型所需的时间，减少多达**20倍**，训练时间可以从数天减少到数小时，或者从数小时减少到几分钟。

此外，CUDA-X还会部署在几大主流的云平台，包括亚马逊AWS、谷歌云等。目前PayPal、沃尔玛等公司已经在使用相关服务。

值得一提的是，在介绍CUDA-X的时候，老黄还皮了一下。他说：每次演讲，我都想让你们记住一个关键词，今天的关键词是：

**PRADA。**

还让现场跟着一起念。看来接下来要发布的东西，可能还挺贵。

PRADA其实是四个单词的强行缩写，它也带代表对CUDA-X介绍的几个方面：PRogrammable(可编程)、Acceleration(加速)、Domains(域)、Architecture(架构)。
![](https://pic4.zhimg.com/v2-8d1d8bfdbbafa72ac054efaa033972e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='607'></svg>)
## **史上最便宜AI硬件**

在老黄两个多小时讲完后，最让网友们念念不忘的，是一款出场非常晚、时间非常短的硬件产品：

**Jetson Nano。**

它是体积小巧、性能并不弱的小电脑，老黄称之为“**全新的机器人计算机**”，共有两个版本。

一是**99美元**（约合人民币666元）的开发者套件，面向个人用户，比如创客、学生、技术爱好者；另一个版本**129美元**，可以部署到生产环境，面向的是想要构建边缘系统的企业。
![](https://pic3.zhimg.com/v2-9b1f250e5cf61c2ac2caaa00631aadd6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
历次发布会都让人深感贫穷的英伟达，终于也价格厚道了。在PRADA的衬托下，更是感动人心。

不少外媒纷纷打call，因为Jetson Nano的价格让业余爱好者、学生之类的大众群体，可以打造自己的自动化设备。

当场，老黄也拿出了一个基于Jetson Nano的无人车Kaya。
![](https://pic1.zhimg.com/v2-673a843f044e470b092fbb80ca8ebadc_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
价格厚道，性能如何呢？Jetson Nano能提供**472 GFLOPS**的计算性能，而耗电量只有**5瓦**。

不久前Google发布的千元TPU开发板，单精度和半精度浮点运算性能分别是32和64 GFLOPs；英特尔神经计算棒的性能是100 GFLOPs。

Jetson Nano搭载了4核的ARM A57 CPU和128核基于英伟达Maxwell架构的GPU，4GB内存，使用MIPI CSI-2 DPHY通道摄像头。

它也支持今天发布的CUDA-X，也就是说有支持很多常见的人工智能框架，装有面向Tegra的Linux操作系统，实现了开箱即用。

英伟达会后还展示了Jetson Nano运行ResNet、Inception、VOLO等各种神经网络模型，在目标检测、姿势估计等各类任务上进行推断的实际性能：
![](https://pic1.zhimg.com/v2-38dabf7ddc7fc047ae8eda9e30ada158_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='538'></svg>)![](https://pic4.zhimg.com/v2-0890e78dd103fb2ea9f41e5a36a9237f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1028' height='1187'></svg>)
此外，它还支持高分辨率传感器，而且可以并行处理多个传感器，并在每个传感器流上都运行多个神经网络。

英伟达称，Jetson Nano能够运行所有AI模型，可以创建数百万个智能系统。

GTC刚结束的时候，就有人在HackerNews上表示，自己想要DIY了。

很快就有“同道中人”回复他，你可以自己做无人驾驶小车、能跟踪猫的无人机、可以自动识别鸟的摄像机等等。

## **自动驾驶**

最后，自动驾驶，老黄说这是最重要的自动化机器的落地场景。

也是三方面。

首先，推出全新**DRIVE Constellation**自动驾驶汽车仿真平台。

基于该平台，在云端就能虚拟仿真各种自动驾驶场景——不用再路测数百万公里了。
![](https://pic3.zhimg.com/v2-26cc39a26d3939fe554366be2650f91e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
老黄说，从常规驾驶，到各种罕见的危机情况，都能在仿真中实现，要风得风，求雨得雨。会比现实世界中实现的效率高出不知多少倍，而且成本低、安全性高。

数据中心方案则去年就有过官宣。包括两个并排服务器：

第一台服务器——DRIVE Constellation Simulator，从虚拟汽车生成传感器输出。

第二台服务器——DRIVE Constellation Vehicle，包含DRIVE AGX Pegasus AI车载电脑。

DRIVE AGX Pegasus接收传感器数据，做出决定，然后将车辆控制命令发送回模拟器。
![](https://pic1.zhimg.com/v2-cea5eeb5f7ba335e233abe82a9845b68_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
老黄还强调，该过程完全闭环，而且定位精准、定时精确，测试环境也能随心所欲——不用怨天求神。

其次，该平台完全开放，提供编程接口，允许DRIVE Sim生态系统合作伙伴集成他们的环境模型、车辆模型、传感器模型和交通场景。

同时也通过整合合作伙伴的场景，让平台可以生成全面，多样化和复杂的测试环境。

换而言之，玩法有点像虚拟仿真领域的Apollo。

英伟达也在这个“开放生态”里，集合了产业链上下游玩家。

第三，英伟达还宣布了基于仿真平台的最新合作。

并且合作方来头不小，是全球第一大车厂——丰田。
![](https://pic4.zhimg.com/v2-9063cfe87629947261acb434551d6407_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
老黄的意思也再明确不过，像丰田这样的大厂都当了首个客户，开放平台DRIVE Constellation的品质肯定值得信赖。

此外，还是在GTC大会上，老黄还宣布推出新自动驾驶软件套件，该软件的主要组成部分是Safety Force Field（SFF），通过加强规划和控制，实现更安全、舒适的驾驶体验，而且其算法策略主打车辆保护、乘客和其他车辆安全。
![](https://pic4.zhimg.com/v2-1efd6e68feca985ab7fbba716ae9b6bb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
在高级辅助驾驶领域，这会最先发挥作用。

## **老传统：视觉渲染新突破**

当然，英伟达的GTC，不炫下最新渲染成绩怎么行？

今年新亮相的图像渲染引擎，让图片的渲染更快、更真实。
![](https://pic1.zhimg.com/v2-e2a11ab31c841b297fb170631c88fce0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
**NVIDIA Omniverse**，3D创作渲染协同工具，即使团队成员身处不同的时区，也能一起为同一个3D场景的创作出力。

同时，英伟达宣布Adobe、Autodesk、Epic、Unity等16家厂商也将得到RTX技术支持，帮助他们在复杂的场景中完成光线跟踪照明，用更快的速度进行图像渲染，这一技术支持在今年内将会服务900万创作者。
![](https://pic2.zhimg.com/v2-b9e30f9271b7fd008a1746a6c22a70e1_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
另外，对于游戏玩家，英伟达推出了**GeForce NOW**云游戏服务，游戏在云端的电脑上运行，而玩家只要打开任何一台PC或者MAC，就能让云端的游戏显示在自己的电脑上，无需下载、安装、升级、更新、装补丁，也不用担心自己的电脑配置不够了。

显然，老黄也最爱这部分的Demo展示。

不断播放视频，不断收获掌声，他还不断告诉大家：鼓掌太早了，好戏还在后面。
![](https://pic2.zhimg.com/v2-4cc1ed008451251f41c0a9e9e1c241f5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='300'></svg>)
甚至到后来，教主直接扔核弹语录：

> 我常说，买得越多，省得越多，现在我觉得我错了……RTX Sever根本就是免费的！省下的电费就能买一台服务器了！

## **老黄の诚意**

不过今年GTC，新品确实便宜得不像英伟达。

会后，大家最主要的评价都集中在Jetson Nano上。

更直接说是99美元售价上。

虽然TheVerge也报道称，这种低价的AI开发套件，英伟达并非独一家。

比如英特尔的Neural Compute Stick只需要79美元，谷歌在Coral下最近也推出了两款设备，分别是150美元的开发者套件和79美元USB加速设备。

但不可否认的是，英伟达正在进入一个有潜力的市场，AI驱动的创新正在增长。

而且，对于今年GTC主题演讲关键词是PRADA的英伟达来说。

99美元，折合人民币666元。老黄的诚意，看得见。

你觉得呢？

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


