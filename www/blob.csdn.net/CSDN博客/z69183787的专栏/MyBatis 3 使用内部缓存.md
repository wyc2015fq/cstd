# MyBatis 3 使用内部缓存 - z69183787的专栏 - CSDN博客
2018年06月08日 14:13:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：121
[https://blog.csdn.net/shootyou/article/details/8235069](https://blog.csdn.net/shootyou/article/details/8235069)
在面对查询多而且修改少的运用场景之下，可以开启MyBatis的内部缓存。开启非常方便，但是要注意缓存刷新的逻辑。
开启二级缓存，你只需要在 SQL 映射文件中加入简单的一行：
<cache/>
这句简单的语句的作用如下：
1.所有在映射文件里的 select 语句都将被缓存。
2.所有在映射文件里 insert,update 和 delete 语句会清空缓存。
3.缓存使用“最近很少使用”算法来回收
4.缓存不会被设定的时间所清空。
5.每个缓存可以存储 1024 个列表或对象的引用（不管查询出来的结果是什么） 。
6.缓存将作为“读/写”缓存，意味着获取的对象不是共享的且对调用者是安全的。不会有其它的调用者或线程潜在修改。
缓存元素的所有特性都可以通过属性来修改。比如：
<cache
eviction="FIFO"
flushInterval="60000"
size="512"
readOnly="true"/>
如果你需要使用EhCache作为MyBatis的缓存，那么可以参考：[http://qiuqiu0034.iteye.com/blog/1162952](http://qiuqiu0034.iteye.com/blog/1162952)
适合使用内部缓存的场景如下：
1.缓存数据量不大，如果缓存数据量大那么要么就需要对cache的配置进行调优，要么就使用ehcache，否则可能不会得到很好的效果。
2.缓存逻辑简单，对于单张表的操作select insert update delete都在同一映射文件中最好了。
