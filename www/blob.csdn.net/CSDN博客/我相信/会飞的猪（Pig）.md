# 会飞的猪（Pig） - 我相信...... - CSDN博客





2014年12月21日 14:37:57[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1589
所属专栏：[大数据修炼道场](https://blog.csdn.net/column/details/12856.html)









猪会飞么?

提高pig的性能,让猪飞起来有如下办法：
1. 尽早地并经常地进行过滤
2. 尽早地并经常地进行映射
3. 正确并合理地使用Join
4. 选择正确的数据类型，合适的并行值
5. 调整pig 的性能属性：pig.cachedbag.menusage 和pig.skewedjoin.reduce.memusage
6. 对中间结果进行压缩





Describe 命令会显示脚本中指定关系的模式。
explain 可以深入到pig内部看如何将用户的脚本编译成MapReduce任务的。

illustrate 对用户的数据进行抽样并且对抽样的数据执行用户的脚本。

在提交的hadoop集群执行之前，pig需要消耗几秒钟来解析脚本，这些M/R任务真正运行时才会显示在JobTracker页面上。

PigUnit提供了一个集成到JUnit中的单元测试框架，在0.8版本引入的。


Hadoop降低了IO约束但并没有消除，如果map任务的个数两倍于实际的槽位来执行，那就将消耗两倍于平均map过程执行时间来执行完所有的map任务，如果再额外增加一个map任务，所消耗时间将增加到平均时间的3倍。

shuffle数据量大小是指从map任务转移到reduce任务的数据量大小，一般需要m X r 个网络连接。





