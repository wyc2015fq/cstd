# DeepMind电竞突破：雷神之锤3竞技场AI夺旗，团战胜率超过人类 - 知乎
# 



> 黑栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-523424a661bf37e49887aa175f06fdc6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
OpenAI战队在5v5刀塔比赛上打败人类，才是几天前发生的事。

如今，DeepMind为了训练AI电竞的**团魂**，也已把触手伸向了雷神之锤3竞技场。

DM的**强化学习**智能体，不止要和AI队友一起攻打人类的阵地，也要和人类队友并肩作战。
![](https://pic1.zhimg.com/v2-305ad1ae6ea872d2ea7c9f2a4441c02c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='405' height='194'></svg>)
至少，在**夺旗** (Capture the Flag) 比赛中，AI的胜率比人类高。

而且，这里的比赛，比原版游戏还要**复杂多变**。

## **拔下对方的大旗**

DeepMind团队这次选择的雷神之锤3竞技场，是款3D第一人称多人游戏，也是培育AI团战技能的好地方。
![](https://pic3.zhimg.com/v2-70317a1a1f48d4ec771950e314c9e4b8.jpg)https://www.zhihu.com/video/997904048972001280
其中的**夺旗游戏**，两队的目标都是拔掉对方的旗子，将己方旗子守在自己的大本营。

我是蓝方，就需要标记 (Tag) 扛着蓝旗往回跑的红方敌人，我方的旗子才能失而复得。
![](https://pic3.zhimg.com/v2-59e3a7262dcc301ef25389655e7ce0ba_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='560' height='315'></svg>)
如果红方把蓝旗搬到了他们的阵地，就不好了。所以，队友之间要紧密配合才行。

规则其实很简单，但**场景的变化**很复杂。

不过，DeepMind团队还想让情况更复杂一些，给AI更有 (bian) 趣 (tai) 的锻炼。
![](https://pic2.zhimg.com/v2-eddf55e4d09bb87c7ec8237cd6c4e38d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='405' height='194'></svg>)
于是，夺旗游戏的地图上，发生了一些可爱的改动。

## **各种地图，训练有素**

每场比赛之间，地图都会发生变化。这个变化，是随着比赛的进行生成的。
![](https://pic1.zhimg.com/v2-ad4b0f7bd2feba4cfdbe29d8f0ee66c8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='225'></svg>)
这样一来，智能体要学会应付，许多没有见过的**新地图**。

DeepMind的强化学习模型，有**三个要点**——

· 第一，既然是团战，当然要把几只智能体扔进竞技场一起训练。

它们要学习，怎样和队友亲密互动，打击敌人。
![](https://pic3.zhimg.com/v2-f757f082d4c0191043a4865be0f5f156_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='225' height='225'></svg>)
· 第二，每个智能体要学习，它自己的**奖励**信号，建立自己的小**目标**，比如拔掉对方的旗。

**优化过程**是双层 (Two-Tier) 的，可以优化智能体内部的奖励，让奖励信号更直接地指向**胜利**。

· 第三，智能体在**两种节奏**里面运行，一种快，一种慢，这样可以增强它们运用**记忆体**、生成稳定的动作套路，的能力。
![](https://pic3.zhimg.com/v2-cff7b5c33eaa0204b94ce69f0d0f2f66_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='845'></svg>)
神经网络的架构，称为**FTW** (For The Win) ，为了赢 (字母顺序不要颠倒) 。

这里，有**快速**和**慢速**训练用的**RNN**，有一个共享存储模块，可以学习怎样把游戏中的某个**点**，转换为相应的**奖励**。

## **稳胜人类一筹**

训练好的智能体，就做好了赢的准备。

面对**地图大小**的变化，**队友数量**的变化，以及敌方的变量，都表现出训练有素的样子。
![](https://pic3.zhimg.com/v2-f6f14aa0d0b7c347803c47dc9b51175e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='447' height='239'></svg>)
这是户外地图，红蓝双方都是AI，斗争非常激烈。

室内地图，则是AI与人类选手组成的混合战队，与纯AI战队的斗争。
![](https://pic3.zhimg.com/v2-e0a85d5b254b1666477a6a651baba6de_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
DeepMind这次一共动用了****40**个人类加入战斗，与AI随机组队。看得出他们的比赛并不轻松。

最终的结果，**人类胜率不及AI**。
![](https://pic1.zhimg.com/v2-f88aa1d90620aa41502a5cdd0f6c4b38_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='553'></svg>)
另外，不管强大的人类，还是普通的人类，**Elo评分**都没有FTW那么高。

## **AI为什么赢？**

除了看到AI拔旗比人类厉害，团队还想知道，它们是凭什么赢。

于是，就研究了AI的行为模式，观察一下它们是怎么理解比赛的。
![](https://pic2.zhimg.com/v2-749d72089316b816cc2df093df7299a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='534'></svg>)
各种颜色的点点，分别代表， 旗在阵地，队友扛走敌方大旗，自身处在敌方阵地等等。

不同战况之下，AI的反应**明显不同**。安全和危险，分得清清楚楚。

甚至有一些神经元，是专为某种特定境况而生的。

没有人教过AI，比赛规则是什么，但它们依然能够在训练中，产生战士的修养和自觉。
![](https://pic3.zhimg.com/v2-cb097446c219c0efad83a385aafa4b8e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='325'></svg>)
另外，团队还发现，AI的战斗力虽然是在**非监督学习**中炼成的，但它们也像人类一样，会跟着队友走，会在敌方阵地上玩耍，如此种种。

果然，即便再放养，AI的爸爸也还是人类吧。
![](https://pic4.zhimg.com/v2-276e961752707d78643d7d52472f82c7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='387' height='541'></svg>)
论文传送门：

[https://arxiv.org/pdf/1807.01281.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1807.01281.pdf)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


