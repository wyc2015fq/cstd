# dao daoimpl service serviceimpl servlet jsp 之间的关系 - xqhrs232的专栏 - CSDN博客
2018年10月26日 15:04:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：113
原文地址::[https://blog.csdn.net/bjbjkjkj/article/details/80580960](https://blog.csdn.net/bjbjkjkj/article/details/80580960)
相关文章
1、关于java中dao,model,service,impl,util的理解----[https://blog.csdn.net/bluefish_flying/article/details/70877961?utm_source=blogxgwz0](https://blog.csdn.net/bluefish_flying/article/details/70877961?utm_source=blogxgwz0)
dao 主要是一些接口，里面定义了一些用于增删改查的方法名
daoImpl  就是对dao的具体实现
Service 也是一些接口，主要是用来调用dao层的一些方法，所以这里定义的方法一般都定义好了参数；
serviceImp 对service的具体实现，这里会调用dao 的一些方法
servlet 调用 Service调用其方法，做一些操作
jsp 视图 显示 和输入数据
