# 少年，这有套《街霸2》AI速成心法，想传授于你…… - 知乎
# 



> 千平 发自 LZYY
量子位 出品 | 公众号 QbitAI



![](https://pic4.zhimg.com/v2-874f5c36284d59bf1c3a651ff2eb58c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='299'></svg>)



“ 少年，我看你骨骼精奇，是万中无一的武学奇才。我这有套《街霸2》心法，见与你有缘，就十块钱卖……”

慢！

苦练《街霸2》可能已经没前途了。

因为AI来了。

少年，不如修习如何炼制一枚格斗游戏AI吧。量子位这有套西方来的《街霸2》AI速成心法，见与你有缘，就免费转送了……



![](https://pic4.zhimg.com/v2-590148a7353d8b6abda2f79141291167_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)



## AI心法

这套心法，乃是“宅肉死抠破”（Gyroscope）所创。基于超任平台（Super Nintendo）。所用乃是强化学习之方法。依靠强化学习，AI观察世界、选择行动方式，目标是最大限度的获得奖励。



![](https://pic3.zhimg.com/v2-ee307ad14691b50bf1fc95c4efd9dede_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='337'></svg>)



在游戏中也是一样。想要炼制一枚《街霸2》AI，需教会这个神经网络每个角色如何跳跃、移动、出招，还得教会它观察血量和剩余时间。我们需要把游戏中的种种信息，提炼成AI能看懂的格式，称之曰：观察空间。

观察空间

为了达到速成的目的，节省训练时间，这套心法在使用强化学习时，使用了手动定义观察空间的方式。具体来说，这个观察空间包括：
- 每个角色的X、Y坐标
- 血量
- 角色是否跳跃
- 角色是否蹲下
- 角色移动的指令
- 两个角色之间的绝对距离
- 游戏时间



![](https://pic4.zhimg.com/v2-32f3ba12a01a1fd5a9f572d769c1c923_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='560'></svg>)



这个观察空间非常庞大。至少有数万亿个参数。

动作空间

AI观察环境之后，必须立刻进行下一步动作。表征可用动作最简单的方法，是使用超任手柄上的按钮：上、下、左、右、A、B、X、Y、L、R。如果考虑按钮同时按下的情况，就有1024中不同的可能。



![](https://pic2.zhimg.com/v2-a23de44f20dc1519c5d228f41c9695b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='404'></svg>)





![](https://pic1.zhimg.com/v2-1f30a3ff88e0d0bde7ceaf49c069a0a8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='312'></svg>)



考虑动作空间的另一种方法是建立可用的动作集，例如高踢、抱摔、上切等。把一个动作转变为对应的按钮组合。这里为了节省训练时间，心法将动作空间简化为一个方向键+一个出招键的组合，例如“上+A”。

这样就把动作空间简化到35种可能性。当然，如果时间允许，我们也 可以尝试更复杂的组合方式。

奖励

一旦采取了行动，就得让AI得到奖励/惩罚。

在《街霸2》种，出招合理与否有个简单的衡量方式：血量。AI要做的就是，却把血量的最大化。如果脚踢对手，能造成10个点的伤害，那么系统就奖励AI同样的得分。

如果下次观察后，AI没有行动，只要能保持血量的差距，仍然会获得10分奖励。当然，如果表现不好，也会被扣分。



![](https://pic4.zhimg.com/v2-730f616e1c276bb98787dc55da088853_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='640'></svg>)



**△** 这是街霸中Dhalsim（印度）训练中的奖励情况

AI训练AI

在训练AI这件事上，Gyroscope有一套算法，可以自动搞定哪个问题用什么算法最好。在《街霸2》这件事上，简化使用了这个方法，并且选择了DQN作为强化学习的方法，当然也做了一些调整。

DQN使用模型来预测哪些动作是最优选择。至于具体的做法，这里卖个关子，稍后会在另一篇心法中详述。

## 模拟器

想要训练AI，得把它带到《街霸2》的世界里。问题是，手头上没有超任的SDK。幸运的是，还是找到了解决的办法。

BizHawk，支持多种模拟器内核，包括超任。BizHawk提供了一系列关键工具：
- 一个Lua语言脚本界面，可以逐帧控制游戏
- 一套主机内存监视工具，用以检查内存中的地址
- 没有速度和显示限制，可以最大化游戏帧率



![](https://pic4.zhimg.com/v2-a2df7f9c3f7b3cd54342496454f82fbf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='250' height='250'></svg>)



BizHawk源代码在此：

[https://github.com/TASVideos/BizHawk](https://link.zhihu.com/?target=https%3A//github.com/TASVideos/BizHawk)

有了Lua界面，就可以在《街霸2》中控制按钮、读取内存位置以及控制模拟器内核。内存探测器让AI可以直接读取对手的血量、动作等数据。

注意，这里只让AI读取了玩家能看到的信息，没有读取额外内容。

## 侵入内存

我们需要从内存中读取一些关键数据，来构成观察空间。在开头我们也提到过，比方角色的位置、血量、行动、时间等。这些都放在内存中的某个地方。



![](https://pic2.zhimg.com/v2-3fa1389bb2f9fe66ec174a65898d8381_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='749'></svg>)



到底在哪呢？这时就可以用BizHawk的工具，来监测内存数值的变化，进而发现内存中被改变的地方。Gyroscope花了几个小时，最终确定了所有数据在内存中的位置，建立了从内存到观察空间的映射。

举个栗子，比方像下面这样：


```
public int get_p1_health()
{
 return _currentDomain.PeekByte(0x000530);
}
public int get_p2_health()
{
 return _currentDomain.PeekByte(0x000730);
}
```


## 搞定代码

BizHawk内嵌了Lua脚本引擎，所以Gyroscope初步尝试在Lua里写SDK。那是一个Lua库，用于访问所有的内存位置。

不过问题在于，Lua接口不支持任何网络I/O。由于服务在云端运行，所以这是一个大问题。可以借助Python从中转送，但会带来同步和速度等方面的问题。

解决的办法是放弃Lua，直接使用原生BizHawk工具，这些工具是用C#编写的。之前写的Python代码仍然保留，当做一个简单的接口，给它起了个名字叫EmulatorController。

最后的结果是，对于游戏中的每一帧画面，获得一个观察结果，然后发送给EmulatorController，这个控制器再去询问AI，得到行动指令后，返回下一帧予以执行。

这个方法的速度够快，是时候开始正式训练了~

## 训练AI



![](https://pic4.zhimg.com/v2-0414f2dae8d83bdddb6c0b7d1993562b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='337'></svg>)



随着一切准备就绪，针对AI的训练立即开始。每个角色训练了8个小时，大概3000场比赛。



![](https://pic2.zhimg.com/v2-accac7e4ea0279ba1fff91717f963549_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='337'></svg>)



在训练过程中，Gyroscope尝试了观察空间、动作空间、奖励函数、DQN参数的各种不同组合，直到找到一个胜率较高的AI配置。



![](https://pic2.zhimg.com/v2-f84dc5cb7b7ba9847d717dbc4bb372b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)



除了标准的调优技术和良好的习惯（一次只改变一个参数），训练AI中最关键的发现是按方向键和按出招键的不均衡权重。

对于每一帧，方向键造成的影响很小，而出招键会引发一系列重大变化。例如，出拳需要很多帧才能完成。也就是说，一帧中的动作会在后续很多帧中继续产生影响。所以，AI被训练为在下一步行动前，会在20帧内持续按下出招键。

换句话说，AI不是逐帧采取行动，而是每⅓秒观察和行动一次。

很多人会问，为什么没把最终赢得胜利作为奖励？简单地说，那样的话会让训练更加困难和冗长。

训练之初，AI对三星级（街霸的星级系统）对手的胜率是20%，训练到最后，胜率已经达到90%。如果训练的时间再长，应该可以获得更好的成绩。



![](https://pic3.zhimg.com/v2-991e8e7e1aa6b7f43f32c413abd37cae_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)



## 街霸2争霸大赛

这个《街霸2》AI第一次亮相，是在刚刚结束的三星开发者大赛上。

Gyroscope搞了一个《街霸2》AI争霸大赛！

一共四位AI选手出战，每个AI控制两个角色。对战表如下图所示。



![](https://pic3.zhimg.com/v2-b2fdf983ae89cde9b596ae59903d973a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='296'></svg>)



最终的决赛，在M.Bison和Dhalsim之间展开。好吧，M.Bison简直是个bug级的存在，他获得了最后的胜利。



![](https://pic4.zhimg.com/v2-ca3f8ee3f9ae32c53bf183f0d17b1fe7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='388'></svg>)



然后换了一组角色，再次展开捉对厮杀。

决赛中，Sagat击败本田，赢得冠军。



![](https://pic4.zhimg.com/v2-efde2c85e7d38a505423db762c8b03ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)



## 快取走这套心法

好吧，讲到这里，再指一条明路。Gyroscope把修改的模拟器放在GitHub上了，地址在此：

[https://github.com/GyroscopeHQ/BizHawk](https://link.zhihu.com/?target=https%3A//github.com/GyroscopeHQ/BizHawk)

好吧，这套心法讲完了。今天就到这里，休息休息一下。



![](https://pic1.zhimg.com/v2-2f13234e4e7b119ee1413c61a2749fb4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='845'></svg>)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)

վ'ᴗ' ի 追踪AI技术和产品新动态


