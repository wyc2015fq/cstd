# AI技术加持，让协作机器人更安全 - 人工智能学家 - CSDN博客
2018年03月17日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：273
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXHEdRfwWOOnjqK4FVjsF419PH8IU63ib9ZBf51jYdakIz5xxwZTticqIYArm4w93SEhAYd9VnFPp5w/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
来源：机器人创新生态丨公众号
来自众家新创公司与实验室的碰撞侦测与追踪技术，将使得在人类与其他移动物体周边的协作机器人更安全。一个美国圣地亚哥大学（University of San Diego）的团队便开发了一种更快速的算法，能协助机器人利用机器学习避开障碍物；此外从麻省理工学院（MIT）独立的公司Humatics，则正在开发人工智能(AI)辅助室内雷达系统，能让机器人精确追踪人类的动作。
圣地亚哥大学所开发的算法名为Fastron，利用机器学习来加速并简化碰撞侦测；该算法是根据一个机器人的组态空间（configuration space，C-space）模型，仅利用少量的碰撞点（collision points）与无碰撞点，来分类移动物体的碰撞与非碰撞。现有的碰撞侦测算法是运算密集方案，因为那些方案会详细标明机器人与障碍物3D几何图形中所有的点，然后检查两者之间每个点可能发生的碰撞；当那些物体移动时，运算量会大幅增加。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXHEdRfwWOOnjqK4FVjsF41En6Df7Hq7iazLHtWv3fKnst9TLw6l92vdXoEweMMTNnSNGBhCkM4ib3g/640?wx_fmt=png)
美国圣地亚哥大学的团队开发出一种更快速的碰撞侦测算法，有助于机器人避开障碍物（来源：David Baillot, UC San Diego Jacobs School of Engineering）
Fastron算法的C-space模型扮演了以运动学为基础（kinematic-based）的碰撞侦测代理（proxy），该算法结合了核心感知学习算法的修改以及主动式学习算法，来减少以运动学为基础之碰撞侦测的次数。这种算法不需要检查每一个点，而是在界线（boundaries）附近检查并分类碰撞与非碰撞；随着物体移动，两者之间的分类界线也会改变，因此算法能迅速更新分类然后继续循环。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Yu8XibjiarTCxvmXcvUP2RDZBnBiaMboshNfsMM8tlvSYF6Gylz83I7fvYwk1HOibfJDIdMOc1rZ2uZwENEiatCEloQ/640?wx_fmt=jpeg)
Fastron演算法原理
（来源：David Baillot, UC San Diego Jacobs School of Engineering）
该团队已经在仿真中证实了，代理碰撞检查的速度是有效多面体检查工具（polyhedral checker）的两倍，是有效高精确度碰撞检查工具的八倍，而且不需要GPU加速器或平行运算；除了能应用于工厂生产线，其他潜在应用包括让外科手术用的辅助机器手臂更安全运作、不会在手术过程中造成干扰。
**AI辅助分析平台**
新创公司Humatics的联合创始人兼CEO David Mindell，也是MIT航天工程与技术史教授；该公司的产品经理Stephen Toebes则是协作机器人领导厂商Rethink Robotics的前任产品开发兼营运资深副总裁。此外该公司副总裁兼首席软件架构师Michael Barbehenn，则在擅长开发行动仓储机器人的Amazon Robotics（前身为Kiva Systems）任职多年，曾担任软件部门副总裁。
Humatics的空间智慧平台（Spatial Intelligence Platform）结合了一个微型定位系统，以廉价的RF技术与AI辅助分析软件为基础；该单一系统能追踪多个移动的应答器（transponder）目标，具备毫米（millimeter）等级的精确度，最长距离可达30公尺。也能以连网方式结合多个系统，已覆盖更宽广的范围，小至工厂的工作区，大致整个物流配送中心。
Mindell表示，该公司的目标是以小型化、廉价的RF信标（beacon），来打造一个支持公分（cm）与毫米等级绝对参考定位（absolute reference positioning）的世界，无论是在室内、室外或是任何一种天候状态；他表示：“协作机器人其实并不知道人们的所在位置，要融入人类环境，它们都应该要配备针对周围物体的定位导航功能；因此我们认为，未来的自主机器人，无论它们是车子或是工厂中的自动化机器人，还是无人机，都会是连网世界的组成部分。”
虽然Humatics目前正在开发的是小型化、短距离雷达，Mindell表示：“我们对于‘雷达’这个名词有点矛盾，因为我们的空间智慧平台并非反向散射系统（backscatter system），而是二次雷达系统（secondary radar system），也就是航空管制系统采用的方案，又称为信标对信标（beacon-to-beacon）方案。”
Mindell指出，目前该公司的系统可达到毫米等级精确度、单一装置3D量测，能追踪大量的电池供电或车辆供电小型移动信标，或被称为“小精灵”（pucks）之人类、机器人移动物体上的小型信标；除支持毫米等级追踪精确度，也支持非常高的更新率。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXHEdRfwWOOnjqK4FVjsF41RH6aGhMbHwELKKuicfqFmC0XibAkt8wuDo6PVspvD4px7o67Tsib2Pw9g/640?wx_fmt=png)
Humatics的空间智慧平台号称能让人类与机器人更安全地并肩工作（来源：Humatics）
而Mindell透露，Humatics正在开发自家分析软件：“所有在周遭的物体都是以毫米等级移动，因此在这个精确度的位置数据非常丰富；”其核心算法是基本的递归评估器（recursive estimator），具备自适应/优化的功能，当它们收集了大量数据就能更妥善分析动作以及位置信息。
Humatics系统采用的软硬件技术号称是廉价且可扩展的，因为产业界已经让微波与毫米波组件的成本降低，而该技术采用的标准化API则能让其数据也被其他应用程序与服务使用；Mindell指出，该架构是可延伸的，因此系统能以连网方式布署于大型厂房或其他空间，以提供广大的覆盖范围。该解决方案预计在2018年进行测试，并在2019年初正式上市。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
