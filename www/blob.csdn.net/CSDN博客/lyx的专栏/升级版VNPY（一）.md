# 升级版VNPY（一） - lyx的专栏 - CSDN博客





2019年04月02日 21:43:58[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：82








之前在读vnpy源代码的时候，一直就有升级改造的想法，也有同学在博客下面互动，说希望能够开源升级后的代码。最近花了点时间，做了一点点修改，包括数据的本地获取、回测后的策略测试。

大概展示一下吧。

回测的结果包括vnpy原来自带的。vnpy自带的是以大图和命令行输出形式，笔者这里变成了网页。但是由于笔者不是前端工作者，所以网页的美化程度几乎没有。

![](https://img-blog.csdnimg.cn/20190402212511509.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

vnpy自己原有的内容呢，暂时笔者也不做修改，毕竟用户习惯还是要有所保留的。

然后就是多出来的东西了。

一个是蒙特卡洛的trading pnl测试，其实原理很简单，就是提取出每一笔交易的pnl，然后打乱顺序，形成一条新的路径。这样的次数多了之后，就会形成一些列的路径，而每条路径都会有一个最大回测，我们就会得到一个最大回撤的分布。

![](https://img-blog.csdnimg.cn/20190402212923945.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

后面这个其实是很多同学想要的，就是k线和指标的可视化。

![](https://img-blog.csdnimg.cn/20190402213707303.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

 可以支持放大缩小等功能，是利用pyecharts来实现的。

这个是本改进项目的开源地址： [https://gitee.com/qtlyx/MYvnpy](https://gitee.com/qtlyx/MYvnpy)

目前还有很多功能没有加入，后续会逐渐加入。

ps.回测引擎等部分，与原版的vnpy部分代码有所不同，个人进行了改写和优化。





