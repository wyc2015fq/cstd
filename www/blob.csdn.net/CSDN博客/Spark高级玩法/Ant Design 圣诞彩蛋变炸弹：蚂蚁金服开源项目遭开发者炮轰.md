# Ant Design 圣诞彩蛋变炸弹：蚂蚁金服开源项目遭开发者炮轰 - Spark高级玩法 - CSDN博客
2018年12月26日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：72
转自：云头条。
参考资料：微博网友资料、知乎、Github、界面
Ant Design是一套由蚂蚁金服开发的企业级开发UI组件库，并已经开源。今天，由于在该框架中设置了一个未事先告知的圣诞彩蛋，且无法手动禁用，开发者社区对该项目发起了连番指责。
该彩蛋的实现方式是，在圣诞节日期间，使用Ant Design组件库的网页与应用，其按钮样式将会出现白色积雪效果。如下所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHAozxGO35Psgppr2KUCib4d5G9Vs7iakOVfs8MD9kbovVF6RPuV3JFeFURaic1iaE2eCR6yT024GnnDEg/640?wx_fmt=png)
并且所有按钮都带有“Ho ho ho！” 标题：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHAozxGO35Psgppr2KUCib4d5eLjGcfbjt4X06ReuB59tH1Vf5WbND6k6OVdTDVhseiaUGRO0AtJ7KHA/640?wx_fmt=png)
这个未事先告知的彩蛋在今日实现后，对开发者造成普遍困扰，有程序员称“以为遇到了BUG”，也有人表示由于该按钮，而遭到客户大量投诉。甚至有开发者因从事政府软件项目，工作受到严重困扰。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/tJ5NYyM9MHAozxGO35Psgppr2KUCib4d5b3gHdPqic1WFibiaXskoqKM6dsB2JNSVs7r5wo367QXLRK0klmI4CZvUw/640?wx_fmt=jpeg)
有开发者质问，为什么不经过允许就在按钮上添加彩蛋。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHAozxGO35Psgppr2KUCib4d5H43JvGpEV9wiaTDnzXYib2icKnTMnlXn08L6Ph22agW7U7BLudibBW5hPw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHAozxGO35Psgppr2KUCib4d5ebCZm5lbC1GoTia03iacGrDch3slb5EDiaA4twbdILEiaHxrXoHC5Giaic3Q/640?wx_fmt=png)
**知乎网友@顾轶灵的评论很到位**，在此分享给大家（云头条已获作者授权，链接：https://www.zhihu.com/question/306858501/answer/559210269）
也有开发者指出，在一个主要面向企业客户的开源组件库中擅自添加彩蛋，Ant Design团队有失专业。有人称，“这个彩蛋推翻了它曾经留给我的这些正面印象，至少我对它的信心没有了。”并质疑开发团队对开源项目的责任意识。
有开发者查询代码库发现，有关该彩蛋的代码在数月前就已经提交。
在开发者提出激烈批评后，Ant Design的一名开发者于今日在开源社区GitHub发布回应解释了该事件，该开发者承认自己在9月10日提交了该彩蛋并设定于12月25日触发，并表示该行为是自身一意孤行且愚蠢的决定，给大家造成了不良影响。该开发者同时给出了修复方案并承诺未来不再会有类似问题发生。
此次事件似乎反应了开源程序项目的一个典型困境，其支持者声称由于开源软件会因开源而公开透明，并因持续受到监督而安全。
但在实际过程中，多数人并不会审读代码。即使有人发现问题，也因为不受重视而无法解决，最终就有可能导致严重的后果。
**开发者@afc163 在Github回应：**
关于 Ant Design 圣诞彩蛋，起源自 2018 年 9 月 10 日我的一次提交：00aebeb 。代码实现会在 12 月 25 日当天给所有按钮添加积雪效果，并增加 Ho Ho Ho! 的浏览器默认提示信息。这完全是我个人的一意孤行且愚蠢的决定，是我的错误给大家造成了不良影响，非常抱歉。
我们已经回滚相关代码并发布了修订版本：3.9.4、3.10.10、3.11.6，各位请更新至相应的版本即可。使用了语义化版本的直接重新安装 node_modules 并重新下载即可。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHAozxGO35Psgppr2KUCib4d5tC7AlxTib6CNmhvibJA6H0pcDibGwkbLlOjK696q3lePpPPXpKIsMFd8w/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
