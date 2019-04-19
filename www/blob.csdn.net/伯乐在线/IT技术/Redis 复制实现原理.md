# Redis 复制实现原理 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Float_Lu](http://www.jobbole.com/members/5334670325) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# 摘要
我的前一篇文章《[浅析Redis复制](http://blog.jobbole.com/101678/)》已经介绍了Redis复制相关特性，这篇文章主要在理解Redis复制相关源码的基础之上介绍Redis复制的实现原理。
# Redis复制实现原理
## 应用场景化
为了更好地表达与理解，我们先举个实际应用场景例子来看看Redis复制是怎么工作的，我们先启动一台master：

Shell
```
$ ./redis-server --port 8000
```
然后启动一个redis客户端和上面那台监听8000端口的Redis实例连接：

Shell
```
$ ./redis-cli -p 8000
```
我们向redis写一个数据：

Shell
```
127.0.0.1:8000> set msg doni
OK
127.0.0.1:8000> get msg
"doni"
```
于是我们可以假设以下场景：

Shell
```
我们有一台master实例master，master已经处于正常工作的状态，接受读写请求，这个时候由于单台机器的压力过大，我们想再启动一个Redis实例来分担master的读压力，假设我们新启动的这个实例叫slave。
已知M1的IP为127.0.0.1，端口为：8000
```
首先我们先启动redis实例,同时启动一个客户端连接这个实例:

Shell
```
$ ./redis-server --port 8001 
$ ./redis-cli -p 8001
```
这个时候slave是没有数据的：

Shell
```
127.0.0.1:8001> get msg
(nil)
```
我们可以用下面命令来让slave和master进行复制：

Shell
```
127.0.0.1:8001> slaveof 127.0.0.1 8000
```
于是，slave就获得了master上写的数据了：

Shell
```
127.0.0.1:8001> get msg
"doni"
```
上面的例子和很直观也很简单，下面我们就在脑海中缓存这个应用场景，来看看redis是如何实现复制的。
## 处理slaveof
我们首先需要看看slave接收到客户端的slaveof命令是如何处理的,下面是slave接收到客户端的slaveof命令的处理流程图：
![1](http://jbcdn2.b0.upaiyun.com/2016/06/28c8edde3d61a0411511d3b1866f06366.jpg)
slaveof命令处理流程图
解释下上图，redis实例接收到客户端的slaveof命令后的处理流程大致如下：
- 判断当前模式是否为cluster，如果是则不支持复制。
- 判断命令是否为slave’of no one，如果是，这表明客户端把当前实例设置为master。
- 如果客户端指定了host和port，则将host和port设置为当前的master信息。
- 将当前实例的复制状态设置为REPL_STATE_CONNECT。
除了上面的几个大步骤之外，在第二步和第三步之间还做了下面一些事情：
- 释放之前被阻塞的客户端，这些通常是使用Redis阻塞列表而被阻塞的客户端。
- 断开当前实例的所有slave。
- 清除缓存的master信息。
- 释放backlog，backlog是堆积环形缓冲区。
- 取消正在进行的握手过程。
上面就是Redis处理slaveof命令的大致流程，诶，好像并没有做关于复制的事情诶。别急，如果看过我的另一篇《[Redis网络架构及单线程模型](http://blog.jobbole.com/100079/)》文章的同学都应该知道redis的单线线程模型，这里slaveof命令处理关键的一步已经将当前redis实例的复制状态设置为了REPL_STATE_CONNECT状态，在redis的eventloop里面自然会对处于这个状态的redis实例进行处理。
## 连接master
复制异步处理的触发逻辑一方面是I/O事件驱动的一部分，另一方面就是eventloop对时间事件处理的一部分，其实也是定时任务，redis定时任务最外面一层是serverCron方法，serverCron方法囊括了其他几乎所有定时处理逻辑的入口，可以列个不完全列表如下：
- 过期key处理。
- 软件watchdog。
- 更新统计信息。
- rehash。
- 触发备份RDB文件或者AOF重写逻辑。
- 客户端超时处理。
- 复制逻辑。
- ……
我们这里只关心复制逻辑，调用代码如下：

Shell
```
run_with_period(1000) replicationCron();
```
run_with_period方法是redis封装的一个帮助方法，最然serverCron的调用频率很高，是1毫秒一次：

Shell
```
if (aeCreateTimeEvent(server.el, 1, serverCron, NULL, NULL) == AE_ERR) {
        serverPanic("Can't create event loop timers.");
        exit(1);
}
```
但是redis通过run_with_period实现了可以并不是每隔1毫秒必须要执行所有逻辑，run_with_period方法指定了具体的执行时间间隔。上面可以看出，redis主进程大概是1000毫秒也就是1秒钟执行一次replicationCron逻辑，replicationCron做什么事情呢，它做的事情很多，我们只关心本文的主线逻辑：

Shell
```
if (server.repl_state == REPL_STATE_CONNECT) {
   if (connectWithMaster() == C_OK) {
       serverLog(LL_NOTICE,"MASTER <-> SLAVE sync started");
   }
}
```
如果当前实例的复制状态为REPL_STATE_CONNECT，我们就会尝试着连接刚才slaveof指定的master，连接master的主要实现在connectWithMaster里面，connectWithMaster的逻辑相对简单一些，大致做了下面三件事情：
- 和指定的master建立连接，获取master的socket句柄，即fd。
- 注册fd的读写事件，事件处理器为syncWithMaster。
- 设置当前实例的复制状态为REPL_STATE_CONNECTING。
## 握手机制
上面已经注册了当前实例和master的读写I/O事件即事件处理器，由于I/O事件分离相关逻辑都由系统框架完成，也就是eventloop，因此我们可以直接看当前实例针对master连接的I/O处理实现部分，也就是syncWithMaster处理器。
syncWithMaster主要实现了当前实例和master之间的握手协议，核心是赋值状态迁移，我们可以用下面一张图表示：
![2](http://jbcdn2.b0.upaiyun.com/2016/06/665f644e43731ff9db3d341da5c827e18.jpg)
slave和msater的握手机制
上图为slave在syncWithMaster阶段做的事情，主要是和master进行握手，握手成功之后最后确定复制方案，中间涉及到迁移的状态集合如下：

Shell
```
#define REPL_STATE_CONNECTING 2 /* 等待和master连接 */
/* --- 握手状态开始 --- */
#define REPL_STATE_RECEIVE_PONG 3 /* 等待PING返回 */
#define REPL_STATE_SEND_AUTH 4 /* 发送认证消息 */
#define REPL_STATE_RECEIVE_AUTH 5 /* 等待认证回复 */
#define REPL_STATE_SEND_PORT 6 /* 发送REPLCONF信息，主要是当前实例监听端口 */
#define REPL_STATE_RECEIVE_PORT 7 /* 等待REPLCONF返回 */
#define REPL_STATE_SEND_CAPA 8 /* 发送REPLCONF capa */
#define REPL_STATE_RECEIVE_CAPA 9 /* 等待REPLCONF返回 */
#define REPL_STATE_SEND_PSYNC 10 /* 发送PSYNC */
#define REPL_STATE_RECEIVE_PSYNC 11 /* 等待PSYNC返回 */
/* --- 握手状态结束 --- */
#define REPL_STATE_TRANSFER 12 /* 正在从master接收RDB文件 */
```
当slave向master发送PSYNC命令之后，一般会得到三种回复，他们分别是：
- +FULLRESYNC：不好意思，需要全量复制哦。
- +CONTINUE：嘿嘿，可以进行增量同步。
- -ERR：不好意思，目前master还不支持PSYNC。
当slave和master确定好复制方案之后，slave注册一个读取RDB文件的I/O事件处理器，事件处理器为readSyncBulkPayload，然后将状态设置为REPL_STATE_TRANSFER，这基本就是syncWithMaster的实现。
## 处理PSYNC
### 全量还是增量
我们已经知道slave是怎么同master建立连接，怎么和master进行握手的了，那么master那边是什么情况呢，master在与slave握手之后，对于psync命令处理的秘密都在syncCommand方法里面，syncCommand方法实际包括两个命令处理的实现，一个是sync，一个是psync。我们继续看看，master对slave的psync的请求处理，如果当前请求不满足psync的条件，则需要进行全量复制，满足psync的条件有两个，一个是slave带来的runid是否为当前master的runid：

Shell
```
if (strcasecmp(master_runid, server.runid)) {
        //如果slave带来的runid“？”，说明slave想要强制走全量复制
        if (master_runid[0] != '?') {
            serverLog(LL_NOTICE,"Partial resynchronization not accepted: "
                "Runid mismatch (Client asked for runid '%s', my runid is '%s')",
                master_runid, server.runid);
        } else {
            serverLog(LL_NOTICE,"Full resync requested by slave %s",
                replicationGetSlaveName(c));
        }
        goto need_full_resync;
    }
```
如果不是，则需要全量同步。第二个条件即当前slave带来的复制offset，master在backlog中是否还能找到：

Shell
```
if (getLongLongFromObjectOrReply(c,c->argv[2],&psync_offset,NULL) !=
       C_OK) goto need_full_resync;
    if (!server.repl_backlog ||
        psync_offset < server.repl_backlog_off ||
        psync_offset > (server.repl_backlog_off + server.repl_backlog_histlen))
    {
        if (psync_offset > server.master_repl_offset) {
         //警告：slave带过来的offset不满足增量复制的条件
        }
        goto need_full_resync;
    }
```
如果找不到，不好意思，还是需要全量复制的，如果两个条件都满足，master会告诉slave可以增量复制，回复+CONTINUE消息。
### 复制是否正在进行
如果在当前slave执行复制请求之前，恰好已经有其他的slave已经请求过了，且master这个时候正在进行子进程传输（包括RDB文件备份和socket传输），那么分下面两种情况处理：
- 如果复制方式是RDB disk方式，则找到当前master状态为SLAVE_STATE_WAIT_BGSAVE_END的slave，复制这个slave的offset到当前slave的offset，这是为了当子进程完成RDB文件备份之后， 当前请求复制的slave可以和之前的slave一起进行master的复制操作。
- 如果复制方式是Diskless方式，则当前进来的slave并不会向上面那个slave这么幸运了，因为基于socket的复制已经正在进行了，当前slave只能参与下一轮的子进程复制，且状态为SLAVE_STATE_WAIT_BGSAVE_START。
如果没有子进程正在复制，这里针对RDB disk方式和diskless方式，又要分两种情况讨论：
- 如果是RDB disk方式，则启动子进程进行RDB文件备份。
- 如果是diskless方式，则等待一段时间，也是为了尽可能让后面的具有复制请求的slave一起进来，参与这一轮复制，复制开始由定时任务异步启动复制。
## 子进程结束后处理
RDB disk方式，当子进程备份RDB文件完毕，什么时候开始发送给slave的呢？diskless方式当子进程传输完毕，接下来又做什么呢？对于RDB disk的方式，这里涉及到一个I/O事件注册的过程，也是由serverCron驱动的，当子进程结束之后，主进程会得知，然后通过backgroundSaveDoneHandler处理器来进行处理，针对RDB disk类型和diskless类型的复制，处理逻辑是不一样的，我们分别来看看。
### RDB disk方式后处理
对于RDB disk复制方式，后处理主要是注册向slave发送RDB文件的处理器sendBulkToSlave：

Shell
```
if (aeCreateFileEvent(server.el, slave->fd, 
                    AE_WRITABLE, sendBulkToSlave, slave) == AE_ERR) {
                    freeClient(slave);
                    continue;
 }
```
然后RDB的文件发送由sendBulkToSlave处理器来完成，master对于RDB文件发送完毕之后会把slave的状态设置为：online。这里需要注意的是，在把slave设置为online状态之后会注册写处理器，将堆积在reply的数据发送给slave：

Shell
```
if (aeCreateFileEvent(server.el, slave->fd, AE_WRITABLE,
        sendReplyToClient, slave) == AE_ERR) {
        freeClient(slave);
        return;
    }
```
这部分的内容即为RDB文件开始备份到发送给slave结束这段时间的增量数据，因此需要注册I/O事件处理器，将这段时间累积的内容发送给slave，最终保持数据一致。
### diskless方式后处理
diskless方式的后处理不同的是当子进程结束的时候，其实RDB文件已经传输完成了，而且其中做了些事情：
- 当slave通过接受完RDB文件之后发送一个REPLCONF ACK给master。
- master接收到slave的REPLCONF ACK之后，开始将缓存的增量数据发送给slave。
因此这里不会注册sendBulkToSlave处理器，只需要将slave设置为online即可。我们还可以发现不同的一点，对于累积部分的数据处理，RDB disk方式是由master主动发送给slave的，而对于diskless方式，master收到slave的REPLCONF ACK之后才会将累积的数据发送出去，这点有些不同。
当子进程结束，后处理的过程中还要考虑到一种情况：

Shell
```
无论是RDB disk方式还是diskless方式，如果复制已经开始了，后来的slave需要同master复制，这部分的slave怎么办呢
```
怎么办呢，对于这类slave，slave的复制状态为SLAVE_STATE_WAIT_BGSAVE_START，语义上表示当前slave等待复制的开始，对于这种情况，Redis会直接启动子进程开始预备下一轮复制。
## RDB文件传输协议
上面握手机制部分提到，当slave和master握手完毕之后注册了个readSyncBulkPayload处理器，用于读取master发送过来的RDB文件，RDB文件通过TCP连接传输，本质上是一个数据流，slave端是如何区分当前传输方式是RDB disk方式还是diskless方式的呢？实际上对于不同的复制方式，数据传输协议也是不同的，假设我们把这个长长的RDB文件流称为RDB文件报文，我们来看看两种方式的不同协议格式：
![3](http://jbcdn2.b0.upaiyun.com/2016/06/38026ed22fc1a91d92b5d2ef93540f207.jpg)
RDB文件传输协议
上面有两种报文协议，第一种为RDB disk方式的RDB文件报文传输协议，TCP流以”$”开始，然后紧跟着报文的长度，以换行符结束，这样slave客户端读取长度之后就知道要从TCP后续的流中读取多少内容就算结束了。第二种为diskless复制方式的RDB文件报文传输协议，以”$EOF:”开头，紧跟着40字节长度的随机16进制字符串，RDB文件结尾也紧跟着同样的40字节长度的随机16进制字符串。slave客户端分别由TCP数据流的头部来判断复制类型，然后根据不同的协议去解析RDB文件，当RDB文件传输完成之后，slave会将RDB文件保存在本地，然后载入，这样slave就基本和master保持同步了。
# 总结
本文主要在了解Redis复制源码的基础之上介绍Redis复制的实现原理及一些细节，希望对大家有帮助。
注：本文由作者原创，如有疑问请联系作者。
redis复制源码注释地址：
[https://github.com/ericbbcc/redis/blob/comments/src/replication.c](https://github.com/ericbbcc/redis/blob/comments/src/replication.c)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7cb05a1a5b9ef06cc9d40cc79a096f293.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/12e72c4df391cc981614cc68aedd44085.jpg)
