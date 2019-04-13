
# 事务隔离机制原理深入分析以及MySQL不同隔离级别分场景下实验对比 - 无界 - CSDN博客

置顶2016年10月18日 18:59:38[21aspnet](https://me.csdn.net/21aspnet)阅读数：3653


这是我总结的事务的四种隔离机制，比较好理解，主要是有些地方文字游戏说不清楚很容易混淆：
Read Uncommitted（**读未提交**）A未完，B已更新，未提交，A读到B已更新的数据，由于未提交，那么可能会回滚，所以这样的数据就是错误的数据也就是脏读。
Read Committed（**读已提交**）A未完，B已更新，已提交，A读到B已更新且提交的数据，由于已提交，所以是正确的数据，但是可能这个事务比较长，读几次都不一样，从业务上说也许对也许不对，所以这个是不可重复读的。
Repeatable Read（**可重复读**）A未完，B不一定可以更新，不管B有无更新/提交，A读不到B的数据（MYSQL默认），自己每次读都是一样的，但是存在幻读，幻读是期间发送新增删除这样的操作导致。
Serializable（**串行化**），绝对无错。
-------------
**[MySQL事务学习总结](https://fdx321.github.io/2016/09/09/MySQL%E4%BA%8B%E5%8A%A1%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93/)**
关于幻读，网上很多描述都是错误的。
**幻读**是指读到了其它事务提交的**新增**数据，
**不可重复读**是指读到了其他事务提交的**更改**数据（更改或删除）。
为了解决不可重复读，只需要通过行级锁来实现就可以了，但是为了解决幻读，则不能仅仅锁住一条数据，因为这样的锁不能阻止别的事务新增记录，MySQL用了**间隙锁**来解决这个问题，而不是表级锁。
**InnoDB实现的是MVCC**，**MVCC**中，读分为**快照读和当前读**
**[MySQL加锁处理分析](http://hedengcheng.com/?p=771)****@****[何登成的技术博客](http://hedengcheng.com/)**
**快照读(**简单的select操作，属于快照读，不加锁)**：**
```python
select * from test where money = 500;
```
**当前读(**特殊的读操作，插入/更新/删除操作，属于当前读，需要加锁)**：**
```python
select * from table where ? for update;
insert into table values (…);
update table set ? where ?;
delete from table where ?;
```
可能会觉奇怪，为什么insert update delete也属于当前读，因为针对这些操作，都是InnoDB先把数据筛选出来，加锁，把数据交给MySQL Server处理，Server处理好后交给InnoDB更新，然后InnoDB释放锁，这里面就有读的操作。
这里说的幻读指的是当前读。这是登博关于幻读的解释，**所谓幻读，就是同一个事务，连续做两次当前读 (例如：select****from t1 where id = 10 for update;)，那么这两次当前读返回的是完全相同的记录 (记录数量一致，记录本身也一致)，第二次的当前读，不会比第一次返回更多的记录 (幻象)。**
-----
**[MySQL数据库间隙锁](https://blog.csdn.net/sinat_27143551/article/details/81736330)**
当我们用范围条件而不是相等条件检索数据，并请求共享或排他锁时，InnoDB会给符合条件的已有数据记录的索引项加锁；对于键值在条件范围内但并不存在的记录，叫做**“间隙（GAP)”**，InnoDB也会对这个“间隙”加锁，这种锁机制就是所谓的**间隙锁**（Next-Key锁）。
举例来说，假如user表中只有101条记录，其empid的值分别是 1,2,...,100,101，下面的SQL：
select * from  user where user_id > 100 for update;
是一个范围条件的检索，InnoDB不仅会对符合条件的user_id值为101的记录加锁，也会对user_id大于101（这些记录并不存在）的“间隙”加锁。
InnoDB使用间隙锁的目的，一方面是为了防止幻读，以满足相关隔离级别的要求，对于上面的例子，要是不使用间隙锁，如果其他事务插入了user_id大于100的任何记录，那么本事务如果再次执行上述语句，就会发生幻读；另外一方面，是为了满足其恢复和复制的需要
---------------------
下面MySQL书《高性能MySQL(第3版)》：
![](https://img-blog.csdn.net/20161018185040070)
隔离机制的比较
![](https://img-blog.csdn.net/20161018185316066)
其实也有人喜欢用锁来控制并发，书中还提到了“隐式”和“显示锁定”，是这么建议的：
![](https://img-blog.csdn.net/20161018185353270)
虽然这样，但是其实如果不经过实际的演练还是很难理解上面说的事务隔离机制到底怎么样可以防止并发。
**1.查看MySQL版本**
![](https://img-blog.csdn.net/20161018185741750)
我们的版本是5.1.7
**2.查看存储引擎**
>show engines;
![](https://img-blog.csdn.net/20161018185853236)
存储引擎是：InnoDB
**3.实验表**
假设有个商品表g，关键字段num表示**库存**，name表示商品名称
![](https://img-blog.csdn.net/20161018190324492)
主要就是看不同事务隔离机制下并发修改库存是否会出现超卖。
假设我们的程序需要先查询库存，如果库存>0都可以卖，update扣库存，否则rollback。
为了制造并发肯定需要2个事务，假设是A和B。
**4.确认事务隔离机制**
修改会话的事务隔离级别
set session transaction isolation level read uncommitted;
set session transaction isolation level read committed;
set session transaction isolation level repeatable read;
set session transaction isolation level serializable;
>select @@global.tx_isolation,@@tx_isolation;
![](https://img-blog.csdn.net/20161018190631668)

## 隔离级别1：Serializable 串行化
**场景一：**
![](https://img-blog.csdn.net/20161018191531805)
显然一开始AB查询的数据是一样的num=1
A开始update
![](https://img-blog.csdn.net/20161018191708698)
这时候在等待，无法update。
![](https://img-blog.csdn.net/20161018191906169)
过一会就超时了。
如果这个时候B也update那么一样会等待超时
![](https://img-blog.csdn.net/20161018192009546)
所以这样，AB就会都超时。
这时即使commit也是返回0，数据库不会变化。
![](https://img-blog.csdn.net/20161018192206626)
**场景二：**
A在update等待的时候，B马上commit，但是B没有update
![](https://img-blog.csdn.net/20161018192454896)
查看结果
![](https://img-blog.csdn.net/20161018192726175)
这次A成功的扣库存。
所以从上面可以得出一个结论：serializable是可以很好的控制并发。
然后需要把库存改为1，便于测试。

## 隔离级别2：read committed 读已提交
>set session transaction isolation level read committed;
>select @@global.tx_isolation,@@tx_isolation;
![](https://img-blog.csdn.net/20161018193052255)
**场景一：**
初始化AB查出来的库存都是1，然后A可以update一条数据，无等待。
![](https://img-blog.csdn.net/20161018193412943)
这时候AB再比较下库存，A已经是0，B是1，因为A没有commit。
![](https://img-blog.csdn.net/20161018193509850)
然后A执行commit操作，这时候B再查已经是库存0；
![](https://img-blog.csdn.net/20161018193654897)
这时候B执行update返回是0行，因为update不能满足where条件，所以B只有Commit，然后重新提交。
![](https://img-blog.csdn.net/20161018193853024)
**场景二：**
一开始AB都是一样的库存1，然后A开始update，然后A的库存是0，B是1，因为A还没有提交。
![](https://img-blog.csdn.net/20161018194242169)
这时候B再update
![](https://img-blog.csdn.net/20161018194357402)
按照前面的经验，B等待其实是再等A提交，A如果一直不提交，B就会超时。
![](https://img-blog.csdn.net/20161018194520970)
这时A提交commit，B查询就得到A更新后的结果，这时B查到库存是0自然不会去更新，也就只能结束事务。
**场景三：**
AB先后update，然后A在B超时之前commit，这时由于B已经读到A更新后的结果0，所以B就不能成功update。
![](https://img-blog.csdn.net/20161018195110030)

## 隔离级别3.repeatable read 可重复读
>set session transaction isolation level repeatable read;
>select @@global.tx_isolation,@@tx_isolation;
![](https://img-blog.csdn.net/20161018195337216)
**场景一：**
然后A开始update，然后A和B分别读到库存是1和0
![](https://img-blog.csdn.net/20161018195702627)
然后A提交commit，这时候再查看A和B的库存还是保持不变。
![](https://img-blog.csdn.net/20161018195910814)
这时候B再次尝试update
![](https://img-blog.csdn.net/20161018200012896)
依然是返回0条，说明更新不成功。
**场景二：**
AB同时update
![](https://img-blog.csdn.net/20161018200537920)
如果A不及时commit那么B肯定会超时
如果A及时commit
![](https://img-blog.csdn.net/20161018200911867)
所以可以看出无论是read committed还是repeatable read只要update的条件where  num>0足够充分都是可以控制并发防止超卖的。
如果没有带where  num>0这个控制条件，那么肯定会可以update成功的。

## 隔离级别4
## .read uncommitted
这个是需要杜绝的，就不讨论了。
如果没有带where  num>0，那么会怎么样呢。其实只要理解了上述流程就可以想明白会怎么样。
对于read committed
A已经update，B读到库存是0自然不会去更新；
A没有update，B读到库存是1，这要看A会不会及时提交；
![](https://img-blog.csdn.net/20161018201748933)
如果A及时提交，B自然会去更新因为满足where条件，且成功，这样就超卖-1；
![](https://img-blog.csdn.net/20161018201930808)
这时候由于B没有提交，所以AB分别查出0和-1
![](https://img-blog.csdn.net/20161018202020642)
然后B提交commit，AB查出的都是-1，就不演示了。
修改会话为repeatable read
AB先后update，B在等待
![](https://img-blog.csdn.net/20161018202824249)
然后A立即提交commit，B马上update得到返回。
![](https://img-blog.csdn.net/20161018202832326)
结果就是-1产生了超卖：
总结：
1.使用serializable是可以防止超卖，但是性能怎么样需要数据说明；
2.read committed和repeatable read带上where条件库存num>0都是可以防止超卖的，不过需要处理超时。
3.其他各种组合情况还会更复杂，具体具体问题具体分析。

## forupdate实验
![](https://img-blog.csdnimg.cn/20190327184603352.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190327184612982.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
A开始 ：
![](https://img-blog.csdnimg.cn/20190327184621670.png)
B开始：
![](https://img-blog.csdnimg.cn/2019032718463041.png)
![](https://img-blog.csdnimg.cn/20190327184636391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190327184720485.png)
![](https://img-blog.csdnimg.cn/20190327184730435.png)
扩展阅读：
《[数据库事务的四大特性以及事务的隔离级别](https://www.cnblogs.com/fjdingsd/p/5273008.html)》
## 事务的四个特征ACID
**⑴ 原子性（Atomicity）**
原子性是指事务包含的所有操作**要么全部成功，要么全部失败回滚**，这和前面两篇博客介绍事务的功能是一样的概念，因此事务的操作如果成功就必须要完全应用到数据库，如果操作失败则不能对数据库有任何影响。
**⑵ 一致性（Consistency）**
一致性是指事务必须使数据库从一个一致性状态变换到另一个一致性状态，也就是说一个事务执行之前和执行之后都必须处于一致性状态。
拿转账来说，假设用户A和用户B两者的钱加起来一共是5000，那么不管A和B之间如何转账，转几次账，**事务结束后两个用户的钱相加起来应该还得是5000**，这就是事务的一致性。
**⑶ 隔离性（Isolation）**
隔离性是当多个用户并发访问数据库时，比如操作同一张表时，数据库为每一个用户开启的事务，不能被其他事务的操作所干扰，**多个并发事务之间要相互隔离**。
即要达到这么一种效果：对于任意两个并发的事务T1和T2，在事务T1看来，T2要么在T1开始之前就已经结束，要么在T1结束之后才开始，这样每个事务都感觉不到有其他事务在并发地执行。
关于事务的隔离性数据库提供了**多种隔离级别**。
**⑷ 持久性（Durability）**
持久性是指一个事务一旦被提交了，那么对数据库中的数据的改变就是永久性的，即便是在数据库系统遇到故障的情况下也**不会丢失提交事务**的操作。
------------
## 事务隔离级别
**三种读现象：**
**⑴ 脏读**
脏读是指在一个事务处理过程里读取了另一个未提交的事务中的数据。
当一个事务正在多次修改某个数据，而在这个事务中这多次的修改都还未提交，这时一个并发的事务来访问该数据，就会造成两个事务得到的数据不一致。
**⑵不可重复读**
不可重复读是指在对于数据库中的某个数据，一个事务范围内多次查询却返回了不同的数据值，这是由于在查询间隔，被另一个事务修改并提交了。
例如事务T1在读取某一数据，而事务T2立马修改了这个数据并且提交事务给数据库，事务T1再次读取该数据就得到了不同的结果，发送了不可重复读。
不可重复读和脏读的区别是，**脏读是某一事务读取了另一个事务未提交的脏数据**，而**不可重复读则是读取了前一事务提交的数据**。
**在某些情况下，不可重复读并不是问题**，比如我们多次查询某个数据当然以最后查询得到的结果为主。但在另一些情况下就有可能发生问题，
**⑶ 幻读**
幻读是事务非独立执行时发生的一种现象。例如事务T1对一个表中所有的行的某个数据项做了从“1”修改为“2”的操作，这时事务T2又对这个表中插入了一行数据项，而这个数据项的数值还是为“1”并且提交给数据库。而操作事务T1的用户如果再查看刚刚修改的数据，会发现还有一行没有修改，其实这行是从事务T2中添加的，就好像产生幻觉一样，这就是发生了幻读。
**幻读和不可重复读都是读取了另一条已经提交的事务**（这点就脏读不同），所不同的是**不可重复读**查询的都是**同一个数据**项，而**幻读**针对的是**一批数据整体（比如数据的个数）**。
**MySQL数据库的四种隔离级别：**
**⑴**Serializable (串行化)：可避免脏读、不可重复读、幻读的发生。
**⑵**Repeatable read (可重复读)：可避免脏读、不可重复读的发生。
**⑶**Read committed (读已提交)：可避免脏读的发生。
**⑷**Read uncommitted (读未提交)：最低级别，任何情况都无法保证。
以上四种隔离级别最高的是Serializable级别，最低的是Read uncommitted级别，当然级别越高，执行效率就越低。像Serializable这样的级别，就是以锁表的方式(类似于Java多线程中的锁)使得其他的线程只能在锁外等待，所以平时选用何种隔离级别应该根据实际情况。在MySQL数据库中默认的隔离级别为Repeatable read (可重复读)。
在MySQL数据库中，支持上面四种隔离级别，默认的为Repeatable read (可重复读)；而在Oracle数据库中，只支持Serializable (串行化)级别和Read committed (读已提交)这两种级别，其中默认的为Read committed级别。
![](https://img-blog.csdnimg.cn/20190323214448943.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

