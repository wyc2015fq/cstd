# Redis主从在线互相切换 - z69183787的专栏 - CSDN博客
2017年02月28日 11:51:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1009
### 本文概述
由于某些原因，我们可能需要将[redis](http://www.ttlsa.com/redis/) master更换机器，我们可以停机进行更换，但是那样可能影响到用户体验。本文简要操作进行不停机迁移。
### 系统环境
CentOS 6.3 x64
redis-server 2.6.16
机器两台s1\s2
### 操作步骤
- 
1我们在新redis服务器上，启动一个redis实例，配置和master配置一致，不同的是配置文件中修改并启用 slave-read-only no，
让slave 能写，因为 “Since Redis 2.6 by default slaves are read-only.”
- 
让新redis成为slave：redis 127.0.0.1:6379>SLAVEOF s1 6379
然后可以查看s2上的log，会有大量的同步信息，也可以使用info命令查看状态。
- 
s2完全同步s1的数据后，我们修改游戏app的redis ip，改为s2的ip。
- 
查看游戏是否正常。
- 
以上没有问题后，在s2上执行 redis 127.0.0.1:6379> SLAVEOF NO ONE
- 
下线s1
PS：
SLAVEOF host port
SLAVEOF 命令用于在 Redis 运行时动态地修改复制(replication)功能的行为。
通过执行 SLAVEOF host port 命令，可以将当前服务器转变为指定服务器的从属服务器(slave server)。
如果当前服务器已经是某个主服务器(master server)的从属服务器，那么执行 SLAVEOF host port 将使当前服务器停止对旧主服务器的同步，丢弃旧数据集，转而开始对新主服务器进行同步。
另外，对一个从属服务器执行命令 SLAVEOF NO ONE 将使得这个从属服务器关闭复制功能，并从从属服务器转变回主服务器，原来同步所得的数据集不会被丢弃。
利用『 SLAVEOF NO ONE 不会丢弃同步所得数据集』这个特性，可以在主服务器失败的时候，将从属服务器用作新的主服务器，从而实现无间断运行。
可用版本：
>= 1.0.0
时间复杂度：
SLAVEOF host port ，O(N)， N 为要同步的数据数量。
SLAVEOF NO ONE ， O(1) 。
返回值：
总是返回 OK 。
