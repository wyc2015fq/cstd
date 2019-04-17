# 拔掉机器人的一条腿，它就重新学走路 | 三次元里优化DRL策略 · Reddit高热 - 知乎
# 



> 行走栗 发自 凹非寺 
量子位 出品 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-01d739c3e8928aa0de58a96727aa3131_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
 迪士尼的机器人，不管剩几条腿 (n>0) ，都能学会走路。

那么，是怎么学的？

研究团队不用**模拟器**，直接在**硬件**上修炼**深度强化学习** (DRL) 的**策略**。

真实世界，或许比模拟器要单调一些。不过，有物理支持的经验，可能更加珍贵。

除了有清新脱俗的训练环境，这只机器人，也并不是**一**只机器人而已。

## **想要几条腿，问过机器人吗？**
![](https://pic1.zhimg.com/v2-39802f3c36e81eb33ad732df5b4ded38_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='560' height='315'></svg>)
 机器人的腿是**模块化**的，就是说，你想给它装上一条、两条、三条腿，都可以。

嫌腿太多，拔掉一些也可以。﻿反正只要有腿，机器人还可以**重新**学走路。

另外，机器人的腿还**分三种**，运动方式各不相同——

在分别介绍之前，先给各位一些方向感。 
![](https://pic3.zhimg.com/v2-161fa3263b4eb1165d37cbbfe616f65e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='391' height='381'></svg>)
**A腿**，Roll-Pitch，横轴加纵轴。 
![](https://pic3.zhimg.com/v2-03d74f920f85f53112a786fa7820e15e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
**B腿**，Yaw-Pitch，竖轴加纵轴。
![](https://pic1.zhimg.com/v2-8a1b7062e906215e3a23a416c98a56b0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
**C腿**，Roll-Yaw-Pitch，横轴加竖轴加纵轴。
![](https://pic3.zhimg.com/v2-0224383c729c2bf9ec92b0f811159d2e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
 于是，C腿比另外两条腿粗壮一些，似乎也可以理解了。

如果按最多能装六条腿来算，一共可以拼出多少种不同的机器人？
![](https://pic4.zhimg.com/v2-dbe69558ebbc80f4ff6ab8294be4b2c7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='371' height='209'></svg>)
 这样一来，即便不是模拟器，也算多姿多彩了。

## **两种DRL同步走**

由于，不知道机器人什么时候，就会多条胳膊少条腿，迪士尼团队准备了两种深度强化学习算法。
![](https://pic3.zhimg.com/v2-409f11db03b30740f378370a37ac69de_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='560' height='315'></svg>)
一是**TRPO** (信赖域策略优化) 算法，沿用既定策略 (On-Policy) 的批量学习方法，适合优化大型非线性的策略。

二是**DDPG** (深度确定性策略梯度) 算法，用“演员-评论家 (Actor-Critic) ”的方法，优化策略。﻿

## **不同的算法，不同的姿势**

那么，在三次元学习过程中优化的**策略**，有多优秀？

按照腿的数量，分别来看一下。

## **一条腿**
![](https://pic3.zhimg.com/v2-d99d316ae33e1bcdaab1e22b3398f36a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
图中下者，是用TRPO学习完毕的A腿，与没有学过的A腿相比，走路姿势已经明显不同，**速度**也真的加快了一点点。

## **两条腿**
![](https://pic4.zhimg.com/v2-2e7c749f0f5806aa183f64c3dda496cb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
这是两条**B腿**在TRPO熏陶之下形成的姿势，轻快地触地，轻快地弹起。
![](https://pic2.zhimg.com/v2-0fa849c56019fbe807cfed39104bb4a1_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
这同样是两条B腿，但算法换成了DDPG，姿势又完全不同了，好像慵懒地向前翻滚。

## **三条腿**
![](https://pic4.zhimg.com/v2-d16460c1d8005dc91f68d934c60589e3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
 这次，机器人长了三条**B腿**。有了TRPO的加成，它用欢脱地节奏点着地，和双腿TRPO的操作很**相似**。

总体看上去，用TRPO训练过后，机器人会比较活跃，用DDPG修炼之后，机器人就有了佛系属性。

不管它有怎样的个性，研究人员都很开心。毕竟，那表示深度强化学习算法，是**有效**的。一看就知道，是谁带出的徒弟。

## **你也想被支配一下？**
![](https://pic2.zhimg.com/v2-a14fbab37668d90ad04501bd8dcf8389_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='896' height='896'></svg>)
 同性交友网站的章鱼猫 (假装) 表示，它也想接受DRL算法的蹂躏，然后解锁更娇嫩的舞姿。

论文传送门：
[https://s3-us-west-1.amazonaws.com/disneyresearch/wp-content/uploads/20180625141830/Automated-Deep-Reinforcement-Learning-Environment-for-Hardware-of-a-Modular-Legged-Robot-Paper.pdf​s3-us-west-1.amazonaws.com](https://link.zhihu.com/?target=https%3A//s3-us-west-1.amazonaws.com/disneyresearch/wp-content/uploads/20180625141830/Automated-Deep-Reinforcement-Learning-Environment-for-Hardware-of-a-Modular-Legged-Robot-Paper.pdf)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


