# 28 天自制你的 AlphaGo (1) :  围棋 AI 基础 - 知乎
# 



知乎对于人机大战的关注度很高，希望这个系列能让大家对于人工智能和围棋有更多的了解。如果有收获，请记得点一下赞。

大家都知道 AlphaGo v13 的三大组件是：
- MCTS（蒙特卡洛树搜索）

- CNN （卷积神经网络，包括：策略网络 policy network、快速走子网络 playout network、价值网络 value network）

- RL （强化学习）



现有的强的围棋 AI 也都是这个思路了，我们也会按这个思路讲。在这一篇先看看围棋和博弈 AI 的一些基本常识。

***本系列已更新多篇，其它几篇的传送门：***
- ***(2) : 安装 MXNet 搭建深度学习环境 [知乎专栏](https://zhuanlan.zhihu.com/p/24879716)***
- ***(3) : 训练策略网络，真正与之对弈 [知乎专栏](https://zhuanlan.zhihu.com/p/24885190)***
- ***(4) : 对于策略网络的深入分析（以及它的弱点所在） [知乎专栏](https://zhuanlan.zhihu.com/p/24939269)***

- ***(4.5)：后文预告（Or 为什么你应该试试 Batch Normalization 和 ResNet）**[知乎专栏](https://zhuanlan.zhihu.com/p/25051435)***
- ***(5)：结合强化学习与深度学习的 Policy Gradient（左右互搏自我进化的基础） [知乎专栏](https://zhuanlan.zhihu.com/p/25098864?refer=mathNote)***
- ***(6)：蒙特卡洛树搜索（MCTS）基础 [知乎专栏](https://zhuanlan.zhihu.com/p/25345778)***
- ***(6.5): 在浏览器 Javascript 中直接运行策略网络（附第78手分析图）[28 天自制你的 AlphaGo (6.5) : 在浏览器 Javascript 中直接运行策略网络（附第78手分析图）](https://zhuanlan.zhihu.com/p/25563959)***


## **一、围棋规则**

围棋博大精深，但基本规则很简洁。推荐这个在线教程：


[熊猫围棋网 - 游戏的内容 - 第一天 围棋的介绍 - 十天即可掌握！『PANDANET围棋入门』](https://link.zhihu.com/?target=http%3A//www.weiqi-pandanet.cn/howtoplaygo/01-01.htm)

**正常来说，黑棋的第 1 手要下到你的右上角，比如说右上角的"星位"或者"小目"**。虽然棋盘是对称的，下其它三个角也可以，但这是对弈的一种礼貌，这样下对方就知道你是懂规矩的（当然，也可以第 1 手下在边上，或者中腹，但目前人类一般认为是亏的）。


如果等不及，看完教程的"第一天"内容，其实就可以立刻玩一下了。下图是我做的纯神经网络走棋（称为策略网络 Policy Network），这是电脑从几十万局人类高手棋谱所训练得到的数据，棋力不错，有奕城段位的水准。大家可以点击打开：

## **点击打开：[Analyzing Go with Policy Network 围棋策略网络预测演示](https://link.zhihu.com/?target=https%3A//withablink.coding.me/goPolicyNet/)**

打开后，123456789代表电脑对于下一手的推荐选点的前9位。试试先按电脑的建议走一走，培养一下"棋感"吧！例如，你将会看到，电脑认为，人类高手最喜欢下的第一手在右上角的"星位"（下图中的1号点）：
![](https://pic3.zhimg.com/v2-56b2a66ad154c5ac3011dc31e101738a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='548'></svg>)
看个有趣的事情。人机大战第四局第78手后，其实策略网络给出的应对是正确的（1号点）。当时AlphaGo犯错（选择了5号点），说明当时价值网络和Rollout的判断出现了问题：
![](https://pic4.zhimg.com/v2-c842ad768ee0c338762ed681b6f95277_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='987' height='987'></svg>)
## **二、围棋规则的细节，目前市面的程序**

围棋规则实际有很多细节，而且并没有世界统一的规则。现有的规则有中国、韩国、日本、应式等等，在大多数情况下不影响胜负的判断结果，但还是有微妙区别，比如说中国是数子，有"粘劫收后"，日韩是数目，终局有死活的判断问题。

较为简洁，适合电脑描述的可能是 Tromp-Taylor 规则： [Tromp-Taylor Rules](https://link.zhihu.com/?target=http%3A//senseis.xmp.net/%3FTrompTaylorRules) 。不妨做几个调整：


a. 再简化一点，去掉"全局禁同形再现"，改成"罕见循环局面由裁判决定"（毕竟三劫以上循环的可能性很小），这样程序就不一定要存一个每个局面的hash表。

b. 禁止自杀，这样更接近其它规则。

c. 虽然目前看最佳贴目可能更接近 5.5，但还是按中国现有规则的 7.5 吧，方便大家统一。

目前市面最强的程序是 银星17 和 Zen6，不过价钱也比较高（虽然都有X版）。而目前免费软件中最强的是 Leela，棋力也还不错，请点击： [chess, audio and misc. software](https://link.zhihu.com/?target=https%3A//sjeng.org/leela.html)


而且 Leela 有 console 的接口，可以接上目前常用的围棋 GTP 协议。如果你有兴趣，现在还有一个电脑围棋的天梯，可以连进去与其它程序对战，看自己的排名： [19x19 All Time Ranks](https://link.zhihu.com/?target=http%3A//www.yss-aya.com/cgos/19x19/bayes.html) 。

## **三、关于蒙特卡洛树搜索**

关于蒙特卡洛树搜索，有一个常见的错误认识，在此先纠正。

在棋类博弈 AI 中，很多年前最早出现的是蒙特卡洛方法，就是到处随机走，然后看哪里的胜率最高。但这有一个问题，举个例子：

a. 如果走在 A，人有100种应对，其中99种，电脑会立刻赢，只有1种电脑会立刻输。


b. 如果走在 B，人有100种应对，但局势很复杂，大家都看不清，如果随机走下去，后续的胜率双方都是50%。


那么如果计算蒙特卡洛的胜率，电脑会发现走在 A 的胜率是 99%，走在 B 的胜率是 50%。

可是，电脑的正解是应该走 B！因为如果走了 A，人如果够聪明，就一定会走电脑立刻输的变化。如果电脑执意要走 A，就只能称之为"骗着"了。

于是有的人会说蒙特卡洛方法有缺陷。但是，蒙特卡洛树搜索在理论上解决了这个问题。

怎么把人想象得尽量聪明？**这就要靠博弈树。蒙特卡洛树搜索是博弈树和蒙特卡洛方法的结合，****它不会犯 A 和 B 的问题，它很快就会发现 B 比 A 好。****容易证明，如果给定足够的计算时间和**足够**的存储空间，蒙特卡洛树搜索可以收敛到完美的博弈树，成为围棋之神。**

然而，实际的计算和存储资源是有限的，实际的 A （不妨称之为陷阱）也会更复杂。比如说：

c. 如果走在 A，经过博弈树的模拟，电脑几乎是怎么走都怎么赢；但是人更清楚后续的走法，人会走出完美的应对，在许多步后电脑一定会突然死亡。


这时，电脑要走遍了博弈树的许多层，才能发现原来走到 A 会存在致命缺陷，掉入陷阱。这种情况下，蒙特卡洛树搜索就容易在 A 和 B 的问题上给出错误的答案。这有好多种表现，比如说"地平线效应"，又像 AlphaGo 被击中的第 78 手。


围棋中是有很多陷阱局面的，比如说"大头鬼"。人工智能如何正确应对陷阱局面，或者避免进入陷阱局面？这就需要 策略网络（policy network）、价值网络（value network）、强化学习 的共同作用。在后续的文章将会逐步介绍，包括具体的训练方法。


## **四、第一篇的作业**

1. 上面的那个纯神经网络走棋是开源的，请改它的代码，做一个基本的围棋界面，包括吃子、打劫、点目等等基本规则。


2. 请阅读它的论文： [http://jmlr.org/proceedings/papers/v37/clark15.pdf](https://link.zhihu.com/?target=http%3A//jmlr.org/proceedings/papers/v37/clark15.pdf) 。


3. 请做一个傻瓜版的 策略网络，比如说会做眼，会吃子。可以参考 GnuGO 的 [6. Move generation](https://link.zhihu.com/?target=http%3A//www.gnu.org/software/gnugo/gnugo_6.html) 。

4. 请做一个傻瓜版的 价值网络，比如说可以用"棋子向外辐射影响"的方法。可以参考 GnuGO 的 [13. Influence Function](https://link.zhihu.com/?target=http%3A//www.gnu.org/software/gnugo/gnugo_13.html) 和 [Bouzy's 5/21 algorithm](https://link.zhihu.com/?target=http%3A//www.delorie.com/gnu/docs/gnugo/gnugo_201.html) 。

***本系列已更新多篇，其它几篇的传送门：***

- ***(2) : 安装 MXNet 搭建深度学习环境 [知乎专栏](https://zhuanlan.zhihu.com/p/24879716)***
- ***(3) : 训练策略网络，真正与之对弈 [知乎专栏](https://zhuanlan.zhihu.com/p/24885190)***
- ***(4) : 对于策略网络的深入分析（以及它的弱点所在） [知乎专栏](https://zhuanlan.zhihu.com/p/24939269)***
- ***(4.5)：后文预告（Or 为什么你应该试试 Batch Normalization 和 ResNet）**[知乎专栏](https://zhuanlan.zhihu.com/p/25051435)***
- ***(5)：结合强化学习与深度学习的 Policy Gradient（左右互搏自我进化的基础） [知乎专栏](https://zhuanlan.zhihu.com/p/25098864?refer=mathNote)***
- ***(6)：蒙特卡洛树搜索（MCTS）基础 [知乎专栏](https://zhuanlan.zhihu.com/p/25345778)***
- ***(6.5): 在浏览器 Javascript 中直接运行策略网络（附第78手分析图）[28 天自制你的 AlphaGo (6.5) : 在浏览器 Javascript 中直接运行策略网络（附第78手分析图）](https://zhuanlan.zhihu.com/p/25563959)***


> 如需转载本系列，请先与本人联系，谢谢。小广告：晚上工作学习是否觉得光线不够舒服，精神不够集中，眼睛容易疲劳？不妨点击看看我们的自然全光谱灯系列：[Blink Sunshine护眼LED灯泡 高显指97显色指数无频闪学习台灯床头](https://link.zhihu.com/?target=https%3A//item.taobao.com/item.htm%3Fid%3D40134613056)  如果需要好用的耳机或钱包，我们也有 :-)


