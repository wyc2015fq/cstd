
# Storm任务提交过程及目录树介绍 - Magician的博客 - CSDN博客


2018年04月06日 22:37:12[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：289所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言：](#前言)
[1、Storm 任务提交的过程](#1storm-任务提交的过程)
[2、Storm相关的目录树](#2storm相关的目录树)
[总结：](#总结)


# 目录
## 前言：
对于任何一个组件来说，了解它相关的任务提交的过程是非常有必要的（毕竟生产中遇到一些Bug时，你如果知道内部执行的过程，那么会对问题的解决会有很大的帮助）。除此之外还会对Storm相关的目录树进行一个简单的介绍，主要是为了了解下在任务提交过程中一些数据的存储过程。博主也是小菜鸟一枚，如有错误或不足，请各位及时指出，必当及时修正。
## 1、Storm 任务提交的过程
**任务提交详细过程如下图所示：**
![这里写图片描述](https://img-blog.csdn.net/20180406222700623?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**过程总结：**
1、客户端提交topology到nimbus（提交的jar包被上传到nimbus下的inbox目录）。
2、jar包中的submitTopology方法会对topology进行一些检查处理（如bolt/spout的id是否违法，storm是否是active等），然后在nimbus服务器上建立topology本地目录进行存储（包含topology的jar包以及topology的序列化对象）
3、之后nimbus进行任务分配（根据topology定义的一些参数来对bolt/spot设定task的数量并分配对应的task-Id），将分配好的task信息发送到zookeeper（task信息包括task的心跳信息，topology的描述信息等发送到zookeeper对应的目录下）；
4、supervisor定期到zookeeper相应目录下查看是否有新的任务，有的话下载下来，根据任务的描述信息启动相应的worker进行工作；
5、worker根据任务的描述信息来创建响应的网络连接来发消息。
## 2、Storm相关的目录树
**Storm任务提交的过程（根据图中的目录信息去理解Storm相关的目录树的目录信息）**
![这里写图片描述](https://img-blog.csdn.net/20180406223244123?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Storm组件本地目录树**
![这里写图片描述](https://img-blog.csdn.net/20180406223043859?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Storm zookeeper目录树**
![这里写图片描述](https://img-blog.csdn.net/20180406223128924?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 总结：
以上的内容主要还是以理解为主，特别是Storm任务提交的过程，需要重点关注！

