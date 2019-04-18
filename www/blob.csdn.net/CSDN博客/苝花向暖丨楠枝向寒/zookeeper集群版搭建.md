# zookeeper集群版搭建 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月28日 10:51:44[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：50标签：[zookeeper集群](https://so.csdn.net/so/search/s.do?q=zookeeper集群&t=blog)
个人分类：[docker																[分布式开发](https://blog.csdn.net/weixin_40247263/article/category/7559957)](https://blog.csdn.net/weixin_40247263/article/category/7567627)


首先zookeeper的集群搭建是基于单机版搭建的，如没搭建过zookeeper单机版请查看我之前的一篇文章

zookeeper 搭建集群一般是奇数台服务器，因为zookeeper只要有半数以上的服务器没有挂就可以使用，如我们有5台，挂了2台，还剩3台，剩下半数以上，可以继续使用。但是如果挂了3台，剩两台就不行了，假设我们再加一台，现在变为6台了，这时挂了3台，正好是半数，这种情况是不可以使用的，所以5台和6台都仅仅允许挂3台，所以我们在搭建的时候往往会选择搭建奇数台。

首先给linux配置几个虚拟ip，作为与docker的映射

zk1              192.168.46.237 

zk2              192.168.46.238  

zk3              192.168.46.239  

进入之前单机版安装了zookeeper的docker容器，做以下修改

在根目录 下 创建文件夹 zk ，在zk下创建 两个文件夹一个 为data，一个为 log，在 data下创建一个文件夹 touch myid

修改 zookeeper目录下的conf文件下的 zoo.cfg配置文件

![](https://img-blog.csdn.net/20180728100620742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

保存退出，然后退出容器 ctrl + D

将zookeeper这个容器 打包成镜像

docker ps -a 查看容器

docker commit 容器id zookeeper:v1

删除之前配置的容器

docker stop 容器id       先暂停

docker rm 容器id        再删除

通过刚才打包好的镜像，生成新的docker容器

docker run --privileged=true -itd -p 192.168.46.237:2181:2181 -p 192.168.46.237:2888:2888 -p 192.168.46.237:3888:3888 7d1aaaf4a688 /bin/bash

进入容器   docker exec -it 7d1aaaf4a688 /bin/bash

# cd /zk/data/

# vi myid

里面写个1，代表配置的第一个 zookeeper

![](https://img-blog.csdn.net/20180726214317621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

保存退出

关闭防火墙 service iptables stop

启动服务 zkServer.sh start

然后退出容器

通过镜像创建第二个容器

docker run --privileged=true -itd -p 192.168.46.238:2181:2181 -p 192.168.46.238:2888:2888 -p 192.168.46.2

5954f4ff835d0acb37f642985165bc741c18321dc1db7614e0ec4be179d92278

同样， cd /zk/data    然后 vi myid ，这次里面写个2 代表第二个zookeeper服务器

保存退出

关闭防火墙 service iptables stop

启动服务 zkServer.sh start

再退出容器，创建第三个容器，同理，不再赘述

![](https://img-blog.csdn.net/20180728102148101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果 主节点挂了，由从节点代替，其中的一个从节点会成为主节点，这个从节点是主节点没有挂之前选出来的。就像古代皇帝没死之前会先选出太子一个道理。

测试

```java
public static void main(String[] args) {
		//配置连接地址
		ZkClient client = new ZkClient("172.17.0.15:2181,172.17.0.16:2181,172.17.0.17:2181");
		//创建一个目录
		client.createPersistent("/zzh/abc");
		//向这个目录中写信息
		client.writeData("/zzh/abc", "aaaaa");
		//从这个目录中读信息
		Object readData = client.readData("/zzh/abc");
		//打印输出
		System.out.println(readData);
		
	}
```

结果

![](https://img-blog.csdn.net/20180728105110983?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果报错，说socket 连接失败，那么需要在命令提示符(管理员) 添加路由

![](https://img-blog.csdn.net/20180728110542567?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



