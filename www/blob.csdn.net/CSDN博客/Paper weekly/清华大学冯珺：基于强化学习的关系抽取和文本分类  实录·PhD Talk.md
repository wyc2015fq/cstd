# 清华大学冯珺：基于强化学习的关系抽取和文本分类 | 实录·PhD Talk - Paper weekly - CSDN博客





2017年11月23日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：2774









本文为 11 月 22 日，清华大学计算机系——**冯珺**博士在第 18 期 PhD Talk 中的直播分享实录。

随着强化学习在机器人和游戏 AI 等领域的成功，该方法也引起了越来越多的关注。本期 PhD Talk，来自清华大学计算机系的博士生冯珺，为大家介绍了如何**利用强化学习技术，更好地解决自然语言处理中的两个经典任务：关系抽取和文本分类**。 




在关系抽取任务中，尝试利用强化学习，解决远程监督方法自动生成的训练数据中的噪音问题。在文本分类任务中，利用强化学习得到更好的句子的结构化表示，并利用该表示得到了更好的文本分类效果。**本次分享的两个工作均发表于 AAAI2018**。




**>>>>**

** 获取完整PPT和视频**




关注“PaperWeekly”微信公众号，回复“20171122”获取下载链接。




![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDicclx90jibSctMWiaLicXAibsD8sfamKNos94BFCt8gq3Hk30hmuAO0ktV3Jg/?wxfrom=5&wx_lazy=1)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccxfPUQFqicE46QNvVz1FsOXLNsVmyVloXdYrjuyFZA5Iibehp1CKcwBbQ/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccvje6muhDnia8icUgQ1icHfsnU5a5jX9Bd87wlayUwwFayuicGGZdczJjZA/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccKPK4xbWTqOH3UcONv4gIuC1mACks8GMlqFgNhFpR5yoJGP5cw4zzTA/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDicc1qRNWOOlzuTUludYVibhOVXHG9wJ9c3wCTAXTwGlUzcy1sv7iaa8SnPQ/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDicc3xiaN5ibd4u7icCUsCLicXibhNYqyPFtVVgibmGkLS5icG4lyOX34NMtjdPew/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccEniaG3byHEax3Y4cOhicnlhsMlvHegF0G4s18lgrX11W6G2QibFYXNV6A/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccCDMRhtuTRYyyiasjeqntFBESLhLXeicQSiaibEcOouRfF5DT29WfUWbMyg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDicchNqPx7stJoM9Y9cwLFTYDgBFIYeuNibfSLSya7LA3QpEpymv8jBZibibA/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccfsDznCSUslO8K69o6j2dDBWnUrmGHmBGGic1lbd4RLfXXBpaOXXCCicQ/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDicclsGzAQtIaSbsxtEslTRIKlsD0FGoyncaumcn6FQ9S4CPVcQnKgzbxA/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccwN3BLs2PlJrU3iaCV5ORtju4KIJt6ULT1qyuy6iakHUc8WlbX7mfz9FQ/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnlMRddFSicHu1qEmNic9yDicczCBgaS6yUMMHPNialtuJicu6pRM92AabXoNLtMDRPAk3r38WysUvmFSw/?)




我是彩蛋

** 解锁新功能：热门职位推荐！**




PaperWeekly小程序升级啦




**今日arXiv√猜你喜欢√**热门职位****√****




找全职找实习都不是问题



** 解锁方式 **

1. 识别下方二维码打开小程序

2. 用PaperWeekly社区账号进行登陆

3. 登陆后即可解锁所有功能




** 职位发布 **

请添加小助手微信（**pwbot01**）进行咨询




**长按识别二维码，使用小程序**

*点击阅读原文即可注册




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)





**关于PaperWeekly**



PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入
 PaperWeekly 的交流群里。





![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)




