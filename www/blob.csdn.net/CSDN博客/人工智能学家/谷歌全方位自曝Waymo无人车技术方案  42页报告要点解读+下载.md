# 谷歌全方位自曝Waymo无人车技术方案 | 42页报告要点解读+下载 - 人工智能学家 - CSDN博客
2017年10月14日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：607
##### 李杉 夏乙 编译整理量子位 出品 | 公众号 QbitAI
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWmiawXo50SJnu36sv8b2DVKD8AxsqahOe1sy2DcLyyGeMTsNTF2vh2iaEibzibOIIcU5VSm8K3RGxN8w/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
谷歌今天发布了一份无人车重磅报告：《通往完全自动驾驶之路》。
这份42页的报告从技术层面详细展示了谷歌Waymo无人车的软件、硬件、测试流程，还讲了无人车行驶的限制条件，“失败”时如何安全停车，甚至车祸后如何处理、记录哪些数据，以及车辆上采取的网络安全措施等等。
按照提纲来说，共有如下几部分内容：
- 
谷歌无人车的安全设计理念
- 
Waymo无人车是如何工作的
- 
测试和验证是如何进行的
- 
无人车相关交互细节
这份报告是谷歌对Waymo无人车自动驾驶技术方案的一次详尽解读。量子位摘录其中的要点如下。
# **无人车要解决的四大问题**
根据Waymo在报告中的介绍，一辆无人车站在马路上，和人类司机一样，通常需要回答4个问题：
- 
我在哪？
- 
我周围有什么？
- 
接下来要发生什么？
- 
我该怎么办？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YicUhk5aAGtBae92XBBCTv9XTBj5kBKRfmOic0hzeWqj1rBpINdFuoWg5mAaulQQDEtndiaHbgaBLFMwEZ0RYpDpw/640?wx_fmt=png)
为了解决“我在哪”这个问题，Waymo的无人车依靠的不是GPS，而是是团队自己建立的详细三维地图，上面突出显示了路况、“倒鸭子”、人行道、车道标记、人行横道、交通信号灯、停车标志和其他道路特征信息，也就是我们常说的高精地图。通过将传感器实时数据和预先建立的高精地图相比对，车辆就能确定自己在哪了。
###### 不知道“倒鸭子”是什么，请抽出4分钟观看这部优秀影视作品：https://v.qq.com/x/page/v0194evja2h.html
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YicUhk5aAGtBae92XBBCTv9XTBj5kBKRfPpMTNuWibctYZCmvDKRnFNtIbibzfpJMic5Ea2icRoorhxPhrKkub6XfUA/640?wx_fmt=png)
解决“我周围有什么”，是靠传感器和软件的配合，来识别车辆、行人、自行车、施工现场、障碍物，以及辨别交通灯的颜色、临时停车标志等等。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YicUhk5aAGtBae92XBBCTv9XTBj5kBKRfmuVQtslD5libia8kb4Wtpse4NofebuWdS0y1ZOlHBiaBtcZeKsic7fCTtw/640?wx_fmt=png)
“接下来要发生什么”则是对车辆周围每一个动态对象未来运动的预测，Waymo的软件能根据对象当前的速度和轨迹预测未来的运动，也知道车辆和行人、自行车的区别。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YicUhk5aAGtBae92XBBCTv9XTBj5kBKRfy8fKRuGyET0o4vaOJnvUP1JAEthf45SS2HqblMhznZEDacdhqibM0Wg/640?wx_fmt=png)
而最后一个问题：“我该怎么办”，指的是无人车的决策。Waymo的软件会根据前三个问题提供的信息，来选择正确的轨迹、速度、车道和转向操作。
Waymo无人车解决这四个“车生终极问题”的目标，是在特定地理区域内、特定条件下，完全不需要人类介入来完成整个动态驾驶任务，也就是国际汽车工程师协会（SAE International）所定义的L4自动驾驶。
要实现这个目标，依靠的是车上搭载的传感器和自动驾驶软件。
# **传感器**
Waymo在报告中以菲亚特-克莱斯勒Pacifica车型为例，详细介绍了他们在无人车上所用的自动驾驶传感器，这些传感器赋予了无人车360度视野，最远能“看清”300米外的物体，有三个橄榄球场那么远。
传感器的分布如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YicUhk5aAGtBae92XBBCTv9XTBj5kBKRfIj1saEj8hG5iaQiayWr4pLupPWGqOCkekHBIyiaUt0PTa12me7X8haQOQ/640?wx_fmt=png)
###### *△ Waymo无人车上的传感器*
**LiDAR（激光雷达）系统**
LiDAR（激光雷达）昼夜均可使用，它每秒可以向360度的方向发出数百万激光脉冲，并且可以测量激光从一个表面反射回汽车所花费的时间。Waymo的系统包含3种自主开发的LiDAR：能够提供四周连贯视野的短距离LiDAR、高清中距离LiDAR，以及能够看到几乎3个橄榄球场之外的新一代长距离LiDAR。
**视觉（摄像头）系统**
这个视觉系统包含为了像人类一样看到周围世界而设计的摄像头，但它具备360度的同步视野，而人类驾驶员只能看到120度的视野。由于高清视觉系统能够探测颜色，因此可以帮助系统识别交通信号灯、施工区、校车和急救车的频闪灯。Waymo的视觉系统由多组高清摄像头组成，目的是在白天和低光照条件下都能看清远方的物体。
**雷达系统**
雷达系统使用多种波长来探测物体和运动。这些雷达波能够绕过雨滴等物体，因此在雨天、雪天和雾天也能发挥效果，而且不受昼夜变化的影响。Waymo的雷达系统具备连续的360度视野，所以可以车辆前后和两侧的道路参与者的速度。
**补充传感器**
Waymo汽车还有很多额外的传感器，包括可以听到警车和急救车警笛的音频探测系统。
# **软件**
无人驾驶软件是车辆的“大脑”。利用传感器获取信息后，还要借助这些信息针对每种情况制定最佳驾驶决策。
Waymo花了8年时间，使用机器学习和其他先进的工程技术开发和改进了软件，并通过数十亿英里的模拟驾驶和超过350万英里的路测对软件进行了训练。
这套系统能够深刻理解世界的背景信息；这是L4级自动驾驶技术的关键差异点。Waymo的无人驾驶软件不仅能探测到其他物体，还能理解这些物体是什么、它可能的行为方式以及会对汽车自身的道路行为产生何种影响。无人车正是凭借这种方式才得以在全自动模式下安全地上路行驶。
这套软件由很多不同的部分组成，主要包括三大组件：感知（perception）、行为预测（behavior prediction）和规划器（planner）。
**感知系统**
感知系统是对路上物体进行探测和归类的组件，它还可以持续测算物体的速度、方向和加速度。无人驾驶软件通过传感器获取详细数据，然后将其转化成现实世界的全面视图。
感知系统帮助无人车区分行人、骑行者、摩托车手、汽车等道路参与者。它还能区分交通信号灯等静态物体的颜色。对于这些物体而言，感知系统使得整套系统可以在语义上理解周围的车辆——信号灯是否变绿，从而允许车辆前行，或者车道是否被前方的锥桶封闭。
**行为预测**
借助行为预测，软件可以对道路上的每个物体进行建模，并对其加以预测和理解。
由于Waymo拥有数百万英里的驾驶经验，无人车能针对不同道路参与者可能的行为方式建立准确的模型。例如，软件能明白尽管行人、骑行者和摩托车手看起来相似，但他们的行为却大不相同。行人行进速度远慢于骑行者和摩托车手，但他们却能更加突然地改变方向。
**规划器**
规划器会考虑软件通过感知和行为预测手机的所有信息，然后为汽车规划一条路径。根据经验，最好的司机都信奉安全第一。正因如此，Waymo植入了防御性驾驶行为。例如，保持在其他驾驶员的盲区之外，以及为骑行者和行人留出额外空间。
规划器还能提前思考几步之后的决策。例如，如果软件认为前方临近的车道因为施工被封闭，而且预计那条车道的骑行者会变道，规划器就可以做出减速决定，提前为骑行者腾出空间。
Waymo还借助路测经验对驾驶模式进行改进，使得行驶过程更加顺畅，令车内乘客更加舒适，让其他道路用户可以预测无人车的动向。
# **Waymo的测试技巧**
Waymo的无人车会分成三部分进行测试：
- 
车厂提供的车辆本身；
- 
Waymo的硬件，包括传感器和计算机；
- 
Waymo的自动驾驶软件。
## **车辆本身**
Waymo现在用于自动驾驶测试的车辆，主要的是经过改装的菲亚特-克莱斯勒混动MPV Pacifica，2017款。根据[The
 Information的报道](http://mp.weixin.qq.com/s?__biz=MzIzNjc1NzUzMw==&mid=2247489361&idx=1&sn=10e610648d250613d7dfb9d9c1fc9a53&chksm=e8d3a023dfa42935fefd245bab84e6d4547776971cd93b5d726c22ffaeee7ef115b4d846c16c&scene=21#wechat_redirect)，Waymo所用的Pacifica在出厂时，顶部就为安装LiDAR等传感器留了一个圆孔。
## **传感器和计算机**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWmiawXo50SJnu36sv8b2DVKWF7LRKQ35APEwc7fWhf7JP58ibKvs9AfbbkWP9zuMR6smaOlcic4voVw/0?wx_fmt=png)
通过和菲亚特-克莱斯勒合作，Waymo可以在改装过的Pacifica上安装传感器等硬件。为了确保自动驾驶系统的正确安装和整合，Waymo还会对车辆进行数千项后续测试。
## **软件测试**
由于自动驾驶软件需要不断地更新、升级，Waymo对于软件的测试，需要持续进行，软件的每一次更改都要经历严谨的发布流程，包括模拟测试、封闭路线测试和公共道路测试。
**模拟测试**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YicUhk5aAGtBae92XBBCTv9XTBj5kBKRfIsxUrib5w34gnjUzrUgDkriaa2Pq5S5VvUIAhOXk5aYiatI3DJSMfb8ibA/640?wx_fmt=png)
Waymo的模拟器可以重现无人车在真实路测中遇到的场景，还能创建新的虚拟场景。《大西洋月刊》8月曾经详细介绍过[Waymo的模拟软件Carcraft](http://mp.weixin.qq.com/s?__biz=MzIzNjc1NzUzMw==&mid=2247488322&idx=2&sn=9dfcc2cb86c42eadd97319352d0726ef&chksm=e8d3a430dfa42d26642f561e5310bba4d25dbdeea8b71984198ae335c48ae6691aac00e51131&scene=21#wechat_redirect)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YicUhk5aAGtBae92XBBCTv9XTBj5kBKRf94AcRoSBunD8T446ZibxyakVQtpN7VSJ455AlJZ1f97Kq1icWxAWguLw/640?wx_fmt=png)
在模拟过程中，Waymo会严格测试软件的所有改变或更新，然后才会部署到车队中。他们会确定车辆在公开道路上遇到的多数挑战，然后将其转化成虚拟场景，以供无人驾驶软件在模拟器中练习。
**封闭路线测试**
在模拟测试之后，新的软件会首先推送到几辆车上，让最有经验的驾驶员在专有试车跑道上测试新软件。在不同车辆上，可以使用不同软件版本，从而在不同的运行设计域（Operational Design Domain，包括地形、道路类型、速度范围、气候、一天中的时间段以及法律法规等）中测试不同的软件版本。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWmiawXo50SJnu36sv8b2DVKwArxLPmXoCbWhQZUOwictyqfbPzay74Dx1c65bQp8OhhT5jbjUKI7hg/0?wx_fmt=png)
为了进行封闭测试，Waymo还建了一个[代号Castle的测试基地](http://mp.weixin.qq.com/s?__biz=MzIzNjc1NzUzMw==&mid=2247488322&idx=2&sn=9dfcc2cb86c42eadd97319352d0726ef&chksm=e8d3a430dfa42d26642f561e5310bba4d25dbdeea8b71984198ae335c48ae6691aac00e51131&scene=21#wechat_redirect)，位于美国加州中央山谷地区的小城默塞德附近，用各种各样的建筑模型和道具，来尽可能模拟真实道路的场景。
**路测**
一旦确认软件符合预期，Waymo便会将新的软件安装到路测车辆上。
首先，还是只安装在一小部分车辆上，这些无人驾驶汽车必须证明它们能够安全而始终如一地沿着既定路径行驶之后，再更新到整个车队。
路测里程越长，就越能监控和评估软件表现。
随着路测里程的增加，Waymo还会进一步调整和更新软件。这种持续的反馈回路证明了软件能够在操作设计域中作出恰当地反应，使得车辆在SAE Level 4的标准下安全运行。
# **350万英里**
Waymo的真实路测依然限制在美国境内，里程已经累积了350万英里。
在这份报告中，Waymo列出了他们路测过的所有地点：4个州、20个城市、350万英里的路程。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWmiawXo50SJnu36sv8b2DVKzphB3SEHsWLJKqAiaErY66ibhiciczbicaIDBqRKiaVpNcSqNqRIPR0nqZOg/0?wx_fmt=png)
这些城市，也是当前Waymo无人车可以运行的区域限制。Waymo的系统设计决定了这些车的自动驾驶系统在规定的领域之外不能运行，例如，乘客无法选择这些区域之外的目的地，车辆规划的路线也都会在这些区域之内。
也就是说，在Waymo开辟新的测试地点之前，他们的无人车只会在这些城市行驶。
以上，是量子位从报告中摘录的要点，更多关于Waymo无人车的详细信息，请见报告原文~
