# AI时代的攻与防 - 人工智能学家 - CSDN博客
2017年12月19日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：527
*![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYic2gQsWtK89H2QAOVdJxF2Q4UnfVmoicOQg7AIjHsicLcFfWHbkB9kcp4g/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)*
*来源：HiSec*
**作者：刘焱**
****具有近十年云安全及企业安全从业经历，**负责Web安全产品，包括防DDoS、Web应用防火墙、Web威胁感知、服务器安全以及安全数据分析等；**
**是FreeBuf、雷锋网专栏作家、i春秋知名讲师，多次在OWASP、电子学会年会等发表演讲，参与编写《大数据安全标准白皮书》；**
**是国内知名AI安全技术书籍《Web安全之机器学习入门》和《Web安全之深度学习实战》作者，2017WIT年度安全人物提名。**
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYic2gOcK7sy3xEyhtBiaq9uXavuZdQYdib3FwYcibnEWYmuN76eB4wlNFfeA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**本期刘焱先生为我们讲述他眼中的AI安全：**
1. 传统安全领域，无论是主动的威胁发现和安全防护还是被动的入侵检测，基本都是基于以往经验总结的静态检测规则和黑白名单；
2. 安全圈有句戏言，安全设备是人多聪明它多聪明，人已经想到的，没准能防住，没想到的肯定防不住。另外，这种基于规则的安全技术，现实中最大问题是规则难以维护，规则之间的重复与冲突更会雪上加霜，堪比一片混乱的机房；
3. 是否可以使用AI技术给安全领域带来一股新的力量呢？事实上，基于AI技术，在安全领域的常见分类问题上，都取得了不错的进展，比如垃圾邮件，恶意网址，黄反鉴别，病毒检测等。
**正文>>**
**生活中的AI **
# 大概是一年前，当我看到这张漫画时，我家里除了苹果系列的手机和MacBook，几乎再难以找到一个与云或者说AI沾边的产品。AI也只是我研究的一个方向但是和我的生活没有太大关系。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYiccqOzUeJbCWuF2pzu1VEdbrdc1tEdhicCFpUtTp0iaR5uVXULSffmArrg/0?wx_fmt=png)
*（智能家居漫画图）*
这个观点很快就被打破了，我所在的公司的门禁和消费系统可以使用人脸识别，真正实现了刷脸上班吃鸡翅。前不久我也赶时髦买了智能音箱，非常意外的是我家的加湿器也居然可以被智能音箱控制，我家的网络电视机顶盒安装一个小软件后也可以和它联动。经过简单调试后，智能手机都不愿意用的老父亲，已经学会使用音箱在电视上选电影看，我的不到三岁的娃娃也学会用智能音箱听小猪佩琪。AI设备已经润物细无声，双十一的智能音箱已经不到100块了。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYic8AASbO4ibN2f3Psa3aKgW9x5Kiayhbklkzib1ngIwLHCNosPgsyiaOOBpA/0?wx_fmt=png)
*（亚马逊的智能音箱）*
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYicSIJ4iagibicmrSldZnHKLUhRUMQYREBA5QUxCXfDdT0ZicibDvaaUnxIQXQ/0?wx_fmt=png)
*（JD的智能音箱）*
**AI是一把双刃剑**
一次偶然的机会，我发现在城铁上我也可以远程管理我家的加湿器和智能音箱，我突然意识到，这些AI家居设备时刻与云联通，同时也与家里其他网络终端共享一个局域网，如果存在安全问题，黑客是否也可以远程控制它们，也可以时刻像音箱一样监听我们的谈话，嗅探我家发生的一切呢？AI是一把双刃剑，它在提升生活效率的同时，也带来了一些安全隐患，被不法分子用来窃取用户隐私，提升攻击技术。
利益驱动的黑产，往往在新技术的使用上超过大家的想象。2017年，腾讯守护者计划安全团队协助警方打掉市面上最大打码平台“快啊答题”，挖掘出一条从撞库盗号、破解验证码到贩卖公民信息、实施网络诈骗的全链条黑产。而在识别验证码这一关键环节，黑产运用AI技术训练机器，极大提升了单位时间内识别验证码的数量，2017年一季度打码量达到259亿次，且识别验证码的精准度超过80%。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYicmhyF9ibXTJCPxLhoDsXdyaBkzR9RkHBMCQWryLupCVaiaibOSmx3kB1uQ/0?wx_fmt=png)
*（黑产破解验证的AI设备）*
**AI模型的安全**
作为一个软件系统，AI算法或者说机器学习模型也是可以被欺骗的。一个经典案例就是针对图像分类模型的攻击，通过对熊猫照片的微小修改，人的肉眼几乎察觉不出任何变化，但是机器却会被欺骗，误判为长臂猿。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYiczGt2vLpLRIXXmnicCdGo3cKicJKK7mD4GPZmtB1lxdcYT0bSNMPPhqqQ/0?wx_fmt=png)
*（精心处理的熊猫图片可以被机器学习模型识别为长臂猿）*
获得美国麦克阿瑟天才奖的DawnSong教授及其团队在这方面做了非常深入的研究，她们可以通过人眼无法识别的微小修改欺骗机器学习模型，把众人都识别为同一个人。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYicNRCEz5YYB0gn44HBXDCwyEBf4psNexSxuxt0XT8cNylRgvCHlFtY6Q/0?wx_fmt=png)
*（微小的改变可以把众人都识别为同一个人）*
本质上机器学习模型是在多维特征向量层面，通过样本学习，迭代计算出分类结果，适当的微小调整也可以对分类结果产生显著影响。以下图为例，X的取值为（2，-1，3，-2，2，2，1，-4，5，1），但是只要稍微修改成（1.5，-1.5，3.5，-2.5，1.5，1.5，-3.5，4.5，1.5），分类结果为1的概率就可以从5%提升为88%。诸如这类针对AI模型的攻击也是AI安全的重要领域。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYicicRyxLo1sTe1sRR8ITJVNu7tyBqmONJrbiabC8aHY2a9gzETZTib2dplg/0?wx_fmt=png)
*（特征向量微小变化也可以对机器学习模型分类结果产生巨大影响）*
# **使用AI做安全**
传统安全领域，无论是主动的威胁发现和安全防护还是被动的入侵检测，基本都是基于以往经验总结的静态检测规则和黑白名单。这些技术在过去很长一段时间已经被证明难以抵抗黑产以及针对性极强的商业间谍，基于经验积累的静态规则技术，总是处于被动挨打的境地，无论是精心的加密混淆还是没有补丁的零日攻击，多可以轻松绕过现有的防护体系。安全圈有句戏言，这种安全设备是人多聪明它多聪明，人已经想到的，没准能防住，没想到的肯定防不住。另外，这种基于规则的安全技术，现实中的最大问题是规则的难以维护，规则之间的重复与冲突更让这些问题雪上加霜，堪比一片混乱的机房。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYicBIXYGiaWDx200gYuQMecWPqK6SEG5B7PL72N1PgxiaJcQBXpG5iahzh6w/0?wx_fmt=png)
（*混乱的机房）*
是否可以使用AI技术给安全领域带来一股新的力量呢？2015年，微软在Kaggle上发起了一个恶意代码分类的比赛，并提供了超过500G的原始数据。有意思的是，取得第一名的队伍三个人所采用的方法与我们常见的方法存在很大不同，展现了AI在安全领域的巨大潜力。早期的反病毒软件大都单一的采用特征匹配方法，简单的利用特征串完成检测。随着恶意代码技术的发展，恶意代码开始在传播过程中进行变形以躲避查杀，此时同一个恶意代码的变种数量急剧提升，形态较本体也发生了较大的变化，反病毒软件已经很难提取出一段代码作为恶意代码的特征码。基于深度学习的病毒检测不用人工提取特征串，它把恶意文件当成一个图像文件，把每个字节当成一个像素点，这样就把恶意文件转换成恶意代码图像，然后基于成熟的图像分类技术，经过大量样本的训练学习，自动提取了恶意程序的特征进行检测。最终他们检测的效果超过了常见传统检测方式取得了冠军。
在移动领域，使用类似的思路也取得了不错的成绩，百度安全使用深度学习识别恶意APK，准确率达到99.96%，召回率达到了80%。2016年，反映该研究成果的论文“AI BASEDANTIVIRUS: CAN ALPHAAV WIN THE BATTLE IN WHICH MAN HAS FAILED?”被blackhat会议收录并做了相关演讲。
随着移动互联网的发展，恶意APK也井喷式爆发，其数量在近几年呈指数级增长，传统的基于规则的检测技术已经无法覆盖如此大量的恶意程序。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYic4GsRBboeZH2GsiaqpT31kmEky3GE2h5aak2H7vBYiaR2stEiagHG7u0Mw/0?wx_fmt=png)
*（恶意APK数量猛增）*
在大量的人工分析恶意APK的工作中发现，人工分析的过程很类似图像识别的过程。深度学习在图像识别领域有着成熟的应用，是否可以通过提取APK特征，通过深度学习的方法来自动化识别恶意APK呢？
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYicCvYdqf3YqDX92hczXLbzmoYdlebPANh240H4T7RmfjjibOILZE2D2eg/0?wx_fmt=png)
*（恶意APK分析与图像识别）*
**对APK的特征提取主要分为三大类：**
第一类是结构化特征，包括APK申请的权限的个数，资源文件中包含的图像文件个数和参数大于20的函数的个数等。
第二类是统计类特征，包括近千条统计特征。
第三类是长期恶意APK检测的经验总结的特征，包括资源文件中是否包含可执行文件，assets文件夹中是否包含APK文件等。
在特征提取的环节，使用结构化特征、统计特征和一些经验积累的特征，把APK文件转换成了一个维度为1235的特征向量，也就是用一个大小为1235的数组描述了一个APK文件。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYica7CQKTDohZK198WpRzwAYPLpvkyVTzNkZqCgR24QpGKkdDVo3IPOiaQ/0?wx_fmt=png)
*（APK特征提取过程）*
归一化处理一千多个特征，取值控制在-1和1之间。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYicMk0rB9M2uoDNDUXT9dDeYvicWyvEMjpOsgxmmATafsH77CxQTiaAFccA/0?wx_fmt=png)
*（归一化APK特征）*
使用深度学习网络训练，训练过程如下图所示：
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWUANfrVULOvMwJZCrQ7yYicEAYlicM1NVkLBiajGJ2pkanNyqNGDJqibW7vLE9v4Nx3a56iapMM10kYPA/0?wx_fmt=png)
*（APK深度学习训练过程）*
整个训练过程中使用超过了15万个样本，训练使用了百度开源的深度学习框架Paddle。
事实上，基于AI技术，在安全领域的常见分类问题上，都取得了不错的进展，比如垃圾邮件，恶意网址，黄反鉴别，病毒检测等。
**总结**
哈佛大学发布《人工智能与国家安全》报告，指出AI的发展将通过变革军事优势、信息优势和经济优势等三方面影响国家安全，建议美国政府保持美国的技术领先地位。而最近，俄罗斯总统普京就AI领域的竞赛警告：“谁成为了这个领域的领导者，谁就将成为世界的统治者”。习近平总书记也在多次讲话中谈到人工智能的迅猛发展，强调“我们不仅要把我国机器人水平提高上去，而且要尽可能多地占领市场”，并且亲自担任中央网络安全和信息化领导小组组长。可见AI技术已经得到大国国家层面的重视。Strategy Analytics的报告指出，到2020年会有500亿台物联网设备在全球部署。埃森哲预测，到2035年，人工智能将让年度经济增长率提升一倍。在这样的背景下，AI安全显得尤其重要，针对AI安全的研究已经成为整个安全行业最重要最前沿的一个方向。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎支持和加入我们。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXoz49H9w8lfGXmBjcGga9W15bmYAvZDqbbI8p0liaLcxEh1iao7YXIgJvp6MLHpskd7jHq8ibX0QzDQ/640?wx_fmt=png)
