# 过山车之星(Planet Coaster)的视角和编辑介绍 - 长歌行 - CSDN博客





2018年01月09日 21:48:48[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2511
个人分类：[游戏																[图形图像																[生活](https://blog.csdn.net/iuhsihsow/article/category/1065737)




### 前言

趁着冬促，30元入手了垂涎已久的过山车之星（Planet Coaster® ）。

它的广告词：“过山车公园模拟游戏的明日之光重磅来袭！建造您的过山车公园帝国，为人们带来难以置信的惊奇、欢乐和刺激 — 放飞您的想象力，与世界分享您的成就。”


大致归纳了下游戏的卖点：

**建造**（游乐设施，道路铺设，建筑构建，地形造型，生成湖泊，掘出洞穴）

**经营** （公园模拟游戏， 逼真的世界，随心建造并改造您的公园，给顾客带来惊奇和刺激）

**分享** （借助steam创意工坊，把其他玩家的创意添加到自己的公园，并修建你的设施，为它命名，将它与整个星球分享）

看宣传动画，觉得应该是和cities，skyline差不多的游戏。但是实际玩起来，还是有不同的趣味。感觉更需要编辑和创造能力。

本文在此简单介绍下游戏的视角控制和模型编辑的交互操作。希望对玩家有所帮助，或者是三维系统设计开发人员开拓思路。

## 视角

视角就是观察的角度。由摄像机与目标场景的相对位置和摄像机的视场角等参数决定。

配一张相关的图，说明下相对关系（配图为KML的lookAt）

![](https://img-blog.csdn.net/20180109222304921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在普通模式下，按住鼠标右键拖拽进行平移，鼠标滚轮控制缩放，按住鼠标中间，可以旋转视角。

但是，游戏不限于此。对于每一个活动的个体，我们都可以设置多种观察模式。

#### 电影化视角

对于过山车或者游客都可以选择电影化的视角。属于第三人称视角。游戏会根据对象的动作，位置和周围场景自动调整角度，以保证玩家能够获得最佳的观赏角度。主要特点是，目标基本在屏幕中心区域，相对角度会随便变化。

目标对象为过山车的电影化视角的例子：

镜头时而在过山车下，时而在侧边，时而切换为俯视。从全方位各个角度，更能全面的展现过山车的全局环境。

![](https://img-blog.csdn.net/20180109222652853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180109222832845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下图是游客的电影化视角，目标任务是屏幕中心的艾米公主表演者（坐标：棕色头发，蓝色裙子，牛肉饼警长广告牌的左方）。这种观察模式给人带来较强的全局感。

![](https://img-blog.csdn.net/20180109223135246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


#### 座位/第一人称视角

这是一种第一人称视角，摄像机就在人眼的位置。给来带来较强的沉浸感，VR的话，效果更佳。




下图为坐在第一排第一座的游客的视角，这时候车在快速向下俯冲，游客吓得尖叫并尝试用手捂住眼睛。;-)




![](https://img-blog.csdn.net/20180109223757227?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这是坐在第6座游客的视角，可以看到前排游客的一举一动，遮挡更小，视野也比较好，画面冲击感比第一排要弱一点（适合一般玩家）

![](https://img-blog.csdn.net/20180109224100946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


接下来是地面游客的第一人称视角。下图是附身到排队买冷饮的第二排游客身上。可以看到店员忙碌的身姿（当排队人很多的时候，店员真的是忙得手舞足蹈）和前面顾客掏钱收货等细节动作。代入感比较强。

![](https://img-blog.csdn.net/20180109224403619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 偏好/朝向游客化视角

这也是一种第三人称视角，目标和摄像机使用同一坐标系，相对位置保持不变（简单说，可以把目标和摄像机作为一个刚体对待）也是一种摄像机的追随模式。我们可以自由调整一个固定的角度来观察目标。




使用这种方式，更利于捕捉精彩镜头。下图是调整摄像机朝向第一节车厢。乘客在经历了几个弯道之后，来到了一个向上的弯道，离心力带来的恐惧感让他们惊吓得举起了双手。

![](https://img-blog.csdn.net/20180109225607162?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





用这个角度来看看排队的人们（虽然没有等待线，大家还都挺守规矩，队列第二个人和柜台保持了一定的距离）

![](https://img-blog.csdn.net/20180109225858956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 模型编辑

过山车之星的模型编辑还是非常复杂的，在此只做简单介绍。编辑分为简单模式和高级模式。简单模式实际上是高级模式的“表面移动”模式（就是贴“地”放置模式）之后会有介绍。系统提供的编辑工具非常全面，可以随心所欲的编辑。先贴一张创意工坊的图吧。

![](https://img-blog.csdn.net/20180110000248253?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下面正式介绍。编辑操作又分平移，旋转两种。坐标系分为世界坐标系和模型相对坐标系。




平移模式的辅助坐标轴

![](https://img-blog.csdn.net/20180109230450043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


旋转模式的辅助环

![](https://img-blog.csdn.net/20180109230958019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





模型相对坐标系（以平移模式为例，有坐标轴可以参考）

![](https://img-blog.csdn.net/20180109231119361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




世界坐标系（以平移模式为例，有坐标轴可以参考，也可以参考我修的路，在这个园区，我修的路都是东西或者南北走向的）

![](https://img-blog.csdn.net/20180109231220568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


"表面移动”模式（游戏就是这么翻译的，我也没看原版英文，我比较喜欢用draped mode这个词），因为可以贴合地形或者模型，所以很适合快速编辑。




下图使用“表面移动”模式，让骑士站到了台阶上。（因为这个道具是活动道具，需要电力和各种机械齿轮驱动，所以下面会有一个铁疙瘩，这个道具每月运行经费大概是5美元 ；-)） 对了，在此模式下，按住shift键，可以调整模型高度位置。这样xyz三个轴都能编辑了。

![](https://img-blog.csdn.net/20180109231413113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180109231433572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


模型编辑还包括很多别的设置，例如更高级的移动，角度捕捉，表面对齐，位置捕捉，就不在这儿一一介绍了。




### 道路编辑

过山车之星里面，有多种道路模板，可以选择路面材质，栏杆种类等等。具体进行编辑的话，会有以下的设置。

![](https://img-blog.csdn.net/20180109232157585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180109232208079?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





就不一一详谈。

P.S. 对于游戏玩家而言，打开曲面斜坡，就可以修建平滑的斜坡道路了。（对于三维开发者而言，程序化的创建三维的，各种弯角，带各种附属设施的路面真是一个复杂的工作）




下图是我做的一个旋转路面的例子，由于时间关系，我没有做平滑，所以还是能看得出路面不是完美的曲线。但是用心的玩家是一定可以使用这个路面编辑工具做出任何自己希望的道路的。。




![](https://img-blog.csdn.net/20180109232432162?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 过山车轨道编辑

这是本游戏的重头戏，自己的公园能不能盈利，自己能不能跻身于一流的建造师的行业，就靠过山车的轨道编辑了。

我先发下截图，看看有系统提供了哪些编辑功能吧。（在此不展开讨论，毕竟我自己建造的过山车还不是公园的核心盈利设施）




![](https://img-blog.csdn.net/20180109232822349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





编辑时，目标区段是高亮状态。一般会有四个图标显示在周围。如下图：

![](https://img-blog.csdn.net/20180109233055606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


#### Roll

按住上方的图标，会显示轨道的倾斜程度，也就是roll值，如果以姿态角来描述的话，习惯称为翻滚角。（想象一下飞机侧向滑翔）。此时，拖拽鼠标可以控制轨道的翻滚倾斜程度。如下图所示：

![](https://img-blog.csdn.net/20180109233310748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


#### Heading/Yaw

按住左方的图标，会显示轨道的朝向，也就是heading/yaw值，可以叫偏航角，航向角。（想象一下直升飞机转向）。此时，拖拽鼠标可以控制轨道的方向。如下图所示：

![](https://img-blog.csdn.net/20180109233713603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





#### Length/Scale

按住下方的图标，会显示轨道的长度。此时，拖拽鼠标可以调整轨道长度。如下图所示：

![](https://img-blog.csdn.net/20180109233930672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





#### Pitch

按住右方的图标，会显示轨道此时的俯仰角，也就是pitch值， 可以叫俯仰角（想象一下，飞机起飞时，机头渐渐抬高的情景），此时，拖拽鼠标可以调整轨道的俯仰程度。如下图所示：

![](https://img-blog.csdn.net/20180109234447729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


通过上面的基本操作和游戏系统提供的模板，就能编辑出别出心裁的过山车轨道了。然后需要进行测试，测试时，轨道上会覆盖显示一层热力图，显示测试者的刺激性，恐惧感，恶心感等多项指标，右侧面板也会给出过山车通过每一个轨道的速度，各个方向上的加速度（失重感的产生源头）。根据这些值就可以科学的设计个性化的轨道了。




引用一张图，便于理解

![](https://img-blog.csdn.net/20180110000931697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 地形编辑

个人感觉比天际线的地形编辑要好用。进入游戏后看说明就可以很快的学会地形编辑。下面是系统提供的地形编辑的几种基本操作模式：

![](https://img-blog.csdn.net/20180109235059069?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


上拉，下推，与地基齐平，与表面齐平，凿刻。平整，磨糙。看名字就能理解。旁边的强度，大小等各种参数。使用过PS的应该很容易理解。（二维的图像PS和三维的地形编辑真的很相似。PS里面对应着画刷，橡皮擦，平滑，锐化，各种滤波，各种模板，画笔大小，画笔强度。。）







编辑好格网之后，可以选择纹理和材质了：




![](https://img-blog.csdn.net/20180109235635454?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





还有各种水的效果：

![](https://img-blog.csdn.net/20180109235702151?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


接下来，花几分钟实战演练下。

编辑前：（圆环是我的画笔）

![](https://img-blog.csdn.net/20180109235746905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





编辑后。挖了个人工湖，做了个湖心岛，岛上放了一个小灯塔。夜晚时，在灯塔的照耀下，湖面波光粼粼。。。




![](https://img-blog.csdn.net/20180109235901975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 后记

以上就是本文的主要介绍内容了。这个游戏也是经过我老婆和小孩“官方”认可的游戏。画风明朗，BGM轻快。同时也是我玩起来最头痛的游戏，玩别的游戏，用手柄玩，玩这个游戏，得用计算器玩：统计月盈利趋势，制定定价策略，制定排班表，工程开始前做预算，工程进行中计算各种加速度对于乘客感觉上的影响。路有多长，路边多少米一个垃圾桶，多少米一个板凳。摄像头覆盖范围，广告牌如何摆放。各种设备的购买成本，折旧速度，运行成本。。




最后附上一张我的公园截图吧。

![](https://img-blog.csdn.net/20180110002038018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下图就是截图的烟火特效的成本。每次给老婆孩子演示完之后，心都在滴血啊。。

![](https://img-blog.csdn.net/20180110002139330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://blog.csdn.net/iuhsihsow/article/category/1065735)](https://blog.csdn.net/iuhsihsow/article/category/7392336)





