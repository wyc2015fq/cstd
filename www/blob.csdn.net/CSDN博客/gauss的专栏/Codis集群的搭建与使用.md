# Codis集群的搭建与使用 - gauss的专栏 - CSDN博客
2016年10月14日 13:02:41[gauss](https://me.csdn.net/mathlmx)阅读数：759
个人分类：[开源库](https://blog.csdn.net/mathlmx/article/category/944352)
一、简介
Codis是一个分布式的Redis解决方案，对于上层的应用来说，连接Codis Proxy和连接原生的Redis Server没有明显的区别（不支持的命令列表），上层应用可以像使用单机的Redis一样使用，Codis底层会处理请求的转发，不停机的数据迁移等工作，所有后边的一切事情，对于前面客户端来说是透明的，可以简单的认为后边连接是一个内存无限大的Redis服务。
Codis架构图：
![](http://images.cnitblog.com/blog2015/645933/201504/141613063233934.png)
以上我们可以看到codis-proxy是单个节点的，因为我们可以通过结合keepalived来实现高可用：
![](http://images.cnitblog.com/blog2015/645933/201504/141610091987680.png)
codis-proxy 提供连接集群redis服务的入口
codis-redis-group 实现redis读写的水平扩展，高性能
codis-redis 实现redis实例服务，通过codis-ha实现服务的高可用
二、组件说明
codis-proxy : 是客户端连接的Redis代理服务，codis-proxy 本身实现了Redis协议，表现得和一个原生的Redis没什么区别（就像Twemproxy），对于一个业务来说，可以部署多个codis-proxy，codis-proxy本身是没状态的。
codis-config ：是Codis的管理工具，支持包括，添加/删除Redis节点，添加/删除Proxy节点，发起数据迁移等操作，codis-config本身还自带了一个http server，会启动一个dashboard，用户可以直接在浏览器上观察Codis集群的状态。
codis-server：是Codis项目维护的一个Redis分支，基于2.8.13开发，加入了slot的支持和原子的数据迁移指令，Codis上层的codis-proxy和codis-config只能和这个版本的Redis交互才能正常运行。
ZooKeeper ：用来存放数据路由表和codis-proxy节点的元信息，codis-config发起的命令都会通过ZooKeeper同步到各个存活的codis-proxy
说明：
    Codis支持按照Namespace区分不同的产品，拥有不同的product name 的产品，各项配置都不会冲突。
实验环境：
```
角色                  ip地址            主机名          
codis-redis-master  192.168.10.128      redis-master             
codis-redis-slave   192.168.10.129      redis-slave
zk,codis-proxy      192.168.10.130      codis-proxy
```
三、安装配置（我这里只部署单个codis-proxy节点的环境），在codis-proxy服务器上操作：
1、安装go：
下载安装包：（貌似海外服务器才能下载）
```
wget https://storage.googleapis.com/golang/go1.4.1.linux-amd64.tar.gz
```
下载后，解压就可以用了：
```
[root@codis-proxy ~]# tar -zxf go1.4.1.linux-amd64.tar.gz -C /usr/local/
```
2、安装JDK，自己在官网下载一个安装即可，版本要求不严格，后面的zookeeper会用到JDK：
```
[root@codis-proxy ~]# sh jdk-6u34-fcs-bin-b04-linux-amd64-19_jul_2012.bin
[root@codis-proxy ~]# mv jdk1.6.0_34 /usr/local/ 
[root@codis-proxy ~]# mv /usr/local/jdk1.6.0_34 /usr/local/jdk
```
3、安装zookeeper
```
[root@codis-proxy ~]# wget http://mirrors.cnnic.cn/apache/zookeeper/zookeeper-3.4.6/zookeeper-3.4.6.tar.gz
[root@codis-proxy ~]# tar -xzf zookeeper-3.4.6.tar.gz -C /usr/local/
```
然后修改环境变量为 vim /etc/profile 在最后添加以下内容：
```
export GOROOT=/usr/local/go
export GOPATH=/usr/local/codis
JAVA_HOME=/usr/local/jdk
CLASS_PATH=$JAVA_HOME/lib:$JAVA_HOME/jre/lib
export ZOOKEEPER_HOME=/usr/local/zookeeper-3.4.6
export PATH=$PATH:$GOROOT/bin:$JAVA_HOME/bin:$ZOOKEEPER_HOME/bin
```
然后执行 source /etc/profile
编辑一个文件来测试一下go是否正常使用了：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy ~]# cat hello.go 
        package main
    import "fmt"
    func main(){
        fmt.Printf("hello,world\n")
 }
[root@codis-proxy ~]# go run hello.go 
hello,world
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
可以看到，正常打印hello,world了，在命令直接输入java，如果出现一大堆使用说明，就意味java安装成功了。
 4、安装codis,安装codis时依赖go，所以在安装codis先装好go,上面已经装好：
获取codis：
```
[root@codis-proxy ~]# yum install -y git
[root@codis-proxy ~]# go get github.com/wandoulabs/codis
package github.com/wandoulabs/codis
        imports github.com/wandoulabs/codis
        imports github.com/wandoulabs/codis: no buildable Go source files in /usr/local/codis/src/github.com/wandoulabs/codis
[root@codis-proxy ~]#
```
可以看到no buildable Go source files in /usr/local/codis/src/github.com/wandoulabs/codis，我们在上面环境就是设置GOAPTH=/usr/local/codis，所以只要执行上面的获取命令，就会下载在/usr/local/codis下：
我们进到提示的路径进行安装，安装过程比较久，耐心等待：
```
[root@codis-proxy ~]# cd /usr/local/codis/src/github.com/wandoulabs/codis
[root@codis-proxy codis]# ls
bootstrap.sh  cmd  doc  docker  Dockerfile  extern  Godeps  Makefile  MIT-LICENSE.txt  pkg  README.md  sample  test  vitess_license
[root@codis-proxy codis]# sh bootstrap.sh 
downloading dependcies, it may take a few minutes...
```
执行脚本完成看，会看到make gotest的结果：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
Hint: To run 'make test' is a good idea ;)
make[2]: Leaving directory `/usr/local/codis/src/github.com/wandoulabs/codis/extern/redis-2.8.13/src'
make[1]: Leaving directory `/usr/local/codis/src/github.com/wandoulabs/codis/extern/redis-2.8.13'
go test ./pkg/... ./cmd/... -race
?       github.com/wandoulabs/codis/pkg/env     [no test files]
ok      github.com/wandoulabs/codis/pkg/models  5.773s
ok      github.com/wandoulabs/codis/pkg/proxy/cachepool 0.009s
?       github.com/wandoulabs/codis/pkg/proxy/group     [no test files]
ok      github.com/wandoulabs/codis/pkg/proxy/parser    0.016s
?       github.com/wandoulabs/codis/pkg/proxy/redisconn [no test files]
?       github.com/wandoulabs/codis/pkg/proxy/redispool [no test files]
ok      github.com/wandoulabs/codis/pkg/proxy/router    16.092s
?       github.com/wandoulabs/codis/pkg/proxy/router/topology   [no test files]
ok      github.com/wandoulabs/codis/pkg/utils   0.008s
ok      github.com/wandoulabs/codis/cmd/cconfig 0.016s
?       github.com/wandoulabs/codis/cmd/proxy   [no test files]
[root@codis-proxy codis]# 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
会在 codis/bin 文件夹生成 codis-config, codis-proxy 两个可执行文件, (另外, bin/assets 文件夹是 codis-config 的 dashboard http 服务需要的前端资源, 需要和 codis-config 放置在同一文件夹下)
将编译好后，把bin目录和一些脚本复制过去/usr/local/codis目录下：
```
[root@codis-proxy codis]# mkdir -p /usr/local/codis/{log,redis_conf}
[root@codis-proxy codis]# cp -rf bin /usr/local/codis/
[root@codis-proxy codis]# cp sample/config.ini /usr/local/codis/bin/
[root@codis-proxy codis]# cp sample/redis_conf/6381.conf /usr/local/codis/redis_conf/
[root@codis-proxy codis]# cp -rf src/github.com/wandoulabs/codis/sample/* ./
```
 到止，codis-proxy的环境几乎搭建好了,下面进行配置：
5、进行配置
在codis-proxy上操作
配置zookeeper，修改配置文件：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy ~]# cp /usr/local/zookeeper-3.4.6/conf/zoo_sample.cfg /usr/local/zookeeper-3.4.6/conf/zoo.cfg
[root@codis-proxy ~]# cd /usr/local/zookeeper-3.4.6/conf/
[root@codis-proxy conf]# vim zoo.cfg
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/data/zookeeper/data
autopurge.snapRetainCount=500
autopurge.purgeInterval=24
clientPort=2181
#server.1=codis-1:2888:3888
#server.2=codis-2:2888:3888
#server.2=codis-3:2888:3888
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
设置myid：
设置myid在我们配置的dataDir指定的目录下面，创建一个myid文件，里面内容为一个数字，用来标识当前主机，conf/zoo.cfg文件配置的srver.X中的X为什么数字，则myid文件就输入这个数字，我只有一台zk，所以配置文件里可以不配置server.X，但还是要配置myid的，echo一个数字1进去即可。如果有多台zk，则分别在zk服务器上echo对应的数字进对应的myid文件
```
[root@codis-proxy conf]# mkdir -p /data/zookeeper/data
[root@codis-proxy conf]# echo "1" > /data/zookeeper/data/myid
```
启动zookeeper，因为环境变量里已经添加了路径,直接执行命令即可,执行zkServer.sh start后，要等一会再执行zkServer status:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy ~]# zkServer.sh start
JMX enabled by default
Using config: /usr/local/zookeeper-3.4.6/bin/../conf/zoo.cfg
Starting zookeeper ... STARTED
[root@codis-proxy ~]# zkServer.sh status
JMX enabled by default
Using config: /usr/local/zookeeper-3.4.6/bin/../conf/zoo.cfg
Mode: standalone
[root@codis-proxy ~]# 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
在那个目录下启动，就在那个目录下产生一个启动的日志zookeeper.out,查看这个日志可以查看是否正常启动了。由于单个zookeeper，所以这里指示Mode: standalone，如果有多个zk节点，就只有一个Mode: leader的状态，别的都是Mode: follower状态。
由于我现在是做一个机器做zk，所以这里几乎不用修改：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy codis]# cd /usr/local/codis
[root@codis-proxy codis]# cat config.ini
zk=localhost:2181  //zookeeper的地址, 如果是zookeeper集群，可以这么写: zk=hostname1:2181,hostname2:2181,hostname3:2181,hostname4:2181,hostname5:2181,如果是etcd，则写成http://hostname1:port,http://hostname2:port,http://hostname3:port
product=test     //产品名称, 这个codis集群的名字, 可以认为是命名空间, 不同命名空间的codis没有交集
proxy_id=proxy_1  //proxy会读取, 用于标记proxy的名字, 针对多个proxy的情况, 可以使用不同的config.ini, 只需要更改 proxy_id 即可
net_timeout=5     //检测状态时间间隔
dashboard_addr=localhost:18087  //dashboard 服务的地址，CLI 的所有命令都依赖于 dashboard 的 RESTful API，所以必须启动
coordinator=zookeeper   //如果用etcd，则将zookeeper替换为etcd
[root@codis-1 bin]# 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
6、查看一下启动流程：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy codis]# cat usage.md 
0. start zookeeper       //启动zookeeper服务
1. change config items in config.ini  //修改codis配置文件
2. ./start_dashboard.sh  //启动 dashboard
3. ./start_redis.sh      //启动redis实例
4. ./add_group.sh        //添加redis组，一个redis组只能有一个master
5. ./initslot.sh         //初始化槽
6. ./start_proxy.sh      //启动proxy
7. ./set_proxy_online.sh  //上线proxy项目
8. open browser to http://localhost:18087/admin //访问web
[root@codis-proxy codis]# 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这只是一个参考，有些顺序不是必须的，但启动dashboard前，必须启动zookeeper服务，这是必须的，后面有很多操作，都可以在web页面完成，例如添加/删除组，添加/删除redis实例等。
7、我们到启动dashboard这一步了，我们修改一下脚本，运行即可启动，修改为如下：（由本来的../修改为./）
```
[root@codis-proxy codis]# cat start_dashboard.sh 
#!/bin/sh
nohup ./bin/codis-config -c config.ini -L ./log/dashboard.log dashboard --addr=:18087 --http-log=./log/requests.log &>/dev/null &
[root@codis-proxy codis]#
```
启动后查看下是否正常启动了：
```
[root@codis-proxy codis]# sh start_dashboard.sh 
[root@codis-proxy codis]# ps -ef |grep dashboard
root      1463     1  2 20:38 pts/0    00:00:00 ./bin/codis-config -c config.ini -L ./log/dashboard.log dashboard --addr=:18087 --http-log=./log/requests.log
root      1470  1277  0 20:38 pts/0    00:00:00 grep dashboard
[root@codis-proxy codis]#
```
8、初始化槽（修改脚本，把../修改为./以及把后面的一些暂时没用的东西注释掉）
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy codis]# cat initslot.sh 
#!/bin/sh
echo "slots initializing..."
./bin/codis-config -c config.ini slot init -f
echo "done"
#echo "set slot ranges to server groups..."
#./bin/codis-config -c  config.ini slot range-set 0 511 1 online
#./bin/codis-config -c  config.ini slot range-set 512 1023 2 online
#echo "done"
[root@codis-proxy codis]# 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
执行初始化槽脚本：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy codis]# sh initslot.sh 
slots initializing...
{
  "msg": "OK",
  "ret": 0
}
done
[root@codis-proxy codis]# 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 9、分别在服务器redis-master和redis-slave上搭建好redis实例：
（1）在redis-master操作：
```
[root@redis-master ~]# tar zxf redis-2.8.19.tar.gz -C /usr/local/
[root@redis-master ~]# mkdir -p /data/cluster/7000
[root@redis-master ~]# cd /usr/local/redis-2.8.19/
[root@redis-master redis-2.8.19]# make && make install
[root@redis-master redis-2.8.19]# cp redis.conf /data/cluster/7000/
```
修改配置文件的端口为7000和把daemonize on修改为daemonize yes（注意：如果要上线到生产环境，配置文件根据需要修改）
```
[root@redis-master redis-2.8.19]# cd /data/cluster/7000/
[root@redis-master 7000]# sed -i '/port/s/6379/7000/' redis.conf                   
[root@redis-master 7000]# sed -i '/daemonize/s/no/yes/' redis.conf
```
启动实例：
```
[root@redis-master 7000]# redis-server redis.conf
[root@redis-master 7000]# ps -ef |grep 7000
root      2902     1  0 21:18 ?        00:00:01 redis-server *:7000    
root      2939  1183  0 21:37 pts/1    00:00:00 grep 7000
[root@redis-master 7000]#
```
redis-master已经启动成功了
（2）配置redis-slave,操作同上:（本以为要在redis-slave配置文件里添加 slaveof 192.168.10.128 7000 来配置主从关系，经过测试发现，是不用进行这样配置的）
在redis-slave服务器上操作：
```
[root@redis-slave ~]# tar zxf redis-2.8.19.tar.gz -C /usr/local/
[root@redis-slave ~]# cd /usr/local/redis-2.8.19/
[root@redis-slave redis-2.8.19]# make && make install
[root@redis-slave redis-2.8.19]# mkdir -p /data/cluster/7001
[root@redis-slave redis-2.8.19]# cp redis.conf /data/cluster/7001/
```
修改配置文件的端口为7000和把daemonize on修改为daemonize yes
```
[root@redis-slave redis-2.8.19]# cd /data/cluster/7001/
[root@redis-slave 7001]# sed -i '/port/s/6379/7001/' redis.conf                   
[root@redis-slave 7001]# sed -i '/daemonize/s/no/yes/' redis.conf
```
启动redis实例：
```
[root@redis-slave 7001]# redis-server redis.conf   
[root@redis-slave 7001]# ps -ef |grep 7001
root      2988     1  0 08:28 ?        00:00:00 redis-server *:7001    
root      3016  1304  0 08:40 pts/1    00:00:00 grep 7001
[root@redis-slave 7001]#
```
redis-slave已经正常启动。实例已经配置完，可以去web页面操作了。
10、添加组ID，为组添加主从实例，一个组里只能有一个redis-master：
访问dashboard的IP加端口：http://192.168.10.130:18087（最好用Firefox浏览器，别的浏览器坑了我N久，说多都是泪啊！！）
![](http://images.cnitblog.com/blog2015/645933/201504/231123430785271.png)
点New Server Group添加组：
![](http://images.cnitblog.com/blog2015/645933/201504/231126598598652.png)
点Add New Redis Instance添加主机：（第一个添加的都是master，也只能有一个master）
![](http://images.cnitblog.com/blog2015/645933/201504/231129300315541.png)
添加redis-slave主机：
![](http://images.cnitblog.com/blog2015/645933/201504/231131029845543.png)
添加完成后可以看到：
![](http://images.cnitblog.com/blog2015/645933/201504/231132089371194.png)
给组group_1分配槽的范围为1-1023：
![](http://images.cnitblog.com/blog2015/645933/201504/231142148754344.png)
成功后返回：
![](http://images.cnitblog.com/blog2015/645933/201504/231142326566545.png)
11、测试一下redis-master和redis-slave是否正常同步数据了：
在redis-master上写数据：
```
[root@redis-master 7000]# redis-cli -c -p 7000
127.0.0.1:7000> set name test
OK
127.0.0.1:7000> set age 24
OK
127.0.0.1:7000>
```
在redis-slave查看是否把数据同步过来了：
```
[root@redis-slave 7001]# redis-cli -c -p 7001
127.0.0.1:7001> get name
"test"
127.0.0.1:7001> get age
"29"
127.0.0.1:7001>
```
可以看到正常同步的。
11、启动codis-proxy,在codis-proxy服务器上操作：
修改脚本start_proxy.sh脚本，把../修改为./（该脚本意思是如果有启动的proxy，先停了，再启动；proxy_1是config.ini里定义的，如果有多个Proxy，只需要修改proxy_id即可）：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy codis]# cat start_proxy.sh 
#!/bin/sh
echo "shut down proxy_1..."
./bin/codis-config -c config.ini proxy offline proxy_1
echo "done"
echo "start new proxy..."
nohup ./bin/codis-proxy --log-level info -c config.ini -L ./log/proxy.log  --cpu=8 --addr=0.0.0.0:19000 --http-addr=0.0.0.0:11000 &
echo "done"
echo "sleep 3s"
sleep 3
tail -n 30 ./log/proxy.log
[root@codis-proxy codis]# 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
启动并检查进程：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
看到日志有点小报错，但可以忽略，后面出现[warning] proxy_1 wait to be online，意思是等待上线：
查看进程：
```
[root@codis-proxy codis]# ps -ef |grep proxy
avahi     1087     1  0 Apr22 ?        00:00:00 avahi-daemon: running [codis-proxy.local]
root      4016     1  0 14:34 pts/0    00:00:00 ./bin/codis-proxy --log-level info -c config.ini -L ./log/proxy.log --cpu=8 --addr=0.0.0.0:19000 --http-addr=0.0.0.0:11000
root      4028  1277  0 14:39 pts/0    00:00:00 grep proxy
[root@codis-proxy codis]#
```
上线proxy_1，修改脚本并运行脚本（把../修改为./）：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@codis-proxy codis]# cat set_proxy_online.sh 
#!/bin/sh
echo "set proxy_1 online"
./bin/codis-config -c config.ini proxy online proxy_1
echo "done"
[root@codis-proxy codis]# sh set_proxy_online.sh
{
  "msg": "OK",
  "ret": 0
}
[root@codis-proxy codis]# 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
去web面面刷新即可看到proxy_1：
![](http://images.cnitblog.com/blog2015/645933/201504/231450506093356.png)
下面做个测试，测试大体内容是：
1）假设redis-master有问题，也可以理解为挂了
2）在页面把redis-slave手动提升为master
3）把有问题的master下线
4）假如刚刚有问题的master已经正常恢复服务了，转为redis-slave
在上面已经测试了，在redis-master写数据,redis-slave上是可以看到数据同步过来的，现在在redis-slave上写数据，看主能不能看到：
```
[root@redis-slave 7001]# redis-cli -c -p 7001
127.0.0.1:7001> set MySQL innodb
OK
127.0.0.1:7001> get MySQL
"innodb"
127.0.0.1:7001>
```
在redis-master查看：
```
[root@redis-master 7000]# redis-cli -c -p 7000
127.0.0.1:7000> keys *
1) "name"
2) "age"
127.0.0.1:7000>
```
可以看到，redis-slave写数据，是不会到master上的。
现在模拟redis-master down机了：
```
[root@redis-master 7000]# ps -ef |grep 7000
root      3063     1  0 Apr15 ?        00:00:18 redis-server *:7000    
root      3460  1183  0 02:21 pts/1    00:00:00 grep 7000
[root@redis-master 7000]# kill -9 3063
[root@redis-master 7000]#
```
在web页面刷新可以看到master已经挂了：
![](http://images.cnitblog.com/blog2015/645933/201504/231532327966525.png)
我们把redis-slave提升master，点Promote to Master按钮即可：
![](http://images.cnitblog.com/blog2015/645933/201504/231513274841990.png)
可以看到本来的master已经变成offline下线状态了，这时我们把它删除掉，点后面的X删除主机。假如刚刚下线的redis服务器恢复正常了，再点上面的Add New Redis Instance按钮添加主机：
![](http://images.cnitblog.com/blog2015/645933/201504/231519325007376.png)
可以看到刚刚下线redis服务器，现在变成了redis-slave的角色了。我们测试下数据是否正常同步：
注意：现在7001端口的角色是master的了，在它上面写数据：
```
[root@redis-slave 7001]# redis-cli -c -p 7001
127.0.0.1:7001> set aa bb
OK
127.0.0.1:7001> set cc dd
OK
127.0.0.1:7001>
```
新的redis-slave上查看数据也是就7000端口对应的redis：
```
[root@redis-master 7000]# redis-cli -c -p 7000
127.0.0.1:7000> get aa
"bb"
127.0.0.1:7000> get cc
"dd"
127.0.0.1:7000>
```
可以看到是正常同步的。嘻嘻
12、下面我们进行redis压力测试说明和样例：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[root@redis-master ~]# redis-benchmark --help 
Usage: redis-benchmark [-h <host>] [-p <port>] [-c <clients>] [-n <requests]> [-k <boolean>]
 -h <hostname>      Server hostname (default 127.0.0.1)  --主机ip地址
 -p <port>          Server port (default 6379) --端口
 -s <socket>        Server socket (overrides host and port) --socket（如果测试在服务器上测可以用socket方式）
 -a <password>      Password for Redis Auth --redis的认证密码
 -c <clients>       Number of parallel connections (default 50) --客户端连接数
 -n <requests>      Total number of requests (default 100000)  --总请求数
 -d <size>          Data size of SET/GET value in bytes (default 2) --set、get的value大小
 -dbnum <db>        SELECT the specified db number (default 0) --选择哪个数据库测试（一般0-15）
 -k <boolean>       1=keep alive 0=reconnect (default 1) --是否采用keep alive模式
 -r <keyspacelen>   Use random keys for SET/GET/INCR, random values for SADD --随机产生键值时的随机数范围
  Using this option the benchmark will expand the string __rand_int__
  inside an argument with a 12 digits number in the specified range
  from 0 to keyspacelen-1. The substitution changes every time a command
  is executed. Default tests use this to hit random keys in the
  specified range.
 -P <numreq>        Pipeline <numreq> requests. Default 1 (no pipeline). --pipeline的个数（如果使用pipeline会把多个命令封装在一起提高效率）
 -q                 Quiet. Just show query/sec values --仅仅查看每秒的查询数
 --csv              Output in CSV format --用csv方式输出
 -l                 Loop. Run the tests forever --循环次数
 -t <tests>         Only run the comma separated list of tests. The test --指定命令
                    names are the same as the ones produced as output. 
 -I                 Idle mode. Just open N idle connections and wait.      --仅打开n个空闲链接
Examples:
 Run the benchmark with the default configuration against 127.0.0.1:6379:
   $ redis-benchmark
 Use 20 parallel clients, for a total of 100k requests, against 192.168.1.1:
   $ redis-benchmark -h 192.168.1.1 -p 6379 -n 100000 -c 20 --测试set、get、mset、sadd等场景下的性能
 Fill 127.0.0.1:6379 with about 1 million keys only using the SET test:
   $ redis-benchmark -t set -n 1000000 -r 100000000 --测试set随机数的性能
 Benchmark 127.0.0.1:6379 for a few commands producing CSV output:
   $ redis-benchmark -t ping,set,get -n 100000 --csv  --使用csv的输出方式测试
 Benchmark a specific command line:
   $ redis-benchmark -r 10000 -n 10000 eval 'return redis.call("ping")' 0 --测试基本命令的速度
 Fill a list with 10000 random elements:
   $ redis-benchmark -r 10000 -n 10000 lpush mylist __rand_int__ --测试list入队的速度
 On user specified command lines __rand_int__ is replaced with a random integer
 with a range of values selected by the -r option.
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
13、命令维护说明：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
总结：
一、搭建环境前，先好好看一下codis的资料还有zookeeper的资料，思路理清了，搭建起来就没那么吃力了，刚刚开始时，我纠结了N久zookeeper到底有什么用，哈哈，好好看看相关的资料就明白它的作用了。
二、codis还有别的测试，我上面就不一一写出了，如果上线到生产环境中，一定要做好各种测试，希望大家也能够把自己研究出来的东西分享出来，一起学习，一起进步^.^。
三、 [reids-cluster](http://www.cnblogs.com/xuanzhi201111/p/4411291.html)计划在redis3.0中推出了，因为刚推出不久，相信会存在不少的坑，所以生产环境中的redis集群可以考虑豌豆荚开源的codis，还有Twitter开源的twemproxy，这两个已经有很多公司在线上使用了，推荐大家使用，后期会补上twemproxy的博客。
参考资料：
codis官网：https://github.com/wandoulabs/codis
https://github.com/wandoulabs/codis/blob/master/doc/tutorial_zh.md
https://github.com/wandoulabs/codis/blob/master/doc/FAQ_zh.md
http://xmarker.blog.163.com/blog/static/226484057201422491547716/ （redis压力测试）
