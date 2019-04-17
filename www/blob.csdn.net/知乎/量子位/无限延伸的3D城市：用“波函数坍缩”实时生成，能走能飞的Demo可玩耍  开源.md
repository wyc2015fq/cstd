# 无限延伸的3D城市：用“波函数坍缩”实时生成，能走能飞的Demo可玩耍 | 开源 - 知乎
# 



> 安妮 栗子 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-2499711ea2739126fb268ae16e178be7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='291'></svg>)



*神说，要有光。*

*于是有了光。*

*神说，要有一座城。*

*于是有了一座城。*

*神说，这座城要无限大。*

*神啊你别再说了，那是另外的价钱。*




不过，有种叫做**波函数坍缩** (Wave Collapse Function) 的算法，可以让一座白茫茫的城，在虚拟世界里**无限延伸**。
![](https://pic2.zhimg.com/v2-c0929ef1f11683c91f49853e8166e9e5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)



你跑得再快，飞得再远，都到不了它的尽头。

因为，这座城可以实时扩张，你往哪里走，它就往哪里生成，不会让你跑出去的。



![](https://pic3.zhimg.com/v2-319af367fed1b44a4887eb03087c2ea2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='540' height='304'></svg>)



而它背后的**波函数坍缩** (**WFC**) 算法自诞生以来，每隔一段时日，便会引起人类蜂拥而至的关注。

不管是因为这赏心悦目的应用：
![](https://pic4.zhimg.com/v2-f44932b00dba179704e360da1f7dbd83_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='820'></svg>)



还是因为有人做了完整而深刻的解读：

![](https://pic1.zhimg.com/v2-2fdc4af33677e40c409764a4ae7f07f4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='928' height='100'></svg>)



## **无限之城一日游**

一位推特名**Marian42_** (简称玛丽安) 的妹子，用WFC建造了无限之城。

这座城里没有街道，有的是各式各样的房屋，和房屋之间的走道与楼梯。



![](https://pic3.zhimg.com/v2-d69aeba0bb41e597d417ca66fc761002_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)



沿着它们，你可以登上高台俯视全城，也可以穿行在安静的小道。

不安分的你，还可以从栏杆上一跃而下，并不会受伤，可以继续往前跑。



![](https://pic1.zhimg.com/v2-79fbb62aa5acd426942d92a33b305a6c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)



当你以为快要走到城市的尽头，前面看不到建筑物了，只要再跑几步，眼前便又**实时生出一片新城**。

![](https://pic4.zhimg.com/v2-e087bd77e1aa48741a62307232ab08c7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='540' height='304'></svg>)



这里没有死胡同，只要你一直跑，城市就会在你前行的方向不断扩张。

除了跑跑跳跳，还可以用M键切换**飞行模式**，以上帝视角观察这座城，是如何流畅地向远方延伸：



![](https://pic4.zhimg.com/v2-a5525fb3231d934ba3bf2cad16117f6f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='540' height='304'></svg>)



当然，进城之后不要只顾着活动筋骨，偶尔也看看风景吧。

你会愉快地发现，**中式**的屋顶与**罗马式**的栏杆和石柱，在同一幢建筑里相敬如宾，丝毫不会忸怩。

有闲情的话，可以细数一下，把这89种元素集齐：



![](https://pic4.zhimg.com/v2-ecbc82b51c495f9e39bdbf7bc3fa3fbb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='662'></svg>)



毕竟，除了标配的屋顶啦、回廊啦，幸运的人类还能在转角遇到优雅的喷泉 (Demo传送门见文底) 。

## **原理揭秘**

无限城市一日游结束，是时候认真了解一下它背后的算法了：**波函数坍缩**，究竟是什么？

这个名字，来自量子力学中的一个概念，不过算法本身倒是比量子力学好理解得多，我们先从一个例子开始科普：

## **婚礼座次问题**
![](https://pic1.zhimg.com/v2-e0626e9d6866053e414d5aea2a9213ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='200' height='200'></svg>)



想象一下你正在计划一场婚礼。问题来了，办几桌宴席、嘉宾坐哪里都需要考虑。不巧的是，为了照顾大家的情绪和满足各种条条框框的习俗，你需要调节几个问题：

- 老爹不能坐在老妈两张桌子以内的距离
- 老妹如果不能和其他兄弟姐妹坐一桌，会开启可怕的狂暴模式
- 最好不要让叔叔和环保主义者挨在一起坐

不好，离婚礼只有五个小时了。此情此景下，你可以选择波函数坍缩来解决这个看似棘手的问题。

先画一张空白的座位表，记录下规则。
![](https://pic2.zhimg.com/v2-3a07f621fe7b592963751f013fc25a95_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='648'></svg>)
我们可以构建一张每个座位对应一个人的一一对应图，每个座位都可以对应任何人，也就是我们的初始波函数。


你的座位计划波函数就从这种变化万千的情况下开始了，开始的座次就是就是每个座位有效布局的完全叠加。

“叠加”的概念用薛定谔的猫来理解最好不过，在未查看猫的状态前，这就是一种叠加态，猫死猫活都可能存在。打开盒子后猫的叠加态小时，坍缩成确定态。
![](https://pic4.zhimg.com/v2-7d94ce33095e4fef65241e554e15ecd3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='792'></svg>)
先选择一个位置，随机安排名单中一个人进去，此时座位波函数开始坍缩。


随后，确定下的每个人都影响后面人员的排布。如果Michele不想坐在2号桌，那他的妹妹Barack也肯定不在这一桌。通过不断更新可以坐在2号桌的人员名单，调整座位计划的波函数。
![](https://pic4.zhimg.com/v2-a5f235ea0f4d91cfae3ffebc4dd09253_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='704'></svg>)
不断重复这个过程，直到**每个座位的波函数都坍缩了**或者**遇到了矛盾无法安排**。

举个例子，如果遇到无法将剩余任何人安排到这个空位的情况，那这就是一种矛盾状态，最好的解决方法就是：重新开始。

再尝试几次，能按规则安排下每个人就算大功告成了。

## **位图（Bitmap）同理**

婚礼排座位如此，波函数生成无限城市，将几十种不同的建筑元素拼接组合也是如此。

这个过程非常相似，只是改变了规则。台阶可以直接和平地连接，但无法和屋顶元素相连，波函数不断坍缩直到矛盾出现或全部坍缩。



![](https://pic3.zhimg.com/v2-2aaebc8ce878aed3f2d60fc84fb5b1da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='562'></svg>)



两种情况也有不同，婚礼座次问题的规则是提前写好的，但无限城市的规则是用示例图像形式呈现的：




算法自己剖析示例，分析模式，自己推断出像素或者图块的规则如何。

那无限城市到底是如何将建筑元素拼接到一起的？我们先考虑一种简单的特殊情况，一种称为**更简单的平铺模型**（Even Simpler Tiled Model）的方法。

在这个模型中，可以将每种元素用像素形式呈现，并赋予一种单独的颜色。举个例子，下图陆地、海岸、海洋和山川四种地形图块都用了不同的颜色来表示。
![](https://pic3.zhimg.com/v2-12779a866a187f74e0a1c5cbcd497e86_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1177'></svg>)
更简单的平铺模型的规则指定了哪些图块可以彼此相邻放置，也规定了放置方向。

每个规则都对应一个坐标，内含两个相邻的图块和一个方向，比如（海洋，海岸，左边）代表一个海洋图块可以放置在一个海岸图块的左边。
![](https://pic4.zhimg.com/v2-f7b29864ec0a4c68cd97a92b7e1c372b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='759'></svg>)
除了明确规则，还需要明确每种图块出现的频率，在生成时这些频率将作为**权重**，决定最终生成的画风。

前面看到的无限城市，生成过程也和这个“比简单还简单的平铺模型”类似，只是涉及的元素、规则更多。

## **坍缩**

规则权重设置好，波函数坍缩可以开始了。
![](https://pic3.zhimg.com/v2-608d015d715e964073b0272a2b0262d2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='755'></svg>)
在无限城市问题中，最先确定的建筑元素是随机的，但算法会按照熵值最小排列方法不断扩充，也就是按系统的混乱程度最小排列。


一般来说，一个低熵的图块集合中，波函数坍缩时能选择的图块类型相对较少，最终图块遇到矛盾的可能性就会小一些了。
![](https://pic3.zhimg.com/v2-28c777c06d0903e3e01715232da7b7fa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='732'></svg>)
遵循单个图块的权重的同时，还要考虑整张图的全局权重。

比如，一个图块周围，**台阶**和**平地**图块出现的概率各占0.5时，在全局权重的指导下，不会出现一块台阶一块平地的尴尬情境，而可能直接用10块平地拼接成“大平地”。



![](https://pic4.zhimg.com/v2-244a56d9e0db3b3d468d426476584df3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='470'></svg>)



此时，波函数坍缩持续进行中。目之所及，无限城市中的地平线在不断快速向远处延伸。


所有输出的波函数，**要么**完全坍缩，支撑不断建起的亭台楼阁，**要么**出现了矛盾情况返回到某个节点重新不断生成。

最终，我们创造出一个虚拟的无尽头世界。

## **玩法还可以很花**

想用波函数坍缩 (WFC) 来创造世界的，不止玛丽安。



![](https://pic3.zhimg.com/v2-e07e5efce993b06200c9cb132e00fd56_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='200'></svg>)



2016年，两位美国少年发表了一个都市场景的滑板游戏，叫**Proc Skater 2016**。

他们说，这是世界上**第一个**用WFC来生成关卡的游戏。



![](https://pic4.zhimg.com/v2-84c57ff5cbf219ae60f6be238065b2bf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='794' height='483'></svg>)



只不过，这个游戏里的空间，并非实时生成，也不是没有尽头。

在那之前，少年们还用同样的方法，建起过一座属于自己的城，有限的城。



![](https://pic1.zhimg.com/v2-1d94edb257a5a83350c5c189d6b57b10_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='697' height='333'></svg>)



当然，除了大兴土木之外，WFC还有其他功能。

比如，有人就用它来写**十四行诗**：



![](https://pic1.zhimg.com/v2-d6d797d92bd10ea03d776e617b9e7dd4_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='588' height='320'></svg>)



> *爱丽丝还不知道自己的路该往哪走，就已经不在这里了。*
*(她来到了一个陌生的地方。)*
*那里有座厅堂，周围排满了门。*
*每一扇门都在说着一些友好的话。*
*(仿佛在请她进去。)*

后面几句有点恐怖：

> *Down down down down.*
*Let me see that would be of very like a candle.*
*I wish you were down here with me.*

大概，走进一扇门，便是不停地降落，降落。停下之后，周围已不是人间。

底下是清冷幽暗的世界，如果有人陪就好了。

**句子再不通顺**，也不妨碍人类的想象。反而越不通顺，越有**梦境**的错觉。
![](https://pic2.zhimg.com/v2-a5749b549de878afc546a93d2282765d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='497' height='225'></svg>)△ 《咒怨》试玩，来自山下智博
说不定有一天，恐怖游戏就不再需要循环场景来告诉你：逃不出去的。

只要有无限生成的世界，和鬼魅的情节或者台词搭配食用，就够了吧？

无限城市Demo试玩：
[https://marian42.itch.io/wfc](https://link.zhihu.com/?target=https%3A//marian42.itch.io/wfc)

无限城市开源代码：
[https://github.com/marian42/wavefunctioncollapse](https://link.zhihu.com/?target=https%3A//github.com/marian42/wavefunctioncollapse)

波函数坍缩原理：
[https://github.com/mxgmn/WaveFunctionCollapse](https://link.zhihu.com/?target=https%3A//github.com/mxgmn/WaveFunctionCollapse)

波函数坍缩论文：
[https://adamsmith.as/papers/wfc_is_constraint_solving_in_the_wild.pdf](https://link.zhihu.com/?target=https%3A//adamsmith.as/papers/wfc_is_constraint_solving_in_the_wild.pdf)

波函数坍缩科普文：
[https://robertheaton.com/2018/12/17/wavefunction-collapse-algorithm/](https://link.zhihu.com/?target=https%3A//robertheaton.com/2018/12/17/wavefunction-collapse-algorithm/)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[a​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


