# 第1篇：数据库需求与ER建模 - Spark高级玩法 - CSDN博客
2018年03月06日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：271
数据库需求与ER建模
前言
在数据库建设过程中，哪一步最重要？绝大多数资料会告诉你，是需求分析阶段。这一步的好坏甚至直接决定数据库项目的成败。
需求分析阶段，也被称为ER建模(entity-relationship modeling)阶段，也常被称为需求可视化，概念建模等。这一阶段数据库系统开发人员将协同需求方以ER图的方式对业务需求进行可视化展现。
本文将详细介绍(陈氏)ER符号体系，并在其中穿插一些具体实例讲解。
基本概念
1. 实体(entity)
实体表示客观世界中的众多概念，比如：人，地点，事件等。
每个实体本身包含多个实体成员，比如实体人可能包含张三，李四王五等。
在ER图中，实体通常用矩形表示，如下所示：
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DulUDcYvdv5x5tj6KaDkB4mJeeQ4qtd6x2Xv0a1QOY8MUCiaFASSOibF6w/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
2. 属性(attribute)
每个实体都有属性，用椭圆表示并用来描述实体各个特征。 比如顾客的特征可能有顾客标识符，顾客姓名，顾客性别，顾客年龄等，如下图所示：
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuMviafByOI83icpl1LXF31blYm6aOoDonqAhzGic3qjnBex1Dw3nkuXuXg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
此外，每个实体至少要有一个唯一属性，用下划线标记，如上图中的id字段。
3. 联系(relation)
实体与实体之间通常具有某种关联，在ER图中用菱形表示。比如某职员向某主管汇报，如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuPsTu61NEicWBHbXj6mXUBgpp8QW7kb6U8zUCkpusF8X46Sicgz6ILdqw/640?wx_fmt=png)
细心的读者相必发现了，实体间连线的两端，写有一些符号。这些符号被称为基数约束(cardinality constraint)，用来表示实体可以有多少实例与另一实体的实例存在联系。
基数约束共有四种形态：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8Duj5pfUQ87UTA8zjDibn8zibrUN4o2mFicXLmyU3sf9VbStNNyUQbJl2uOQ/640?wx_fmt=png)
此为形态一，即强制多个对应，表示一个实体A对应多个实体B。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DudiaxYBkyHbqyzHnehDpr3sWGTh4kkAztJxPkibQcAibaRic6EgiaoibN4KJg/640?wx_fmt=png)
此为形态二，即可选多个对应，表示一个实体A对应0个或多个实体B。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8Du7RJBsmH0GVgmRbtL8TYiboXlYwkhbtsgAr8p5Lhb8MdwWSjGU5PXOtQ/640?wx_fmt=png)
此为形态三，即强制单个对应，表示一个实体A对应一个实体B。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuxxXuWxtC3aEny1G4Gf7VwISficVnQ9JbVKLSE15yPd7lCRb3JN9B4SQ/640?wx_fmt=png)
此为形态四，即可选单个对应，表示一个实体A对应0个或1个实体B。
我们知道联系是双向的，所以实际ER建模中常见的版本长这样：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuQADYw7Od8vqnvUHO4ukagEl9tOjrZfwADIktWhKoeiaFmdqdiczic7Zmw/640?wx_fmt=png)
理解这个联系的方法是从两个方向进行解读，“实体A对应0个或1个实体B，实体B对应一个或多个实体A”。
扩展概念
使用前面介绍的这些概念，已经能完成基础ER建模了。然而，为了更为细致的刻画出用户需求，又有了下面这些建模规则。
1. 复合属性(composite attribute)
部分属性具有复合的特点，比如地址属性。地址可能包含了省份，城市，街道等子属性。
ER图上这类属性的属性名应当标记圆括号，然后扩展为多个子属性。可参考下面这个商店实体定义：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuHNq3ztaH6oOhMiaLIq8Uibmog3X0K38lKyP91wFJ9QFibXT9xfB86Bkwg/640?wx_fmt=png)
2. 多值属性(multivalued attribute)
部分属性具有多值的特点，比如一个职工可能有多个电话号码。
ER图上这类属性用双层椭圆标识，可参考下面这个职工实体定义：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DudRCqy6weqfoplWa1vqGUGMdD7xhLuFbT2SHg9SDA8KMDcBDLmCRribQ/640?wx_fmt=png)
3. 派生属性(derives attribute)
部分属性可从其他属性或者其他数据(如当前日期)派生出来，这类属性在ER图上用虚线椭圆标识。
可参考下面这个士多店实体定义：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuPcKStTlP15iaV1dMYy9TZ4c4etKkn7ctkG8ZibmDkMnQicO4qf0sZNuAA/640?wx_fmt=png)
上图中士多店的YearsInBusiness属性表示店铺开张了多少年，这个属性可以结合当前日期与OpeningDate属性算到，因此用虚线椭圆标识。
4. 可选属性(optional attribute)
部分属性可能有也可能没有取值，比如说职工奖金。
ER图上这类属性通过在属性名后面添加(0)标识，可参考下面这个职工实体定义：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuuNdRKibZOEgHicVvZuLodVicG9PmcicVRr4SwjzSic8UDu12oBJXWtXcQkg/640?wx_fmt=png)
5. 联系的进一步描述
a. 可以在联系中表明联系中的最大最小基数，如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DusldpB01EJcIs8kHET571KvHptNlxcGwqSR8ASnCKDccKW942d1BdEQ/640?wx_fmt=png)
在上面这个例子中，每个学生具体对应到了2-6间教室；同时每间教室对应到了5-40名学生。
b. 也可以在联系中说明联系中的角色。这在一元联系中尤为常见，如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuB0DUcMx3SfaCrCXPFwibWyfd1X1BJXdQqBKcPHd4XtF6R4kjYnoMExg/640?wx_fmt=png)
每个人只能送给其他人一份礼物，但可以收到0或多份礼物。
6. 关联实体(associated entity)
关联实体示用于描述M：N联系的一个替代方式，用一个内部有菱形的矩形表示，它没有唯一属性也没有部分唯一属性，且通常来说没有任何属性。
如下两个图可以说是等价的：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuEgzu16kKhBawDUGWjPrm6CtYNxohF1BcvbHfl02miag86mgOwhOpIiag/640?wx_fmt=png)
关联实体基本都是在多元联系的场景下用到，后面的高级话题部分会讲。
7. 弱实体(week entity)
通常来说，实体至少要有一个唯一属性。因为这样才能精确定位到需要处理的记录。但在ER建模这一层，也并非总是如此。
举例来说，假如现在需要为某个连锁酒店管理系统进行ER建模。该公司在全国各地都开有酒店。现在需要记录下各地酒店的房间使用情况。
可以将房间使用相关信息作为酒店的建模一个多值复合属性，如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8Du6HTo6G2QfDbuSwBn3lM3kw6lnkWOeewVTQ4dqNdmTdGhs9yDENgVuQ/640?wx_fmt=png)
这样做算是对的，但是并没有体现出部分码地位，也就是说各RoomID在各Building的唯一性。同时，很多时候需要将房间实体化与其他实体相联系。比如每个房间对应的清洁工。
引入弱实体机制后，便可顺利解决这两个问题。如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuasicKvpJGh3RM3ia5oG7icmb90E7nEBEBtibezDEEiadGEEbfurQlibPGoFw/640?wx_fmt=png)
两个地方要注意一下，一是弱实体的“主码”称为部分码，码名下方用虚线标记；
再一个就是弱实体必须至少有一个属主实体，它们之间的联系需用双框菱形标识。弱实体部分码同其属主实体候选码的组合可以唯一定位到任何一个弱实体记录。
高级话题
1. 相同实体之间具有多个M:N关系
某人为一个学生选课系统进行ER建模，得到如下结果：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuEF1akpicxMTIyRSOtMTmAmf4tvgaRw1ImNyrlqxNTqwtmMp4QJF4Jng/640?wx_fmt=png)
假如需求中有说明：一个同学一门课只能选一次，那这样的设计没问题。可是如果需求中说明，一个同学可以选一门课几次(可能是挂了好几次)，这样的设计就有问题了。
对此，正确的做法之一是使用有两个属主实体的弱实体：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuGI4ZqiahABAMpibdbC1PF9lCHGTyWkliabicQtgZVVMhCOPeXBlX5HS3JQ/640?wx_fmt=png)
或者为每次预定生成一个唯一的id，如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuPRegBpPibzibNRcBmZy5va6djdicno9tDrapLwc72TIT59CePl9Ek6ZkQ/640?wx_fmt=png)
2. 三元(或更多)关系
在ER图中，联系一般是将两个实体关联起来，又或者自己关联自己。但是也有些时候，需求方需要同时将多个实体联系起来。这怎么办呢？要知道表示联系的菱形有且只有两个接口。
答曰：使用关联实体。下面这个ER图中，使用了关联实体描述了某工厂的供货商，生产产品，零件三方联系：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuXMlakAkDGAiaa8pibP15jR6KVjGBlxaVIPiaQyl3PR1wKztqswI5zZY5g/640?wx_fmt=png)
但如果现在需求又变更了，需要给关联增加某些属性，比如说供货商每次提供的货物量，这个ER图就不能用了。
因为这样就没办法区分同一家供应商为同一产品提供等数量的同一零件的不同实例了。解决的办法是把关联实体改成一般的实体，并增设一个唯一标识符：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXrWFCpSwTHnnBuV32NB8DuUoUYWYQrtJF0JHJibZcwPITdicQWMNdVknM4bm2Eh2Ivm6aEIuYGFZKQ/640?wx_fmt=png)
其他说明
1. 本文实体名全大写，属性和关系名则用首字母大写的驼峰法，同时尽量保证所有命名都全局唯一；
2. 用户的更多个性需求应当以注释，标签等方式一并标记在ER图中；
3. 建模工具可使用PowerDesigner，Workbench等。不过笔者在这里推荐一款轻量级的在线数据库建模工具，网址是https://erdplus.com/#；
小结
需求分析，ER建模是贯穿整个数据库生命周期的工作。这部分工作要求开发人员和业务方，数据库的使用者，公司领导等方面协同好需求，并将需求以ER图的模式可视化展现出来。
只有绘制好ER图之后，才能顺利进入到接下来的关系表设计阶段。这也是下篇要讲解的内容。
本文转载自：http://www.cnblogs.com/muchen/p/5258197.html ，作者：穆晨，本文已授权，未授权请勿转载。
**推荐阅读：**
1，[数据仓库②-数据仓库与数据集市建模](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484505&idx=1&sn=a4204bed85dc3f5402c5d01b87d1c24e&chksm=9f38e771a84f6e67a5a34ee74111bd2aadd214b8db07085e65f4ca4032c868e5391e1accb48d&scene=21#wechat_redirect)
2，[数据仓库③-实现与使用(含OLAP重点讲解)](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484507&idx=1&sn=ba2d2b5b1b3b8fd918ed3c4a6a142296&chksm=9f38e773a84f6e655337dbfa492641018cbbe01ac0eb50db960858604496588148892a2e52a8&scene=21#wechat_redirect)
3，[kafka连接器两种部署模式详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484229&idx=1&sn=d5e87da8a05c40b617f343044ba6d808&chksm=9f38e06da84f697b5181c1aedbeb9205e3af0a81dde6d19487cbde803e01d86fb9367b3a3b3b&scene=21#wechat_redirect)
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
