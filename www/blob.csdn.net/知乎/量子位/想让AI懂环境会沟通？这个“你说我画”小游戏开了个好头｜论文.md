# 想让AI懂环境会沟通？这个“你说我画”小游戏开了个好头｜论文 - 知乎
# 



安妮 夏乙 编译整理

量子位 出品 | 公众号 QbitAI

晚上9点，一下午开了3个会的你终于回到家，换了衣服瘫倒在沙发里。放空了三分钟之后，你缓过神来，喊了一声：

Siri，把我刚才挂在衣架上的浅色衣服放到洗衣机里去！
![](https://pic1.zhimg.com/v2-5b752115117c4af486774d2c76591ed8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='361'></svg>)
这样的任务，大概只能在科幻片里实现。

不过我们可以设想一下，如果你在几十年后真的拥有一个有机器人身体的Siri，要完成你给出的任务，它需要哪些能力呢？
- 听懂这句话，明白你的意图，理解“衣架上”、“洗衣机里”分别是什么地方，“衣服”又是什么东西；
- 导航到“衣架”，定位到上面的衣服；
- 从里选出“浅色的”那几件，抓住；
- 导航到“洗衣机”，打开门，把抓着的衣服送进去，放下。

在这个过程中，她可能还需要问你一些问题，比如说：哪个衣架？包括这件衣服吗？……

抛开导航、定位、抓取物体这些机器人在真实3D环境中的行动能力不谈，单是理解指令、规划出怎么完成任务，就需要你家的AI能够**理解语言**、**感知环境**、**理解物体之间的关系**、主动**交流**、规划行为。

不容易啊！

不过，已经有科学家开始在这方面努力了，比如说最近有一项研究，就把上面描述的这类任务简化简化再简化，做成了一个“你说我画”的游戏，给两个AI智能体玩。

这个游戏名叫**CoDraw**，需要两个智能体协作完成。

一个智能体面前摆着一幅画，它需要把这幅画上有什么、某个东西放在哪用语言描述出来；

另一个面前摆着一张空白画布，要按照之前那个智能体的描述，画一幅画，遇到不确定的地方，还要提问。

这两个智能体，分别叫描述者（Teller）和作图者（Drawer）。

玩CoDraw游戏的具体过程是怎样的呢？我们来举个例子（T代表描述者，D代表画图者）：
![](https://pic1.zhimg.com/v2-81b9dd6a367d48c892e3d86f664acbdc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='320'></svg>)
T：图里左侧中上方有棵浓密的树，树节在右侧,树干左边部分不在画面里。

D：好的。（根据对T的描述的理解，画一棵树。）

T：右侧中上方有一棵常青树，树干贴着画面右边的边缘。

D：第二棵树的树干在哪？高还是低？

T：在画面上半部分。图上边还有个太阳，有一部分被那棵浓密的树挡住了。

D：（画了常青树和太阳。）大小呢？

T：（没回答这个问题。）有个男孩盘腿坐在离画面左边两指的地方，笑着看向右边，左手拿着热狗，右手拿着汉堡包。

D：OK。（照着描述画个男孩。）

……（双方配合着又画出了女孩、烧烤架）……

T：（看了一眼D的画。）浓密的树再往上移一点，太阳再大一点，烧烤架也往上移一点。其他都挺好的。

D：（不说话，改改自己的画。）

> 完成撒花~ 把画出来的作品跟原画比较，就能判断两个AI智能体配合得怎么样了。

这个游戏是怎样做出来的？智能体玩得怎么样？

在国立首尔大学、乔治亚理工大学、Facebook人工智能实验室（FAIR）和Surromind Robotics合著的论文CoDraw: Visual Dialog for Collaborative Drawing中，研究人员将为你解答全部疑问。知乎大神田渊栋，也是作者之一。

搞定绘画素材

## 想让AI玩上“你说我画”，第一个问题就是：画图素材从哪来呢？

放眼望去，研究人员觉得Zitnick和Parikh两位同行之前提出的Abstract Scenes数据集还不错。

这个数据集包含10020个语义一致的场景，每个场景包含最少6种、最多17种物体。

整个数据集共包括58个物体的clip arts，在这里我们将其称为“贴纸”。这些贴纸包括在公园中游玩的男孩麦克和女孩珍妮的7种姿势和5种表情，此外还包括树、玩具、帽子、动物、食物等各种物体。

每张贴纸都可被拖放到画布上的任意(x,y)位置，来创建一个抽象场景。这些贴纸还支持大小和空间转换，包括大中小三种型号、前中后三种景深和面朝左右2种方位，总共3×3×2=18种变体。
![](https://pic4.zhimg.com/v2-6cc4f28965d85b4c362f659e63aee89b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='367'></svg>)
**△** 作画者界面

描述者看图说话，作图者听话作画，就靠这些贴纸了。

## 对话数据

画图素材敲定后，研究人员开始考虑对话数据的来源了，去哪收集这部分数据呢？

亚马逊的Amazon Mechanical Turk是一个向全球人类发布任务的数据标注平台。需要标注、处理数据的机构将任务发布在面，有时间的兼职者去认领任务。虽然这个平台在国内并不出名，但在国外早已是研究机构处理数据的好帮手。所以，对话数据的收集有了大量人手。

每次数据的收集过程需要两名兼职协作。和描述者与作图者的任务相同，一名兼职负责看着面前图像准确描述图中信息，而另一名兼职需要在空白画板上作图。为了方便收集，研究人员基于Visual Dialog聊天界面构建了一个拖放界面方便收集数据。
![](https://pic4.zhimg.com/v2-188fc380310791b793255cf427df80bb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='471'></svg>)
**△** 兼职人员看到的拖拽界面

在每轮对话中，每方一次只能发一条消息，之后需要等待对方回复。每轮对话限制在140字内。负责作图的人根据描述逐步修改场景，并且可以询问是否画得正确。
![](https://pic1.zhimg.com/v2-81b9dd6a367d48c892e3d86f664acbdc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='320'></svg>)



**△** CoDraw数据集演示示例，作图者根据描述者的内容逐步生成的图像

等到双方都对做出的图像满意，这轮对话也就结束了，完成的质量好的兼职会获得额外的奖金。

怎样判断双方完成的质量如何？这还得根据与描述者看到原图的相似度来判断。研究人员用Scene Similarity Metric (SSM)来评判原图S和重构图S^之间的差距：
![](https://pic1.zhimg.com/v2-d2c5bfadc9ed17c6add45df12d2d3158_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='581' height='71'></svg>)



最后，研究人员收集了9993段对话，总共包含138000个句子。他们将整个数据集的80%（7989）的示例用于训练、10%（1002）用于验证、10%（1002）用于测试。

整个对话集是什么画风？研究人员用以下三个维度进行了可视化——
![](https://pic1.zhimg.com/v2-2166b73a20a69123e5befd3baa0ab9ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='588' height='396'></svg>)
**△** token数量

上图中，蓝色和绿色分别代表描述者和作图者消息中token的个数。可以发现，作图者发出单token消息41195条，占比（62.6%）。其中，描述者和作图者token的中位数分别为16和1。
![](https://pic3.zhimg.com/v2-4d31a03094315df23b5732bcaed48cf6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='534' height='356'></svg>)



**△** 对话轮数的中位数为7分钟
![](https://pic2.zhimg.com/v2-38ff9e7fa51d64ab957d87515aded0e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='394' height='351'></svg>)



**△** 对话时间的中位数为6分钟

## 构建模型

搞定了数据集后，研究人员就要分别为描述者和作图者构建模型、开始训练了，还要通过重建场景的质量评估模型的好坏。

研究人员选取训练集里常见的贴纸并用分类属性的模式和中间位置作为基线，如下图所示：
![](https://pic4.zhimg.com/v2-3026bbc0c09be86d8456d506f7724b9b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='512'></svg>)
**△** 基准图

研究人员从目标图像S中提取出特征φ(S)后，采用了两种方法对对话建模，这也是为了从中选出更适合这项任务的方法。

一种是用**连续单注意（Sequential single attention）法**构建的模型SeqAtt，遵循预定义的、内容独立顺序随机的原则，描述者每次顺序选择一个贴纸Si，之后生成表达uˆ (T) t。一旦作图者接收到这个消息，它将在画布上添加这项内容，并将结果uˆ (D) t反馈给描述者。

这种方法不考虑历史对话，并且描述者是顺序选择物体而不是首先描述图中最重要的物体。用这种方法模拟对话使用了训练集中37.6%的对话回合。

为了对会话历史建模，并让注意机制学会感知内容，研究人员提出了第二种方法，即**动态多注意（Dynamic multi-attention）法**构建的模型DynAtt。
![](https://pic2.zhimg.com/v2-a72e25d5f77ff32a2b8e1331c72fbb5d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='209'></svg>)



**△** DynAtt模型完成协同绘图任务示意图

它能赋予描述者和作图者记忆，损失函数可以写成下面的形式——
![](https://pic2.zhimg.com/v2-49e4ffe3bbc6d19e7b9854a5655b69dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='576' height='199'></svg>)



在这里，研究人员使用到了对话中的监督信号u，注意力α和作图者的行为a。没有^的符号表示真实值（ground-truth value）。

根据所用编码器和解码器的不同，研究人员组合出了五种模型，即SeqAtt（GRU）、SeqAtt（BoW）、DynAtt（LSTM）、DynAtt（BoW）和DynAtt-NJ（LSTM）。需要注意的是，DynAtt-NJ和DynAtt是同一种模型，只不过DynAtt-NJ在训练中添加了共同注意力模型（Joint attention models）。

## 实验结
![](https://pic1.zhimg.com/v2-097000e12e7865dcd50a230821913fac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='535' height='69'></svg>)



提出的评价指标SSM对模型进行评估，总体来看，基线模型（Mode）的总分为0.9024，与之相比SeqAtt(BoW)的总分最高，为2.3983。
![](https://pic1.zhimg.com/v2-7312badb4d624f0e75390afe94b2591c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='331'></svg>)



**△** 各模型分数汇总

可能这些数字对比起来还不够直观，不如直接看看各模型最后画出的图像。研究人员给描述者下面这张原图，要求描述者描述给作图者——
![](https://pic3.zhimg.com/v2-c9791bdc98f7cf46e043002189ccd1d6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='268' height='212'></svg>)



**△** 描述者看到的原图

之后，各模型中的描述者和作图者争分夺秒进行对话，还原图像。结果如何呢？

有这样的——
![](https://pic3.zhimg.com/v2-9f1f30d3ca8be4b880c70599bf4bb0de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='259' height='230'></svg>)



这样的——
![](https://pic2.zhimg.com/v2-5f0dd30509c3b7d7027b8d49d63e299d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='260' height='233'></svg>)



这样的——
![](https://pic3.zhimg.com/v2-429deb8f8e8ed02e22fdae50a6adb9fe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='264' height='230'></svg>)



和 这样的——
![](https://pic1.zhimg.com/v2-3714d0fdf17f01b2dbbbb5e9aa8a5088_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='263' height='245'></svg>)



差距还不小~

那么……每个模型之间，描述者和作图者是如何对话的呢？请看下图——
![](https://pic3.zhimg.com/v2-1c12b33deb7e0420a07de30ee8ff1112_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='308'></svg>)



得分最高的，是一组人类（左一）。他们是这样对话的（T代表描述者，D代表作图者）：

T：右侧有个面朝左的大滑梯，一只大型猫头鹰坐在滑梯上也面朝左，梯子的顶部和底部稍稍被切掉了。

D：OK。

T：梯子上方有一大朵正在下雨的云，并且上部被切掉了。

D：OK。

T：图画左边有两个人，小灌木的树洞朝左。

D：OK。

T：树的左边有个面朝右的生气的盘腿男孩，男孩的右手被切掉了。

D：OK。

T：树右边的坐着一个脸朝右的生气的女孩。

D：OK。

T：干得漂亮，需要将物体整体向下移动，但不要碰到底面，谢谢！

D：OK

> 得分最低的DynAtt(BoW)模型（右一）就看起来有些不正常了——

T：一个 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：梯子 D：<无语>

T：一个 D：<无语>

T：女孩 D：<无语>

T：女孩 D：<无语>

T：女孩 D：<无语>

T：女孩 D：<无语>

T：女孩 D：<无语>

T：女孩 D：<无语>

T：女孩 D：<无语>

T：女孩 D：<无语>

> 得分最高、生成图像质量比较好的SeqAtt(BoW)，对话方式其实和人类不太一样，可以说……非常简练：

T：有云下雨。

D：OK。

T：右侧滑梯。

D：<无语>

T：左侧树小。

D：<无语>

T：左侧男孩生气。

D：<无语>

T：女孩坐着生气。

D：<无语>

T：树猫头鹰。

D：OK

> 不过从结果来看，作图者似乎不太理解云和雨是什么，反倒给出了一个大太阳。

排在左二的SeqAtt(GRU)和左四的DynAtt(LSTM)，对话的语句乍一看还算正常，但描述的顺序总是有些问题，前后还常有矛盾之处。

总的来说，无论是从得分上来看，还是从对话内容上来看，这些模型都和人类还有不小的差距，或者乐观一点说：提升到空间。

对了，研究人员为什么要教智能体玩游戏呢？

“训练智能体用自然语言与人类交互、并基于它所处环境决策和行动是对AI的基本要求。从这个目标出发，我们需要赋予智能体感知环境、理解语言、主动交流、准确理解目标最后做出正确决策的能力。”研究人员在论文中是这样写的。

最后，附论文地址：

[https://arxiv.org/abs/1712.05558](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1712.05558)

— 完 —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态






