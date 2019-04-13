
# mysql事务，select for update，及数据的一致性处理 - luzhensmart的专栏 - CSDN博客


2019年04月11日 17:01:18[芦震](https://me.csdn.net/luzhensmart)阅读数：2个人分类：[mysql](https://blog.csdn.net/luzhensmart/article/category/7937885)


在MySQL的InnoDB中，预设的Tansaction isolation level 为REPEATABLE READ（可重读）
**在SELECT 的读取锁定主要分为两种方式：**
SELECT ... LOCK IN SHARE MODE
SELECT ... FOR UPDATE
这两种方式在事务(Transaction) 进行当中SELECT 到同一个数据表时，都必须等待其它事务数据被提交(Commit)后才会执行。
而主要的不同在于LOCK IN SHARE MODE 在有一方事务要Update 同一个表单时很容易造成死锁。
简单的说，如果SELECT 后面若要UPDATE 同一个表单，最好使用SELECT ... UPDATE。
举个例子:
假设商品表单products 内有一个存放商品数量的quantity ，在订单成立之前必须先确定quantity 商品数量是否足够(quantity>0) ，然后才把数量更新为1。代码如下:
```python
SELECT quantity FROM products WHERE id=3; UPDATE products SET quantity = 1 WHERE id=3;
```
**为什么不安全呢?**
少量的状况下或许不会有问题，但是大量的数据存取「铁定」会出问题。如果我们需要在quantity>0 的情况下才能扣库存，假设程序在第一行SELECT 读到的quantity 是2 ，看起来数字没有错，但
是当MySQL 正准备要UPDATE 的时候，可能已经有人把库存扣成0 了，但是程序却浑然不知，将错就错的UPDATE 下去了。因此必须透过的事务机制来确保读取及提交的数据都是正确的。
于是我们在MySQL 就可以这样测试，代码如下:
```python
SET AUTOCOMMIT=0; BEGIN WORK; SELECT quantity FROM products WHERE id=3 FOR UPDATE;
```
此时products 数据中id=3 的数据被锁住(注3)，其它事务必须等待此次事务 提交后才能执行
SELECT * FROM products WHERE id=3 FOR UPDATE 如此可以确保quantity 在别的事务读到的数字是正确的。
```python
UPDATE products SET quantity = '1' WHERE id=3 ; COMMIT WORK;
```
提交(Commit)写入数据库，products 解锁。
注1: BEGIN/COMMIT 为事务的起始及结束点，可使用二个以上的MySQL Command 视窗来交互观察锁定的状况。
注2: 在事务进行当中，只有SELECT ... FOR UPDATE 或LOCK IN SHARE MODE 同一笔数据时会等待其它事务结束后才执行，一般SELECT ... 则不受此影响。
注3: 由于InnoDB 预设为Row-level Lock，数据列的锁定可参考这篇。
注4: InnoDB 表单尽量不要使用LOCK TABLES 指令，若情非得已要使用，请先看官方对于InnoDB 使用LOCK TABLES 的说明，以免造成系统经常发生死锁。
**MySQL SELECT ... FOR UPDATE 的Row Lock 与Table Lock**
上面介绍过SELECT ... FOR UPDATE 的用法，不过锁定(Lock)的数据是判别就得要注意一下了。由于InnoDB 预设是Row-Level Lock，所以只有「明确」的指定主键，MySQL 才会执行Row lock (只锁住被选取的数据) ，否则MySQL 将会执行Table Lock (将整个数据表单给锁住)。
举个例子:
假设有个表单products ，里面有id 跟name 二个栏位，id 是主键。
例1: (明确指定主键，并且有此数据，row lock)
```python
SELECT * FROM products WHERE id='3' FOR UPDATE;
```
例2: (明确指定主键，若查无此数据，无lock)
```python
SELECT * FROM products WHERE id='-1' FOR UPDATE;
```
例2: (无主键，table lock)
```python
SELECT * FROM products WHERE name='Mouse' FOR UPDATE;
```
例3: (主键不明确，table lock)
```python
SELECT * FROM products WHERE id<>'3' FOR UPDATE;
```
例4: (主键不明确，table lock)
```python
SELECT * FROM products WHERE id LIKE '3' FOR UPDATE;
```
**乐观所和悲观锁策略**
悲观锁：在读取数据时锁住那几行，其他对这几行的更新需要等到悲观锁结束时才能继续 。
乐观所：读取数据时不锁，更新时检查是否数据已经被更新过，如果是则取消当前更新，一般在悲观锁的等待时间过长而不能接受时我们才会选择乐观锁。

