# 【Spark】Spark的checkpoint机制 - zkq_1986的博客 - CSDN博客





2018年01月18日 14:47:16[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：137










Checkpoint 到底是什么和需要用 Checkpoint 解决什么问题：
- Spark 在生产环境下经常会面临 Transformation 的 RDD 非常多(例如一个Job 中包含1万个RDD) 或者是具体的 Transformation 产生的 RDD
 本身计算特别复杂和耗时(例如计算时常超过1个小时) , 可能业务比较复杂，此时我们必需考虑对计算结果的持久化。
- Spark 是擅长多步骤迭代，同时擅长基于 Job 的复用。这个时候如果曾经可以对计算的过程进行复用，就可以极大的提升效率。因为有时候有共同的步骤，就可以免却重复计算的时间。
- 如果采用 persists把数据在内存中的话，虽然最快速但是也是最不可靠的；如果放在磁盘上也不是完全可靠的，例如磁盘会损坏，系统管理员可能会清空磁盘。
- Checkpoint 的产生就是为了相对而言更加可靠的持久化数据，在 Checkpoint 可以指定把数据放在本地并且是多副本的方式，但是在正常生产环境下放在 HDFS 上，这就天然的借助HDFS
 高可靠的特征来完成最大化的可靠的持久化数据的方式。
- Checkpoint 是为了最大程度保证绝对可靠的复用 RDD 计算数据的
 Spark 的高级功能，通过 Checkpoint 我们通过把数据持久化到 HDFS 上来保证数据的最大程度的安任性
- Checkpoint 就是针对整个RDD 计算链条中特别需要数据持久化的环节(后面会反覆使用当前环节的RDD)
 开始基于HDFS 等的数据持久化复用策略，通过对 RDD 启动 Checkpoint 机制来实现容错和高可用；



## Checkpoint 运行原理图

[下图是 Checkpoint 运行原理图]

![](https://images2015.cnblogs.com/blog/1005794/201704/1005794-20170417193252102-1295283782.png)




RDD中的checkpoint，在进行这个操作的时候，其所依赖的所有的RDD都会从计算联调中清空掉，保存在之前设置的路径下，且所有parent级别的RDD都会被清空掉。




转载自：https://www.cnblogs.com/jcchoiling/p/6513569.html



