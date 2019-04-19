# Nebula3中的Visibility系统 - 逍遥剑客 - CSDN博客
2010年04月30日 00:43:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1774标签：[ps3																[工作																[任务																[扩展																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=ps3&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
Visibility是Mar2010新增加的一个模块. 严格上来说, 也不是”增加”啦, 其实是原有的场景管理可见性更新部分的一个扩充, 同时主要的目的是把可见性更新多线性化, 方便充分利用Win32的CPU和PS3的SPU.
大体结构, 如图
![](http://hi.csdn.net/attachment/201004/29/0_1272559482Z23q.gif)
l**InternalStage**是这个系统它妈, 不用说
l**VisibilityChecker**是核心类, 相当于一个管理器吧.所有加入Stage的实体都会在这里注册一下, 加入VisibilitySystem. 有一点值得学习的是VisibilityQuery, 这里持有俩实例, 每次取的都是上一帧的结果, 跟”BackBuffer&FrontBuffer”很像, 呵呵.
l**VisibilityQuery**就是实际操作查询的类了.功能是把查询请求变成一个Job任务提交, 并取得查询结果. 至于Job的工作原理, 下回再说.
l**VisibilitySystemBase**可见性系统基类. 可以看到, 它有3个子类(当然, 可以自己扩展). 
n**VisibilityQuadtree **这就是老的那一套场景管理, 只不过多了个爹而已.
n**VisibilityBoxSystem **VisibilityBox其实就是个包围盒, 里面一堆实体. 这个子系统的管理方式很简单: 如果摄像机在一个box内, 那么它只能看到这个box和邻近box内的实体; 如果摄像机不在任何box内, 那么所有box都是可见的. 简单的说, 就是你在这个房间里的话, 只能看到这个房间和隔壁房间里的东西.
n**VisibilityClusterSystem **VisibilityCluster, 嗯, 这个, 怎么说呢, 跟上面这个类似, 不过它是一个逻辑上的概念, 在关卡编辑器里指定的, 不深入讨论.
没几句话就说完了, 其实还是Job的工作原理才是核心, 抽时间仔细看看那一块儿.

