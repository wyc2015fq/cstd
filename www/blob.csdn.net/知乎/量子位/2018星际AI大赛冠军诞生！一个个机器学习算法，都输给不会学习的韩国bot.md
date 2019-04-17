# 2018星际AI大赛冠军诞生！一个个机器学习算法，都输给不会学习的韩国bot - 知乎
# 



> 夏乙 问耕 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-767ff96fa0dea56f2b3bef68cad3ab54_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='142'></svg>)
刚刚，星际争霸AI大赛新王诞生！

二十多个掌握了机器学习技能的AI，全都在一个不会学习、只会基于规则机械行动的bot面前，俯首称臣。

包括三巨头之一Yann LeCun治下的Facebook AI团队。他们的AI获得亚军，但在与新王的100回合大战中，战绩是83负17胜，劣势明显。

其他AI更不是对手。

新霸主与全部对手的2600场厮杀中，胜率达到了95.91%。而往届比赛中，冠军胜率最高的一次，也不过89%。

到底是谁？哪个团队的人工规则，能如此碾压人工智能？

答案是：**赛达（SAIDA）**。
![](https://pic2.zhimg.com/v2-2c03cedbf24ad969017767279d141cd9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='313'></svg>)
**△** SAIDA对战第二名CherryPi

来自三星，首次参赛的赛达，完全基于规则，在这个机器学习大爆炸的时代，一个如此“复古”的系统，俾睨一众AI。

赛达之所以如此厉害，一个重要原因是，背后有韩国职业星际选手助力。要知道，星际一度被称为韩国的国技。

这次星际争霸AI大赛的组织者戴夫·丘吉尔（Dave Churchill）总结说，今年的比赛成了两家巨头之间的争夺。

第一名三星赛达，代表顶级人类玩家的经验总结；第二名Facebook CherryPi，有顶级程序员坐镇，3位顶尖bot作者都在麾下。

目前，三星职业选手组更胜一筹。

这还不是赛达的全部成就。在另一个星际赛事——星际AI锦标赛（SSCAIT）上，它的ELO等级分也排在第一名。
![](https://pic1.zhimg.com/v2-6ca5cef4d1aec5697571482da99c6ae8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='846'></svg>)
总体来看，它已经是地表最强星际AI了。

我们来认识一下这匹黑马。

## **基于规则的复古新秀**

这次大赛还是基于星际争霸：母巢之战（Starcraft Broodwar）。

赛达使用人族（Terran），以稳定的防守优先战略开局，到中期过后，它会寻找最佳rush时机，一波强攻击败对手拿下比赛。
![](https://pic2.zhimg.com/v2-70239d6dcedf83b29e2ce6b77f5cca5d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='452'></svg>)
**△** SAIDA对战画面

和现在主流方法不一样的是，赛达没有用机器学习方法。

三星的程序员们尝试了用卷积神经网络和编码器-解码器从游戏视频中学习战斗时机，也尝试了用多智能体强化学习算法来控制单位……然后决定，在参赛算法里不用这些东西了。

这些研究当然也没有白费，他们把研究结果写进论文，投稿到了AAAI 2019。哪位朋友见到这篇论文的预印本，欢迎通知我们，和更多量子位读者分享。
![](https://pic3.zhimg.com/v2-c4de2c1a22b6592e464d30a37622a44a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='448'></svg>)
**△** SAIDA对战画面

我们说回参赛版本的赛达，它都是手工编写的基于规则的算法，一共49702行。

不过，这并不是说它的打法就是一成不变的。它所用的策略不是一开始就预设好的，而是会根据侦查和扫描收集的信息来构建。

除了选择策略的程序，赛达还包含用来控制单位和建筑物的有限状态机，用来寻找建基地位置或者敌人基地的几种搜索算法。

那么，人类程序员们是如何给这个算法制定规则的呢？

背后有韩国职业星际选手的帮助。戴夫说，把大赛官方提供的样本程序UAlbertaBot和专业软件开发者、韩国职业星际选手放到一起，就有了赛达。
![](https://pic2.zhimg.com/v2-1ad8f7567f831a154590a9c2ec695a79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='835'></svg>)
## **距击败顶尖职业选手还有一两年**

赛达的源头，可以追溯到2017年三星SDS举办的算法大赛。

当时的主题刚好是星际。比赛中成绩不错的程序员们惺惺相惜，向公司提出要联手打造一个超级厉害的星际AI。
![](https://pic4.zhimg.com/v2-898c66f67eb959e1661663d90a8e363f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='683' height='384'></svg>)
于是，2018年1月，赛达诞生了。

现在，这个诞生还不到一年的新秀不仅是两大星际AI赛事的第一名，还已经和人类业余玩家、职业选手都交过手。人类业余玩家已经不是它的对手。

虽然赛达目前，暂时，还打不过职业选手，但开发团队认为“已经不远了”，内心充满了希望。

他们说，现在的星际AI已经达到了高端业余玩家的水平，战网天梯1800分左右的那种。再有一两年，就能击败战网3000多分的最强7位职业选手之一。

组织者戴夫对这个展望似乎半信半疑，他说，“I’ll believe that when I see it :)”，“眼见为实”吧。

他们的目标是：成为第一个击败人类星际职业选手的AI。
![](https://pic4.zhimg.com/v2-1a7fc8d0af14a3f12eb578a340c8e9ef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
**△** 赛达团队

赛达的8名创造者有：Iljoo Yoon、Daehun Jun、Junseung Lee、Hyunjin Choi、Changhyeon Bae、Hyunjae Lee、Yonghyun Jeong、Uk Jo。其中，Changhyeon Bae是这支队伍的leader。

## **Facebook进步明显、B站参赛**

下面，说说今年的其他选手。

先看看最终的排名。
![](https://pic1.zhimg.com/v2-80b2cb89296c85df6dfe1d03cda816a4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1054'></svg>)
**△** 今年的星际AI大赛成绩单

**第二名CherryPi。**

如果你关注过这项赛事，一定不会对这个名字陌生。樱桃派这个选手，家世显赫，出自著名的Facebook AI Research团队，领军人物是大名鼎鼎的Yann LeCun。

战队成员包括：Jonas Gehring, Vegard Mella, Daniel Gant, Zeming Lin, Da Ju, Danielle Rothermel, Nicolas Carion, Nicolas Usunier, Gabriel Synnaeve

去年，CherryPi参赛获得第六名。

今年，经过一番车轮鏖战，CherryPi最终夺得亚军，仅次于三星SAIDA战队。要知道去年的冠军ZZZKBot，今年只获得第10名的成绩。

CherryPi会有8-13个候选策略。每场比赛开始时，它会根据与对手之间的历史表现，基于具有时间衰减权重的老虎机模型，选择一个策略。（通俗的说，也就是越是最近的比赛，权重越高）

随着比赛进行，CherryPi会根据当前情况，使用预训练的模型，使用每种策略估计获胜的可能性，满足一些条件的情况下，会切换到胜率最高的策略。

所以，这也是一种来回切换的“混合”策略。
![](https://pic1.zhimg.com/v2-b2f34f7fcaf68ca18fdda48bf163ed74_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='452'></svg>)
**△** CherryPi对战画面

策略选择上有几个基本的要素。一是多样性，能够考虑到尽可能多的情况。二是稳健性，以及追求100%的胜率。三是更好的扬长避短。

CherryPi使用了非常多的**AI技术**，包括：

基于搜索的AI。CherryPi使用了区域级寻路来指挥单位绕过地图上的障碍，使用威胁感知寻路来风筝或者引导单位脱离战斗。

CherryPi也会基于人类数据学习建筑摆放的位置，还用了离线强化学习，以及在线学习，也使用了BWEM进行自定义的地图分析。

与去年相比，现在的CherryPi使用了LSTM模型进行高级策略的选择，使用部分观察的游戏状态作为输入；在宏观管理方面进行了优化，微观管理也进行了改进。

CherryPi使用了Torch框架，全部的代码是大约5万行的C++。
![](https://pic2.zhimg.com/v2-333badeb549d43cee827bf637109db21_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='315'></svg>)△ SAIDA对战FB另一Bot



**从第三名到第八名，有个共同特点：全是神族。**

第三名CSE。这是一个中国战队。

成员包括：Junge Zhang、Wei Guo、Qiyue Yin、Dong Zhan、Qiwei Wang、Yihui Hu、Shengqi Shen、Kaiqi Huang。

其中第一位应该是中科院自动化所的张俊格，去年他也带领团队参加了星际AI大赛，只不过去年他们的AI是CPAC，今年改了名字。

这支战队算是“改装”派打法，他们的AI去年基于Steamhammer bot，而今年则基于Locutus。他们在Locutus的基础上，做了一些策略和微观层面的优化。

第四名BlueBlueSky，也基于Locutus，作者包括中科院自动化所的张恂、兴军亮、地平线机器人工程师侯鹏飞等。

第五名是正牌的Locutus。在八月的CIG星际AI大赛上，丹麦独立游戏AI开发者编写的Locutus一举夺魁。（当时ZZZKBot也参赛了，最终排名第七。）

可能正是由于Locutus的成功，今年3-8名的战队，全都是使用神族的AI。全部战队中，有11支使用了神族，虫族的热度有所下降。
![](https://pic1.zhimg.com/v2-c71b143859cca9bf42a76f34873a3298_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='616'></svg>)
其实Locutus是基于Steamhammer改进的，不同之处在于，Steamhammer玩虫族，而Locutus玩神族。

Steamhammer本尊这次排名第十一。
![](https://pic2.zhimg.com/v2-02631694332d09d99008ff02679eebe1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='449'></svg>)
**其他中国战队**

除了CSE和BlueBlueSky，这届AIIDE星际AI大赛还有其他的中国团队参赛。

排名第六的ISAMind，作者Fang Gao，来自中国电子科技集团公司认知与智能实验室。

排名第七的DaQin，作者Lion Gis，看GitHub的画风应该也是来自中国：
[https://github.com/liongis](https://link.zhihu.com/?target=https%3A//github.com/liongis)。

而排名第13位的LastOrder，来自B站（Bilibili AI Research）。LastOrder使用了TensorFlow机器学习框架。

B站这个bot，据说用了1000台机器进行分布训练，也被对手认为很有潜力。

## **One More Thing**
![](https://pic2.zhimg.com/v2-d9589042219c14fd352cc15b87815809_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='283'></svg>)
虽然“不会学习”的赛达击败了各路AI，但有个细节很有意思。

我们注意到，如上图所示，随着比赛时间的推移，三星赛达的胜率虽然一直很高，但总体上在轻微但持续的下降。

与此同时，获得第二名的CherryPi的胜率，在轻微而持续的上升。

尽管星际很难，但对于AI来说，也许未来有一天也能跟围棋一样，不用依靠人类的知识，自己成长为一代高手。

最后，给几个开源地址。

赛达的源代码目前还没有放出，不过，GitHub页面已经有了，谁知道官方什么时候会补一些除了readme之外的东西呢：

[https://github.com/TeamSAIDA/SAIDA](https://link.zhihu.com/?target=https%3A//github.com/TeamSAIDA/SAIDA)

CherryPi的GitHub页面传送门：

[https://github.com/TorchCraft/TorchCraft](https://link.zhihu.com/?target=https%3A//github.com/TorchCraft/TorchCraft)

BlueBlueSky的GitHub页面传送门：

[https://github.com/biug/bluebluesky](https://link.zhihu.com/?target=https%3A//github.com/biug/bluebluesky)

Locutus的GitHub页面传送门： 

[https://github.com/bmnielsen/Locutus/](https://link.zhihu.com/?target=https%3A//github.com/bmnielsen/Locutus/)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




