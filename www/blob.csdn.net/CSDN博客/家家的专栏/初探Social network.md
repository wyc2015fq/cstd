# 初探Social network - 家家的专栏 - CSDN博客





2012年11月06日 16:48:02[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1396
个人分类：[图像处理算法](https://blog.csdn.net/yihaizhiyan/article/category/715109)









前几日，看到sina微博中的几个CV界的人在讨论这篇有关Social network的文章~

“Discovering social circals in Ego networks”  (stanford).

所以down下来，粗读一下~



人的社交网络是很复杂，也需实时更新，很难去自动的组织他们。

在Google+上的circles， Facebook上的list，qq上的friend分组等，这些社交网站都允许用户分类friends成为circles~

目的：试图自动识别用户的社交圈子（Social circles）。

问题定义：把这种问题，看为一个多成员节点的聚类问题，朋友间构造一个连接网络。

解决方法：联合网络结构和用户配置文件信息来构建模型来检测这些社交圈子。

                    对每个社交圈子，我们学习它的成员和社交圈的特定用户配置文件的相似性度量。

                   节点成员可归属于多个圈子，所以是检测重叠和分层嵌套圈子。

 社交圈子复杂，所以也不容易建模，存在重叠，嵌套等情况~

![](https://img-my.csdn.net/uploads/201211/06/1352192094_5435.png)





