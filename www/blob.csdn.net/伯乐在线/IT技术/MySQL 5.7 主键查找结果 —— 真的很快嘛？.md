# MySQL 5.7 主键查找结果 —— 真的很快嘛？ - 文章 - 伯乐在线
原文出处： [Vadim Tkachenko](https://dzone.com/articles/mysql-57-primary-key-lookup-results-is-it-really-f)   译文出处：[开源中国](http://www.oschina.net/translate/mysql-57-primary-key-lookup-results-is-it-really-faster?print)
这篇博客测试MySQL5.7的主键查询结果并确定MySQL5.7是不是真的比之前的版本查询的更快。
前段时间MySQL5.7刚刚发布，现在尘埃落定，是时候来测试一下它在性能上面的提升了。
我可不是仅仅为了满足自己的好奇心才做的这些测试！许多还在使用MySQL5.6（甚至是MySQL5.5）的用户经常问升级到5.7能获得多少的性能提升呢？ 或者它会不会反而降低性能呢,特别是当Peter写了这篇报告之后：https://www.percona.com/blog/2013/02/18/is-mysql-5-6-slower-than-mysql-5-5/?
为了弄清这些问题的答案，我们将会用测试数据说话。 测试有许多的方面需要考虑，我们从最简单的开始：MySQL主键查找已经加载到内存中的数据。这个工作不涉及事务，完全由CPU决定。
整个实验的结果，运行脚本，相关配置信息可以在我们的github主页上找到。
本次测试的实验环境为一台具备56个逻辑线程的服务器（2个cpu插槽，14个核心，2个超线程）
cpu为Intel(R) Xeon(R) E5-2683 v3 @ 2.00GHz CPUs.
下图为主要的测试结果：
![](http://jbcdn2.b0.upaiyun.com/2016/04/19b7ca95b0d227142d46f136328fb057.png)
由图示可以看出，在处理的线程数目小于20个时，MySQL5.5的性能表现是比MySQL5.7的要好，但是超过了20个线程之后，它的可伸缩性以及吞吐量方面出现了压力。在处理的线程数目不到120个时，MySQL5.6的性能时优于5.7的，但是当线程数超过了120个，MySQL5.7开始展现出优势，它可以维持吞吐量到1000个线程。
以上的结果是在客户端和服务端在相同的服务器系统上。为了验证这个结果的有效性，我也在系统配置10GB网络连接，客户端和服务端在不同服务器上，做了同样的测试。
这里是基于以上配置的测试结果：
![](http://jbcdn2.b0.upaiyun.com/2016/04/8eb26eeb541c3281da8d0c420e9578ca.png)
在这种情况下，我们把更多的负载置于服务器（因为客户机不共享资源），我们可以看到当线程增加到68个以后，MySQL5.7相比MySQL5.6，优势就立马显现出来了（MySQL5.6更早的暴露出扩展性问题）。
这里有另一个提高MySQL5.6在处理大量线程时性能的方法：设置 innodb-thread-concurrency 参数。我们来看一下当设置了innodb-thread-concurrency=64时，MySQL5.6的测试结果：
![](http://jbcdn2.b0.upaiyun.com/2016/04/688a231c751e158b9832032732562edb.png)
可以看到在处理数百个线程的时候，设置innodb-thread-concurrency参数可以提高MySQL5.6的性能。
在寻找办法提高整体吞吐量时，我发现在MySQL启动的时关闭PERFORMANCE_SCHEMA是个不错的选择 。这样做了之后，测试的表现明显变得更好。下面就是5.6和5.7在禁止了PERFORMANCE_SCHEMA之后的测试表现：
MySQL 5.6:
![](http://jbcdn2.b0.upaiyun.com/2016/04/5606d3e877b14171a3a96ccff6cd30a1.png)
MySQL 5.7:
![](http://jbcdn2.b0.upaiyun.com/2016/04/fd3cc088a520cc47a0915dc3890995ef.png)对于MySQL5.7，PERFORMANCE_SCHEMA的系统开销非常明显。
## **结论**
我可以说甲骨文在 MySQL 5.7 上做得很好，他们把焦点集中在主键的查找上。他们上报的每秒查询率（QPS）达到1.6M。
我没能达到1.6M；我达到的是每秒查询率（QPS）470K（配置 PERFORMANCE_SCHEMA 不启用）。我使用了 sysbench 0.5 的 LUA 脚本，并没有在测试期间做 statements 的准备。甲骨文使用了更老的 sysbench 0.4 （有准备的 statements ），并且他们的系统有144个逻辑线程。
MySQL 5.7 延续了其在线程较少时慢的传统。MySQL 5.6 甚至比 MySQL 5.5 更慢，而 MySQL 5.7 比 MySQL 5.6 更慢。
主键（PRIMARY KEY）查找在很多情况下不是唯一的工作负载，还有一些更有意思的！在即将发布的帖子里我将会展示其他工作负载的性能度量。
