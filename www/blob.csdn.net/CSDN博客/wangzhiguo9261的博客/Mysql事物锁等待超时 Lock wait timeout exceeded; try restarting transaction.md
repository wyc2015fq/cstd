
# Mysql事物锁等待超时 Lock wait timeout exceeded; try restarting transaction - wangzhiguo9261的博客 - CSDN博客


2018年02月28日 11:07:38[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：77个人分类：[MySQL](https://blog.csdn.net/wangzhiguo9261/article/category/7454395)



# [Mysql事物锁等待超时 Lock wait timeout exceeded; try restarting transaction](http://blog.csdn.net/wo541075754/article/details/50717842)
标签：[mysql](http://so.csdn.net/so/search/s.do?q=mysql&t=blog)[数据库](http://so.csdn.net/so/search/s.do?q=%E6%95%B0%E6%8D%AE%E5%BA%93&t=blog)[等待锁超时](http://so.csdn.net/so/search/s.do?q=%E7%AD%89%E5%BE%85%E9%94%81%E8%B6%85%E6%97%B6&t=blog)[Spring](http://so.csdn.net/so/search/s.do?q=Spring&t=blog)
2016年02月22日 16:50:5726472人阅读评论(0)收藏举报
![](http://csdnimg.cn/release/phoenix/images/category_icon.jpg)分类：
MySQL*（3）*![](http://csdnimg.cn/release/phoenix/images/arrow_triangle%20_down.jpg)
版权声明：本文为博主原创文章，转载请写明出处。
[[-]](http://csdnimg.cn/release/phoenix/#)
[问题场景](http://blog.csdn.net/wo541075754/article/details/50717842#t0)
[原因分析](http://blog.csdn.net/wo541075754/article/details/50717842#t1)
[解决方案](http://blog.csdn.net/wo541075754/article/details/50717842#t2)
[参考信息](http://blog.csdn.net/wo541075754/article/details/50717842#t3)
[深入研究](http://blog.csdn.net/wo541075754/article/details/50717842#t4)






工作中同事遇到此异常，查找解决问题时，收集整理形成此篇文章。
## 问题场景
问题出现环境：
1、在同一事务内先后对同一条数据进行插入和更新操作；
2、多台服务器操作同一数据库；
3、瞬时出现高并发现象；
不断的有一下异常抛出，异常信息：
```python
org.springframework.dao.CannotAcquireLockException:
```
```python
### Error updating database.  Cause: java.sql.SQLException: Lock wait timeout exceeded; try restarting transaction
```
```python
### The error may involve com.*.dao.mapper.PhoneFlowMapper.updateByPrimaryKeySelective-Inline
```
```python
### The error occurred while setting parameters
```
```python
### SQL:-----后面为SQL语句及堆栈信息--------
```
1
2
3
4
5
## 原因分析
在高并发的情况下，Spring事物造成数据库死锁，后续操作超时抛出异常。
Mysql数据库采用InnoDB模式，默认参数:innodb_lock_wait_timeout设置锁等待的时间是50s，一旦数据库锁超过这个时间就会报错。
## 解决方案
1、通过下面语句查找到为提交事务的数据，kill掉此线程即可。
```python
select
```
```python
*
```
```python
from
```
```python
information_schema.innodb_trx
```
1
2、增加锁等待时间，即增大下面配置项参数值，单位为秒（s）
```python
innodb_lock_wait_timeout
```
```python
=
```
```python
500
```
1
3、优化存储过程,事务避免过长时间的等待。
## 参考信息
1、锁等待超时。是当前事务在等待其它事务释放锁资源造成的。可以找出锁资源竞争的表和语句，优化SQL，创建索引等。如果还是不行，可以适当减少并发线程数。
2、事务在等待给某个表加锁时超时，估计是表正被另的进程锁住一直没有释放。
可以用 SHOW INNODB STATUS/G; 看一下锁的情况。
3、搜索解决之道，在管理节点的[ndbd default]区加：
TransactionDeadLockDetectionTimeOut=10000（设置 为10秒）默认是1200（1.2秒）
4、InnoDB会自动的检测死锁进行回滚，或者终止死锁的情况。
> InnoDB automatically detects transaction deadlocks and rolls back a transaction or transactions to break the deadlock. InnoDB tries to pick small transactions to roll back, where the size of a transaction is determined by the number of rows inserted, updated, or deleted.

> 如果参数innodb_table_locks=1并且autocommit=0时，InnoDB会留意表的死锁，和MySQL层面的行级锁。另外，InnoDB不会检测MySQL的Lock Tables命令和其他存储引擎死锁。你应该设置innodb_lock_wait_timeout来解决这种情况。

> innodb_lock_wait_timeout是Innodb放弃行级锁的超时时间。
参考文章：[http://www.51testing.com/html/16/390216-838016.html](http://www.51testing.com/html/16/390216-838016.html)
## 深入研究
由于此项目采用Spring+mybatis框架，事物控制采用“org.springframework.jdbc.datasource.DataSourceTransactionManager”类进行处理。此处还需进行进一步调研Spring实现的机制。


