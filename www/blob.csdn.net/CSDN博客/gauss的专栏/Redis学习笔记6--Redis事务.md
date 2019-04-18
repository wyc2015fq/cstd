# Redis学习笔记6--Redis事务 - gauss的专栏 - CSDN博客
2014年10月17日 20:07:43[gauss](https://me.csdn.net/mathlmx)阅读数：297
redis对事务的支持目前还比较简单。redis只能保证一个client发起的事务中的命令可以连续的执行，而中间不会插入其他client的命令。 由于redis是单线程来处理所有client的请求的所以做到这点是很容易的。一般情况下redis在接受到一个client发来的命令后会立即处理并 返回处理结果，但是当一个client在一个连接中发出multi命令有，这个连接会进入一个事务上下文，该连接后续的命令并不是立即执行，而是先放到一 个队列中。当从此连接受到exec命令后，redis会顺序的执行队列中的所有命令。并将所有命令的运行结果打包到一起返回给client.然后此连接就
 结束事务上下文。  
下面可以看一个例子：
redis 127.0.0.1:6379> multi
OK
redis 127.0.0.1:6379> incr a
QUEUED
redis 127.0.0.1:6379> incr b
QUEUED
redis 127.0.0.1:6379> exec
1. (integer) 1
2. (integer) 1
从这个例子我们可以看到incr a ,incr b命令发出后并没执行而是被放到了队列中。调用exec后俩个命令被连续的执行，最后返回的是两条命令执行后的结果。
       我们可以调用discard命令来取消一个事务。接着上面例子：
redis 127.0.0.1:6379> multi
OK
redis 127.0.0.1:6379> incr a
QUEUED
redis 127.0.0.1:6379> incr b
QUEUED
redis 127.0.0.1:6379> discard
OK
redis 127.0.0.1:6379> get a
"1"
redis 127.0.0.1:6379> get b
"1"
可以发现这次incr a incr b都没被执行。discard命令其实就是清空事务的命令队列并退出事务上下文。
       虽说redis事务在本质上也相当于序列化隔离级别的了。但是由于事务上下文的命令只排队并不立即执行，所以事务中的写操作不能依赖事务中的读操作结果。很可能有两个client同时做这个操作，也就是说如果我们要实现一个类似incr命令的功能，先get a取出我的值，再将a++，然后set a，我们期望是加两次a从原来的1变成3. 但是很有可能两个client的get a，取到都是1，造成最终加两次结果却是2。主要问题我们没有对共享资源a的访问进行任何的同步。也就是说redis没提供任何的加锁机制来同步对a的访问。
还好redis 2.1后添加了watch命令，可以用来实现乐观锁。看个正确实现incr命令的例子，只是在前面加了watcha ：
redis 127.0.0.1:6379> watch a
OK
redis 127.0.0.1:6379> get a
"1"
redis 127.0.0.1:6379> multi
OK
redis 127.0.0.1:6379> set a 2
QUEUED
redis 127.0.0.1:6379> exec
1. OK
redis 127.0.0.1:6379> get a
"2"
watch 命令会监视给定的key，当exec时候如果监视的key从调用watch后发生过变化，则整个事务会失败。也可以调用watch多次监视多个key.这 样就可以对指定的key加乐观锁了。注意watch的key是对整个连接有效的，事务也一样。如果连接断开，监视和事务都会被自动清除。当然了 exec、discard、unwatch命令都会清除连接中的所有监视.
       redis的事务实现是如此简单，当然会存在一些问题。第一个问题是redis只能保证事务的每个命令连续执行，但是如果事务中的一个命令失败了，并不回滚其他命令，比如使用的命令类型不匹配。
redis 127.0.0.1:6379> set a 5
OK
redis 127.0.0.1:6379> lpush b 5
(integer) 1
redis 127.0.0.1:6379> set c 5
OK
redis 127.0.0.1:6379> multi
OK
redis 127.0.0.1:6379> incr a
QUEUED
redis 127.0.0.1:6379> incr b
QUEUED
redis 127.0.0.1:6379> incr c
QUEUED
redis 127.0.0.1:6379> exec
1. (integer) 6
2. (error) ERR Operation against a key holding the wrong kind of value
3. (integer) 6
可以看到虽然incr b失败了，但是其他两个命令还是执行了。
还有一个问题是当事务的执行过程中，如果redis意外的挂了。很遗憾只有部分命令执行了，后面的也就被丢弃了。当然如果我们使用的append-only file方式持久化，redis会用单个write操作写入整个事务内容。即是是这种方式还是有可能只部分写入了事务到磁盘。发生部分写入事务的情况下，redis重启时会检测到这种情况，然后失败退出。可以使用redis-check-aof工具进行修复，修复会删除部分写入的事务内容。修复完后就能够重新启动了。
