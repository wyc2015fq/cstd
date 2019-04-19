# 减小SQL SERVER的日志文件 - 左直拳的马桶_日用桶 - CSDN博客
2006年04月11日 14:31:00[左直拳](https://me.csdn.net/leftfist)阅读数：2708标签：[sql server																[数据库																[活动																[go																[出版																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=出版&t=blog)](https://so.csdn.net/so/search/s.do?q=go&t=blog)](https://so.csdn.net/so/search/s.do?q=活动&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
减小SQL SERVER的日志文件
左直拳
在使用数据库的过程中，频繁的新增、修改、删除，导致日志文件越来越大，甚至比数据库文件本身还大。
在企业管理器里面，好象并不能直接将该日志文件删除。限制文件的大小，只能是停留在目前的水平，比如，当前的日志文件是500M，那么你只能限制它大小为501M。当然在创建数据库的时候，可以指定日志文件的大小，例如10M。不过，也不知道到达了10M后系统会怎么样。不会报错吧？应该是一个环形的模式，到达临界点后又自动覆盖最前面的地方，这样循环反复，周而复始。
可是对于已经巨大无比的日志文件该怎么办呢？如果不想想办法，看样子还会继续发酵下去，直到塞满整个硬盘为止。
查阅资料。书上说（《SQL SERVER疑难解析》，电子工业出版社），SQL SERVER的日志文件有两种意义上的大小，一是逻辑大小，一是物理大小。物理大小指的是日志文件占用的硬盘空间，而逻辑大小是指实际存储了多少日志。要减少数据库日志文件的大小，可以采用收缩的办法，即删除不活动的逻辑日志。
这里分为两种情况：
1、数据库的故障还原模式为简单模式（可以在数据库的属性-选项中看到故障还原模式），可以用以下命令直接收缩：
DBCC SHRINKFILE(日记文件逻辑名称，收缩后以MB为单位的文件大小)
如：
DBCC SHRINKFILE(gcjs_log,1)
GO
运行后，日志文件将会收缩到1M。
2、数据库的故障还原模式为完全模式或大容量恢复模式，要先截断，将够多的逻辑日志标为不活动：
BACKUP LOG 数据库名 WITH NO_LOG
GO
然后再
DBCC SHRINKFILE(日记文件逻辑名称，收缩后以MB为单位的文件大小)
GO
这样，就可以将日志文件收缩到指定的大小。
日志文件可用来做数据恢复，书上教导我们，收缩日志文件应该建立在经常做数据库完全备份的基础上。
