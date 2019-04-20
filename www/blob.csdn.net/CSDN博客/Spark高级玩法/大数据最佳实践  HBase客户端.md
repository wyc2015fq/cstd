# 大数据最佳实践 | HBase客户端 - Spark高级玩法 - CSDN博客
2018年01月11日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：580
1**减少RPC调用的方法**
## 1.1.**问题提出**
HBase中rowkey是索引，任何对全表的扫描或是统计都需要用到scan接口，一般都是通过next()方法获取数据。而每一个next()调用都会为每行数据生成一个单独的RPC请求，这样会产生大量的RPC请求，性能不会很好。
## 1.2.**解决思路**
如果执行一次RPC请求就可以获取多行数据，那肯定会大大提高系统的性能。这一块主要分为面向行级的缓存以及面向列级的缓存：
**1）面向行级的缓存**
我们可以通过使用扫描缓存方法来实现，不过这个缓存默认是关闭的，要用得打开。在表的层面使用时，这个表所有的扫描实例的缓存都会生效，在扫描层面也只会影响当前的扫描实例。
用户可以使用HTable.setScannerCaching()方法设置表级的扫描缓存，以及使用Scan.setCaching()方法设置扫描级的缓存。
**2）面向列级的批量**
用户可以使用Scan.setBatch()方法设置返回多少列。
通过组合使用扫描器缓存和批量大小的方式，可以让用户方便的控制扫描一个范围内的行健时所需要的RPC调用次数。
## 1.3.**实践情况**
举例如下：
我们建立了一张有两个列族的表，添加了10行数据，每个行的每个列族下有10列。这意味着整个表一共有200列（或单元格，因为每个列只有一个版本），其中每行有20列。公式如下：
RPC请求的次数 =（行数×每行的列数）/Min（每行的列数，批量大小）/扫描器缓存
表说明如下：
|缓存|批量处理|Result个数|RPC次数|说明|
|----|----|----|----|----|
|1|1|200|201|每个列都作为一个Result实例返回。最后还多一个RPC确认扫描完成。|
|200|1|200|2|每个Result实例都只包含一列的值，不过它们都被一次RPC请求取回（加一次完成检查）。|
|2|10|20|11|批量参数是一行所包含的列数的一半，所以200列除以10，需要20个Result实例。同时需要10次RPC请求取回（加一次完成检查）。|
|5|100|10|3|对于一行来讲，这个批量参数太大了，所以一行的20列都被放入了一个Result实例中。同时缓存为5，所以10个Result实例被两次RPC请求取回（加一次完成检查）。|
|5|20|10|3|同上，不过这次的批量值与一行的列数正好相同，所以输出与上面一种情况相同。|
|10|10|20|3|这次把表分成了较小的Result实例，但使用了较大的缓存值，所以也是只用了两次RPC请求就取回了数据。|
要计算一次扫描操作的RPC请求的次数，用户需要先计算出行数和每行列数的乘积（至少了解大概情况）。然后用这个值除以批量大小和每行列数中较小的那个值。最后再用除得的结果除以扫描器缓存值。
## 1.4.**效果评价**
合理的组合使用扫描器缓存和批量大小，可以有效的减少client端和服务器的RPC交互次数，提供系统整体性能。
## 1.5.**注意事项**
- 
scanner需要通过客户端的内存来维持这些被cache的行记录，合理设置catching大小，防止出现OOM；
- 
cache使用的内存计算公式为：并发数×cache数×单个result大小。
2**客户端其它最佳实践方法**

## 2.1.**问题提出**
平常情况下，很多的应用主要是通过使用客户端来访问HBase集群，进而完成业务。因此整个系统的性能有很大一部分依赖于客户端的性能。客户端的开发主要是使用HBase提供的API，往往又由于不同的程序员对API的掌握程度不一，导致了客户端的性能差别很大。
## 2.2.**解决思路**
客户端是使用HBase提供的API来完成读写数据，因此我们针对API的使用整理了一些最佳实践。
**1）禁止自动刷新**
当有大量的写入操作时，使用setAutoFlush(false)方法，确认HTable自动刷新的特性已经被关闭。否则Put实例将会被逐个传送到region服务器。通过HTable.add(Put)添加的Put实例都会添加到一个相同的写入缓存中，如果用户禁用了自动刷新，这些操作直到写缓冲区被填满时才会被送出。如果要显示地刷写数据，用户可以调用flushCommits()方法。调用HTable实例的close()方法也会隐式地调用flushCommits()。
默认的客户端写缓存是2M，我们可以通过修改hbase.client.write.buffer配置来设置大小，以满足应用的需要。
**2）使用扫描缓存**
如果HBase被用作一个MapReduce作业的输入源，最好将作为MapReduce作业输入扫描器实例的缓存用setCaching()方法设置为比默认值100大得多的值。使用默认的值意味着map任务会在处理每条记录时请求region服务器。例如，将这个值设置为500，则一次可以传送500行数据到客户端进行处理。这里用户需要权衡传输数据的开销和内存的开销，因为缓存更大之后，无论是客户端还是服务器端都将消耗更多内存缓存数据，因此大的缓存并不一定最好。
**3）限定扫描范围**
当Scan被用来处理大量行时（特别是被用作MapReduce输入源时），注意哪些属性被选中了。如果Scan.addFamily(byte [] family)被调用了，那么特定列族中的所有都将被返回到客户端。
如果只处理列，则应当只有这列被添加到Scan的输入中，如scan.addColumn(byte [] family,byte [] qualifier)，因为选中了过多的列将导致大数据集上极大的效率损失。
如果是选择多列，可以使用scan. setFamilyMap(Map<byte[], NavigableSet<byte []>> familyMap)添加多个列族下的多列。
**4）关闭ResultScanner**
这不会带来性能提升，但是会避免可能的性能问题。如果用户忘记关闭由HTable.getScanner()返回的ResultScanner实例，则可能对服务器端造成影响。
所以建议在在try/catch的finally块中关闭ResultScanner，例如：
   Scan scan = newScan();
   ResultScannerscanner = table.getScanner(scan);
   try {
   for (Resultresult: scanner) {
   //procrss result...
   }
   } catcah (IOExceptione){
   //throwexception
   } finally {
   scanner.close();
   }
   table.close();
**5）优化获取行健的方式**
当执行一个表的扫描以获取需要的行键时（没有列族、列名、列值和时间戳），在Scan中用setFilter()方法添加一个带MUST_PASS_ALL操作符的FilterList。FilterList中包含FirstKeyOnlyFilter和KeyOnlyFilter两个过滤器，使用以上组合的过滤器将会把发现的第一个KeyValue行键（也就是第一列的行键）返回给客户端，这将会最大程度地减少网络传输。
**推荐阅读：**
1，[Hbase源码系列之BufferedMutator的Demo和源码解析](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483750&idx=1&sn=8f86e86ff26111155d42ba55e23cda8d&chksm=9f38e24ea84f6b58405e0b751bb0d19d31a3adcdf422e6c1bf231d6741ebb2b0ef42661eac26&scene=21#wechat_redirect)
2，[HBase原理和设计](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484241&idx=1&sn=2f4d59fe83d74a1bc764230a574972dc&chksm=9f38e079a84f696f32c2b823024164ea2c83d4775d24f02ca1d3b3ac6731a1172688001e9666&scene=21#wechat_redirect)
3，[HBase的安装部署](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484117&idx=2&sn=b5cbd15e1259d5727f7c36ee83c60989&chksm=9f38e1fda84f68ebf4a0e52b888dc7b8885d1c899426067a6fa4b2ca3b903b1594d566efab2f&scene=21#wechat_redirect)
4，[大数据基础系列之kafka011生产者缓存超时，幂等性和事务实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483812&idx=1&sn=d49338e7a38fdc30a8d20c054b61d3fd&chksm=9f38e28ca84f6b9aa0d8a4f42ca48fa8711d1653bf5f57a8948cf1cb8e1c5967c1b83e46516c&scene=21#wechat_redirect)
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
