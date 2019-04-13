
# 实录分享 | 计算未来轻沙龙：让机器拥有理解语言的能力（视频 + PPT） - Paper weekly - CSDN博客


2018年10月15日 12:58:45[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：238


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

10 月 13 日（周六）上午，PaperWeekly 携手清华大学计算机系推出了**计算未来轻沙龙**第二期线下活动——**让机器拥有理解语言的能力**。来自中国科学院自动化研究所、国防科技大学计算机学院和清华大学计算机系的三位博士，**系统而深入地分享了他们各自在机器阅读理解领域的最新研究进展。**

本文将独家分享本期活动的**完整****视频回顾**和**嘉宾课件下载**。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnnE9qUvB5ztlEukASviax5ibla0IbZXXhpNqR4aEF7PTiaR1UTM9qoYYkO4iaYEtCqBR5ZOtsH8tpIoA/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pDIiaV5g92XZhjbfhrqLmMSHSndlVf2JxWeLQM0SPnt9RPia0TtMAXc3w/640?)**王炳宁 / 中国科学院自动化研究所博士**
王炳宁，中国科学院自动化研究所博士毕业生，主要研究方向为自然语言处理，机器阅读理解。在 IJCAI，ACL 等人工智能、自然语言处理顶级会议上以第一作者身份发表论文四篇。现负责搜狗立知项目阅读理解部分。
**机器阅读理解研究进展概述**
机器阅读理解是现今问答技术发展的核心之一。在本次报告中，我将从认知的角度介绍机器阅读理解的发展以及和人类阅读理解的关联和区别。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyZgKKKxI6x6J0Uv8Xo5rExZLW8ZOo8IPKOX8PgPJCrMMRqFyzhurQfg/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyKUlcqjzQDq4d88vrYibYpy886gHKDEwSqa0FncTIURDRzS4lvd7Zp4w/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyetsUZXqyyGwNMcqJNYlEx56XybVRwDr84JOJZI2trKaIBcQ5awmNbg/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyyuYkCOwm49Sv0uVqkey99Y1nMWL1QiaAzlqM5FBbGkgJCjRjMNHjL9Q/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyancSokGlDb1cZ9h1ibA0JnVjicvGA0nzOonQar7xc4G10lHbCuHc2fkg/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyX9DeqAb5DChmNphGfyxiarxd2BrVnvsqCULJkyw8TPMdxRYDRwc1R7w/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyQvFfShibCdpesaTQ7ZJr7q0FCpATnb41Rukiboq0M5asDpEcUCt0nWxA/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyfpRIBTjlpApyXnn1wG8JAibcjyvx62b5eBQjWRCmfptohGIeHmFrpSA/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pmWN4vEyZBhRNfzjQ3cTNfYzfWoOZWowbLL2u8kxnIUj225exgqiceFQ/640?)**胡明昊 / 国防科技大学计算机学院博士生**
胡明昊，博士在读，本科毕业于国防科技大学计算机学院，研究领域为自然语言处理和机器阅读理解。博士期间在 IJCAI、EMNLP 等国际顶尖学术会议上发表文章。
**针对机器阅读理解的高效性和鲁棒性研究**
近年来，抽取式机器阅读理解任务取得了显著的进展，最顶尖模型的性能甚至超过了人类。然而，当前方法却存在着效率低下、易受对抗样本攻击和无法拒绝给出答案等问题。在本报告中，我们将介绍如何利用知识蒸馏技术来提升阅读理解系统的效率和鲁棒性，并且讨论一个阅读+验证的框架来检测问题是否可回答。我们期望上述探索能够进一步增强阅读理解系统的实用性。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyDwFwC4cibyMkPM1r3KEpZdFVqjZkuwytMzukfZ59B39bZEWN55Qgictw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUykzMKFPl7805zwXKEtYicmuHY1pCslibAtvd71ibKJIoIJtE24Esr70c3A/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyjcFmaicDiaazjDc7u6MTYNPiaFdoCFDCtHAZteHw2QueFviazb2DvJlA7A/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUy1IYaeOibX3zpq57SCJ4SeEDRlkrPSCSD0vQbACYOtKDc5uktiaKWRfCw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyZbtwfkiaqRoOYLQRnrzsZ9p9BSmFRtaCalS0Mv7Q8vDrns6icgCqRwUw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyB5oW2ibML0Yn79OEWQb3juxqZMiclFoe8tBjUjmu4iaSuRfl8oGXPQmQw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyAqSXicyeX2XRKBXmruBPiaA8PGM2pblfCDCneNp0mv1lnK3oU5kfH08Q/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyEQz1DLJcCln1mG2Luj3WoQN93lTJ8icFzGGkk0uLQUIOFr7BAC2LASw/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pMhCAjCIyhgfGJB6ia7otSnswOVEZQZ6BicFLSZ5rmbbhB8N4RzicOicBKg/640?)**林衍凯 / 清华大学计算机系博士生**
林衍凯，清华大学计算机系博士生五年级，来自清华大学自然语言处理组，由孙茂松教授和刘知远副教授共同指导，主要研究方向为知识图谱表示、构建和应用。目前已在人工智能、自然语言处理等领域的著名国际会议 IJCAI，AAAI，EMNLP，ACL 发表相关论文多篇，Google Scholar 引用数超过 700。
**考虑噪音过滤的开放域问答**
开放域问答着眼于从大规模的无标注文本中提取出给定问题的答案。现有开放域问答系统一般采用机器阅读理解技术对问题检索到的文本段落进行分析来提取出答案。但是，这些方法无法有效综合考虑不同文本段落的信息，还会受到文本段落中噪音的影响。此次报告，我们将介绍我们根据人类浏览、精读和总结的阅读模式提出的开放域问答模型，并介绍我们如何进一步利用有监督的数据帮助模型进行噪音过滤。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyTJW51mhibCQwGJUlBmXyAic6OSwnDZB3r5LEONTugx5ZriaDiastAyOnxg/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyiaL3I4uljDv33Zg3JCjM85lMRUByvicNpQJreribVWtcfPp8VGIJU7WYA/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyqgEpBDptia2qplUKtmmwZicm4qFX6L8eAvB4iarulINrZEqh3taicz7ctg/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyoknTMEZsjJ6SKPZ94bNENTNEibZGVNcicb6Zplr5ASSrfYgjGcnuLFnA/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyHfUJVZIyHtUBYk5nUSDcichMEvyOT128BntI1olxgzib2mBNSrIiaaXHw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUy9ZAric92goCKFgbun41bx32ZcAlNopicXf3sGfgpZ8s47CzWvwRKJYOA/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyEDaH9fOUKibibncxqB0kLqqcRGXdt6LPuPFH7ziaWubQQia4Lz6K3enficA/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl4n8MC2HRkqEvClqEyrEUyLUVecLHSHGku22VEsoWrCSXx0KHNBqQxhbYY8gVmDErsy1VYsr5Gcw/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnnE9qUvB5ztlEukASviax5ibhCRbLjb9pvYeAPTGbV8QMQpc6at4VtcMzQDFdAqGadNByajibovDRBw/640)
**1 / 扫码关注**
长按识别二维码，关注PaperWeekly
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnnE9qUvB5ztlEukASviax5ibhoNicrX9IMkibAgP6fZpvXyoWIsffPePf1H99CBhCvewzhtuib6dpiausQ/640)
**2 / 回复暗号**
在PaperWeekly微信公众号后台
**回复暗号****181013**
**即可获取视频和嘉宾课件**



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向）
• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接
• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志

**📬 投稿邮箱：**
• 投稿邮箱：hr@paperweekly.site
• 所有文章配图，请单独在附件中发送
• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通



🔍

现在，在**「知乎」**也能找到我们了
进入知乎首页搜索**「PaperWeekly」**
点击**「关注」**订阅我们的专栏吧


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 获取最新论文推荐


