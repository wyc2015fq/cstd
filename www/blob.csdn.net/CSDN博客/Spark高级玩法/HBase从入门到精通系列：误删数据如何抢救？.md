# HBase从入门到精通系列：误删数据如何抢救？ - Spark高级玩法 - CSDN博客
2018年06月28日 00:13:53[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：179
**云栖君导读：**有时候我们操作数据库的时候不小心误删数据，这时候如何找回？mysql里有binlog可以帮助我们恢复数据，但是没有开binlog也没有备份就尴尬了。如果是HBase，你没有做备份误删了又如何恢复呢？
**数据保护**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJFRPVdyMBRiarT8sS7qRld7vibK5Uxg0qFOevKDMbdriag9SLbhbXfMO0g/640?wx_fmt=png)
当误删数据发生时候，不管三七二十一，第一要务是进入hbase shell，执行如下命令：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJg3RrGhsbvficmXB1d92nM1iad4wBMyj9vaqibviawy1386GI1nAaOHFIDA/640?wx_fmt=png)
如果误删一张表的有多个family里的数据，需要都执行一下：
alter 'tt', { NAME => 'f1', KEEP_DELETED_CELLS => TRUE }, { NAME => 'f2', KEEP_DELETED_CELLS => TRUE }
设置 KEEP_DELETED_CELLS 为 True 的目的在于防止数据被物理删除。这里有必要解释一下HBase清理数据的原理：
- 
首先HBase是一个LSM架构，不断发生着数据文件的写入和合并
- 
当删除操作发生时，不会去清理数据文件中的数据，而是写入一个删除标记到新文件中。
- 
当某一刻major compaction发生时，在合并文件的同时会根据删除标记清理数据，新合并出来的数据文件不会再有旧数据。
KEEP_DELETED_CELLS 的作用就是在major compaction发生的时候，决定要不要清理旧数据。这里需要注意一点，即便 KEEP_DELETED_CELLS 设置为True，数据仍然会因为过期而被清理（HBsae表中的TTL属性）。这个设定无可厚非，既然过期了，误删不误删也无所谓了。
**数据恢复**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJFRPVdyMBRiarT8sS7qRld7vibK5Uxg0qFOevKDMbdriag9SLbhbXfMO0g/640?wx_fmt=png)
数据恢复的前提数据没有被物理删除，也就是上文提及的。你只需要在查询（Scan）的时候，指定raw模式来搜索数据，就能看到被删除的数据，之后你要做就是把数据再写入一次。我们来看一个简单的例子，还是以hbase shell为例子：
1.首先我们准备几行数据
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJomWUIFcDUz25m8xllneXpibph7G62NxqqaTnjalh95hXFWnHIYNLNYA/640?wx_fmt=png)
2.然后我们删掉y开头掉数据
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJPmfrHlZsZ2Z3zWiaejqo3qdbT1fa2a8CWuSlMLjoa13WYhPnwssB42w/640?wx_fmt=png)
3.查一下，现在只有3行了
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJ0ibDmWFwhPaPOzHAWvAbz3gEuM17cGTb43TtbCLia7JTDmQ8IpvqPo8g/640?wx_fmt=png)
4.现在我们带上raw再次查找数据，不仅能看到被删除数据，还能看到删除标记。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJmXPLU6owt4xTaTRg6fP0bZdoYlGxFsD3menKibhsB8xLrf38OiaEibuIQ/640?wx_fmt=png)
这里我设置了TIMERANGE， 指定的是数据写入的时间。对于我这个case其实并没有什么用，我只是想说明几点：
- 
即便不设置RAW，也可以通过时间搜索到被删数据。比如数据写入时间是T，delete时间是T+2，那么查找[0, T+1]的话就能看见数据。前提是设置了 KEEP_DELETED_CELLS=TRUE
- 
如果你后续写入重复的Key，那你必须指定好TIMERANGE，不然你可能看到的不是原先删除的keyVlaue。
- 
delete操作默认的时间不是当前server的时间，也不是构造Delete对象的时间，而是被删除的这个keyValue的写入时间。当然这个得看版本，测试时候发现1.x和2.x还是不一样的，有点坑，还以为高版本不能仅通过TIMERAGE搜素被删数据了。
- 
如果你的Delete mark的时间和数据的时间一样，那只能通过RAW看到。
数据恢复完，建议关闭KEEP_DELETED_CELLS，节省空间，提高查询效率。
**其他**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJFRPVdyMBRiarT8sS7qRld7vibK5Uxg0qFOevKDMbdriag9SLbhbXfMO0g/640?wx_fmt=png)
上文使用的是hbase shell演示，你可以使用任何语言的API完成上面的操作。
如果你使用的是云HBase，即便因为major compaction物理删除了数据，只要你开启了备份功能，依然可以恢复。
end
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tMJtfgIIibWJOTIpvjjynScictlTAXrjmJzvC4dwrOrNeTiaw45eicKhSez3sdicLosXk4cvNGicmO4TMI245RWSgr9g/640?wx_fmt=png)
如果，Google 早已解决不了你的问题。
如果，你还想知道 Apple、Facebook、IBM、阿里等国内外名企的核心架构设计。
来，我们在深圳准备了知识星球，想助你成长：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFU7Oic5ibSSV5sJGB2RgzSeqQpuBicxOPgCBglwQPt9rnatXN0VblKhw9WicqSGsuTU9o0Sj12698QpuQ/640?wx_fmt=jpeg)
