# hbase性能优化之bloomfilter - Spark高级玩法 - CSDN博客
2019年01月02日 23:22:52[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：77
**简介**
不了解bloomfilter的可以参考我以前的文章：
[海量数据处理之BloomFilter](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485970&idx=1&sn=37fe6a9d46625ad06ba58a0e6ad44d1b&chksm=9f38e93aa84f602c7bd7c9078fa3877bdc17b2b37b366aa9a33e30ad2d6941f6731aef490f9c&scene=21#wechat_redirect)
在判断元素是否存在的情形确实很高效。在hbase中的应用也是如此，可以使用bloomfilter在采用get方式获取数据的时候，过滤掉某些storefile文件，进而提升性能，当然会存在构建bloomfilter导致的性能开销。
从HBase 0.96开始，默认情况下启用基于行的布隆过滤器。可以选择禁用它们或更改某些表以使用行+列布隆过滤器，具体取决于数据的特征以及如何将其加载到HBase中。
可以选择为行，或行+列组合来启用Bloom过滤器。如果通常扫描整行，则行+列组合使用bloomfilter是无效的。也即是基于行的布隆过滤器可以对行+列进行Get操作。当每个数据条目的大小至少为几千字节时，Bloom过滤器效果最佳。
当数据存储在几个较大的StoreFiles中时，读取开销将降低，以避免在低级扫描期间额外的磁盘IO找到特定的行。
布隆过滤器需要在删除时重建，因此可能不适合具有大量删除的环境。
**常用场景**
1、根据key随机读时，在StoreFile级别进行过滤
2、读数据时，会查询到大量不存在的key，也可用于高效判断key是否存在
**案例说明**
a) ROW
根据KeyValue中的row来过滤storefile 
举例：假设有2个storefile文件sf1和sf2， 
sf1包含kv1（r1 cf:q1 v）、kv2（r2 cf:q1 v） 
sf2包含kv3（r3 cf:q1 v）、kv4（r4 cf:q1 v） 
如果设置了CF属性中的bloomfilter为ROW，那么get(r1)时就会过滤sf2，get(r3)就会过滤sf1 
b) ROWCOL
根据KeyValue中的row+qualifier来过滤storefile
举例：假设有2个storefile文件sf1和sf2， 
sf1包含kv1（r1 cf:q1 v）、kv2（r2 cf:q1 v） 
sf2包含kv3（r1 cf:q2 v）、kv4（r2 cf:q2 v） 
如果设置了CF属性中的bloomfilter为ROW，无论get(r1,q1)还是get(r1,q2)，都会读取sf1+sf2；而如果设置了CF属性中的bloomfilter为ROWCOL，那么get(r1,q1)就会过滤sf2，get(r1,q2)就会过滤sf1
启用Bloom过滤器
在列族上启用Bloom过滤器。您可以使用HColumnDescriptor的setBloomFilterType方法或使用HBase API来完成此操作。有效值是NONE，ROW（默认值），或ROWCOL。
以下示例创建一个表，并在colfam1列族上启用ROWCOL Bloom过滤器。
hbase> create 'test',{NAME =>'cf1', BLOOMFILTER =>'ROWCOL'}
相关配置
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWnqticwYpwT4bRIL8bIg3CwFKK1ZP6DMvNMgtHiaFK7FSiaaRZfQTYgPY8GIYnrTTLq9ZfG9aZkJnhA/640?wx_fmt=png)
推荐阅读：
[面试 | mr的的那些事儿](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486195&idx=1&sn=e9fcaf97c17e5f2854cf42f29a6f7cfb&chksm=9f38e9dba84f60cd7da00d86ea3a529749d0412adaa1f54a32fa842b23f49f2e9a613e42e2b6&scene=21#wechat_redirect)
[Hbase Rowkey设计及索引](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485928&idx=1&sn=3d9dd4e2d083f3b48bb85034f9322be3&chksm=9f38eac0a84f63d6e973cccf78f9b615c812a3c6261b82fab4293dd2982d818e3bf7d327d8ec&scene=21#wechat_redirect)
[Spark 下操作 HBase（1.0.0 新 API）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485050&idx=1&sn=56a578f0f69f992041e1ce566bc6952f&chksm=9f38e552a84f6c44891a8bb397b952a13fb9ff555f30178a27f3c330b30af1e0da4c1aee7fba&scene=21#wechat_redirect)
[面试|大数据中无处不在的ServiceLoader](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486173&idx=1&sn=58b4429640968709d11ad600168ad24d&chksm=9f38e9f5a84f60e38d1aa6a9977bc6df56a2975726683511481a034b9bb035a7e4ed9dd0da78&scene=21#wechat_redirect)
欢迎与近460好友一起学习大数据，冲刺2019！
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640?wx_fmt=jpeg)
