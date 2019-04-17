# 特斯拉无人车AI处理器背后，站着一位芯片界传奇 - 知乎
# 



千平 发自 LZYY

量子位 出品 | 公众号 QbitAI

Legendary，热门游戏里代表“超神”。

本周特斯拉正在研发无人车AI芯片这条热门消息的背后，就有一位经常被西方媒体冠以Legendary或者Guru的芯片界大神。

这个人，名叫：凯勒（Jim Keller）。



![](https://pic4.zhimg.com/v2-8dd1ac4e37aa313a5b083c1e8e28a7fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='248'></svg>)



## 芯片界的传奇

去年一月，凯勒加入特斯拉出任副总裁，负责Autopilot自动驾驶系统的软件和硬件。从那时开始，特斯拉研发芯片就只是早晚的事了。

这个结论，来自他的职业生涯。

凯勒本科毕业于宾夕法尼亚州立大学，其后供职于DEC并参与了Alpha 21164和21264两款处理器的设计。

Alpha也是当年备受关注的芯片系列。国产的申威处理器，最初就是基于Alpha 21164发展而来。

1998年，凯勒从DEC离职加入AMD。那个时候，CPU市场的竞争日趋激烈。起初，他是Athlon K7架构的关键人物，还参与研发了HyperTransport总线，以及64位x86规范。



![](https://pic1.zhimg.com/v2-773ba3c959e8e61642765c1404e344ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='400'></svg>)



接着，他成为K8的首席架构师。正是K8架构，真正让当时的AMD有了足以匹敌英特尔的能力。使用K8架构的产品包括Athlon 64等多款处理器，当时英特尔的产品是Pentium 4系列处理器。

这之后凯勒从AMD离职，加入SiByte，转向为1Gbit/s的网络设备设计基于MIPS的处理器。随后SiByte被Broadcom收购，凯勒继续作为Broadcom的首席架构师，直到2004年他离职加入P.A. Semi。

P.A. Semi是一家专注低功耗移动处理器的公司。2008年，苹果公司出手收购P.A. Semi，凯勒也成为苹果公司的员工。

在苹果工作期间，凯勒主持设计了A4、A5两代移动处理器，用在iPhone 4/4s、iPad/iPad 2等设备上。



![](https://pic3.zhimg.com/v2-de181d5271132c29acaa64af381e902e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='336'></svg>)



他还负责定义Macbook Air的硬件规格，让乔布斯相当满意。

2012年8月，凯勒重回AMD，开始着手主持设计新一代微架构，代号为：Zen（禅）。这是一个革命性的架构，外界一度误传这个架构将把AMD处理器性能提升40%，但其实严格的说是每时钟周期指令数(IPC)提升。



![](https://pic2.zhimg.com/v2-699a9673b4207a52747e8ba8682fbc61_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='359'></svg>)



据说凯勒在AMD每天工作12-14个小时，当时他已经50多岁。

首款基于Zen架构的处理器，就是AMD Ryzen。无意间透露出特斯拉研发芯片秘密的格罗方德（GlobalFoundries），就是Zen架构处理器的代工方。

## 特斯拉的野心

把这样背景的人请到特斯拉，不开发芯片才令人意外。

去年的特斯拉财报电话会议上，大摩分析师就曾直接问伊隆·马斯克：是否确认传奇人物凯勒加盟？以及特斯拉是否在自行设计芯片？

特斯拉CEO确认了第一个问题，对于第二个问题他的回答是：

> “we just… no comment, yeah”。

凯勒不是孤身来到特斯拉，而是伴随一个团队，其中包括：
- 班农（Peter Bannon）：来自苹果的顶级芯片研发人才，再次追随凯勒加盟特斯拉，此前班农曾和凯勒搭档领导苹果芯片研发部门。
- David Glasco：AMD服务器芯片架构高级总监，先后供职于英特尔、IBM、英伟达。
- Thaddeus Fortenberry：AMD云服务架构师
- Debjit Das Sarma：AMD Fellow，CPU主任架构师
- Keith Witek：AMD战略合作副总裁
- Junli Gu：AMD机器学习主任工程师



![](https://pic1.zhimg.com/v2-2641bba70dab3ec5b197c8bb8891f724_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='289'></svg>)



不过，这次挖角似乎没有影响特斯拉和AMD之间的关系。特斯拉的AI芯片研发，据报道还是跟AMD联手进行的。毕竟对AMD来说，赚钱才是目标。而汽车行业也是AMD一直想要涉足的。

9月20日，也是在格罗方德的技术大会上，AMD首席技术官宣布明年Ryzen CPU和Vega GPU将使用12nm LP技术。这将让AMD的产品可以用于汽车行业。



![](https://pic4.zhimg.com/v2-dd8c34e041daebd71ca5335e4820c01f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='528' height='315'></svg>)



特斯拉与AMD合作，能减少对英伟达的依赖，并最大化公司的利润。这是一个符合逻辑的也非常马斯克的选择。AMD进入这个市场也许会采用低价策略，至少比英伟达的产品看起来更便宜。

Ronald Liu在Seeking Alpha上分析说，英伟达为特斯拉提供Drive PX 2平台，假设双方达成1000美元的协议价格，而特斯拉2018年计划生产50万辆车。所以英伟达明年潜在的收入损失可能就达5亿美元。

不过一切还没落定。

特斯拉和AMD没有否认也没有确认双方的合作关系，而惹出事端的代工厂格罗方德只是出面澄清自己跟特斯拉没关系。这中间的微妙，不知你能看出来么。

还有，毕竟黄仁勋还有好几辆特斯拉，也算是重要客户~



![](https://pic3.zhimg.com/v2-8de5bf31bfc4a98943848a3660395ae2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)



— 完 —

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

量子位 QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态


