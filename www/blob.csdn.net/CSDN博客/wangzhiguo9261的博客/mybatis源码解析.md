
# mybatis源码解析 - wangzhiguo9261的博客 - CSDN博客


2018年06月25日 14:09:19[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：43


最近阅读了许久的mybatis源码，小有所悟。同时也发现网上有许多优秀的mybatis源码讲解博文。本人打算把自己阅读过的、觉得不错的一些博文列出来。以此进一步加深对mybatis框架的理解。其实还有一个目的是，本人也想写一系列博文来分享自己对mybatis的理解。然而。个人文笔太差，列出的这些别人的文章我也会细心研读，并希望在阅读这些文章的同时既加深我对mybatis的理解，又能探索出一种更加通俗易懂的文风，以便于我本人与大家一起分享自己对mybatis的理解。所以，本篇博客其实算是我在开讲mybatis源码前的准备和预热工作。
接下来，我会陆续完善这篇文章，尽量把一些优秀的讲解mybatis源码的博文罗列进来。之后，就轮到我来讲解啦。有兴趣的朋友可以关注我的这一些列博文，大家一同进步，一起探讨啦。
[通过源码分析MyBatis的缓存](http://www.cnblogs.com/fangjian0423/p/mybatis-cache.html)
[Mybatis解析动态sql原理分析](http://www.cnblogs.com/fangjian0423/p/mybaits-dynamic-sql-analysis.html)
[MyBatis拦截器原理探究](http://www.cnblogs.com/fangjian0423/p/mybatis-interceptor.html)
[myBatis源码之Configuration](http://blog.csdn.net/ashan_li/article/details/50379458)
[MyBatis架构设计及源代码分析系列(一):MyBatis架构](http://www.cnblogs.com/mengheng/p/3739610.html)
[深入理解mybatis原理》 MyBatis的架构设计以及实例分析](http://blog.csdn.net/luanlouis/article/details/40422941)
[《深入理解mybatis原理》 MyBatis事务管理机制](http://blog.csdn.net/luanlouis/article/details/37992171)
[Mybatis3.3.x技术内幕（一）：SqlSession和SqlSessionFactory列传](https://my.oschina.net/zudajun/blog/665956)
[Mybatis3.3.x技术内幕（二）：动态代理之投鞭断流（自动映射器Mapper的底层实现原理）](https://my.oschina.net/zudajun/blog/666223)
[初看Mybatis 源码 （三） SQL是怎么执行的](http://www.cnblogs.com/atio/p/3388281.html)
[深入了解MyBatis参数](https://my.oschina.net/flags/blog/381199)
[MyBatis直接执行SQL的工具SqlMapper](https://my.oschina.net/flags/blog/385126)
[教你如何开发Mybatis的通用Mapper](https://my.oschina.net/flags/blog/354573)
[偶尔记一下没事看看 - MyBatis工具](http://blog.csdn.net/isea533/article/category/2092001/2)
[Mybatis解析动态sql原理分析 - format](http://www.tuicool.com/articles/qAFFBf)
mybatis源码阅读心得
[http://xpenxpen.iteye.com/blog/1508749](http://xpenxpen.iteye.com/blog/1508749)
Mybatis源代码分析之parsing包
[http://www.cnblogs.com/sunzhenchao/p/3161093.html](http://www.cnblogs.com/sunzhenchao/p/3161093.html)
Mybatis之reflection包源代码解析（一）
[http://www.cnblogs.com/sunzhenchao/p/3328053.html](http://www.cnblogs.com/sunzhenchao/p/3328053.html)
mybatis源代码分析之binding包
[http://www.cnblogs.com/sunzhenchao/archive/2013/05/13/3075854.html](http://www.cnblogs.com/sunzhenchao/archive/2013/05/13/3075854.html)
Mybatis源码分析之Mapper执行SQL过程（三）
[http://www.ccblog.cn/88.htm](http://www.ccblog.cn/88.htm)
Mybatis源码分析之Cache一级缓存原理（四）
[http://www.ccblog.cn/89.htm](http://www.ccblog.cn/89.htm)
Mybatis源码分析之Cache二级缓存原理 （五）
[http://www.ccblog.cn/90.htm](http://www.ccblog.cn/90.htm)
Mybatis插件原理和PageHelper结合实战分页插件（七）
[http://www.ccblog.cn/92.htm](http://www.ccblog.cn/92.htm)
如何细粒度地控制你的MyBatis二级缓存(mybatis-enhanced-cache插件实现)
[http://www.tuicool.com/articles/jqqMbaj](http://www.tuicool.com/articles/jqqMbaj)
[http://blog.csdn.net/luanlouis/article/details/41800511?utm_source=tuicool&utm_medium=referral](http://blog.csdn.net/luanlouis/article/details/41800511?utm_source=tuicool&utm_medium=referral)
MyBatis缓存源码分析
[http://www.codeceo.com/article/mybatis-cache.html](http://www.codeceo.com/article/mybatis-cache.html)
MyBatis源码分析——Cache接口以及实现
[http://www.cnblogs.com/jabnih/p/5705640.html](http://www.cnblogs.com/jabnih/p/5705640.html)
MyBatis源码分析——Plugin原理
[http://www.cnblogs.com/jabnih/p/5656194.html](http://www.cnblogs.com/jabnih/p/5656194.html)
MyBatis源码分析（1）——整体依赖关系图
[http://www.cnblogs.com/jabnih/p/5656182.html](http://www.cnblogs.com/jabnih/p/5656182.html)
深入浅出Mybatis-改造Cache
[http://blog.csdn.net/keda8997110/article/details/20908425](http://blog.csdn.net/keda8997110/article/details/20908425)
[http://blog.csdn.net/keda8997110/article/details/20908425](http://blog.csdn.net/keda8997110/article/details/20908425)
MyBatis缓存源码分析
[http://www.codeceo.com/article/mybatis-cache.html](http://www.codeceo.com/article/mybatis-cache.html)
[http://www.codeceo.com/article/mybatis-cache.html](http://www.codeceo.com/article/mybatis-cache.html)
Mybatis增强型注解简化SQL语句
[http://blog.csdn.net/ExcellentYuXiao/article/details/53262928](http://blog.csdn.net/ExcellentYuXiao/article/details/53262928)
Mybatis增强型注解简化SQL语句
[http://blog.csdn.net/ExcellentYuXiao/article/details/53262928](http://blog.csdn.net/ExcellentYuXiao/article/details/53262928)
Mybatis3源码分析(11)-Sql解析执行-BoundSql的加载-1
[http://blog.csdn.net/ashan_li/article/details/50370035](http://blog.csdn.net/ashan_li/article/details/50370035)
Mybatis解析动态sql原理分析
[http://www.cnblogs.com/fangjian0423/p/mybaits-dynamic-sql-analysis.html](http://www.cnblogs.com/fangjian0423/p/mybaits-dynamic-sql-analysis.html)
通过源码分析MyBatis的缓存
[http://www.cnblogs.com/fangjian0423/p/mybatis-cache.html](http://www.cnblogs.com/fangjian0423/p/mybatis-cache.html)
MyBatis拦截器原理探究
[http://www.cnblogs.com/fangjian0423/p/mybatis-interceptor.html](http://www.cnblogs.com/fangjian0423/p/mybatis-interceptor.html)
Mybatis 中$与\#的区别
[http://www.cnblogs.com/hellokitty1/p/6007801.html](http://www.cnblogs.com/hellokitty1/p/6007801.html)
csdn: pfnie
深入理解mybatis原理(一)》 Mybatis初始化机制详解
[http://blog.csdn.net/pfnie/article/details/51491934](http://blog.csdn.net/pfnie/article/details/51491934)
深入了解mybatis参数
[http://www.bubuko.com/infodetail-1219821.html](http://www.bubuko.com/infodetail-1219821.html)
mybatis之 \# 与 $ 区别以及 sql 预编译
[http://blog.csdn.net/wo541075754/article/details/54292751](http://blog.csdn.net/wo541075754/article/details/54292751)
[http://www.cnblogs.com/slwenyi/p/6393808.html](http://www.cnblogs.com/slwenyi/p/6393808.html)
ConcurrentHashMap的锁分段技术
[http://blog.csdn.net/yansong_8686/article/details/50664351](http://blog.csdn.net/yansong_8686/article/details/50664351)
Zookeeper(2)-Zookeeper可以干什么
[http://blog.csdn.net/zmx729618/article/details/52709942](http://blog.csdn.net/zmx729618/article/details/52709942)
[http://www.cnblogs.com/slwenyi/p/6393808.html](http://www.cnblogs.com/slwenyi/p/6393808.html)
[http://blog.csdn.net/yansong_8686/article/details/50664351](http://blog.csdn.net/yansong_8686/article/details/50664351)
[http://blog.csdn.net/qq_26562641/article/details/51035189](http://blog.csdn.net/qq_26562641/article/details/51035189)
[http://uule.iteye.com/blog/1513976](http://uule.iteye.com/blog/1513976)
[http://blog.csdn.net/zmx729618/article/details/52709942](http://blog.csdn.net/zmx729618/article/details/52709942)
AbstractQueuedSynchronizer原理及代码分析
[http://www.cnblogs.com/noahsark/p/sbstract_queued_synchronizer.html](http://www.cnblogs.com/noahsark/p/sbstract_queued_synchronizer.html)



