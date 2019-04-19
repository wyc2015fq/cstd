# 【MySQL】表锁和行锁 - Big Smile - CSDN博客
2018年08月01日 10:13:07[王啸tr1912](https://me.csdn.net/tr1912)阅读数：868
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
# 一、表锁
        特点：偏向MyISAM存储引擎，开销小，加锁快；无死锁；锁定粒度大，发生锁冲突的概率最高,并发度最低。
       我们在编辑表，或者执行修改表的事情了语句的时候，一般都会给表加上表锁，可以避免一些不同步的事情出现，表锁分为两种，一种是读锁，一种是写锁。
      我们可以手动给表加上这两种锁，语句是：
`lock table 表名 read(write); `
释放所有表的锁：
`unlock tables;`
查看加锁的表：
`show open tables;`
![](https://img-blog.csdn.net/20180801093302594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如图所示，in_use为1的就是加锁的表。
**加读锁（共享锁）：**
        我们给表加上读锁会有什么效果呢？
        1、我们加读锁的这个进程可以读加读锁的表，但是不能读其他的表。
        2、加读锁的这个进程不能update加读锁的表。
       3、其他进程可以读加读锁的表（因为是共享锁），也可以读其他表
       4、其他进程update加读锁的表会一直处于等待锁的状态，直到锁被释放后才会update成功。
**加写锁（独占锁）：**
       1、加锁进程可以对加锁的表做任何操作（CURD）。
       2、其他进程则不能查询加锁的表，需等待锁释放
**总结：**
     读锁会阻塞写，但是不会堵塞读。而写锁则会把读和写都堵塞。（特别注意进程）
**分析：**
`show status like 'table%';`
输入上述命令，可得：
![](https://img-blog.csdn.net/20180801095416465?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- Table_locks_immediate：产生表级锁定的次数，表示可以立即获取锁的查询次数，每立即获取锁值加1 。
- Table_locks_waited：出现表级锁定争用而发生等待的次数(不能立即获取锁的次数，每等待一次锁值加1)，此值高则说明存在着较严重的表级锁争用情况。
# 二、行锁
**    特点：**偏向InnoDB存储引擎，开销大，加锁慢；会出现死锁；锁定粒度最小，发生锁冲突的概率最低,并发度也最高。
行锁支持事务，所以  有关事务的知识下篇博客再总结。
**      行为：**
     1、当我们对一行进行更新但是不提交的时候，其他进程也对该行进行更新则需要进行等待，这就是行锁。
     2、如果我们对一行进行更新，其他进程更新别的行是不会受影响的。
**行锁升级为表锁：**
     当我们的行锁涉及到索引失效的时候，会触发表锁的行为。
|Session_1|Session_2|
|----|----|
|正常情况，各自锁定各自的行，互相不影响，一个2000另一个3000||
|![](https://img-blog.csdn.net/20180801100722162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|![](https://img-blog.csdn.net/20180801100725983?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|
|由于在column字段b上面建了索引，如果没有正常使用，会导致行锁变表锁||
|比如没加单引号导致索引失效，行锁变表锁![](https://img-blog.csdn.net/20180801100800261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|被阻塞，等待。只到Session_1提交后才阻塞解除，完成更新![](https://img-blog.csdn.net/20180801100816375?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|
所以，由此，我们还是要善用索引查询啊。
**间隙锁：**
         当我们用范围条件而不是相等条件检索数据，并请求共享或排他锁时，InnoDB会给符合条件的已有数据记录的索引项加锁；对于键值在条件范围内但并不存在的记录，叫做“间隙（GAP)”，InnoDB也会对这个“间隙”加锁，这种锁机制就是所谓的间隙锁（Next-Key锁）。
        因为Query执行过程中通过过范围查找的话，他会锁定整个范围内所有的索引键值，即使这个键值并不存在。
        间隙锁有一个比较致命的弱点，就是当锁定一个范围键值之后，即使某些不存在的键值也会被无辜的锁定，而造成在锁定的时候无法插入锁定键值范围内的任何数据。在某些场景下这可能会对性能造成很大的危害  
**优化建议：**
-    尽可能让所有数据检索都通过索引来完成，避免无索引行锁升级为表锁。
-    合理设计索引，尽量缩小锁的范围
-    尽可能较少检索条件，避免间隙锁
-    尽量控制事务大小，减少锁定资源量和时间长度
-   尽可能低级别事务隔离
