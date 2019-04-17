# 不好，两群AI打起来了！“幕后主使”是上海交大~ - 知乎
# 



> 夏乙 若朴 发自 凹非寺
量子位 出品 | 公众号 QbitAI

你们这些科学家在干什么！怎么教会AI打群架啦！
![](https://pic1.zhimg.com/v2-feb306efa2c7468fb58d1adc10f43dd8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
成百上千的AI agent，分成两支AI大军对战起来。从动图中还可以看出来，它们的战斗方式并不是简单的平A，包围、游击等等高级战术在这三场AI战役中悉数登场。
![](https://pic4.zhimg.com/v2-a5aa0e780879c2c31669d35d012acb93_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)![](https://pic1.zhimg.com/v2-94747ffa3c049ada9cbb14195f95a468_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
战役发生的地点，是上海交大中英联合AI研究小组Geek.ai最新发布的**强化学习平台：MAgent**。

强化学习平台不少，MAgent是一个异类。OpenAI Gym、微软Malmo等等前辈几经改进，却都只能训练一个或者几个智能体，就算它们强调“multi-agent”，最多也不过支持几十个而已。

而在MAgent平台上，可以训练比更多还更多的agent，支持的量级可达到从**数百到数百万**。开发团队称之为“**many-agent**”。

量子位暂时按捺住把它翻译成“智能体大军”的冲动，在本文中就叫“群智能体”吧。

把这么多agent放在一起，要研究什么呢？我们先来看一段视频：
![](https://pic4.zhimg.com/80/v2-4d8b1c5f12a480a5ac362417702f5677_b.jpg)https://www.zhihu.com/video/921737992870195200
**△** 搭配BGM看小方块跑来跑去，莫名的燃~

Geek.ai团队说，把成百上万的agent聚集在一个环境中，是为了研究**Artificial Collective Intelligence（ACI）**，也就是AI如何群策群力，创造集体智慧。ACI的研究虽然还很初步，但已经在股票交易、策略博弈、城市交通优化等等领域有所应用。

## AI的集体智慧

在上面的视频里，成群的AI聚在一起形成了一个小社会，出现交流、竞争、合作等社会现象，里面的agent个体也会有种种社会行为。

视频就展示了基于MAgent设计的三个群智能体环境：追逐(Pursuit)、采集(Gathering)和战斗(Battle)，大致相当于人类老祖宗们常做的三件事：一起打猎、出去摘果子、和另一拨老祖宗打架。
![](https://pic3.zhimg.com/v2-f87cb4cf1cd1b3577e21b1f56c306066_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='179'></svg>)
我们可以来看看agent在这三种环境中的行为：

**追逐**



![](https://pic1.zhimg.com/v2-8d1327d0a10ca195c258000b76eec2fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='408'></svg>)
在追逐环境中，红色智能体代表捕食者、蓝色表示猎物、灰色的是墙。捕食者包围、锁定猎物，接下来就能在攻击时就能得到奖励，而猎物被攻击会受到惩罚。



![](https://pic4.zhimg.com/v2-af745745386dc184cd4b56e5b7aec5f3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='421'></svg>)
我们可以看到局部合作行为的出现。经过训练，四个捕食者会合作包围一个猎物，还会在包围猎物时利用墙壁作为辅助。

**采集**

在采集环境中，红色方块是食物，蓝色的是agent，agent可以互相攻击，吃到（采集到）食物就会获得奖励。有两个选择摆在他们面前：是直接去吃掉食物，还是杀死竞争者独占食物。

于是，竞争出现了。
![](https://pic4.zhimg.com/v2-07d4d701e24654dd8e1cef2eaf5b37ef_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
经过训练，agent在一开始时会选择直接冲过去吃掉食物，如果两个agent相遇，就会尝试杀死对方、独占食物、获得奖励。食物越少，agent越好斗。

**战斗**

战斗环境，就是我们在文章最开头看到的AI打群架啦。
![](https://pic2.zhimg.com/v2-97dcc84714c0f9f1b4bf68814056c019_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
这其中既有合作又有竞争，红色和蓝色agent分别组成两支军队，杀死对手就能获得奖励，被杀死就会被惩罚。也就是说，agent在这个环境中的任务，是和队友合作去消灭敌军。

这些agent通过简单的自我对局训练，学会了全局和局部策略，包括包围、游击等等。

## MAgent详情

在MAgent平台上，每个agent都是一个小方块，带有详细的局部信息，有些还带有全局信息。agent可以移动、旋转、攻击。

它们背后有一个C++引擎，支持多样化的agent，通过简单设置状态空间、行为空间和agent的属性，可以创造出多样的环境。

为了让用户灵活地设计环境，MAgent还提供一种用来描述奖励、agent标志、事件的语言，还支持“与/或/非”逻辑运算。

另外，MAgent还提供一个简单的图形化渲染器，对环境和agent的当前状态进行交互式展示，用户可以拖动、缩放窗口，甚至可以操纵游戏中的agent。

更多详情，可以看看MAgent的论文，或者把代码拿来上手试一试。

**论文（将在AAAI-18 Demonstrations Program上展示）：**

MAgent: A Many-Agent Reinforcement Learning Platform for Artificial Collective Intelligence

Lianmin Zheng, Jiacheng Yang, Han Cai, Weinan Zhang, Jun Wang, Yong Yu

[http://cn.arxiv.org/abs/1712.00600](https://link.zhihu.com/?target=http%3A//cn.arxiv.org/abs/1712.00600)

**代码：**

[https://github.com/geek-ai/MAgent](https://link.zhihu.com/?target=https%3A//github.com/geek-ai/MAgent)

## 背后的Geek（们）

这篇论文的共同第一作者，是上海交大的**郑怜悯**（Lianmin Zheng）和**杨嘉成**（Jiacheng Yang）。

公开信息显示，今年陈天奇团队发布TVM时，郑怜悯的名字曾出现在致谢名单中。去年上海交大第六届智能车校内赛上，郑怜悯和同学组队获得信标导引组的一等奖。中学时期，郑怜悯还曾获得温州市“小科学家”称号。

郑怜悯和杨嘉成，都是上海交大致远学院（基础学科拔尖学生培养试验计划）2015级计算机科学班的学生。
![](https://pic3.zhimg.com/v2-00bb4964b64e3257df86d2945187f952_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='193'></svg>)
这篇论文的其他作者还包括，Han Cai、Weinan Zhang（张伟楠）、Jun Wang、Yong Yu（俞勇）。

其中，**张伟楠**是上海交通大学，计算机科学与工程系助理教授。他的研究兴趣包括机器学习和大数据挖掘，尤其是深度学习和（多智能体）强化学习的架构、机制、训练方法以及在真实场景中的应用。

张伟楠2016年从伦敦大学学院获得博士学位，2011年在上海交大ACM班获得学士学位。他曾在MediaGamma、微软研究院、Google和DERI实习。

**Jun Wang**是伦敦大学学院的信息系统和数据科学教授。他同时也是一家AI创业公司MediaGamma的联合创始人兼首席科学家，这家公司专注于实时决策。

Jun Wang博士毕业于荷兰代尔夫特理工大学，硕士毕业于新加坡国立大学，学士毕业于南京的东南大学。

**俞勇**是上海交通大学计算机科学与工程系教授，致远学院计算机科学方向项目主任，也被称为上海交大ACM总教头。

**Han Cai**的信息量子位没有查询到。
![](https://pic1.zhimg.com/v2-d3e9405e6263c77e22ef2860b17fc908_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='415'></svg>)
另外补充一个信息，Jun Wang和张伟楠等人，成立了一个名为Geek.AI的团队，这是一个中英联合AI研究小组。

这个研究小组专注于机器学习的高级问题和方法，包括监督/无监督学习（涵盖大多数预测应用，如推荐系统、图像识别和网页排名等），强化学习（涵盖所有决策应用，例如下围棋、自动驾驶、竞拍和智能选股等）。

除了他们两位，小组成员还包括**Dell Zhang**（张德），伦敦大学伯贝克学院，计算机科学高级讲师。他主要从事机器学习、数据挖掘、信息抽取、NLP、社会网络分析等领域的研究。

## OMT

还有一事。

郑怜悯在GitHub上公布了他参与的几个项目，其中有一个Python写成的网页版“狼人杀”，中午没事儿可以试一下~

地址在此：[http://wolf-killer.herokuapp.com/](https://link.zhihu.com/?target=http%3A//wolf-killer.herokuapp.com/)

初次登录，需要你填写用户名和密码，如果想下次继续登录，千万记好密码。登录之后就能创建游戏，或者加入房间了。

大概就是下面这个样子。
![](https://pic4.zhimg.com/v2-7cc647c052ec5ca78cb700df2581f383_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='1280'></svg>)
—完—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者


