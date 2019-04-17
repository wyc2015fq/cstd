# zookeeper之客户端和服务端的区别 - qq_37385726的博客 - CSDN博客





2018年09月25日 17:50:20[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：671










客户端是集群外的访问，服务端才是集群上的提供服务的。  

使用bin/zkServer.sh start开启的zookeeper上的一个服务端，而使用bin/zkCli.sh是将客户端连到服务端上。

客户端可以通过服务端创建znode,删除znode，写znode,读znode，设置监视等等。

其中zookeeper提供的master选举选的是客户端的master，根据他们登录后在GroupMember目录下创建的临时目录的id来选的，最小的是master。  

这就区别于集群中服务端的各个servers的角色了，servers角色是leader和follow（或者还有observer）。 简而言之，客户端通过服务端来获取到zookeeper提供的服务。



