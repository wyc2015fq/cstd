# 【Zinx应用-MMO游戏案例-(1)案例介绍】Golang轻量级并发服务器框架 - weixin_33985507的博客 - CSDN博客
2019年02月22日 10:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
【Zinx教程目录】
Zinx源代码: [https://github.com/aceld/zinx](https://github.com/aceld/zinx)
[Zinx第一章-引言](https://www.jianshu.com/p/23d07c0a28e5)
[Zinx第二章-初识Zinx框架](https://www.jianshu.com/p/e112e850440f)
[Zinx第三章-基础路由模块](https://www.jianshu.com/p/3fbd69ec3dcc)
[Zinx第四章-全局配置](https://www.jianshu.com/p/f2758c1b8f28)
[Zinx第五章-消息封装](https://www.jianshu.com/p/95b57c7e8c5c)
[Zinx第六章-多路由模式](https://www.jianshu.com/p/7e3462f1d942)
[Zinx第七章-读写分离模型](https://www.jianshu.com/p/d69e97a5e45e)
[Zinx第八章-消息队列及多任务](https://www.jianshu.com/p/febcd455627b)
[Zinx第九章-链接管理](https://www.jianshu.com/p/f52c9598fce6)
[Zinx第十章-连接属性设置](https://www.jianshu.com/p/7e7ec64af1e2)
*[连载中...]*
【Zinx应用案例-MMO多人在线游戏】
[(1)案例介绍](https://www.jianshu.com/p/5084c8688d93)
[(2)AOI兴趣点算法](https://www.jianshu.com/p/e5b5db9fa6fe)
[(3)数据传输协议protocol buffer](https://www.jianshu.com/p/aa27113b3daf)
[(4)Proto3协议定义](https://www.jianshu.com/p/dfc9386f70a2)
*[连载中...]*
## 一、应用案例介绍
​   好了，以上Zinx的框架的一些核心功能我们已经完成了，那么接下来我们就要基于Zinx完成一个服务端的应用程序了，整理用一个游戏应用服务器作为Zinx的一个应用案例。
​   游戏场景是一款MMO大型多人在线游戏，带unity3d 客户端的服务器端demo，该demo实现了mmo游戏的基础模块aoi(基于兴趣范围的广播), 世界聊天等。
![11093205-593bb6246327e900.png](https://upload-images.jianshu.io/upload_images/11093205-593bb6246327e900.png)
13-Zinx游戏-示例图.png
## 二、服务器应用基础协议
|MsgID|Client|Server|描述|
|----|----|----|----|
|1|-|SyncPid|同步玩家本次登录的ID(用来标识玩家)|
|2|Talk|-|世界聊天|
|3|MovePackege|-|移动|
|200|-|BroadCast|广播消息(Tp 1 世界聊天 2 坐标(出生点同步) 3 动作 4 移动之后坐标信息更新)|
|201|-|SyncPid|广播消息 掉线/aoi消失在视野|
|202|-|SyncPlayers|同步周围的人位置信息(包括自己)|
### 关于作者：
作者：`Aceld(刘丹冰)`
简书号：`IT无崖子`
mail: [danbing.at@gmail.com](mailto:danbing.at@gmail.com)
github: [https://github.com/aceld](https://github.com/aceld)
原创书籍gitbook: [http://legacy.gitbook.com/@aceld](http://legacy.gitbook.com/@aceld)
> 
**原创声明:未经作者允许请勿转载,  如果转载请注明出处**
