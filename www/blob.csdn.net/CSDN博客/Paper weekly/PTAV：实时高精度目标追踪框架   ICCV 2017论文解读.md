
# PTAV：实时高精度目标追踪框架 | ICCV 2017论文解读 - Paper weekly - CSDN博客


2018年03月20日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：509



![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**52**篇文章

# 关于作者：陈泰红，小米高级算法工程师，研究方向为人脸检测识别，手势识别与跟踪。

■ 论文 | Parallel Tracking and Verifying
■ 链接 | https://www.paperweekly.site/papers/1613
■ 源码 | http://tinyurl.com/y94wn2yg

# 论文动机

目标追踪是视频分析的基本任务，比目标识别更为复杂：目标追踪可以把每一帧看作单独的图片，再进行识别，但目标追踪不会这样做，因为没有考虑视频帧之间的冗余信息，效率低且慢。

目标追踪一直有两大阵营，CF 相关滤波和 CNN，当然也可以 CF+CNN 融合。
**本文基于以下动机：**

1. 物体运动时，大部分时间运动平缓，外观改变较慢。但是对于偶然发生的剧烈运动，需要复杂信息处理，需要单独验证。

2. CV 领域已经普及多线程计算，尤其 SLAM（Simultaneous localization and mapping、同步定位于地图构建）领域。PTAM 算法的一个关键观点是每一帧不是有必要构建地图。对于目标追踪而言，每一帧图片也不需要单独的验证。

3. tracking performance 和 efficiency 之间需要达到一种平衡。

**论文将 tracking 过程分解为两个并行但是相互协作的部分**：一个用于快速的跟踪（fast tracking），另一个用于准确的验证（accurate verification）。

其实作者之前发布的论文版本[1]已被 ICCV 2017 录用，此版本在原文基础上做了进一步的修订。**本文所有讨论以 2018 年 1 月 在 arXiv 上发布的版本为准**。

**该版本在原文基础上做了以下改进：**

1. 使用更具有鲁棒性的跟踪器（Staple）提升性能。

2. 动态目标模板池用于自适应验证，放置目标外观的变化。

3. PTAV 的 V 和 T 进行了多方面验证，比如 T 使用 VGGNet 和 AlexNet ，V 使用 KCF、fDSST 和 Staple。

4. 使用更多实时性算法和基准，进行更彻底实验验证和分析。

# 论文模型

**PTAV 算法架构包括三部分**：Base Tracker T，Base Verifier V，以及它们之间的协调环节。

对于 T，论文选择的是 Staple 算法[2]。验证环节 V 则选择采用 Siamese network 验证目标之间的相似性。对于协调环节，T 在合适的频率发送结果给 V，保证足够的时间验证。

为了改善 PTAV 的精度和速率，论文使用 k 均值聚类用于保持动态目标模板池用于自适应验证。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglTaoKUOn1cR6pmWaqSY1jsHcIEYBPW0HoiaNLLXG3a7oAXZyVKOsc9chUHn4sIzkYKutShN60as7Q/640)

# 实验

论文的实验数据详实，在主要数据集 OTB2015，TC128，UAV20L 和 VOT2016 上均和典型算法进行了对比。

从精度和实时性考虑各个模型，如图所示，OTB2015 数据集显示效果。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglTaoKUOn1cR6pmWaqSY1jsxS4O7rbYDFmAvXKggy6dBXOjkV5lAQBm5kkSqhZPtSrI0gBvoZSeNw/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglTaoKUOn1cR6pmWaqSY1jseOQEsc78I3zHZib2DWJiburibrllljicfg5HRLMPL0TyqMHOHt2g1GQr4A/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglTaoKUOn1cR6pmWaqSY1jsJbqKcgsNg52Tg3Vs4zwY5QzIqic3cR59ib1lYCZU9lArm72MCWQQ5kRQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglTaoKUOn1cR6pmWaqSY1js8MQia9UHumGUNWhibU7ecbp0N2HXsalyicG593aj70HheXNah2EmRebZw/640)

# 总结

PTAV 的假设是大部分时间目标运动平滑，位置改变很慢。而较难的情况是运动的无规律性。论文提出的 verifying 时刻并不一定是运动改变的时刻，具有偶然性，所以对“打架斗殴”等运动剧烈的情况会失败。10 帧更新速率对 verifying 不是最好的选择。

对于同样视频序列处理的 video segmentation，可以同样考虑。因为视频的逐帧操作比较耗费时间，这种间隔性的验证可大量节约资源。但是，对于剧烈变化的活动，更新会滞后。是否存在自使用速率变化的方法，这样的坑可以继续填。

# 参考文献

[1] Parallel Tracking and Verifying: A Framework for Real-Time and High Accuracy Visual Tracking (ICCV2017)
[2] Staple: Complementary Learners for Real-Time Tracking

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkvSxCRgqK914dI363k7VWmXJBVCNtexC0iaXJXUohTRgBpX4wGrDNYNpPB6SrZHiccz24zYcN2th2Q/640?)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****榜 单 公 布****\#**

**[2017年度最值得读的AI论文 | NLP篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487348&idx=1&sn=8ee8bf57418342a419fe73829cb14e75&chksm=96e9d0f4a19e59e288dcb105bd90b1e13f419ee7268ac69eba7cd6dac12e2e64aa84c56e5c07&scene=21#wechat_redirect)**
**[2017年度最值得读的AI论文 | CV篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487362&idx=1&sn=980153481f88ba5d6ba2929fd08240f3&chksm=96e9d002a19e5914f845973111b7056d24a28a8f7932479178c9cfcdb10c05c3284d5c0eb602&scene=21#wechat_redirect)**


我是彩蛋

**解锁新功能：热门职位推荐！**

PaperWeekly小程序升级啦

**今日arXiv√猜你喜欢√****热门职位****√**

找全职找实习都不是问题
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**职位发布**
请添加小助手微信（**pwbot01**）进行咨询

**长按识别二维码，使用小程序**
*点击阅读原文即可注册

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)
▽ 点击 |阅读原文| 查看原论文


