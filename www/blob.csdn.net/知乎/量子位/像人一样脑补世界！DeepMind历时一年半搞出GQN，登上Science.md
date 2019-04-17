# 像人一样脑补世界！DeepMind历时一年半搞出GQN，登上Science - 知乎
# 



> 夏乙 安妮 发自 凹非寺
量子位 出品 | 公众号 QbitAI

历时一年半的研发之后，新成果浮出水面 。

被称为人工智能“梦之队”的DeepMind，刚刚在Science上发表论文，提出一种像人类一样具备“脑补”世界能力的AI：**GQN（Generative Query Network）**。

GQN能够基于2D图像，重建/脑补出整个场景的3D表示。

“与婴儿和动物非常相似，GQN通过理解对周围环境的观察来学习。”DeepMind在官方博客中称这个学习方式，就像人类一样。

DeepMind的创始人兼CEO哈萨比斯表示，他一直都对大脑如何重现图像非常着迷，而这次提出的GQN模型可以根据几个2D快照重现3D场景表示，还能从任何角度渲染出来。
![](https://pic2.zhimg.com/v2-4bd57b787329ad084703b093494b5df1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='876' height='428'></svg>)哈萨比斯Twitter截图
## **这个AI高级在哪？**

你可能已经从前边的描述中注意到，它是一种无监督学习模型，连训练的图像都是自己获取的。

而现在那些很厉害的计算机视觉系统，都是监督学习的成果，它们需要用人类制造的大规模标注图像数据集来训练，这就限制了数据集包含场景的范围，进而限制了这种数据所训练出来的视觉系统的能力。
![](https://pic1.zhimg.com/v2-fa7fb3176ba7d053b921cabe256af464_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
论文一作、DeepMind研究员Ali Eslami说，监督学习超级成功，但它还是无法让人满足。有这样两个原因：第一，人类需要手动创建数据集来训练它，这种操作又贵又不全面；第二，人类婴儿和高级哺乳动物都不是这么学习的。

如果要开发在现实世界里可用的复杂机器，想让AI完全理解周边环境，知道能就近坐在哪、旁边的沙发是什么材质、地上的影子是哪个光源制造出来的、又该去哪关灯，就需要换一个思路。

DeepMind新提出的GQN，就是这样一种新思路。

## **两个网络**

GQN模型由两部分组成：一个表示网络、一个生成网络。

**表示网络**将智能体观察到的图像作为输入，然后生成一个描述潜在场景的**表示（向量）**。
![](https://pic3.zhimg.com/v2-2bd366c096f03383aa9aa3e58f2969c2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
**生成网络**的任务是从一个之前没有观察到的角度，来**预测（也可以叫“想象”）**出这个潜在的场景。
![](https://pic2.zhimg.com/v2-8d1196f7b37ed7115ed556606b846e29_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
表示网络并不知道人类会让生成网络从哪些视角来预测，所以，它必须有效地描绘出整个场景的真实布局。

在这个过程中，它会先构建一个简单的分布表示，抓住最重要的元素，比如物体的位置、颜色和整个房间的布局。在训练过程中，生成器了解了这个环境中典型的物体、特性、关系、规则。这些两个网络共享的“概念”让表示网络可以用高度压缩、概略的方式来描述场景，让生成网络在有必要的时候去补全细节。

比如说，一个蓝色立方体，在表示网络那里就是一组数字，而生成网络知道在不同角度该用怎样的像素来呈现它。

## **四大特性**

DeepMind在模拟的三维世界中，基于多个程序生成的环境，对GQN进行了一系列受控实验。其中的多个物体的位置、颜色、形状、纹理、光源都是随机生成，并且伴随着重度的遮挡。

在这些环境中进行训练之后，GQN被用于新的场景。DeepMind在实验中，发现了GQN的几个重要特性。

第一，GQN能以非常精确的方式，从全新的视角“想象/脑补”出以前从未见过的场景。只要给出一个场景表示和新的摄像机视点后，GQN就能生成清晰的图像，无需实现规定透视、遮挡或光线规则。

效果请看下面几个演示，左边是观察到的2D图片，右边是GQN“脑补”出的世界。
![](https://pic1.zhimg.com/v2-e8636598d3b6e4685ee6467c5f5cb4e8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='299'></svg>)![](https://pic1.zhimg.com/v2-0cca7e110e3bb2dd6544d107082f6c74_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='299'></svg>)![](https://pic2.zhimg.com/v2-445db937e67c07defccd68664582ffad_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='299'></svg>)
第二，GQN学会对对象进行计数、定位和分类，而且无需任何对象级标签。尽管露出的表示部分可能非常小，但GQN的预测仍然高度准确，几乎可以乱真。这意味着GQN能够准确感知。

效果请看下图，同样左边是观察，右边是脑补：
![](https://pic4.zhimg.com/v2-9daffca8bdc1b6fe4b349324bd7df2a7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='300'></svg>)
第三，GQN能够表示、衡量和减少不确定性。即便内容不是完全可见，GQN也能应付场景中的不确定性，并将场景中的多个局部视图组合起来，形成一个整体。

这事儿挺难，人都不一定能做好。GQN能力如何？见下图所示。有一类是第一人称视角的预测：
![](https://pic1.zhimg.com/v2-665942845b9e2cbdad27b8f4c2a7f480_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='293'></svg>)![](https://pic2.zhimg.com/v2-33c69849d292f87416df492c2bb944c5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='293'></svg>)
有一类是自上而下的视角预测：
![](https://pic4.zhimg.com/v2-e8657310a4f3e64e0510718407bf2cbb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='293'></svg>)![](https://pic3.zhimg.com/v2-28faa4000336827bca71d8d5232012da_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='293'></svg>)
第四，GQN能够进行稳健、数据高效的强化学习。与无模型的基线智能体相比，把GQN的紧凑表示赋予最先进的强化学习智能体后，可以更加数据高效的方式完成任务。

对这些智能体而言，GQN中的信息可以看做是对环境的“先天”知识。
![](https://pic4.zhimg.com/v2-bba9d0a571dc2a34767da06680ab6b47_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='409'></svg>)
## **相关研究**

GQN同样构建于诸多前人的探索基础之上，DeepMind正式列出的相关研究就达到128篇，而且声明仅列出了一小部分。

这些相关的研究从1971年，一直延续到2018年3月。
![](https://pic1.zhimg.com/v2-49257c181cac8be9f2143516fbd6430c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='650' height='385'></svg>)
其中排在第一位的，也是曾经刊载在Science上的著名研究：心理旋转实验。这个实验表明：表象这一心理现象是客观存在的，外界刺激进入大脑并非进入黑箱，而这个实验也揭露了信息在大脑中进行加工的过程。

如果你对128篇相关研究感兴趣，传送门在此：

[http://t.cn/RB9WmLX](https://link.zhihu.com/?target=http%3A//t.cn/RB9WmLX)

与之前的研究相比，DeepMind展示了一种学习物理场景紧凑、基础表示的新方法，而且这个方法不需要特定领域工程或耗时的场景内容标记。也就是说，这个模型可以应用于各种不同的环境。

而且这个方法还雪城了一个强大的神经渲染器，可以从新的视角生成精确的场景图像。

当然DeepMind也在博客中表示，与更传统的计算机视觉技术相比，新方法仍然有很多限制，目前也只接受过合成场景的训练。

不过随着新的数据源出现，加之硬件能力的进展，DeepMind准备研究GQN在更高分辨率的真实场景中的应用。展望未来，DeepMind表示会探索GQN更多的场景理解能力，例如跨空间和时间学习物理和运动的常识概念，以及在虚拟和增强现实中的应用。

“尽管在实际应用之前还有非常多的研究需要完成，但我们相信这个工作是迈向完全自主场景理解的一大步。”DeepMind说。

## **多方评价**

毕竟也是登上Science的论文了，DeepMind官宣几个小时后，在Twitter和Reddit收到了大量围观和评价。

“话痨”型谷歌大脑的研究人员David Ha称赞这是来自“梦之队”DeepMind的炫酷研究。“环境生成模型可为自动学习理解周围世界的机器铺平道路。”他给出了肯定评价。

日本AI独角兽[Preferred Networks（PFN）](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247498553%26idx%3D1%26sn%3Dcc33552be9b21400bca1f2c0cc353249%26chksm%3De8d04c4bdfa7c55dadb0982f139c8f0d1fb8d611628b522acd9761d431de648c61ab099f63a7%26scene%3D21%23wechat_redirect)联合创始人冈野原大辅（Daisuke Okanohara）认为，GQN通过几个视角的观察就能将3D世界表示成本征向量，又能从新视角重现出场景，“这项研究显示出神经网络可以通过无监督方式学习这样的能力，真是个突破。”
![](https://pic3.zhimg.com/v2-9d60ba67355fba841915f52f2824df76_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='609'></svg>)

Twitter机器学习界网红AlteredQualia认为这是“相当疯狂的”的一项研究，澳大利亚数据科学家、[http://psyoa.org](https://link.zhihu.com/?target=http%3A//psyoa.org)董事会成员Tom Wallis评价GQN“在场景理解上效果惊人”……
![](https://pic3.zhimg.com/v2-42bd167011bc8c9a62f8ddc32f2cbb22_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='862' height='484'></svg>)
虽然很多大牛力挺，但……Reddit上的很多程序员却认为这项研究没有所说的那么完美。

ID为court_of _ai的网友认为这项研究有些让他失望：

> 视觉效果不错，但也只是一个典型的过拟合练习。就用一堆玩具似的世界，用大量数据提炼出vanilla conditional deconvs。……但这怎么能算一个突破？

网友seann999认为，若想应用这项成果到实际研究中，那恐怕有点难度：

> 结果真的很酷，但是在典型的导航任务(比如IRL或3D迷宫游戏)中，你通常不会得到真实的当前相机视角/位置，所以我认为它应用起来非常困难。
仅从图像和动作序列进行三维表示学习和环境重构可能更具挑战性，特别是在随机环境中。

还有一些网友因此开始怀疑Science是不是给DeepMind放水了：

> Deepmind技术性地“收买”了这些大型期刊，他们近期发表在《科学》/《自然》上的论文我们很难当真。他们的很多研究都非常棒，但为什么要这样炒作呢:（

## **论文传送门**
![](https://pic4.zhimg.com/v2-34331330444ed8f4a3f1c636afd82aeb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='974' height='584'></svg>)
Anyway，这篇论文还是非常值得一读，传送门在此：

[http://science.sciencemag.org/content/360/6394/1204.full](https://link.zhihu.com/?target=http%3A//science.sciencemag.org/content/360/6394/1204.full)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




