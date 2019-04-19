# Zookeeper - LC900730的博客 - CSDN博客
2017年11月11日 15:15:52[lc900730](https://me.csdn.net/LC900730)阅读数：326
### Linux下下载Zookeeper
wget [http://mirrors.hust.edu.cn/apache/zookeeper/zookeeper-3.4.10/zookeeper-3.4.10.tar.gz](http://mirrors.hust.edu.cn/apache/zookeeper/zookeeper-3.4.10/zookeeper-3.4.10.tar.gz)
### Zookeeper配置文件
dataDir=’/XX/XX’(数据的存放目录)
server.1=192.168.xx.xx(或者主机名)：2888（leader与follower通信端口） ：3888（投票端口） 
server.2=192.168.xx.xx(或者主机名)：2888（leader与follower通信端口） ：3888（投票端口） 
server.3=192.168.xx.xx(或者主机名)：2888（leader与follower通信端口） ：3888（投票端口）
需要把myid记录到数据文件中
文件名字就是myid，内容是1；
scp -r apps/     root@mini2: /root
### 启动zookeeper
bin/zkServer.sh start 
bin/zkServer.sh status:查看状态
### zookeeper客户端工具
bin/zkcli.sh 
connect mini2:2181   连接到mini2主机上
## Zookeeper数据结构
类似于文件系统的树；
／下面默认有1个zookeeper 
//创建节点 
create  /app1 “this is app1 servers parent“  
节点除了数据之外，还有cZxid，ctime等
//-s 创建带序号 
create -s /test/aa 999 
created /test/aa00000000
create -s /test/bb 999 
created /test/aa00000001
get /app1 watch:打出app1的数据，然后提供观测功能(获取数据的时候设置监听) 
比如在另一个客户端上更改了app1的数据，那么此处显示： 
WATCHER：： 
watchedEvent state：SyncConnected type:NodeDataChanged path:/app1
get 就是监听这个节点的数据，如果子节点发生变化，是不会检测到响应 
 如果监听字节点，那么 ls /app1 watch ,  那么字节点发生变化会响应
## 集群启动zookeeper
某个脚本执行的时候在bash进程中，可能看不到当前定义的变量。 
如果export a=1
```bash
//s1.sh
#!/bin/bash
b=2
echo "in s1,b="$b
echo "in s1,a="$a
./s2.sh
```
```bash
//s2.sh
#!/bin/bash
b=2
echo "in s2,a="$a
echo "in s2,b="$b
```
./s1.sh 
in s1,b=2 
in s1,a=1 
in s2,b= 
in s1,a=1
如果在s1.sh中export b=2，那么s2可以正常打印
如果是export，那么当前进程及其子进程中会有。
```bash
#！/bin/bash
echo "start zkServer..."
for i in 1 2 3
do
ssh hdp-node-0$i "source /etc/profile;/home/hadoop/apps/zookeeper/bin/zkServer.sh start"
done
```
在mini1里source一下，那么在ssh mini2启动发现成功
用户登录 的时候，会执行一个脚本  .bash_profile 
```bash
#.bash_profile
if  [-f ~/.bashrc ]; then
    .  ~/.bashrc   //.代表source
fi
```
```bash
#.bashrc
alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'
if  [-f /etc/bashrc ]; then
    .  /etc/bashrc   //.代表source
fi
```
/etc/profile中也会有
在script.sh中定义的变量，在当前登录的shell进程中，source script.sh时候，脚本中定义的变量也会进入到当前登录进程中。
scp-copy-id mini1 
scp-copy-id mini2 
scp-copy-id mini3 
那么mini1免密登录到mini2和mini3
./zkCli.sh:使用客户端命令行连接到zookeeper，默认连接到本机。如果连接到其他机器，那么继续使用connect centos:2181(客户端连接默认端口)
### 动态增加机器的时候
zookeeper集群 
需求：客户端能够实时洞察到服务器上下线的变化。
启动时候去zookeeper注册，比如写到 /server/server-01 mini1 
                第二台来的时候 server-02 mini2 
                第三台来的时候 server-03 mini3 
注册的数据节点必须是临时节点(序列化的临时节点)， 
客户端启动的时候去getchildren，获取当前在线服务器列表，并且注册监听， 
如果有服务器下线，那么zookeeper就会通知到客户端
