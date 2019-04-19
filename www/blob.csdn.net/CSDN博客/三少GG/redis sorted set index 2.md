# redis sorted set index 2 - 三少GG - CSDN博客
2014年05月18日 11:25:52[三少GG](https://me.csdn.net/scut1135)阅读数：724
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

0.
http://qing.blog.sina.com.cn/tj/4a1a3ea33300004d.html
直接用压力测试代码压线上服务器，果然也慢。继续看 log ，似乎慢的请求有一个共性：某个时间点，几十个请求的耗时是一样的，都是类似 127ms，132ms 这样的。如果是真实的处理耗时，不应该会如此雷同才对，那么就应该是排队等待耗时了。这一批请求应该是同时发起的，因为某种原因，一同在某个地方排队，等轮到它们的时候，真实的处理耗费的时间连 1 ms 都不到，所以在 log 里看来，耗时如此相似。接下来的问题就是，为什么会排队？**在哪排队？**后面一个问题比较好回答，**Redis 是单线程的，按照网络接收到的包的顺序处理请求，前一个请求没有处理完之前，后面的请求都在 server 的 tcp/ip buffer　里排队，等待处理**。那剩下的就只有一个问题了：为什么会有排队现象？
仔细看业务代码，终于找到一个可疑的地方：hgetAll 。难道是一个 hgetAll 堵住了后面所有的请求？压测的脚本一般都只压 get，set，顶多也就试试 hget，hset，肯定不会压 hgetAll 这么变态的命令的。而用 strace 查看 redis server 的状态，也只能看到网络调用耗时。那么也就只能这么解释了：hgetAll 命令占用了大量的**网络带宽，导致它完成之前，其它的命令的回包都在 tcp buffer 里面堵着了。**
1.
### [记Redis那坑人的HGETALL](https://www.google.com.hk/url?sa=t&rct=j&q=&esrc=s&source=web&cd=6&ved=0CFAQFjAF&url=http%3A%2F%2Fhuoding.com%2F2013%2F01%2F21%2F214&ei=pAV4U4XAJ5Tr8AW7v4KACw&usg=AFQjCNEVUHsDz8vgdIaovHB3157F1PeBFQ)
Redis是单线程的！当它处理一个请求时其他的请求只能等着。通常请求都会很快处理完，但是当我们使用HGETALL的时候，必须遍历每个字段来 ...
http://huoding.com/2013/01/21/214
2.
http://www.kazaff.me/2013/02/21/hiredis%E5%85%A5%E9%97%A8/
3. http://www.searchdatabase.com.cn/showcontent_72701.htm
低效操作产生的延迟
单线程的一个结果是，当一个请求执行得很慢，其他的客户端调用就必须等待这个请求执行完毕。当执行GET、SET或者LPUSH 命令的时候这不是个问题，因为这些操作可在很短的常数时间内完成。然而，对于多个元素的操作，像SORT,LREM, SUNION 这些，做两个大数据集的交叉要花掉很长的时间。文档中提到了所有操作的算法复杂性。
 在使用一个你不熟悉的命令之前系统的检查它会是一个好办法。
　　如果你对延迟有要求，那么就不要执行涉及多个元素的慢操作，你可以使用Redis的replication功能，把这类慢操作全都放到replica上执行。可以用Redis 的Slow Log来监控慢操作。此外，你可以用你喜欢的进程监控程序(top, htop, prstat, 等...)来快速查看Redis进程的CPU使用率。如果traffic不高而CPU占用很高，八成说明有慢操作。
4.
http://lingqi1818.iteye.com/blog/1182429
结论： 
1.由于服务器采用单进程单线程的模型，服务器实际响应客户请求其实是先来后到的串行模式，只要服务器算法OK，那么单个请求响应时间绝对OK。 
2.假如你要提升服务器的并发能力，那么可以采用在单台机器部署多个redis进程的方式。
*********************
1.Data Modeling In Redis    http://openmymind.net/Data-Modeling-In-Redis/
2. 
# [Storing two way relational data in Redis](http://stackoverflow.com/questions/20451682/storing-two-way-relational-data-in-redis)
3. https://github.com/jiedan/redis-search-py/blob/master/redis_search/query.py
4. 
## MongoDB范围查询的索引优化
**(联合索引)**
http://blog.nosqlfan.com/html/4117.html
