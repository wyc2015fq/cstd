# 谷歌大脑QT-Opt算法，机器人探囊取物成功率96%，Jeff Dean大赞 - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

用于工业生产中的普通机器人，往往只会“给鸡抓鸡，给狗抓狗”，像一个对生活失去了向往的流水线工人，重复着日复一日不用动脑的苦劳力。

但，谷歌大脑昨天发了一个新的算法，让这些穷苦的机械臂开始从事“脑力劳动”：

**从一群物品中，抓起需要的东西**。

比如从拼好的积木组合里，抓单个积木：
![](https://pic1.zhimg.com/v2-211cbbcbb7e89ff2b728081f07fb575c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)
所使用的方法是深度强化学习，将大规模分布式优化和新型拟合深度Q学习算法——**QT-Opt**相结合，来让机器人从过去的每一次训练中学习，获取经验。

## **7个葫芦娃的4个月修炼**

这次训练的参与“队员”有7名，他们用10个GPU开始训练：
![](https://pic4.zhimg.com/v2-dccdfcd583b5c521d7c2a145248eaf27_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1280' height='718'></svg>)△ 一根藤上7个机器人
每个机器人由一个带双指夹具的机械臂和一个RGB摄像头组成：
![](https://pic1.zhimg.com/v2-ebef2042c833c0edb9e5bad247c2eebc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='670' height='1194'></svg>)
为了让机器人尽快get新的探囊取物技能，谷歌大脑的工作人员准备了1000样不同的物品用来训练：
![](https://pic1.zhimg.com/v2-97ac3e4f14a7b7b49fb54a3a27ca0d44_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)
仔细看一眼，其中包括各种形状、大小、材质不一的物体：
![](https://pic4.zhimg.com/v2-63a27057fdc3471e262b333aa2ed7dbb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='590'></svg>)


![](https://pic1.zhimg.com/v2-28a844bc05092e29e6fad4122ad4ea30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='576'></svg>)
训练的过程首先从工作人员手动设计的策略开始，逐步切换到深度强化学习模型。

从论文上发现，原理大概是这样的：
![](https://pic3.zhimg.com/v2-90f2575481795468e44476ea23bd98c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1029' height='607'></svg>)
## **学有所成**

经过4个月的训练后，7位机器人迎来了他们的“考试”：成绩不错，在**700次试验**中，机器人找东西抓起来的**成功率高达96%**，比此前**监督学习方法78%的成功率**提升了很多。

Jeff Dean觉得它们棒棒的：
![](https://pic2.zhimg.com/v2-eb8b6a6e296a1e6787fa63081fc81065_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='749' height='721'></svg>)△ 凌晨4点的北京，Jeff老师发推夸奖自家机器人
除了提升准确率之外，经过QT-Opt算法训练过的机器人还主动get了4个新技能：

## **会破除阻碍**

如果目标物体和其他东西连在一起，机器人会主动把它分开然后抓取。

比如前面示范的抓积木，机器人可以把影响自己发力的其他积木推开，再抓自己需要的那块积木。
![](https://pic3.zhimg.com/v2-1425af6f8d7fe55865e60999cb25e4ca_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)
## **“筷功”强**

如果碰到难抓的东西，比如外形奇特或是外表光滑的物品，机器人会分析角度，重新定位，然后牢牢抓住不松手。
![](https://pic2.zhimg.com/v2-38ef143163aef1e95c9b35ae0c7c6045_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)
## **随手抓也要分析挑选**

如果机器人一下子抓住了一堆东西，它可以自己选出需要的物品，在举起手臂之前牢牢的抓住它。

## **抢我的一定抢回来**

如果人为的把机器人已经抓起来的物体拿掉，它还会锲而不舍的再抓一遍：
![](https://pic4.zhimg.com/v2-ef7f4c10bd41650bf293137b7c98fdb3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)
重要的是，**以上这些技能都不是人为设置的，均是在训练过程中，机器人自行get到的。**

最后，谷歌还提供了一个视频，来讲述7位机器人盆友的心路历程：
![](https://pic4.zhimg.com/v2-7a02a1cd4865333fa49ba101941f8cc0.png)https://www.zhihu.com/video/996005603889672192
最后，附论文传送门~

**QT-Opt: Scalable Deep Reinforcement Learning for Vision-Based Robotic Manipulation**

作者：Dmitry Kalashnikov, Alex Irpan, Peter Pastor, Julian Ibarz, Alexander Herzog, Eric Jang, Deirdre Quillen, Ethan Holly, Mrinal Kalakrishnan, Vincent Vanhoucke, Sergey Levine

谷歌博客地址：
[https://ai.googleblog.com/2018/06/scalable-deep-reinforcement-learning.html​ai.googleblog.com](https://link.zhihu.com/?target=https%3A//ai.googleblog.com/2018/06/scalable-deep-reinforcement-learning.html)
arXiv：
[Scalable Deep Reinforcement Learning for Vision-Based Robotic Manipulation​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1806.10293)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


