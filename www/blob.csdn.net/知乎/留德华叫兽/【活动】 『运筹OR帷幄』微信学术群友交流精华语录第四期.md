# 【活动】 『运筹OR帷幄』微信学术群友交流精华语录第四期 - 知乎
# 

> **文章作者：**张宁
文章首发于**微信公众号【运筹OR帷幄】：**[【活动】 『运筹OR帷幄』微信学术群友交流精华语录第四期](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/5G5mx18zXm8WPvvnIiX1iA)
*欢迎原链接转发，转载请私信*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

**运筹学爱好者快点看过来在这里一定会有你所感兴趣的话题，让你茅塞顿开。**

**在学习运筹学过程中遇到问题的小伙伴也要看过来在这里你一定会有所收获。**

**这里还有大牛经典语录以及学界、业界前沿动态定会让你满载而归。**

**随着由『运筹OR帷幄』建立的微信学术群的发展与壮大，越来越多的各界OR从业者和爱好者也加入进来，与大家一起聊学术和技术，为大家带来了很多经典的、实用的想法与建议；也会跟大家闲聊一些名人轶事，带大家在轻松的氛围里长知识。**

截止现在，我们的『运筹OR帷幄』微信学术群已经有10个，共计3500+人，群成员为全球华人Operations Research, Optimization, O.M., I.E., SCM，Data Scientist，Machine Learning， Computer Vision等方向的researchers，包括常青藤教授，各大Top互联网公司大佬，世界各地知名大学OR相关的硕博，**加群方式详见文章底部**。

前几期的群友交流精华语录受到了大家广泛的关注与好评，我们会为大家持续整理**微信学术群**中的精华语录，请持续关注我们公众号，会不定期举行各种活动哦！

由于微信学术群有很多学界和业界大佬参与，因此我们在文末做了一个关于群聊语录**称呼是否匿名的投票**，希望大家参与给我们反馈意见。也希望大家对【活动】板块提出宝贵的意见，让我们为大家提供更优质的学习交流平台。

下面就来看看我们第四期是整理了哪些**话题**吧

## **2018年8月19日****  时间序列的预测方法**

## **（出自微信群：【3】Global O.R./OM/IE Community）**

**胡-NE-OR-Quantitative Research：**有LSTM用的比较熟悉的朋友吗？有试过金融或经济数据没？谢谢！

**郭-合肥工大-机器学习：**试过，但效果不行。主要是多步预测。

**胡-NE-OR-Quantitative Research：**我看有人说，预测步数少一些会好些？还有人说，LSTM仍然要像传统的VAR或ARIMA一样，输入是平稳的时间序列才好，

**郭-合肥工大-机器学习：**对，越长越不靠谱，其实这种时间序列预测，拿特征工程做效果最好，把时间序列y和对应的特效x做关联。

**Z-Utoronto-OR&ML：**纯粹拿时间序列数据直接往LSTM里面套，最终结果基本就是overfitting。

**郭-合肥工大-机器学习：**这种就是预测效果。
**![](https://pic4.zhimg.com/v2-3d94fe9f941d25e058c839aae00992df_b.jpg)**
**胡-NE-OR-Quantitative Research：**你发的这个图，原文我看过。

**郭-合肥工大-机器学习：**对，效果几乎都是这样。

**胡-NE-OR-Quantitative Research：**我不觉得LSTM会比VAR或ARIMA差，说实在的，本质上都很类似，只是后者只是简单的线性，如果要对等比较，LSTM的输入，也应该是一阶差分后的平稳序列。我自己再多试试把。

**郭-合肥工大-机器学习：**lstm确实未必比var或者arima差，但是和xboost这些方法做预测差一些啊。

**胡-NE-OR-Quantitative Research：**您是说时间序列的预测方面，xgboost一般说来会比lstm好些吗？

**郭-合肥工大-机器学习：**小结一下：预测主要是两大类，一大类是时间序列预测，就是arima这些，它们根据时间序列本身的特性进行预测，目前在时间序列大赛中表现最好的是theta model，是2002年提出来的。另一大类就是基于机器学习的预测，主要是根据外部特征对时间序列进行预测，构造一个特征工程，发现特征与时间序列的关系，进而进行预测。

有说的不对的地方，请大家多多指出。

**S-潍坊-智能调度：**本身特性是指那种周期性、季节性吗？

**郭-合肥工大-机器学习：**长期趋势等。 还有一种分类方法是点预测和概率密度预测，点预测就是只给出一个预测值，概率预测就是给出一系列预测值，完后每个预测值都有个概率，就是个概率分布😂

**S-潍坊-智能调度：**后面那种是贝叶斯思想吗？我记得prml里面好像看过，但是没太看懂，是条件概率吗？

**郭-合肥工大-机器学习：**不是，概率密度预测主要是拿分位数回归做。prml里说的是分类里边的一种生成模型，是说分类当中每一类对应一个概率，和这个概率密度预测有着本质的不同。

**S-潍坊-智能调度：**前面有个贝叶斯做多项式拟合，和这个不一样吗？这个概率预测我应该找哪个资料看一下？

**郭-合肥工大-机器学习：**😂先得看quantile regression，2005年的一本专著。

## **2018年8月19日   组合优化，多面体凸包**

## **（出自微信群：****Global O.R. Optim PhD Community）**

**Chao-Yahoo！-组合优化：**给一个H-polyhedron (say, Ax<=b为input). output这个polyhedron是不是integral的. 用什么程序?

**许-UTDallas-优化组合：**[http://www.mathematik.uni-kl.de/fileadmin/AGs/opt/Lehre/WS1314/IntegerProgramming_WS1314/ip-chapter4_2_.pdf](https://link.zhihu.com/?target=http%3A//www.mathematik.uni-kl.de/fileadmin/AGs/opt/Lehre/WS1314/IntegerProgramming_WS1314/ip-chapter4_2_.pdf)
**![](https://pic2.zhimg.com/v2-dc7ca29c06a080b1f0111c24c7224601_b.jpg)**
**杨-电科院-电力系统最优化：**啥原理？没太看懂。

**杨-华东理工-组合优化：**tdi的验证用程序不太方便吧。

**Chao-Yahoo！-组合优化：**我需要程序...

**王-澳洲国立-优化系统平台：**@ Chao-Yahoo！-组合优化[https://wwwproxy.iwr.uni-heidelberg.de/groups/comopt/software/PORTA/porta.1.3.2.tar](https://link.zhihu.com/?target=https%3A//wwwproxy.iwr.uni-heidelberg.de/groups/comopt/software/PORTA/porta.1.3.2.tar)

**留德华叫兽-海德堡-组合优化AI：**我们组（海德堡大学离散与组合优化实验室）出品

**周-北大-通信：**check一个任意给定的H-polyhedron是integeral，感觉是NPC……

嗯…我猜想除了枚举没有本质上更好的办法。一个通用的check思路是把H-polyhedron转换成V-polyhedron，vertex enumeration程序应该有不少。

**王-澳洲国立-优化系统平台：**我没看源代码，应该是枚举，这方面发展很慢，一些有用的理论都是conjectures，等待大牛们去证明了。

**Chao-Yahoo！-组合优化：**是coNP-hard的。

**留德华叫兽-海德堡-组合优化AI：**Porta貌似是老版本，俩三年前出了个新版本，叫Panda。提高了一些效率，主要是用了分布计算。

## **2018年8月19日求解LP或者MIP的Distributed Method**

## **（出自微信群： Global O.R. Optim PhD Community）**

**鲁-MIT-ORC：**问一下现在解lp或者mip有啥distributed method吗？感觉现有solver都是single machine，虽然barrier method可以multi-thread。

**周-北大-通信：**基本上还没有特别有效的并行算法框架吧。

**留德华叫兽-海德堡-组合优化AI**：[http://ug.zib.de/](https://link.zhihu.com/?target=http%3A//ug.zib.de/) 有两篇reports/paper。

**周-北大-通信：**有一篇2012年的论文 Could we use a million cores to solve an integer program。

**覃-MIT-OM&ML：**@鲁-MIT-ORC 如果限定positive LP的话，allen-zhu/di wang是有分布式工作的，虽然说你主要目标是MIP。

**鲁-MIT-ORC：**谢谢。 Cplex好像说是最多十个machine，之后再多也不会有啥提高了？而且十个machine之内是否会提高还depends on problem。

**覃-MIT-OM&ML：**@鲁-MIT-ORC di wang: [https://scholar.google.com/citations?user=gn2qlUoAAAAJ&hl=en](https://link.zhihu.com/?target=https%3A//scholar.google.com/citations%3Fuser%3Dgn2qlUoAAAAJ%26hl%3Den)

其实我最近有考虑过一点点分布式LP的事情，我们可以私下讨论啊。

**崔SH-OPT：**大家多读gurobi或者cpl

ex关于分布式算法的文档吧，里面写的很清晰。

[https://www.ibm.com/support/knowledgecenter/SSSA5P_12.8.0/ilog.odms.cplex.help/CPLEX/UsrMan/topics/parallel_optim/distribMIP/01_distributed_mip_title_synopsis.html](https://link.zhihu.com/?target=https%3A//www.ibm.com/support/knowledgecenter/SSSA5P_12.8.0/ilog.odms.cplex.help/CPLEX/UsrMan/topics/parallel_optim/distribMIP/01_distributed_mip_title_synopsis.html)

## **2018年8月20日  传统统计中的方法 VS 机器学习**

## **（出自微信群：【3】Global O.R./OM/IE Community）**

**胡-NE-OR-Quantitative research ：**Tianqi & Carlos: XGBoost A Scalable Tree Boosting System，关于XGBoost，这篇论文如何？

**郭-合肥工大-机器学习：**这是陈天奇的论文啊，当然好，方法就是他创的

**张-清华IE-OM：**机器学习和统计中用到的一些方法究竟是啥区别啊？

**胡-NE-OR-Quantitative research：**看来大家都有这方面的困惑

**小陌-硕士-山理工-无人驾驶：**嗯？统计学习中的方法和机器学习中的方法难道不一样吗？？比如训练、应用的时候，还会有很大的差别吗？以前从没有考虑这个哎。

**刘-CQU-智能调度与数据挖掘：**传统的机器学习就是基于数据的统计模型拟合问题，Google的工程师这么认为的。

**郭-合肥工大-机器学习：**不一样，统计的模型理论上都需要拿极大似然这一套计算出来参数，而且要保证参数有好的性质，机器学习是从损失函数的角度对参数进行求解。但是呢，又有一部分模型，既可以拿极大似然求解，也可以拿机器学习里边损失函数求解，比如回归，logistics 回归。但是共同的这一部分很小，但是影响力有大。比如线性回归解决不了非线性拟合，就发展出来多项式回归，样条回归，非参数回归。但是机器学习为了拟合非线性现象，发展出来神经网络。

**刘-CQU-智能调度与数据挖掘：**概率模型，那个是统计学上的贝叶斯学派的方法，频率学派又有其他方法。机器学习EM算法、K-means算法和逻辑回归的损失函数都用到了极大似然估计。

**郭-合肥工大-机器学习：**是都用到了，但是不是全部。它是通过极大似然得到损失函数，再通过优化算法求解，而不是拿极大似然一步到位。

还有一个是，机器学习为了解决方差和误差的权衡，发展出VC维理论。而传统统计模型则是把重点放到要估计的参数的性质上了。

**胡-NE-OR-Quantitative research：**它是通过极大似然得到损失函数，再通过优化算法求解，而不是拿极大似然一步到位”——这里的“拿极大似然一步到位”，你指的是传统的统计吗？

**郭-合肥工大-机器学习：**对**。**这里的一步到位不是真的就一步。

**胡-NE-OR-Quantitative research：**它凭什么一步到位？咱们知道，很多函数其实是非凸？非凸，它如何做到一步到位。

**郭-合肥工大-机器学习：**这个都已经不算纯统计模型范畴了，算机器学习了，具体我也说不清。

**胡-NE-OR-Quantitative research：**我觉得这个是核心。也许前提假设和方法论的根本区别在这儿？

**郭-合肥工大-机器学习：**我刚才那句话是说，最早我们没有那么多优化算法，传统统计模型都是拿极大似然算参数，如果很复杂，这个时候统计方法是无法求解的。现在界限很不清楚了，都叫统计学习理论了。有些问题也是直到80年代以后才慢慢解决的。

只要模型涉及到优化算法，一般都不能算纯统计了，因为纯统计模型一定要保证参数的各种性质，比如无偏性啥的，一旦你拿优化算法把这个参数求出来，这个性质谈都没法谈了。

**胡-NE-OR-Quantitative research：**那我要求LSTM的输入时间序列必须是平稳的，是不是也是犯了类似的错误？

**郭-合肥工大-机器学习：**LSTM是标准的机器学习模型。

**胡-NE-OR-Quantitative research：**时间序列回归，要求输入时间序列必须具有平稳性，这个是传统统计的要求，但我之前只是纯粹为了对比两个模型，LSTM和VAR。因为VAR要求输入为平稳序列，所以我觉得要公平对比的话，LSTM的输入也必须为平稳序列

**留德华叫兽-海德堡-组合优化AI：**极大似然估计最后不就是个优化问题么？

**郭-合肥工大-机器学习：**从回归角度看是这样，等价的，但是其他模型都不是这样了，两者出现分歧了，但是回归的这个等价影响非常大。反应出一个非常本质的东西。但是存在很大不等价的，但是影响又是很小。如果大家看了多项式回归，样条回归，或者其他半参数模型，你就发现回归里边的那个等价有多大的意义了

**saraki-潍坊-智能调度：**李航的那本书挺好的，全书贯彻三个东西，模型 策略 算法。给出模型和策略，就给出目标函数，通常是无约束优化问题，用普通一阶梯度法就能求解，但是分析了目标函数之后，人们又针对各种问题，给出不同的优化算法，em，smo，但是还是基于梯度法。模型思想要很重要。还有an introduction to glm，prml都挺不错。

**小陌-硕士-山理工-无人驾驶：**那李航老师的《统计学习方法》还适合机器学习入门吗？？

**郭-合肥工大-机器学习：**适合。

## **2018年8月19日    彩蛋：滴滴数据公开了**

## **（出自微信群： Global O.R. Optim PhD Community）**

**石-顺丰科技：**滴滴在kdd发的，赞！
**![](https://pic2.zhimg.com/v2-4b209ad21a1b14efeecce31b49669081_b.jpg)**
**L-大连理工-组合优化：**滴滴数据公开了？在哪里能down？

**叶-滴滴：**follow这个步骤就可以申请。
**![](https://pic3.zhimg.com/v2-a5931d46df08d9fa5e1b9b2d05fc9b7a_b.jpg)**
近期收到小伙伴的反馈，建议我们增开【数据科学|挖掘】微信学术群，我们后面会尽快上线。欢迎大家在**文末留言**想和群友交流的**OR/AI方向**，我们会根据大家的反馈增开微信 **| **QQ学术群 

> **文章来源申明：**本篇文章由『运筹OR帷幄』活动版块原创发起，未经授权，禁止转载。

**【数据科学】**版块现招募1-2名责任编辑，需要爬虫，数据分析，可视化至少掌握其中两项技能者优先。如果对这个板块感兴趣，可以点击下图，记得备注【DS】板块哦！期待你的加入。

**【DS】**主编简介：德国慕尼黑工大AI博士，前Amazon数据科学家，现慕尼黑阿里云资深DS。

**『运筹OR帷幄』**长期招收志愿者，如果你有一技之长，爱分享，欢迎加入我们。点**击下方图片**即可了解详情！（如果对活动板块感兴趣，欢迎备注【活动】哦！）

[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486166%26idx%3D1%26sn%3Da598e1475ae4f91afb7c33114ee61a85%26chksm%3Df9706d88ce07e49ec590fc2589d251bbf2b7d605cd127b1a0bdff61b8bad985eddf42365d823%26scene%3D21%23wechat_redirect) ：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)
[点击查看【骥寻伯乐】板块说明及投稿方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486137%26idx%3D2%26sn%3D24e9487d15a8c590645a4d83950c8f20%26chksm%3Df9706de7ce07e4f1507edd01511920bd8f0a3a80384aef8ee8768b68931f872bc6cff5d3485f%26scene%3D21%23wechat_redirect) ：
![](https://pic3.zhimg.com/v2-88ba2272456e93bb2f6b9bf2d55ef1d2_b.jpg)
扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)

