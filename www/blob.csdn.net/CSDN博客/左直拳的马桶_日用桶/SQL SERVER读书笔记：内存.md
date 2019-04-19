# SQL SERVER读书笔记：内存 - 左直拳的马桶_日用桶 - CSDN博客
2013年09月17日 23:32:36[左直拳](https://me.csdn.net/leftfist)阅读数：2630
系统先操作地址空间，真正要用的时候才申请物理内存，进行使用。
Reserved Memory  保留内存，虚拟内存
Commited Memory 提交内存，物理内存
【如何判断SQL SERVER 内存不足？】
如果进程的用户态CPU时间比较高，一般说明它没有遇到其他瓶颈，正全力运行。如果内存有瓶颈，一般用户态的CPU时间不会很高，但核心态的CPU时间反而升高，因为系统要做的paging动作（pages/sec计数器）。同时磁盘也会比较繁忙。但磁盘繁忙，而paging不忙，却不能说明内存不足。
process%pressor Time：用户态 + 核心态时间
【如何合理配置SQL SERVER内存】
1、为系统和其他应用程序预留足够的内存。如果可能，数据库服务器应该专供数据库使用
2、安装64位操作系统
3、给SQL SERVER启动账号 Lock pages inmemory的权限
用户修改数据之后，SQL SERVER会在内存中将数据所在页面修改，但不会立刻将这个页面写回硬盘，等到checkpoint 或 lazy write的时候集中处理。如果没有内存压力，则这些页面会一直留在内存中，不必再从硬盘上读取。
【内存使用分类】
一、按用途分
1、DataBase Cache：数据页面缓冲区
2、各类 Consumer
二、按申请方式分
1、DataBase ，先Reserve一大块，再一小块一小块地commit
2、stolen，直接commit
三、按申请大小分类
1、buffer pool，单位<=8kb
2、MemToLeave，单位>8kb
【内存分布及大小限制】
在32位
SQL SERVER可以用2GB的内存，其中默认384M是MemToLeave，而BufferPool = 2GB - 384M = 1.6G
为什么叫MemToLeave？是因为SQL SERVER刚启动的时候，就会把这一块虚拟地址预留出来。
如果开启了AWE，Buffer Pool里的DataBase Cache可以使用AWE扩展出来的内存，而Buffer Pool里的Stolen，则还是用原有的那份虚拟地址空间，最多可用满至1.6G而MemToLeave则不变。
【MemToLeave的使用情况】
SQL SERVER会先用默认的256M，用完了再看看预留给线程的那一块。
MemToLeave = 256 + 线程数 * 0.5 = 256 + 128 = 384M
64位
MemToLeave没有限制。有时候反而会挤占了DataBase Cache。
【增加MemToLeave】
加启动参数 -g 512
【增加内存一定会提高SQL SERVER的性能吗？】
不一定。主要看瓶颈在哪一部分。如果是Buffer Pool，32位系统中，DataBase Cache存在瓶颈，增加内存是对的，但如果是stolen欠缺，即使是应用了AWE，极限值也是1.6GB而已；如果是MemToLeave，就更有限制了，增加内存并无裨益。
【内存动态管理视图（DMV)】
DMV可用于监视SQL SERVER的内存情况，有些信息。使用性能监视器，DBCC语句等传统方法也能得到，但DMV更详细、准确。
【Lazy Writer】
惰性编写器。系统进程，用于成批刷新脏的老化得缓冲区，包括更改、写回磁盘，清理最久没有被重用到的数据页和执行计划。如果经常被触发，内存应该存在瓶颈。
【索引】
索引可以避免查询时将表的全部记录装入内存，从而节约内存使用量。
【stolen Memory】
stolen memory对sql server正常运行不可或缺，一条语句的分析、优化、执行计划缓存、排序、计算等，都需要用到stolen。
stolen不太会因为使用太多而侵占了DataBase Pages一来是因为它不太大，二来也会定期的清除，除非有游标不关，或内存泄露什么的。
【Multi_Page（MemToLeave）】
在32位的SQL SERVER上，Multi-Page的数目是有限制的，默认为384M，这个地址是预留的，Buffer Pool不能占用。
而在64位 SQL SERVER上，Multi-page没有限制，要多少有多少。所谓的MaxServerMemory是对Buffer Pool来讲的。
作用：
1、SQL SERVER 进程
2、超过8K的stolen内存
3、第三方、如CLR、Link Server、COM
如果其配额不足，会出现新用户很难登入SQL 的错误
