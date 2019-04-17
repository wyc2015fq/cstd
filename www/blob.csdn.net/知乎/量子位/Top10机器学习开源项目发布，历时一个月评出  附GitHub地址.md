# Top10机器学习开源项目发布，历时一个月评出 | 附GitHub地址 - 知乎
# 



> 原作：Mybridge
问耕 补充整理
量子位 出品 | 公众号 QbitAI

从将近250个机器学习开源项目中，综合各种条件进行打分排序，最终Mybridge团队评选出十大最新、最棒的机器学习开源项目。

这份Top10名单中包括对象检测、换脸、预测等等最热的AI明星、话题性研究和代码。它们在GitHub上的平均标星数量是2500多颗。

希望这10大开源项目，对你有所帮助~

## **Rank 10**

> **NPMT**
作者：Posenhuang等（微软研究院）
GitHub地址：[https://github.com/posenhuang/NPMT](https://link.zhihu.com/?target=https%3A//github.com/posenhuang/NPMT)
★Star：68

NPMT，基于短语的神经机器翻译，这是一项来自微软研究院团队的研究。这个机器翻译领域的新突破，没有使用任何注意力机制。

这个方法通过Sleep-WAke网络（SWAN）明确地建模输出序列中的短语结构。SWAN是一种基于分割的序列模型方法。

NPMT的源代码基于Torch中的fairseq工具箱建立。fairseq是Facebook AI研究院开源的序列到序列工具箱，这个方法使用卷积神经网络来做语言翻译，比循环神经网络提速9倍。
![](https://pic4.zhimg.com/v2-7d3e3af9d736e58d3a5266eb425a0b57_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='491'></svg>)
## **Rank 9**

> **Deep-neuroevolution**
作者：Uber AI实验室
GitHub地址：[https://github.com/uber-common/deep-neuroevolution](https://link.zhihu.com/?target=https%3A//github.com/uber-common/deep-neuroevolution)
★Star：392

这是共享出行巨头Uber开源的算法，他们此前集中发布了5篇论文，支持一种正在兴起的认识：通过用进化算法来优化神经网络的神经进化（neuroevolution）也是为强化学习（RL）训练深度神经网络的一种有效方法。

量子位也有过详细报道《[深度神经进化大有可为？Uber详解如何用它优化强化学习 | 5篇论文](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247492338%26idx%3D3%26sn%3D692e2b5bd06723ce6e06d724504d7238%26chksm%3De8d05580dfa7dc965a39dab3088a093fe5035c7668c33d3a50a2a340e9df0987e0262dff823a%26scene%3D21%23wechat_redirect)》

而这次GitHub中公布的代码，包括以下算法的分布式实现：

1、Deep Neuroevolution: Genetic Algorithms Are a Competitive Alternative for Training Deep Neural Networks for Reinforcement Learning

论文地址：[https://arxiv.org/abs/1712.06567](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1712.06567)

2、Improving Exploration in Evolution Strategies for Deep Reinforcement Learning via a Population of Novelty-Seeking Agents

论文地址：[https://arxiv.org/abs/1712.06560](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1712.06560)

这些代码基于OpenAI此前公布的源代码和论文。
![](https://pic1.zhimg.com/v2-0bc0f9876203be75403c35a1c0b085bc_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='383'></svg>)
## **Rank 8**

> **Simple**
作者：chrisstroemel
GitHub地址：[https://github.com/chrisstroemel/Simple](https://link.zhihu.com/?target=https%3A//github.com/chrisstroemel/Simple)
★Star：235

Simple是贝叶斯优化的更具可扩展性的替代方法。像贝叶斯优化一样，它的样本效率很高，能用尽可能少的样本收敛到全局最优。

对于典型的优化工作负载，贝叶斯优化消耗的CPU时间以分钟计，而Simple使用的CPU时间以毫秒计。如下图所示：
![](https://pic4.zhimg.com/v2-8be9f52437c0b66a5a3ebe95a83d1cd7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)
## **Rank 7**

> **DeepJ**
作者：Henry Mao等（加州大学圣迭戈分校）
GitHub地址：[https://github.com/calclavia/DeepJ](https://link.zhihu.com/?target=https%3A//github.com/calclavia/DeepJ)
★Star：313

DeepJ是一种端到端生成模型，能够以特定的混合风格来实时创作钢琴曲。这个算法能够生成可以调整参数的音乐，这种可调整的属性，能为艺术家、电影制作人、作曲家等带来实际的帮助。

使用这套代码需要Python 3.5。

访问下面的Demo地址，可玩、可感受，亦可当背景音听。

Demo地址：[https://deepj.ai/](https://link.zhihu.com/?target=https%3A//deepj.ai/)
![](https://pic4.zhimg.com/v2-3e8b25042b8134b132d7e56326c14117_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='600'></svg>)
## **Rank 6**

> **Psychlab**
作者：Charles Beattie等（DeepMind）
GitHub地址：[https://github.com/deepmind/lab/tree/master/game_scripts/levels/contributed/psychlab](https://link.zhihu.com/?target=https%3A//github.com/deepmind/lab/tree/master/game_scripts/levels/contributed/psychlab)
★Star：4774

Psychlab，DeepMind为AI开设的心理学实验室。

其实就是个第一人称视角3D游戏世界，这个心理学实验室当然也是个模拟环境，研究对象是其中的深度强化学习智能体（Agents）。Psychlab能够实现传统实验室中的经典心理学实验，让这些本来用来研究人类心理的实验，也可以用在AI智能体上。

智能体在这个实验室里还能干什么呢？DeepMind自己在论文中展示了8种任务。详情可以点击这里，查看量子位之前的详细报道《[嗯？DeepMind开了个心理学实验室](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493660%26idx%3D2%26sn%3Df1769de24334fd5dfacafe58c1b36c86%26scene%3D21%23wechat_redirect)》。
![](https://pic2.zhimg.com/v2-98d44840973f5ada926372d9bce9ba75_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='393'></svg>)
## **Rank 5**

> **Dm_control**
作者：DeepMind
GitHub地址：[https://github.com/deepmind/dm_control](https://link.zhihu.com/?target=https%3A//github.com/deepmind/dm_control)
★Star：882

火遍全球的AlphaGo让我们知道了强化学习打游戏究竟有多6，这么强大的算法什么时候才能打破次元壁，走进现实、控制物理世界中的物体呢？

DeepMind已经开始往这方面努力。[他们此前发布的控制套件“DeepMind Control Suite”](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247492811%26idx%3D4%26sn%3Dd5044902e41df54018b75f0c718d7c65%26chksm%3De8d053b9dfa7daafef87e21200c73a98cc04c4f8414c0f7da3c7773bde17e36994953c0ab168%26scene%3D21%23wechat_redirect)，就为设计和比较用来控制物理世界的强化学习算法开了个头。

Control Suite设计了一组有着标准化结构、可解释奖励的连续控制任务，还为强化学习Agent提供一组性能测试指标。
![](https://pic1.zhimg.com/v2-6045f2f784fb76c36876439bc8854d8c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='418'></svg>)
Control Suite中的任务可以分为14个领域，也就是14类物理模型，上排从左到右分别是：

体操机器人Acrobot，（两节钟摆）、杯中小球、倒立摆、猎豹形机器人、手指、鱼、单足跳跃机器人，下排从左到右分别是人形机器人、机械手、钟摆、质点、形似两节手臂的Reacher、游泳机器人、步行者。

## **Rank 4**

> **Lime**
作者：Marco Ribeiro等（华盛顿大学）
GitHub地址：[https://github.com/marcotcr/lime](https://link.zhihu.com/?target=https%3A//github.com/marcotcr/lime)
★Star：3148

在这次的Top10项目中，这个算是“老资格”了。主要基于KDD2016上发表的论文：《“为什么我应该相信你？”解释任何分类器的预测》。

这个研究提出了局部可理解的与模型无关的解释技术(Local Interpretable Model-Agnostic Explanations: LIME），一种用于解释任何机器学习分类器的预测的技术，并在多种与信任相关的任务中评估了它的可用性。

下面这段视频，是一个更直观的解释。
[undefined_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/x0555td6xoc.html)![](https://pic2.zhimg.com/v2-2c1cba611507a9589b059a8700a7c1fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='517'></svg>)
## **Rank 3**

> **Gradient-checkpointing**
作者：OpenAI
GitHub地址：[https://github.com/openai/gradient-checkpointing](https://link.zhihu.com/?target=https%3A//github.com/openai/gradient-checkpointing)
★Star：1107

GPU内存太小可能是神经网络训练过程中最大的拦路虎。

不怕，用这个OpenAI推出的gradient-checkpointing工具程序包，对于前馈模型来说，仅仅需要增加20%的计算时间，就能让GPU处理十倍大的模型。

这个工具包的开发者是OpenAI的研究科学家Tim Salimans和前Google Brain工程师的数据科学家Yaroslav Bulatov。

这个工具包使用了“用亚线性的存储成本训练神经网络”的技术，为简单的前馈网络提供了等价的内存存储，同时能为一般的神经网络节省内存，比如多层架构。

详细内容，可见之前[量子位的报道](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493280%26idx%3D3%26sn%3D311f72f8b6139e3b060fe232791966a5%26scene%3D21%23wechat_redirect)。
![](https://pic2.zhimg.com/v2-8df86f5d66d3172bc1d28dcb69a5a64d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='564'></svg>)
## **Rank 2**

> **FaceSwap**
作者：Hidde Jansen
GitHub地址：[https://github.com/deepfakes/faceswap](https://link.zhihu.com/?target=https%3A//github.com/deepfakes/faceswap)
★Star：3629

最近Deepfakes在AI、AV两届掀起轩然大波。简单的说，就是AI可以帮你给AV小片换脸，替换成任何你想看的明星。

量子位在两篇报道中，对此都有介绍：《[不可描述，技术进步](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493660%26idx%3D1%26sn%3Dadfebdd9ab976ee19b17f29ff383dac4%26scene%3D21%23wechat_redirect)》、《[艾玛不哭！AI引起的“换脸”问题，AI正在解决](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247494566%26idx%3D1%26sn%3Dd137dacef9ec4e3ebd0742038488bb48%26chksm%3De8d05cd4dfa7d5c2802be9c9c9bd6fe0c48946da101417df5fb143671e8989a230de49bf7d19%26scene%3D21%23wechat_redirect)》。

而FaceSwap是一个基于deepfakes的非官方开源项目。
![](https://pic1.zhimg.com/v2-a6955289fe82ea7b874f5098466f97a4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='594'></svg>)
## **Rank 1**

> **Detectron**
作者：Facebook AI研究院
GitHub地址：[https://github.com/facebookresearch/Detectron](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/Detectron)
★Star：11248

这个应该是当之无愧的第一吧。

Detectron是Facebook的物体检测平台，今年初宣布开源，它基于Caffe2，用Python写成，这次开放的代码中就包含了Mask R-CNN的实现。

除此之外，Detectron还包含了ICCV 2017最佳学生论文RetinaNet，Ross Girshick（RBG）此前的研究Faster R-CNN和RPN、Fast R-CNN、以及R-FCN的实现。

量子位在此前的报道中也有过详细的介绍：《[Mask R-CNN源代码终于来了，还有它背后的物体检测平台](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493527%26idx%3D4%26sn%3D77a24fe78c953bceea26e3cf0d28e0ce%26scene%3D21%23wechat_redirect)》。

据说好多人一直在苦等这个~
![](https://pic1.zhimg.com/v2-1ec4bfa77d674e18c2065380ea9d24a4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='678'></svg>)
就酱~

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


