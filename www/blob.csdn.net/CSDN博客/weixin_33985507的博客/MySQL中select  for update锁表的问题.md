# MySQL中select * for update锁表的问题 - weixin_33985507的博客 - CSDN博客
2016年11月21日 14:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
## MySQL中select * for update锁表的问题
由于InnoDB预设是Row-Level Lock，所以只有「明确」的指定主键，MySQL才会执行Row lock (只锁住被选取的资料例) ，否则MySQL将会执行Table Lock (将整个资料表单给锁住)。
举个例子:
假设有个表单products ，里面有id跟name二个栏位，id是主键。
例1: (明确指定主键，并且有此笔资料，row lock)
SELECT * FROM products WHERE id='3' FOR UPDATE;
SELECT * FROM products WHERE id='3' and type=1 FOR UPDATE;
例2: (明确指定主键，若查无此笔资料，无lock)
SELECT * FROM products WHERE id='-1' FOR UPDATE;
例2: (无主键，table lock)
SELECT * FROM products WHERE name='Mouse' FOR UPDATE;
例3: (主键不明确，table lock)
SELECT * FROM products WHERE id<>'3' FOR UPDATE;
例4: (主键不明确，table lock)
SELECT * FROM products WHERE id LIKE '3' FOR UPDATE;
注1: FOR UPDATE仅适用于InnoDB，且必须在交易区块(BEGIN/COMMIT)中才能生效。
注2: 要测试锁定的状况，可以利用MySQL的Command Mode ，开二个视窗来做测试。
在MySql 5.0中测试确实是这样的
**另外：MyAsim 只支持表级锁，InnerDB支持行级锁添加了(行级锁/表级锁)锁的数据不能被其它事务再锁定，也不被其它事务修改（修改、删除）是表级锁时，不管是否查询到记录，都会锁定表**
