# 一款芯片训练推理全搞掂，Hinton为其背书，Graphcore完成2亿美元融资 - 知乎
# 



> 「我认为我们需要走向另一种计算机，幸运的是我这里就有一个」，Hinton 说道，顺势从他的钱包里掏出了一枚 Graphcore IPU-Processor 芯片。
这是「深度学习教父」Geoff Hinton 上周接受《连线》（wired）采访，当被问及如何构建新的机器学习系统时的回答。

文| 四月

昨天消息（美国时间 12 月 18 日），英国 AI 芯片公司 Graphcore 宣布完成新一轮 2 亿美元融资，宝马旗下风投公司 iVentures 和微软作为战略投资方加入，公司估值达到 17 亿美元。截至目前，Graphcore 的融资总额超过 3 亿美元。

与一般的新锐公司不同，Graphcore 自创立之初就底气十足。他们自称，在人工智能学习的反应速度以及低能耗方面，其开发的产品要比目前市场上所有的图像处理解决方案好 10 到 100 倍。CEO NigelToon 曾在采访中提到，希望 Graphcore 能够上市，做到英国最领先的芯片制造商。

在本月的 NeurIPS（神经信息处理系统大会，前称 NIPS）大会展台上，机器之心就发现了一台由 Graphcore IPU 拼装的简易超算电脑「IPU-Pod」。
![](https://pic3.zhimg.com/v2-70151bfd4dbe0ebd5b3e78d39fb3868a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1440'></svg>)
它包括 32 片 IPU-Machines，128 片 IPU，运算性能可达到 16PFLOPS（1petaFLOPS= 10³ teraFLOPS），即每秒可完成 16 千兆次浮点计算，功耗为 24Kw，可实现亿亿次（exascale, 10^16）的超级计算。

## **不容小觑的背景**

Graphcore 成立于 2016 年，其总部位于英国布里斯托，两位联合创始人 CEO Nigel Toon、CTO Simons Knowles 此前还共同创立过市值 4.35 亿美金的 3G 蜂窝芯片公司 Icera，后卖给英伟达。所以在 Graphcore 的创立过程中，Toon 尤其强调其独立性，为避免多轮融资股份被过度稀释，还特意找来了知名投行高盛作为顾问。
![](https://pic2.zhimg.com/v2-1e62b53a56db828218b3432818dfd45d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='563'></svg>)左为Graphcore CTO Simons Knowles，右为CEO Nigel Toon
本轮融资为 Graphcore 的 D 轮融资，在最近一个月里敲定，新晋投资人和早期投资人各占一半金额。由其早期投资人 Skype 创始人 NiklasZennstrm's Atomico 以及欧洲大型投资公司 Sofina 领投。宝马 iVentures 和微软、美利安全球投资者（Merian Global Investors）均是该轮融资的新晋投资方。

此外，罗伯特博世风险投资公司、C4 风险投资公司、戴尔科技资本、Pitango（以色列最大风投）、三星和红杉资本等跟投。一年前，红杉资本参与了 Graphcore 的 C 轮融资，投资了 5000 万美元。

红杉资本在欧洲的投资项目屈指可数，他们认为，Graphcore 可能在快速发展的芯片市场与 Nvidia 和英特尔等行业巨头展开竞争。

英国历来位于芯片行业的中心地带，但其大型企业纷纷被外国企业收购，例如芯片设计商 ARM 去年被软银收购，而另一家英国芯片设计公司 ImaginationTechnologies 则被有中资背景的私募基金 Canyon Bridge 收购。

Graphcore 力争继承英国企业在芯片行业的「优良传统」。自创立以来，Graphcore 就吸收了不少战略投资方，其中 A 轮包括三星和博世，以及该轮融资中的宝马和微软。合理的推测是，博世和宝马将为 Graphcore 推进自动驾驶和车联网市场提供便利，而三星则是手机和语音助手领域的窗口。Graphcore 方面还表示，戴尔还将为其提供云计算芯片市场的 DOE 和渠道资源。

此外，Graphcore 还吸引了诸多人工智能领域的知名学术投资人为其背书，包括 DeepMind 的联合创始人 Demis Hassabis、剑桥大学的 Zoubin Ghahramani 和 Uber 的首席科学家、加州大学伯克利的 Pieter Abbeel 以及 OpenAI 的 Greg Brockman、Scott Grey 和 Ilya Sutskever。
![](https://pic1.zhimg.com/v2-560a82a720b0155f4535dfcbe54be138_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='564'></svg>)
Graphcore 频受资本和学界青睐，主要来自于其核心产品一款名为 IPU 的 AI 芯片，与常见的 IPU（Image Processing Unit，图像处理器）不同，这里的 IPU 是指 Intelligence Processing Unit，主要应用于无人驾驶汽车和云计算，专门为强大的机器习得系统设计。

「人工智能标志着计算新时代的开始，这个时代需要完全不同类型的处理器和软件工具」，CEO Nigel Toon 认为目前市场上所使用的 GPU 并不适应未来的发展趋势。他表示，「GPU 是完全用来跑算法程序的，但是机器学习不一样，你要教会系统去使用数据，这就需要不同的计算方式。」

作为英伟达的挑战者，Toon 并不否认竞争，他表示，「现在我们正在面临很重要的转型期。其挑战难度比从电脑端到移动端的时代还要大。」

## **IPU杀手锏**

「用于 Training 还是 Inference？」，面对百花齐放的 xPU 的 AI 芯片市场，这是最常见的一个问题。但 Graphcore 并没有直面回应，而是试图从另一个角度去解答，即这种分类本身就是不对的，是目前某些厂商提出的不合适的说法。

Toon 解释，如果我有一块可以连接在一起并使用多个处理器来进行速度训练的计算硬件，那么我可以使用其他处理器在不同的时间进行部署或推断；只要设计的处理器不必以特定的方式进行调整和控制来实现高性能，这便可能实现。

总的来说，Graphcore 认为先做 Training 然后做 Inference 的机制，未来一定会被 Learning 取代，即未来理想的人工智能应该能够在部署之后还能保持持续的学习和进化。
![](https://pic2.zhimg.com/v2-1b3c82d8d57043f1824f134f680b4fd5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='215'></svg>)
根据公开资料显示，Graphcore IPU 的一些关键 Feature 包括：

> 1. 同时支持 Training 和 Inference，对于这一点，Grophcore 有自己的解释。

2. 采用同构多核（many-core）架构，超过 1000 个独立的处理器。每个处理器核的处理能力和具体支持的操作还不清楚。支持 all-to-all 的核间通信，采用 Bulk Synchronous Parallel 的同步计算模型（上个世纪 80 年代提出的）。

3. 采用大量片上 memory，不直接连接 DRAM。这可能是他们的架构中最激进的一个选择。

宝马旗下风投公司 iVentures 负责人 Tobias Jahn 对于同时支持推理和训练的方案表示了认可，他谈道，「Graphcore IPU 的多功能性，即支持多种机器学习技术的高效表现适合各种应用，无论是智能语音助手还是自动驾驶车辆。因为 Graphcore IPU 的灵活性，能够在数据中心和车辆中使用相同的处理器，以减少用户的开发时间和工作量。」

此外，Graphcore IPU 的规模很大，通常包括数千到数百万个顶点，这也意味着巨大的并行性。同时 IPU 是稀疏的，大多数顶点只连接到其他一小部分顶点，即低精度。

对于定制开发型 IPU 处理器而言，要在同一台设备上进行训练抑或推理无疑是巨大挑战，在硬件架构设计之外，软件堆栈的开发更为核心。Graphcore 采用了自研的 Poplar 软件框架来解决其中可能遇到的划分、映射与通信问题。
![](https://pic1.zhimg.com/v2-624823fa7c598455a10d575237879334_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='475'></svg>)
Poplar 是一个驱动以 PCIe 为接口的 16 核 GraphcoreIPU 组件，其中插入了 Graphcore 所开发的许多库元素之一（类似于英伟达为其 GPU 提供的 cuDNN 元素），并插入诸如卷积或不同的基元来取代 TensorFlow 中编写的高级描述。该软件能够对处理元件和内部通信资源进行映射和划分，以完成处理图形任务并创建可以馈送至处理器的程序。

在《连线》的采访中，Geoff Hinton 谈到了对于 Graphcore 这种新架构的看法： 

> 几乎我们运行神经网络模型的所有计算机系统，甚至包括 Google 的特殊硬件，都使用 RAM 存储。从 RAM 中获取神经网络的权重需要耗费大量的计算资源，所以处理器可以帮忙解决。一旦每个系统都确保获得了权重，处理器就会不断计算。这是一项巨大的成本，也就是说你根本无法改变你为每个训练样本耗费的资源。

在 Graphcore 的芯片上，权重存储在处理器中的高速缓存中，而不是存储在 RAM 中，也不需要移动，因此，事情就变得比较简单了。可能我们会得到一个系统，比如一万亿个权重。这更像是人脑的规模。

今年，Graphcore 成长迅速，团队规模扩大了两倍。自一年前上次融资以来，其员工人数从大约 75 人增加到大约 200 人，并在 2018 年在伦敦、帕洛阿尔托和北京和台湾新竹开设了新办公室。

目前，Graphcore 已经完成第一批订单的产品交付，并获得了第一笔创造了收入。Graphcore 方面表示，为了满足客户对其智能处理器单元（IPU）处理器卡的需求，公司正在加大订单生产中。

值得一提的是，Graphcore 的芯片量产也经历了一年左右的爬坡期。2016 年 11 月接受第一财经采访时，Toon 曾表示 IPU 处理器将于 2017 年投放市场。不过现在看来，Graphcore 首批产品的交付在 2018 年完成。
![](https://pic3.zhimg.com/v2-193584c0dd32013f8c819c0fac3ca39e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)



