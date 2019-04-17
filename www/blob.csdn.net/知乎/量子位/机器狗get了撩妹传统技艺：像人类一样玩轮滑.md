# 机器狗get了撩妹传统技艺：像人类一样玩轮滑 - 知乎
# 



> 安妮 夏乙 编译整理
量子位 出品 | 公众号 QbitAI

波士顿动力那些踹不倒的机器狗、脚踩风火轮的机器人可谓是颠倒众生，迷死一片网友。

今天，瑞士传来最新消息，那里的科学家们在教机器狗学轮滑。
![](https://pic4.zhimg.com/v2-a811a465f776480f48e94ba21ac94dff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)就是这项运动
来我们一起看下。

## **脚底抹油的轮滑机器狗**

首先机器狗身体微倾，然后右脚向后滑，推动整个身体前进，接着身体再向另一侧倾斜，左脚向后滑。持续反复，向前滑行。
![](https://pic2.zhimg.com/v2-68710e5aa4788f12e4e834fee26ac755_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
这只机器狗，简直是按照人类的动作在滑行嘛！

相比之下，波士顿动力那只Handle所用的方式，虽说性能更强更炫酷，但远没这么“仿生”，也没能给机器狗保留四条分开的腿，在不平整地面上的机动性就要打些折扣。
![](https://pic3.zhimg.com/v2-5594a6c3c20dc37a6abef122abebb5d2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
轮滑机器狗从正面看是这样的。即便穿过不同材质的表面，也没有任何影响。
![](https://pic2.zhimg.com/v2-3ed4a202903998dac17b6403a312b73d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
机器狗还能注意到下坡，并直接享受这个过程，停止蹬腿滑行。在下坡的过程中，感觉是整个狗都要飞翔起来~
![](https://pic1.zhimg.com/v2-106158fb49ff27f22a8f04c410e89dac_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
好，滑得好。但怎么转向呢？目前这个机器狗的前腿并不能支持常见的滑动转向，团队给出的解决方案也很有意思：原地踏步转向……

具体来说，是这样的：
![](https://pic2.zhimg.com/v2-35759970c0217804989cae19ee29d6e5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
这个会轮滑的机器狗，什么来历？其实这个机器狗就是苏黎世联邦理工学院ETH Zurich团队此前已经开发出来的ANYmal，只不过这次get了轮滑新技能。

作为一个四足机器人，ANYmal能多种不同的地形中自主移动和执行任务，包括工业场景下室内和室外的检查，自然或者危险地区的搜索和救援任务，甚至在娱乐节目中充当表演嘉宾等等。

ANYmal提供了IP67防护等级（也就是防尘防水，现在手机用户也应该常听这个概念了）；可由单人安全操作；所有关节都能360°旋转；移动速度可达1米/秒；自身电源能支持工作2-4小时；依靠激光传感器和摄像头导航。

总之，可以跑步、舞蹈、攀爬、负重携带、旋转、跳跃（但不能闭着眼）。
![](https://pic1.zhimg.com/v2-103a91beb529dafc0be9904767c3b944_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='845' height='684'></svg>)和小朋友一起玩
ANYmal的核心技术包括两个个方面。
![](https://pic2.zhimg.com/v2-1f45f8c0a54fdcb91d38e9cf393c40bd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='374'></svg>)
从**右边**开始说，那是一个高度集成的模块化机器人关节执行器ANYdrive，所有的电机、齿轮、钛弹簧、各种电子元件都集成在一个IP67级防护的密闭单元中。

使用这个关节，无需额外的轴承、编码器或者电力电子装置就能构建任何一种运动力学结构，例如机器人的手臂、机器人的腿等。
![](https://pic4.zhimg.com/v2-2d3404e817d81251eaec0261373ffc93_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)稳健性表现（锤子砸）
ANYdrive的峰值功率和标称功率分别是720W和240W。更多参数可以看下面这张图。
![](https://pic1.zhimg.com/v2-925c8ab691387d65d889c4612a6ec14c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1089'></svg>)
然后说**左边**，这是一个被称为机器人大脑的ANYbalance，一种平衡控制软件，即便在崎岖地形中也能从容应对，提高四足机器狗的稳定性和安全性。

ANYmal用了哪些软件？

官方列出了一部分，还提供了地址，量子位搬运如下：

**Elevation Mapping**
![](https://pic2.zhimg.com/v2-b549aa1b436f494eb9ba0d8717b5f8f5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='360'></svg>)
用于粗糙地形导航的高度图测绘ROS包，为机器狗提供高程图，以便控制姿态。地址：

[ethz-asl/elevation_mapping](https://link.zhihu.com/?target=https%3A//github.com/ethz-asl/elevation_mapping)

**Grid Map**

这是一个配备ROS接口的C++库，用于管理具有多个数据层的网格地图，可以存储高程、方差、颜色、摩擦系数、立足点质量、表面法线、可穿越性等数据。地址：

[ethz-asl/grid_map](https://link.zhihu.com/?target=https%3A//github.com/ethz-asl/grid_map)

**LSE库**

这个库为有腿系统提供可观测性约束扩展卡尔曼滤波器。地址：[https://bitbucket.org/ethz-asl-lr/c_lse](https://link.zhihu.com/?target=https%3A//bitbucket.org/ethz-asl-lr/c_lse)

**Free Gait**

自由步态是一个软件框架，用于有腿机器人的多功能控制。地址：

[leggedrobotics/free_gait](https://link.zhihu.com/?target=https%3A//github.com/leggedrobotics/free_gait)
![](https://pic2.zhimg.com/v2-3f715442e478494b166d3be669260405_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1030' height='687'></svg>)
**Kindr**

为机器人提供运动学和动力学的C++库，实现各种旋转的参数化。地址：

[ethz-asl/kindr](https://link.zhihu.com/?target=https%3A//github.com/ethz-asl/kindr)

**Kindr ROS**

ROS包装成Kindr。地址：

[ethz-asl/kindr_ros](https://link.zhihu.com/?target=https%3A//github.com/ethz-asl/kindr_ros)

**Traversability Estimation**

可穿越性估计，为可穿越的地形提供可遍历的映射。地址：

[ethz-asl/traversability_estimation](https://link.zhihu.com/?target=https%3A//github.com/ethz-asl/traversability_estimation)

## **不过……**

那个……其实ANYmal还没学会急刹车。

所以，当它飞一般的从高处冲下来之后，结局是这样的……
[undefined_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=http%3A//v.qq.com/x/page/h056430jdfw.html)
## **机器人界的“冬奥会”**

就算没有轮子，机器人也一样可以“滑”。比如说，滑雪。

前不久，韩国出现了不少会滑雪的机器人。我们看看一段来自“全球首届机器人滑雪比赛”的现场视频：
[undefined_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/h13337rqxod.html)
比赛现场的情况看起来有点“惨烈”。

不断有机器人以各种姿势摔倒在地，之后被人扶起。不断有机器人被“吊起”再次推向起点，有些机器人需要让主人帮它“穿鞋”……

不过，从视频中看来，这些机器人有一点要比瑞士的轮滑狗厉害：在滑动过程中，它们能够转弯，避开路上的障碍，不用停下来“原地踏步”。

## **比赛规则**

这场比赛由韩国国家商贸部和机器人产业发展研究所联合举办。比赛中，8个参赛机器人需要绕开障碍物，到达终点。
![](https://pic4.zhimg.com/v2-9db51ee62137c314fd174cb826e40913_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='472'></svg>)障碍物由红色、蓝色旗杆和红蓝布构成
比赛规则相当复杂——
- 参赛者必须是“人形”机器人，并且至少包含15个机械自由度。
- 不欢迎“侏儒”机器人参赛。如果测量机器人从脚底到肩膀的距离还没到50厘米，不好意思，还是让它回家先长长个吧。
- 机器人需要使用滑雪杖和滑雪板，并且姿势一定要标准。当机器人的肘部弯曲站立时，雪杖必须接触地面。
- 赛场滑雪坡道长80米、宽20米，每支竞赛团队有3分钟的时间避开红色和蓝色的障碍物完成比赛。
- 机器人每绕过一个障碍物，就得到一分。比分相同时用时最短的机器人获胜，如果时间也相同，那么……身材更高挑的机器人获胜！

值得注意的是，所有机器人都是全自动的。此外，还有一个专门的遥控比赛可选，但研究人员好像对那个并不感兴趣。

## **冠军Taekwon V**

经过一系列跌跌撞撞的角逐，最终，参赛宣言为“个子小但强壮”的Taekwon V赢得了比赛。

这个小个子Taekwon何许人也？我们调出这位参赛队员的资料看看。
![](https://pic2.zhimg.com/v2-f05c7a281fd3cf20385be27d33b2730d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='318' height='375'></svg>)
> **小型机器人：Taekwon V**

**身高**：75厘米
**体重**：12kg

**特征&技能**：
- 是一个刚性机器人
- 身体构成复杂，具有21个关节
- 对人体协调运动有独到的见解
- 能够像人一样走路
- 造价低为你省钱

Taekwon V来自仁川的机器人制造商Minirobot，在18秒内越过5扇障碍门，以4秒明显优势领先于来自韩国机器人与融合研究所（KIRO）的机器人SKIRO。

无视频无真相，请欣赏Taekwon V的英姿。
[undefined_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=http%3A//v.qq.com/x/page/p05649rqcbp.html)
从起点出发后，Taekwon V被障碍物绊倒了几次，但……即使被绊倒姿势也依然帅气。之后，Taekwon V仿佛开了挂，正滑、转向、倒滑，然后稳稳停在终点。

## **技术支持**

这些穿着人类滑雪服的人形机器人，究竟是怎么滑雪的？

这场比赛中的大部分机器人，都离不开视觉感知和运动动作控制技术的结合。
![](https://pic4.zhimg.com/v2-9e235240dfdee50880ac1e07f516c963_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='423' height='225'></svg>)
上面的动图展示了本次自动滑雪比赛冠军Taekwon V的视角，从中能看出这款机器人具备的视觉能力。它可以比较顺利地认出赛场上设置的障碍物。
![](https://pic3.zhimg.com/v2-39a64793327b1b68e9d2cee6133f6046_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='264'></svg>)
对于赛场上穿行的人类，它偶尔能够检测出来，大多数时候都视而不见。

据《韩国先驱报》介绍，这些机器人上装备了雷达和视觉传感器，用来确定它们自己的位置，识别周围那些红色蓝色旗杆的位置。

量子位在Taekwon V所属公司MiniRobot官网的各种产品中，没能找到这位身高75厘米体重12公斤的选手，似乎暂时无法对它的智力水平做更详细的介绍。

不过，我们可以看看这家公司的一款小型人工智能机器人
![](https://pic1.zhimg.com/v2-e734ab92302a98a595b122ff24f07bac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='492'></svg>)
这款小机器人没有雷达，但是配置了用于图像处理用的摄像头。官方介绍中说，它可以认出10种颜色，包括色块的面积和中心坐标。

从滑雪选手Taekwon V的表现来看，它很可能也是靠颜色来识别障碍物的。这种方法可以说简单易行，但对于临时出现在场上的人类等障碍物，就不太容易认出来了。

另外，Minirobot公司的Sam Kim对《韩国先驱报》透露，外部环境的微小变化都会对机器人的表现有影响。“它们不是人类，红色的深浅有变化，机器人有时就会认不出来。所以在比赛中，阳光的变化就可能导致它们找不到旗杆认不清路。”

**认出障碍物之后，下一步任务就是避开它们。**

运用这些传感器获取的信息，机器人内置的程序会指导它们做出正确的运动动作，比如说通过操纵腿部的动作左右转弯，躲过那些障碍物。

在顺坡向下滑的过程中，这些机器人面临的最大挑战是反应速度要快。它们需要快速处理接收到的视觉数据，并足够快速地调整自身动作，才能及时转弯。毕竟不是平地，滑着滑着像轮滑狗那样停下来原地踏步显然不太现实。

当然，减速还是可以的。腿部的动作还能帮这些机器人实现另一项轮滑狗缺失的技能：减速。

## **赛场“八公子”**

其实，其他七名参赛其实也挺帅的，满满都是荷尔蒙科技感的味道。
![](https://pic3.zhimg.com/v2-6cb2fb2d22dd1c0ec352a4d5c580f4da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='743' height='499'></svg>)参赛“八公子”
好像每个机器人都必须得穿一件小外套，这让滑雪场面看起来异常可爱。现在它们只需要一些润滑油恢复下身体又能活蹦乱跳了。

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


