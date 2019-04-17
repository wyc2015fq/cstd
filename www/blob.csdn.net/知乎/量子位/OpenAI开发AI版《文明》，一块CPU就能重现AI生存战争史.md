# OpenAI开发AI版《文明》，一块CPU就能重现AI生存战争史 - 知乎
# 



> 郭一璞 发自 凹非寺
量子位 报道 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-956542bebe2865db89912fb99c990570_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
如果把AI放在资源有限的世界里，他们也会像人类一样竞争。

OpenAI就做了这样一款名叫**Neural MMO**的AI训练游戏，让AI在一块限定的土地上求生，我们发现，AI也一样可以演化出悲壮的生存史诗。

这是一场**12800**个AI之间的生存战争，就像AI版的《文明》系列一样，在地图上争夺资源、扩大地盘、发动战争。

而且，你可以克隆到自己的电脑上，只需**一块桌面CPU**即可调动100个并发服务器，重现这一AI战争史。

## **AI世界的野蛮规则**
![](https://pic3.zhimg.com/v2-59570dec826f9e79c715052a177bd7f2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
当每个AI降生时，会发现自己有三种属性：

血条、蓝条和经验值

**食物**、**水**和**健康**
![](https://pic1.zhimg.com/v2-2099ceb84075c1ae1a4ede049987fbcc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='286' height='232'></svg>)



从上到下，黄色的是食物，蓝色的是水，绿色的是健康，也可以理解为我们平常玩游戏的血条。

每个AI出生时都分别有32点食物和水，10滴血；每个时间单位会分别消耗一点食物和水，当食物或者水降为0时，每个时间单位会消耗一滴血，如果血条降为0，这时AI就会死掉。

而AI天生被深度强化学习的规则赋予了求生欲，要保证自己的食物和水分供应，让自己活下来。

**AI的拓荒之旅**

为了活下来，AI开始认识这个世界，并从世界中获取食物和水。

世界是由棋盘状的格子构成的，一共有六种格子：
![](https://pic3.zhimg.com/v2-12250335abb43b22395935ed203e95aa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='501'></svg>)
> 石头：不可通行的坚固壁垒；
岩浆：别碰，会死；
水：AI不会游泳，无法通过，但是可以在附近打水喝，如果AI在紧挨着水的格子移动，会获得5点水；
草地：可以通行；
森林：不仅可以通行，还可以采集食物，当AI通过森林时，会获得5点食物，而后森林会成为荒芜的灌木丛；
灌木丛：可以通行，虽然灌木丛荒芜没有食物，但每秒钟都有2.5%的可能性重新生长为森林。

每个AI的视野是周围方圆15×15范围内的格子，AI想要谋求生存的话，就要在附近寻找森林和水源。

**AI的生存战争**

在上帝面前，人类不得不艰苦求生；

而在OpenAI面前，AI们也不得不努力奋斗。

如果只是在自然环境中觅食饮水，那么AI大可无忧无虑的生活。然而，天不随AI愿，这个自然环境中的AI太多，竞争非常激烈。

为了争夺有限的食物和水，AI们同室操戈，拉开了一场关乎生死存亡的战争序幕。
![](https://pic1.zhimg.com/v2-877633d6bbff41b36d3744cc12467044_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='558'></svg>)
战场上，AI们有三种技能：

> **近战（Melee）**：在1格的距离内给对方造成10点伤害；
**范围攻击（Range）**：在2格的距离内给对方造成2点伤害；
**魔法攻击（Mage）**：在3格的距离内给对方造成1点伤害并把对方冻在原地无法移动持续两个时间段。

每点伤害，意味着从对方手里夺取一点食物和一点水，1格距离意味着以AI当前坐标为中心的3×3网格内，而每个新出生的AI都有15个时间段的新手保护期。

从战争规则来看，近战固然伤害高，但也可能被对方以同样的方式反击，毕竟这个游戏里没有护甲机制；

范围攻击则相对保守，怂一点以防被打死；

魔法攻击就很有套路了，和平主义的AI可以用魔法来正当防卫，把对手冻住之后跑路，而主战派AI就可以先把对方冻住然后再开大。
![](https://pic2.zhimg.com/v2-d3dff8038cae91187ab965aafb701f19_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='428' height='247'></svg>)
大概是秉承着“猥琐发育，别浪”的原则，AI们似乎更喜欢使用远距离的范围攻击或者魔法攻击。
![](https://pic2.zhimg.com/v2-154ba665c209e00d8750035476efcac5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='364'></svg>)
远距离攻击虽然伤害低，但是打着打着，就可以在自身毫发无损的情况下消灭对方，取得一场战斗的胜利。
![](https://pic3.zhimg.com/v2-a2781b2a7ec208c2e78ae78e47d3290a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='405'></svg>)
## **文明の终极奥义**

许多只AI聚在一起，便形成了部落族群，官方的训练结果，透露着人类文明演进的奥秘。
![](https://pic2.zhimg.com/v2-9024433615a86ae5761f9a7001e528a5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='463'></svg>)
在人口数量、竞争激烈程度不同的情况下，不同族群的生存世界也有所不同。

**竞争激烈导致背井离乡**
![](https://pic2.zhimg.com/v2-8a6a333fb6ceedac09d4805ffbf2192d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='574'></svg>)
当出生地的竞争过于激烈时，一些AI就不得不进行迁徙，勇敢的探索无人区，寻找新的生存资源，避免和其他AI争夺资源而引发战争。

就像人类的演变，没有资源的部落，只能二选一：要么靠战争争夺资源，要么背井离乡，去寻找新的家园。

**民族国家的形成**

而当AI被划分成不同的族群后，不同的族群有了不同的迁徙方案。
![](https://pic3.zhimg.com/v2-cecd927c7b8ad9591826e20e56ab4e1e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='350'></svg>)
相比单一族群，在有8个族群的情况下，每个族群各选择了一种不同的迁徙方向，有的沿着山麓前进，有的顺流而下，逐渐形成了自己的疆域。

这看起来有些类似人类社会中“民族国家”的概念，同一族群的人，为了谋求更广阔的生存空间，不得不进行迁徙，扩大地盘，形成了有地理区隔的单一民族聚集地。

那么，在你的电脑上，AI们又会演化出怎样的文明呢？

## **传送门**

OpenAI官方博客

[https://blog.openai.com/neural-mmo/](https://link.zhihu.com/?target=https%3A//blog.openai.com/neural-mmo/)

论文

**Neural MMO: A Massively Multiagent Game Environment for Training and Evaluating Intelligent Agents**

作者：Joseph Suarez, Yilun Du, Phillip Isola, Igor Mordatch

[https://s3-us-west-2.amazonaws.com/openai-assets/neural-mmo/neural-mmo-arxiv.pdf](https://link.zhihu.com/?target=https%3A//s3-us-west-2.amazonaws.com/openai-assets/neural-mmo/neural-mmo-arxiv.pdf)

游戏环境

[https://github.com/openai/neural-mmo](https://link.zhihu.com/?target=https%3A//github.com/openai/neural-mmo)

3D浏览器客户端

[https://github.com/jsuarez5341/neural-mmo-client](https://link.zhihu.com/?target=https%3A//github.com/jsuarez5341/neural-mmo-client)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


