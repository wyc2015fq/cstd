# 如何丝滑地入门神经网络？写个AI赛车游戏，只训练4代就能安全驾驶 - 知乎
# 



> 车栗子 发自 凹非寺 
量子位 出品 | 公众号 QbitAI




要了解机器学习、了解神经网络，一种非常有趣的方式，就是找个**小游戏**，自己实现一下。

游戏里的每一位AI玩家 (智能体) ，都是一个小小的神经网络。
![](https://pic4.zhimg.com/v2-086456e5ff665fdc7aa934df50e981eb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='381'></svg>)△ 训练前
一开始，它们什么都不懂，刚开局就GG。但有了**进化算法** (Evolution) ，AI可以在一代一代更迭中，掌握强大的游戏技能。
![](https://pic2.zhimg.com/v2-42a7036ab638bc095952b7ce5f43a641_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='332'></svg>)△ 成效显著
比如，在这个赛车游戏里，萌新车手跑出去不久便撞上了马路牙子。不过，用不了几代进化，就有AI车手可以毫发无损地跑完一周。

## **进化的赛车手**

AI赛车手如何快速炼成？

**首先，要有一个庞大的车队。**

赛车AI的开发者，是一位叫做Johan Eliasson的程序猿。他的训练计划里，有**650辆车**同时训练。

**第二，AI开车需要一些空间信息。**
![](https://pic4.zhimg.com/v2-7159767a4133ff822558fbd8b6bcd1b7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='453'></svg>)
这5条**黄线**代表5个距离，指示离马路牙子还有多远；

除此之外，AI还需要知道自己当前的**速率**，以及**方向**。

这样算来，神经网络一共会接受**7个输入**。

**第三，就是让神经网络不断进化。**

输入七个值，处理一下，**输出两个值**：

> 一个代表方向盘，按照数值大小，分为**左转**、**右转**、**方向不变**三种操作；
一个代表油门/刹车，按照数值大小，分成**加速**、**减速**、**速率不变**三种操作。

最开始，谁不知道游戏规则，输出值很随机。
![](https://pic3.zhimg.com/v2-8ca4c894cb0020d14fbcb393687bc496_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='453'></svg>)△ 第一代，死得快
650辆车冲出去没多久，路边就出现了大批车辆的尸体。

但重要的是，依然有硕果仅存的汽车，不止通过了第一次的**右转**考验，也机智地发现下一处弯道应该**猛烈左转**。



![](https://pic1.zhimg.com/v2-a93a1169536726593e188138de7370d0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='815'></svg>)
虽然，它并没有成功掉头，但依然是**下一代的希望**：进化算法就是要在每一代里，选出最优秀的一只或者几只智能体，繁育出色的后代。

(这里，程序猿没有找到一种很好的方法，让系统自动选择出优质的个体，于是就手动马克了，反正也没有训练很多代。)

所以，第二代的650辆车，全是这一只智能体的子嗣，各自有些轻微的**变异**：神经网络的权重发生小小的变化。

变异是为了保持车队的多样性，期待从中生产出更加优秀的个体，繁育下一代。
![](https://pic2.zhimg.com/v2-7d79f763aee2d2fc9d895a1ab925b669_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='453'></svg>)△ 第二代，马克三只
由于继承了优良传统，第二代汽车大部分都完成了第一次右转，还有一小部分掉头成功。

这一次，把冲到最前面的**3辆车**马克一下，它们差一点就能达成两次连续掉头的S形操作。

既然，第三代的父母有3位，那么繁育过程中除了变异之外，还涉及**杂交**：就是把不同的神经网络揉到一起：
![](https://pic3.zhimg.com/v2-7d721acb45c9ccf03673c891f5b82342_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='462' height='200'></svg>)
第三代的表现更加精进，有的智能体完成了多次连续转弯，胜利在望。

**第四代**，便有智能体**跑完一周**，值得纪念：
![](https://pic3.zhimg.com/v2-9b64ba2b044581c0682a35bc70e42daa_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='453'></svg>)



程序猿说：这效果比想象的好多了，之前我还有点怀疑，现在只能说机器学习好厉害。

第四代车队中的佼佼者，已经不太撞到路肩，只是**速度还很慢**，就像一大波僵尸。

车队的主人表示，后面的主要任务就是**训练速度**。毕竟，这是赛车。

到了第十代，速度已经有了明显的提升，不过依然不算快：
![](https://pic4.zhimg.com/v2-50bfe3b95f70d8f6c4f872313bf47f1f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='453'></svg>)△ 第十代
不知训练了多少代，现在把每一代的优质选手放在一起跑。你看，好快：
![](https://pic1.zhimg.com/v2-c83fac8beb0b8b3c0e5feee90baa9094_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='362'></svg>)△ 这才是赛车

成果斐然之余，程序猿还请大家注意一个重点：

AI不知道自己在开车，不知道有跑道。它只接收七个输入的数值，不了解它们代表什么意思，也不知道自己输出的每一个决策，产生了怎样的效果。

就算这样，它还是学会了怎样在游戏世界里更好地生存。

## **不想开车？还有其他游戏**

虽然，赛车游戏的视频，没有讲到程序猿用了怎样简单的神经网络。不过，可以通过另一个简单的游戏，来感受一下：

## **Flappy Bird**



![](https://pic4.zhimg.com/v2-74b3917c010d5e2ccc9b16241ead149f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
这个HTML 5实现， 也是进化算法和神经网络共同的结晶。而且开源了。

它的作者ssusnic说，神经网络部分，用的是突触神经网络 (**Synaptic Neural Network**) 库。

没有650辆车那么多，种群里只有**10只小鸟**。每一只，都是一个三层的神经网络：

> 输入层：两个神经元
隐藏层：六个神经元
输出层：一个神经元
![](https://pic3.zhimg.com/v2-0e9cd8912de65321154b3fc3ca067f5e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='612'></svg>)



输入两个值，代表小鸟当前距离下一个障碍物的相对位置，分成水平距离 (x) 和竖直距离 (y) 。

经过隐藏层的处理，输出一个0-1之间的数值，决定下一步要不要扇翅膀：

如果大于0.5，就扇翅膀，反之就不操作。
![](https://pic2.zhimg.com/v2-1ce589ca9a4d07272858723f97cf2871_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)△ 第一代
第一代的小鸟，都是**随机神经网络** (Random Neural Networks) ，集体见光死。

全部阵亡之后，要选出四只最优质的小鸟去繁殖。问题来了，肉眼看去相差无几，要选哪几只？

并不需要手动选择，而是用一个**适应度函数** (Fitness Function) ：

> 适应度 = 小鸟存活的最远距离 - 小鸟到下一个障碍物的距离
![](https://pic1.zhimg.com/v2-fb3dbf25fd5722f150ceabf5e02d90bc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='605'></svg>)△ 这里，讨论水平距离
适应度分数排名前四的小鸟，按下面的规则繁殖10个后代：

> · 前两名，杂交出一个后代
· 4只中随机选两只，杂交出三个后代
· 4只中随机选两只，分别直接复制，生成两个 (和上代一样的) 后代
· 给每个后代加入一些变异

就这样，来到第11代，已经有小鸟飞出很远且毫发无伤：
![](https://pic4.zhimg.com/v2-aa62b94842def41127a0fb8e87223aa7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
第23代，完全碾压，可以换个游戏了：
![](https://pic3.zhimg.com/v2-0542a1444013386627d39e89e2c1c212_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
## **吃豆豆**

当然，不是所有游戏都像小鸟扇翅膀这样简单。

**吃豆人** (Pacman) 就是个更复杂的栗子。比如，前面要躲避的障碍物都是静止的，Pacman的敌人是移动的，穷追不舍。
![](https://pic1.zhimg.com/v2-8aaef3ab89d860d184c8f2da63316a84_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='350'></svg>)
重点是，为了抓到Pacman，四个敌人还有各自不同的运动规则：

> **红色鬼**，直接瞄准Pacman的位置进发；
**粉色鬼**，瞄准Pacman前方的第四格；
**蓝色鬼**，利用Pacman和红色鬼的位置来搞伏击；
**橙色鬼**，原本和红色鬼一样，但当它和Pacman的距离近到8格以内，就会朝一个角落退缩，那是它出发的地方。
![](https://pic2.zhimg.com/v2-ecfb9ed8ae123049b5bf908006acd991_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='480'></svg>)
并且，Pacman吃到**无敌大豆豆**的时候，敌人还会从追击模式转成逃跑模式。

复杂的游戏，自然也需要更加精密的算法，来帮智能体进化。

名叫Code Bullet的程序猿，就用了一种叫做**NEAT**的神经进化算法。

这种方法很特别，神经网络在迭代过程中，不止权重会变化，**网络结构也会变复杂**。
![](https://pic3.zhimg.com/v2-e9013be30b5cb9e3f695d71b312c1492_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='599' height='320'></svg>)



而网络结构越复杂，就可以支持越复杂的行为，让Pacman在险恶的世界里存活。

虽有强大的算法，训练还是要**由浅入深**。

先不加无敌大豆豆，也不加敌人，给AI一个简单的世界。

这样它就能吃光所有豆豆？不存在的。
![](https://pic2.zhimg.com/v2-e4156ad4c687ebd67757e4912e89a11d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)



第一代最优秀的智能体，也只会右转不会左转，还把自己**困在一处**，永远走不出去。

第二代，有的选手已经学会了左转，但依然会困住。

……

**第十八代**，眼看快要吃光豆豆，AI还是困死了自己。



![](https://pic4.zhimg.com/v2-a13415d7d413e5cec4bb5992115dc5e7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
在平静中训练**20代**之后，把敌人放出来，AI又要从手忙脚乱开始重新适应：
![](https://pic3.zhimg.com/v2-8f11a37cdcf7ff85ce56341f4e6204f6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
不过好在，智能体学以很快便学会了躲避敌人：
![](https://pic1.zhimg.com/v2-768ab61b3ca90ceee7f221446b02caf4_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
又过了40代，是时候加入**无敌大豆豆**了。

这一层防护令AI如有神助，就像在经历了40代卧薪尝胆之后，开启了复仇模式，疯狂追击。

终于，有智能体在**第七十三代**通关成功：
![](https://pic3.zhimg.com/v2-66d0010e30e744fb247339a1d8aa14fe_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='450'></svg>)
视频的画外音里，程序猿的声音都激昂了起来，这场战役的胜利他期待已久。

除了发表获奖感言，谢过各路亲友的支持，他也没忘在GitHub上分享代码 (传送门见文底) 。

## **动手吧，少年**

同学，你也去搭个神经网络打游戏吧，呆萌的游戏也能打出热血的气质。

以后，神经网络可能就是你女朋友了。

(友情提示：虽然赛车游戏没有开源代码，但OpenAI Gym里面有赛车场，也可以去试试。)

赛车游戏视频传送门：
[https://www.youtube.com/watch?v=wL7tSgUpy8w](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Fv%3DwL7tSgUpy8w)

OpenAI Gym赛车场：
[Gym: A toolkit for developing and comparing reinforcement learning algorithms](https://link.zhihu.com/?target=https%3A//gym.openai.com/envs/CarRacing-v0/)

Flappy Bird代码传送门：
[ssusnic/Machine-Learning-Flappy-Bird](https://link.zhihu.com/?target=https%3A//github.com/ssusnic/Machine-Learning-Flappy-Bird)

Pacman代码传送门：
[Code-Bullet/PacNeat](https://link.zhihu.com/?target=https%3A//github.com/Code-Bullet/PacNeat) (NEAT算法)
[Code-Bullet/PacmanGame](https://link.zhihu.com/?target=https%3A//github.com/Code-Bullet/PacmanGame) (搭建游戏环境)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


