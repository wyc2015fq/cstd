# SQL调优日记：并行等待的原理和问题排查 - 文章 - 伯乐在线
原文出处： [owen zeng](http://www.cnblogs.com/OwenZeng/p/5632490.html)
# 概述
今天处理项目，客户反应数据库在某个时间段，反应特别慢。需要我们提供一些优化建议。
# 现象
由于是特定的时间段慢，排查起来就比较方便。直接查看这个时间段数据库的等待情况。查看等待类型发现了大量的CXPAKET等待类型且等待时间长.
![](http://jbcdn2.b0.upaiyun.com/2016/10/ccb5c4f177ef78b2f8ba1a6d99c37cd3.png)
有的看官可能知道，出现这个等待类似时，可以适当降低最大并行度来解决。但是为什么这么做呢？降低并行度就一定可以解决问题吗？
# CXPAKET原理
那什么是CXPAKET 等待呢。 当数据库引擎分析查询的开销超过设定的阈值时，SQL SERVER会选择并行执行。数据库引擎会为这个请求创建多个任务。每个任务处理数据的一个子集。每个任务可以在一个分开的CPU/核上执行。请求主要使用生产-消费 队列跟这些任务交互。如果这个队列是空的，（即生产者没有推入任何数据到这个队列）。这个消费者必须暂停并且等待。相应等待类型就是CXPACKET 等待类型。显示这个等待类型的请求 说明这个任务应该提供，但是没有提供任何（或足够）数据来消费。这些生产商任务反过来可能会暂停,等待一些其他类型的等待.
如下图：索引扫描就是一个并行执行的动作。
![](http://jbcdn2.b0.upaiyun.com/2016/10/b67d616fabbf50af251ff73912db3ab8.png)
## 打个比方
客户端程序就是老板，数据库引擎是部门领导，老板发出一个要求（request），查看最近一年的销售数据。领导一看这任务工作量大，一个人查太慢，要查到猴年马月。果断决定多派几个人。一次最多可以派多少个攻城狮呢？（就取决于最大并行度）这里假设是4个。这就分配4个人 小李、小王、小张、小陈去完成。 那这一年的任务怎么分配呢？ 以后再细说。 因为各种原因，其他人都做得了，小王还没有完成。领导不可能拿着半成品的数据就去找老板，只能等着小王。这就是CXPACKET.
# 排查
弄懂了CXPACKET的原理，那我们怎么来排查这类问题呢？首先，小王并不是偷懒，他的工作能力和其他人是相同的。所以，我们需要找出小王慢的原因，
使用下面的脚本：

MySQL
```
select r.session_id,
         status,
         command,
         r.blocking_session_id,
         r.wait_type as[request_wait_type],
         r.wait_time as[request_wait_time],
         t.wait_type as[task_wait_type],
         t.wait_duration_ms as[task_wait_time],
         t.blocking_session_id,
         t.resource_description
from sys.dm_exec_requests r
LEFT join sys.dm_os_waiting_tasks t
         on r.session_id = t.session_id
where r.session_id >=50
and r.session_id <> spid;
```
通过上面的语句我们找到，并行等待正在等待LCK_M_S.说明查询是被其他的操作阻塞了。上面的问题是由于一个写入语句引起的。这个语句是一个很简单的插入动作，为什么写入会这么慢呢。可以查看磁盘响应时间，，磁盘队列
![](http://jbcdn2.b0.upaiyun.com/2016/10/5ed7194f07b7a478b37ef8469131f1f4.png)
发现都出奇的高。
# 建议
看来问题是由于磁盘本身引起的。
1.更换读写速度更快的磁盘
2.目前数据文件和日志文件在同一物理磁盘，分割开来
3.从业务出发。经过和客户沟通后发现，这个表是操作日志表。每次做业务操作都会记录日志。所以特别的大。
对应这样的表，可以单独建立文件夹组，文件，并把表放在单独的磁盘，缓解IO压力
