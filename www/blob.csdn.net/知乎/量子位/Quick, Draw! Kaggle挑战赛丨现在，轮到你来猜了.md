# Quick, Draw! Kaggle挑战赛丨现在，轮到你来猜了 - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

还记得用Quick, Draw! 或**猜画小歌**小程序给谷歌AI画过的那些四不像的涂鸦么？
![](https://pic3.zhimg.com/v2-cbf341f40f083db2491b4284fef77992_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='529'></svg>)
现在，风水轮流转，你画过的四不像，要自己再去把它认出来了。

谷歌AI推出了一项新的Kaggle挑战赛：

**识别那些Quick, Draw! 中的涂鸦**。

这下，自己挖的坑，要自己填了。

## **Quick, Draw! 涂鸦识别挑战赛**

Quick, Draw! 是一个“你画AI猜”的小游戏，也就是猜画小歌的前身。

这项新Kaggle比赛识别的是Quick, Draw! 游戏的5000万张涂鸦，它们由来自全球各国的超过1500万玩家贡献，画得大多是一些苹果啊，浴缸啊，计算器啊之类的日常用品。
![](https://pic1.zhimg.com/v2-d5f5e20aa873ba32fd0f46842efc311c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='624' height='642'></svg>)
每个种类里都有N张涂鸦，画风大概是这样的：
![](https://pic1.zhimg.com/v2-fe5a7b97adbfd82244db59b909635244_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='658'></svg>)
你还可以看到每一幅涂鸦作品的来源，比如下面这个苹果，是一位美国用户在去年3月19日画的。
![](https://pic4.zhimg.com/v2-8b2c24aaa977b177de1882e71c89c503_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='279'></svg>)



所以，这项比赛就是识别这些奇奇怪怪的涂鸦是否为苹果了。

另外，并不是所有的涂鸦都和上面的苹果那样网站，数据集中总共分了4类标签：

> **正确**：用户画完之后，AI识别了出来。
**正确但不完整**：用户还没画完呢，AI就识别出来了。当然这种情况下系统设定不会让用户接着把涂鸦画完，所以只收集到了画了一半的涂鸦数据。
**正确但AI认不出来**：用户画对了，但是AI认不出来的涂鸦，于是用户之后再补充一些细节试一试。
**不正确**：用户画得压根不是同一个东西。

谷歌在存储这些数据的时候，记录了每张涂鸦的每一个笔画，或许你可以利用这一点更快的识别出来。

## **评比方式**

这场Kaggle挑战赛的评分依据是一个名为**MAP@3**的数据，它的计算公式长这样：
![](https://pic3.zhimg.com/v2-ff1becf9795e3ec077de54bb43620922_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='470' height='148'></svg>)
其中，U是指有效涂鸦的编号，P(k)是精度，n是每幅涂鸦的预测数字。

针对测试集中的每幅涂鸦，参赛者最多预测3个结果，以英文单词形态展现，并用下划线代替空格，比如“过山车”roller coaster应该写成roller_coaster。

## **比赛规则**

**时间**

这场比赛在3天前的9月27日就已经开始了，11月27日报名截止，12月4日停止提交。

当然，所有的时间都是按照UTC 11:59PM来计算的。

**奖励**

最终，获得前三名的团队会有奖金，第一名的团队有12000美元，第二名奖励8000美元，第三名奖励5000美元。

**组队参加**

本场挑战赛可以组队参加，每队不超过8个人，注意组队的时间必须在11月27日前。

**提交方式**

每个团队每天最多可提交5个参赛作品，最终评分的时候只能选其中两个。

目前，这项Kaggle挑战赛有103个队伍参加，其中多数都是都只有一位队员。现在最好的成绩是0.820，他尝试了7次。

现在看来，竞争还不太激烈，感兴趣的读者可以进入传送门参赛。

祝抱走12000美金。
![](https://pic1.zhimg.com/v2-cdd292a82ce37ed7814ef478a5a9b1cc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='334' height='326'></svg>)
## **传送门**

谷歌博客：

[https://ai.googleblog.com/2018/09/introducing-kaggle-quick-draw-doodle.html](https://link.zhihu.com/?target=https%3A//ai.googleblog.com/2018/09/introducing-kaggle-quick-draw-doodle.html)

Kaggle挑战赛地址：

[https://www.kaggle.com/c/quickdraw-doodle-recognition](https://link.zhihu.com/?target=https%3A//www.kaggle.com/c/quickdraw-doodle-recognition)

“猜画小歌”数据集：

[https://quickdraw.withgoogle.com/data](https://link.zhihu.com/?target=https%3A//quickdraw.withgoogle.com/data)

—**完**—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




