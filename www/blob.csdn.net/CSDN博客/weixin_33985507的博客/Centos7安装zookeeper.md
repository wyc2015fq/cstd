# Centos7安装zookeeper - weixin_33985507的博客 - CSDN博客
2018年05月18日 23:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# zookeeper安装配置
zookeeper下载地址：[http://apache.fayea.com/zookeeper/](http://apache.fayea.com/zookeeper/)
**请在安装前关闭防火墙，否则会有一些预想不到的错误**
```
systemctl stop firewalld.service   #停止
firewall systemctl disable firewalld.service  #禁止firewall开机启动
firewall-cmd --state     #查看默认防火墙状态（关闭后显示notrunning，开启后显示running）
```
**如果你使用的是阿里云或腾讯云的服务器请自行配置安全组规则**
## 开始安装
- 解压安装zookeeper安装包；
`tar -zxvf zookeeper-3.4.6.tar.gz`
- 进入解压后的文件夹内手动创建一个data文件夹
`mkdir data`
- 在data下在建一个名为myid的文件，内容对应server.数字
`如：server.1 server.2`
- 将 zookeeper/conf 目录下的 zoo_sample.cfg 文件改名，命名为zoo.cfg
`mv ./zoo_sample.cfg ./zoo.cfg`
- 进入zoo.cfg文件修改dataDir的值为刚才创建的data文件夹所在的绝对路径；
然后新增一行 server.1=localhost(ip):2888:3888 其中2888 端口号是 zookeeper服务之间通信的端口 3888 是 zookeeper与其他应用程序通信的端口
`dataDir=/home/demo/zookeeper-3.4.6/data`
`server.1=10.211.55.5:2888:3888`
> 
server.A=B:C:D：其中 A是一个数字，表示这个是第几号服务器；B是这个服务器的IP 地址或/etc/hosts 文件中映射了IP 的主机名；C 表示的是这个服务器与集群中的Leader 服务器交换信息的端口；D 表示的是万一集群中的Leader 服务器挂了，需要一个端口来重新进行选举，选出一个新的Leader，而这个端口就是用来执行选举时服务器相互通信的端口。如果是伪集群的配置方式，由于B 都是一样，所以不同的Zookeeper实例通信端口号不能一样，所以要给它们分配不同的端口号。
- 启动zookeeper
进入zookeeper的bin目录启动zookeeper执行命令
`sh zkServer.sh start`
输入jps 命令查看进程
```
$jps
1456 QuorumPeerMain
1475 Jps
```
查看状态
`sh zkServer.sh status`
停止zookeeper进程
`sh zkServer.sh stop`
