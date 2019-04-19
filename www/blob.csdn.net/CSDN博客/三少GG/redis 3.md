# redis 3 - 三少GG - CSDN博客
2014年05月18日 18:44:39[三少GG](https://me.csdn.net/scut1135)阅读数：616
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

1.
http://www.kazaff.me/2013/02/19/phpredis%E7%9A%84%E4%BA%8B%E5%8A%A1%E6%93%8D%E4%BD%9C%E7%BB%86%E8%8A%82/
2.
放假没事学习了redis这个key-value数据库。顺便写了点学习笔记和大家分享下，希望对想了解redis的同学有所帮助。
[一 redis学习笔记之环境搭建](http://www.cnblogs.com/xhan/archive/2011/02/01/1948751.html)
[二 redis学习笔记之数据类型](http://www.cnblogs.com/xhan/archive/2011/02/02/1948891.html)
[三 redis学习笔记之排序](http://www.cnblogs.com/xhan/archive/2011/02/03/1948970.html)
[四 redis学习笔记之事务](http://www.cnblogs.com/xhan/archive/2011/02/04/1949151.html)
redis对事务的支持目前还比较简单。**r****edis只能保证一个client发起的事务中的命令可以连续的执行，而中间不会插入其他client的命令(指真正执行exec时）。由于redis是单线程来处理所有client的请求的所以做到这点是很容易的。**一般情况下redis在接受到一个client发来的命令后会立即处理并 返回处理结果，但是当一个client在一个连接中发出multi命令有，这个连接会进入一个事务上下文，该连接后续的命令并不是立即执行，而是先放到一
 个队列中。当从此连接受到exec命令后，redis会顺序的执行队列中的所有命令。并将所有命令的运行结果打包到一起返回给client.然后此连接就 结束事务上下文。
http://database.51cto.com/art/201208/351762.htm
http://blog.csdn.net/liutingxu1/article/details/17137407
乐观锁：大多数是基于数据版本（version）的记录机制实现的。即为数据增加一个版本标识，在基于数据库表的版本解决方案中，一般是通过为数据表添加一个“version”字段来实现取出数据时，将此版本号一同读出，之后更新时，对此版本号+1.此时，将提交数据的版本号与数据表对应记录的当前版本号进行对比，如果提交的数据版本号大于数据库当前版本号，则予以更新，否则认为是过期数据。-------svn 的版本控制就是最好的例子
Redis的乐观锁实例：假设有一个money的key，我们开2个session来对money进行赋值操作，我们来看一下结果如何？
Session1:
**127.0.0.1:6379> get money"301"127.0.0.1:6379> watch moneyOK127.0.0.1:6379> multiOK127.0.0.1:6379> incrby money 49QUEUED**
Session2:
**127.0.0.1:6379> set money 400OK127.0.0.1:6379> get money"400"**
Session1:
**127.0.0.1:6379> exec(nil)127.0.0.1:6379> get money"400"**
首先我们在session1中开启了对money的监控（watch key开启对key键的监控），然后开启一个事务，把命令incrby money 49放到事务队列中，等待exec执行处理。
之后我们开启session2，在session2中我们直接set money 400，这个时候我们在session2中get一下money发现已经变成400
之后我们回到session1中再用exec去执行事务队列中得incrby money 49这条命令，发现结果是nil也就是没有成功，用get命令看一下money发现是400，而不是449.
整个过程就实现了一个乐观锁。
[五 redis学习笔记之pipeline](http://www.cnblogs.com/xhan/archive/2011/02/05/1949276.html)
通过pipeline方式当有大批量的操作时候。我们可以节省很多原来浪费在网络延迟的时间。需要注意到是用 pipeline方式打包命令发送，redis必须在处理完所有命令前先缓存起所有命令的处理结果。
[六 redis学习笔记之发布订阅](http://www.cnblogs.com/xhan/archive/2011/02/06/1949473.html)
[七 redis学习笔记之持久化](http://www.cnblogs.com/xhan/archive/2011/02/07/1949640.html)
[八 redis学习笔记之主从复制](http://www.cnblogs.com/xhan/archive/2011/02/07/1949660.html)
[九 redis学习笔记之虚拟内存](http://www.cnblogs.com/xhan/archive/2011/02/07/1949717.html)
