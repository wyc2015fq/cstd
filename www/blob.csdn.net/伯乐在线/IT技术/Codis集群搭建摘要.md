# Codis集群搭建摘要 - 文章 - 伯乐在线
原文出处： [Qiannan](http://my.oschina.net/vampire/blog/639329?fromerr=oJyeOExe)
# 准备方案
## Golang环境搭建
环境搭建很简单，下载go1.4.2.linux-amd64.tar.gz安装包，直接解压并添加到环境变量就可以。 假设解压到/usr/local/go下，这个目录就是GOROOT，另外需要定义一个go开发目录，假设为/workspace/golang。 go开发目录未来会产生一些主要的子目录：
1. src 存放源码
2. pkg 编译后生成的文件
3. bin 编译后生产的可执行文件(比如godep命令在安装后就会放在这个目录下)
环境变量添加：

Shell
```
export GOROOT=/usr/local/go
 export GOPATH=/workspace/golang
 PATH=".:$PATH:$GOROOT/bin:$GOPATH/bin:$ZOOKEEPER/bin"
```
生效后就可以执行go version查看go的版本信息。 将$GOPATH/bin加入PATH是为了让一些开源的或者自己安装的go扩展命令(如godep)可以全局使用。
## Codis下载和编译
最新版的Codis位于[[https://github.com/CodisLabs/codis](https://github.com/CodisLabs/codis)] 通过下面命令获得源码

Shell
```
go get -u -d github.com/CodisLabs/codis
```
进入源码路径，执行make开始编译。
Codis的编译使用了godep，如果没有安装的话就会报godep command not found的错误。 godep是golang的一个包管理工具，通过
go get github.com/tools/godep
安装，成功后执行godep就会有对应的命令信息，如果没有可以在$GOPATH/bin目录下查找这个命令，然后加入环境变量。
如果你是用golang 1.5 beta3以上的版本进行编译，还有可能出现的一个问题是

Shell
```
GOPATH=godep path godep restore
 Error: GO15VENDOREXPERIMENT is enabled and the vendor/ directory is not a valid Go workspace.
 godep: Error restore requires GOPATH but it is empty.
 make: *** [godep] Error 1
```
这是因为golang 1.5 beta3之后go添加了GO15VENDOREXPERIMENT这个特性，并在1.6默认开启，你可以参照Codis issue715 里面的方案解决。 最简单就是在编译前

Shell
```
export GO15VENDOREXPERIMENT = 0
```
## 编译后的文件
编译后会产生一个bin目录，下面有三个可执行文件和一个目录

Shell
```
assets //dashboard的静态文件目录
 codis-config //codis 命令式配置管理组件
 codis-proxy //codis 代理也就是核心组件
 codis-server //codis 基于redis2.8.21的定制版redis
```
这就是我们最终集群部署的时候需要用到的文件，当然跨平台的话主要要交叉编译或者在特定平台下编译，不然可能无法运行。
## 集群方案
这里使用三台机器做一个小的集群。每台机器部署一个zookeeper实例，两个codis server实例(也就是redis server)。 其中两台机器部署codis proxy，另外一台开启codis dashboard。
|host|ip|zookeeper port|redis server master/slave port|codis proxy|codis dashboard|
|----|----|----|----|----|----|
|slave1|192.168.4.124|2181|6379/6380|Y||
|slave2|192.168.4.126|2181|6379/6380||Y|
|slave3|192.168.4.128|2181|6379/6380|Y| |
# 集群搭建
## zookeeper集群
zookeeper的搭建这里就不详细介绍了，最终节点是

Shell
```
1. slave1:2181
 2. slave2:2181
 3. slave3:2181
```
先把zookeeper起起来
## codis服务集群
#### 创建codis服务目录

Shell
```
[root@slave1]# mkdir -p /usr/local/codis/{log,redis_conf}
 [root@slave1]# cp -rf bin /usr/local/codis/
 [root@slave1]# cp config.ini /usr/local/codis/conf/
 [root@slave1]# cp extern/redis-test/6379.conf /usr/local/codis/redis_conf/
```
对每台机器执行以上操作
#### 根据注释修改config.ini文件

Shell
```
##### 这些配置是为dashboard和proxies服务的
# zookeeper or etcd
 coordinator=zookeeper
# Use comma "," for multiple instances. If you use etcd, you should also use this property.
 zk=slave1:2181,slave2:2181,slave3:2181
# 这个是标识zk下的命名空间，比如命名test在zookeeper会/zk/codis/db_test下
 product=test
# dashboard的地址和监听端口
 dashboard_addr=192.168.4.126:18087
password=
##### 下面是proxies的配置
# Proxy will ping-pong backend redis periodly to keep-alive
 backend_ping_period=5
# If there is no request from client for a long time, the connection will be droped. Set 0 to disable.
 session_max_timeout=1800
# Buffer size for each client connection.
 session_max_bufsize=131072
# Number of buffered requests for each client connection.
# Make sure this is higher than the max number of requests for each pipeline request, or your client may be blocked.
 session_max_pipeline=1024
# If proxy don't send a heartbeat in timeout millisecond which is usually because proxy has high load or even no response, zk will mark this proxy offline.
 # A higher timeout will recude the possibility of "session expired" but clients will not know the proxy has no response in time if the proxy is down indeed.
 # So we highly recommend you not to change this default timeout and use Jodis(https://github.com/CodisLabs/jodis)
 # which watches the available proxies and will skip the offline proxy or add new online proxy automatically.
 # If you are not using Java in client, you can DIY a zk watcher accourding to Jodis source code.
 zk_session_timeout=30000
##### 每个代理的id，不能相同 比如slave1 命名为proxy_1, slave2命名为proxy_2
 proxy_id=proxy_1
```
#### 启动dashboard
在slave2上执行 bin/codis-config dashboard, 该命令会启动 dashboard
#### 初始化slots
在任一机器上执行 bin/codis-config slot init，该命令会在zookeeper上创建slot相关信息
#### 启动Codis Redis
和官方redis的参数一下，三台机器上分别执行

Shell
```
[root@slave1]# /usr/local/codis/bin/codis-server /usr/local/codis/redis_conf/6379.conf &
 [root@slave1]# /usr/local/codis/bin/codis-server /usr/local/codis/redis_conf/6380.conf &
```
#### 添加Redis Server Group
每一个 Server Group 作为一个 Redis 服务器组存在, 只允许有一个 master, 可以有多个 slave, ***group id 仅支持大于等于1的整数***

Shell
```
$ bin/codis-config server -h usage:
 codis-config server list
 codis-config server add
 codis-config server remove
 codis-config server promote
 codis-config server add-group
 codis-config server remove-group
```
如: 添加三个 server group, 每个 group 有两个 redis 实例，group的id分别为1、2和3， redis实例为一主一从。
添加一个group，group的id为1， 并添加一个redis master到该group

Shell
```
$ bin/codis-config server add 1 slave1:6379 master
```
添加一个redis slave到该group

Shell
```
$ bin/codis-config server add 1 slave1:6380 slave
```
类似的，再添加group，group的id为2

Shell
```
$ bin/codis-config server add 2 slave2:6379 master
 $ bin/codis-config server add 2 slave2:6380 slave
```
类似的，再添加group，group的id为3

Shell
```
$ bin/codis-config server add 3 slave3:6379 master
 $ bin/codis-config server add 3 slave3:6380 slave
```
#### 设置 server group 服务的 slot 范围
Codis 采用 Pre-sharding 的技术来实现数据的分片, 默认分成 1024 个 slots (0-1023), 对于每个key来说, 通过以下公式确定所属的 Slot Id : SlotId = crc32(key) % 1024 每一个 slot 都会有一个且必须有一个特定的 server group id 来表示这个 slot 的数据由哪个 server group 来提供.

Shell
```
$ bin/codis-config slot -h
 usage:
 codis-config slot init
 codis-config slot info
 codis-config slot set
 codis-config slot range-set
 codis-config slot migrate [--delay=]
```
如:
设置编号为[0, 334]的 slot 由 server group 1 提供服务, 编号 [335, 669] 的 slot 由 server group 2 提供服务, 编号 [670, 1023] 的 slot 由 server group 3 提供服务,

Shell
```
$ bin/codis-config slot range-set 0 334 1 online
 $ bin/codis-config slot range-set 335 669 2 online
 $ bin/codis-config slot range-set 670 1023 3 online
```
#### 启动 codis-proxy

Shell
```
bin/codis-proxy -c config.ini -L ./log/proxy.log --cpu=8 --addr=0.0.0.0:19000 --http-addr=0.0.0.0:11000
```
刚启动的 codis-proxy 默认是处于 offline状态的, 然后设置 proxy 为 online 状态, 只有处于 online 状态的 proxy 才会对外提供服务

Shell
```
bin/codis-config -c config.ini proxy online <---- proxy的id, 如 proxy_1
```
#### 浏览器管理
访问http://192.168.4.126:18087/admin ，现在可以在浏览器里面完成各种操作了。
