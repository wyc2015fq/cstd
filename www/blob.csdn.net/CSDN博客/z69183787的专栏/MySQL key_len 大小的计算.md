# MySQL key_len 大小的计算 - z69183787的专栏 - CSDN博客
2016年12月31日 13:00:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：592
http://www.cnblogs.com/xuanzhi201111/p/4554769.html
背景：
   当用Explain查看SQL的执行计划时，里面有列显示了 key_len 的值，根据这个值可以判断索引的长度，在组合索引里面可以更清楚的了解到了哪部分字段使用到了索引。
环境：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
CREATE TABLE `tmp_0612` (
  `id` int(**11**) NOT NULL,
  `name` varchar(**10**) DEFAULT NULL,
  `age` int(**11**) DEFAULT NULL,
  `address` varchar(**100**) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
![复制代码](http://common.cnblogs.com/images/copycode.gif)
插入数据：
```
insert into tmp_0612 values(**1**,'a',**11**,'hz'),(**2**,'b',**22**,'gz'),(**3**,'c',**33**,'aa');
```
创建索引：
```
alter table tmp_0612 add index idx_name(name);
alter table tmp_0612 add index idx_age(age);
```
测试：
```
explain select * from tmp_0612 where name ='a';
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
| id | select_type | table    | type | possible_keys | key      | key_len | ref   | rows | Extra                 |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_name      | idx_name | **33**      | const |    **1** | Using index condition |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
```
从上面的执行计划可知，索引的长度是33。比预想的30（10*3(utf8)）要高出3字节，为什么呢？进一步测试：
修改name 成 not null
```
alter table tmp_0612 modify name varchar(**10**) not null;
```
再看执行计划：
```
explain select * from tmp_0612 where name ='a';
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
| id | select_type | table    | type | possible_keys | key      | key_len | ref   | rows | Extra                 |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_name      | idx_name | **32**      | const |    **1** | Using index condition |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
```
发现上面的执行计划和第一次的有区别(key_len)，经过多次测试，发现字段允许NULL的会多出一个字节。想到了之前的一篇[文章](http://www.cnblogs.com/zhoujinyi/archive/2012/10/17/2726462.html)，NULL是需要一个标志位的，占用1个字符。那还有2个字节怎么算？这里想到的是会不会和 多字节字符集 相关？那改字符集试试：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
alter table tmp_0612 convert to charset latin1;
explain select * from tmp_0612 where name ='a';
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
| id | select_type | table    | type | possible_keys | key      | key_len | ref   | rows | Extra                 |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_name      | idx_name | **12**      | const |    **1** | Using index condition |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
![复制代码](http://common.cnblogs.com/images/copycode.gif)
发现还是多了2个字节，看来和多字节字符集没有关系了。那会不会和 变长字段 有关系？再试试：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
alter table tmp_0612 convert to charset utf8;
alter table tmp_0612 modify name char(**10**) not null;
explain select * from tmp_0612 where name ='a';
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
| id | select_type | table    | type | possible_keys | key      | key_len | ref   | rows | Extra                 |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_name      | idx_name | **30**      | const |    **1** | Using index condition |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
![复制代码](http://common.cnblogs.com/images/copycode.gif)
和预料的一样了，是30=10*3。到这里相信大家都已经很清楚了，要是还比较模糊就看反推到33字节。
改成允许NULL，应该会变成31。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
alter table tmp_0612 modify name char(**10**);
explain select * from tmp_0612 where name ='a';
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
| id | select_type | table    | type | possible_keys | key      | key_len | ref   | rows | Extra                 |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_name      | idx_name | **31**      | const |    **1** | Using index condition |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
![复制代码](http://common.cnblogs.com/images/copycode.gif)
改成变长字段类型，应该会变成33。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
alter table tmp_0612 modify name varchar(**10**);
explain select * from tmp_0612 where name ='a';
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
| id | select_type | table    | type | possible_keys | key      | key_len | ref   | rows | Extra                 |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_name      | idx_name | **33**      | const |    **1** | Using index condition |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
![复制代码](http://common.cnblogs.com/images/copycode.gif)
改成单字节字符集，还是还是需要额外的3字节（1：null ；变长字段：2），和字符集无关。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
alter table tmp_0612 convert to charset latin1;
explain select * from tmp_0612 where name ='a';
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
| id | select_type | table    | type | possible_keys | key      | key_len | ref   | rows | Extra                 |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_name      | idx_name | **13**      | const |    **1** | Using index condition |
+----+-------------+----------+------+---------------+----------+---------+-------+------+-----------------------+
![复制代码](http://common.cnblogs.com/images/copycode.gif)
反推上去都和预测的一样。
其他测试：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
explain select * from tmp_0612 where age = **11**;
+----+-------------+----------+------+---------------+---------+---------+-------+------+-------+
| id | select_type | table    | type | possible_keys | key     | key_len | ref   | rows | Extra |
+----+-------------+----------+------+---------------+---------+---------+-------+------+-------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_age       | idx_age | **5**       | const |    **1** | NULL  |
+----+-------------+----------+------+---------------+---------+---------+-------+------+-------+
alter table tmp_0612 modify age int not null;
explain select * from tmp_0612 where age = **11**;
+----+-------------+----------+------+---------------+---------+---------+-------+------+-------+
| id | select_type | table    | type | possible_keys | key     | key_len | ref   | rows | Extra |
+----+-------------+----------+------+---------------+---------+---------+-------+------+-------+
|  **1** | SIMPLE      | tmp_0612 | ref  | idx_age       | idx_age | **4**       | const |    **1** | NULL  |
+----+-------------+----------+------+---------------+---------+---------+-------+------+-------+
![复制代码](http://common.cnblogs.com/images/copycode.gif)
int 是占4个字符的，上面key_len的也是符合预期。关于组合索引的，可以自己去测试玩。
总结：
      变长字段需要额外的2个字节，固定长度字段不需要额外的字节。而null都需要1个字节的额外空间，所以以前有个说法：索引字段最好不要为NULL，因为NULL让统计更加复杂，并且需要额外一个字节的存储空间。这个结论在此得到了证实。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
key_len的长度计算公式：
varchr(10)变长字段且允许NULL      : 10*(Character Set：utf8=3,gbk=2,latin1=1)+1(NULL)+2(变长字段)
varchr(10)变长字段且不允许NULL    : 10*(Character Set：utf8=3,gbk=2,latin1=1)+2(变长字段)
char(10)固定字段且允许NULL        : 10*(Character Set：utf8=3,gbk=2,latin1=1)+1(NULL)
char(10)固定字段且允许NULL        : 10*(Character Set：utf8=3,gbk=2,latin1=1)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 
2、
 以前我很好奇通过执行计划Explain去分析SQL的时候看到的key_len值有时很小，有时看到很大，那时也不知道它是怎么算出来的，现在终于搞懂了，嘻。因为网上对key_len的长度的计算的资料也很少，官网也如此。我相信还有很多人不关心key_len的值是怎么来的，或者key_len有什么用的。key_len表示索引使用的字节数，根据这个值，就可以判断索引使用情况，特别是在组合索引的时候，判断所有的索引字段是否都被查询用到。好啦，废话不多说，我们通过例子来说明吧！
 
在说key_len长度计算长度之前，先和大家温习字符类型的知识：
   char和varchar是日常使用最多的字符类型。char（N）用于保存固定长度的字符串，长度最大为255，比指定长度大的值将被截短，而比指定长度小的值将会用空格进行填补。
   varchar（N）用于保存可以变长的字符串，长度最大为65535，只存储字符串实际实际需要的长度（它会增加一个额外字节来存储字符串本身的长度），varchar使用额外的1~2字节来存储值的的长度，如果列的最大长度小于或者等于255，则用1字节，否则用2字节。
   char和varchar跟字符编码也有密切的联系，latin1占用1个字节，gbk占用2个字节，utf8占用3个字节。（不同字符编码占用的存储空间不同）
Latinl如下：
![](http://images0.cnblogs.com/blog2015/645933/201506/051533589731230.png)
Gbk如下：
![](http://images0.cnblogs.com/blog2015/645933/201506/051534371915072.png)
Utf8如下：
![](http://images0.cnblogs.com/blog2015/645933/201506/051534558794313.png)
 
一、字符串类型的key_len计算
测试表的表结构如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> show create table t1\G
*************************** **1**. row ***************************
       Table: t1
Create Table: CREATE TABLE `t1` (
  `id` int(**11**) NOT NULL AUTO_INCREMENT,
  `name` char(**10**) DEFAULT NULL,
  `addr` varchar(**20**) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
**1** row in set (**0.00** sec)
》
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1、索引字段为char类型的key_len计算：
（1）允许为Null时
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> explain select * from t1 where name='xuanzhi';
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra                 |
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | t1    | ref  | name          | name | **31**      | const |    **1** | Using index condition |
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
**1** row in set (**0.00** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
可以看到key_len=31,这个31是字节长度，它是怎么算出来的呢？让我们一起来分析下：
从表结构可以看到字符集是utf8，那就一个字符3个字节，那么char(10)代表的是10个字符相当30个字节，Null 占1个字节，char类型不需要额外的字节来存储值的的长度，所以得到：key_len：10x3+1=31,可以看到跟上面的结果一致的。
（2）不允许为Null时
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> alter table t1 change name name char(**10**) not null;
Query OK, **2** rows affected (**0.06** sec)
Records: **2**  Duplicates: **0**  Warnings: **0**
mysql> explain select * from t1 where name='xuanzhi';
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra                 |
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | t1    | ref  | name          | name | **30**      | const |    **1** | Using index condition |
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
**1** row in set (**0.01** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
算法和上面差不多，只是字段不允许为Null，所以比上面的例子少了一个字节，key_len=10x3=30
2、索引字段为varchar类型且允许为Null时的key_len计算：
(1)允许为Null时
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> alter table t1 change name name varchar(**10**);
Query OK, **2** rows affected (**0.06** sec)
Records: **2**  Duplicates: **0**  Warnings: **0**
mysql> explain select * from t1 where name='xuanzhi';
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra                 |
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | t1    | ref  | name          | name | **33**      | const |    **1** | Using index condition |
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
**1** row in set (**0.02** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
还是utf8的字符集，所以还是一个字符3个字节，那么varchar(10)就是10个字符30个字节，Null占一个字节，由于varchar类型需要额外的1~2字节来存储值的的长度：所以key_len：10x3+1+2=33
（2）不允许为Null时
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> alter table t1 change name name varchar(**10**) not null;
Query OK, **2** rows affected (**0.07** sec)
Records: **2**  Duplicates: **0**  Warnings: **0**
mysql> explain select * from t1 where name='xuanzhi';
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra                 |
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
|  **1** | SIMPLE      | t1    | ref  | name          | name | **32**      | const |    **1** | Using index condition |
+----+-------------+-------+------+---------------+------+---------+-------+------+-----------------------+
**1** row in set (**0.00** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
相信大家都会算了吧，哈哈，不允许为Null就少了一个字符，所以Key_len：10x3+2=32
 
 二、数值类型的key_len计算，先来回顾数值类型的一些根本知识：
![](http://images0.cnblogs.com/blog2015/645933/201506/051834178327217.png)
    所有整数类型可以有一个可选(非标准)属性UNSIGNED。当你想要在列内只允许非负数和该列需要较大的上限数值范围时可以使用无符号值。如果设置了ZEROFILL扩展属性试，默认就有了无符号属性(UNSIGNED)，所以INT(1)与INT(11)后的括号中的字符表示显示宽度，整数列的显示宽度与MySQL需要用多少个字符来显示该列数值与该整数需要的存储空间的大小都没有关系，INT类型的字段能存储的数据上限还是2147483647(有符号型)和4294967295(无符号型)。其实当我们在选择使用INT的类型的时候，不论是INT(1)还是INT(11)，它在数据库里面存储的都是4个字节的长度
 
（1）int型允许为Null
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> alter table t1 add age int(**3**);
Query OK, **0** rows affected (**0.09** sec)
Records: **0**  Duplicates: **0**  Warnings: **0**
mysql> alter table t1 add key (age);
Query OK, **0** rows affected (**0.07** sec)
Records: **0**  Duplicates: **0**  Warnings: **0**
mysql> explain select * from t1 where age=**20**;
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
|  **1** | SIMPLE      | t1    | ref  | age           | age  | **5**       | const |    **1** | NULL  |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
**1** row in set (**0.00** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
分析：int(3)，就是4个字节，上面已经提及到，int（N）都是4个字节长度，允许为Null占一个字节，所以key_len：4+1=5
（2）不允许Null时（不能通过alter table t1 change age age int(3) not null，要drop掉才能添加为not null）
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> alter table t1 change age age int(**3**) not null;
ERROR **1138** (**22004**): Invalid use of NULL value
mysql> alter table t1 drop  age;            
Query OK, **0** rows affected (**0.07** sec)
Records: **0**  Duplicates: **0**  Warnings: **0**
mysql> alter table t1 add age int(**3**) not null;       
Query OK, **0** rows affected (**0.03** sec)
Records: **0**  Duplicates: **0**  Warnings: **0**
mysql> alter table t1 add key (age);
Query OK, **0** rows affected (**0.05** sec)
Records: **0**  Duplicates: **0**  Warnings: **0**
mysql> explain select * from t1 where age=**20**;
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
|  **1** | SIMPLE      | t1    | ref  | age           | age  | **4**       | const |    **1** | NULL  |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
**1** row in set (**0.00** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
如果是Not null的话,int(N)的key_len都是4个字节
当结合可选扩展属性ZEROFILL使用时， 默认补充的空格用零代替。例如，对于声明为INT(5) ZEROFILL的列，值4检索为00004，看例子：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> desc aa;
+--------+--------------------------+------+-----+---------+-------+
| Field  | Type                     | Null | Key | Default | Extra |
+--------+--------------------------+------+-----+---------+-------+
| id     | int(**3**)                   | YES  |     | NULL    |       |
| number | int(**5**) unsigned zerofill | YES  |     | NULL    |       |
+--------+--------------------------+------+-----+---------+-------+
**2** rows in set (**0.00** sec)
mysql> 
mysql> insert into aa (id,number) values (**11**,**4**);
Query OK, **1** row affected (**0.00** sec)
mysql> select * from aa;
+------+--------+
| id   | number |
+------+--------+
|   **11** |  **00004** |
+------+--------+
**1** rows in set (**0.00** sec)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
其它数值类型也是同理的，相信大家找到共同点了，这里我就不作测试。
 
三、日期时间型的Key_len计算，先来回顾一下日期时间型的基本知知识：
![](http://images0.cnblogs.com/blog2015/645933/201506/051935129578778.png)
 
Datetime类型key_len计算（针对MySQL5.5版本之前）：
1、允许为Null时：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> desc t1;
+-------+----------+------+-----+---------+-------+
| Field | Type     | Null | Key | Default | Extra |
+-------+----------+------+-----+---------+-------+
| id    | int(**11**)  | YES  |     | NULL    |       |
| date  | datetime | YES  | MUL | NULL    |       |
+-------+----------+------+-----+---------+-------+
**2** rows in set (**0.00** sec)
mysql> explain select * from t1 where date='2015-05-03 12:10:10';
+----+-------------+-------+------+---------------+------+---------+-------+------+-------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra       |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------------+
|  **1** | SIMPLE      | t1    | ref  | date          | date | **9**       | const |    **1** | Using where |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------------+
**1** row in set (**0.00** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
分析：因为datetime类型存储8个节点，允许为Null，所以多占一个字节，所以key_len: 8+1=9
2、不允许为null时：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> desc t1;
+-------+----------+------+-----+---------+-------+
| Field | Type     | Null | Key | Default | Extra |
+-------+----------+------+-----+---------+-------+
| id    | int(**11**)  | YES  |     | NULL    |       |
| date  | datetime | NO   | MUL | NULL    |       |
+-------+----------+------+-----+---------+-------+
**2** rows in set (**0.00** sec)
mysql>  explain select * from t1 where date='2015-05-03 12:10:10';
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
|  **1** | SIMPLE      | t1    | ref  | date          | date | **8**       | const |    **1** |       |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
**1** row in set (**0.00** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
分析：当不为空时，datetime都是存储8个字节，所以key_len=8。
MySQL5.6 datetime的key_len计算：
1、允许为空时：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> select version();
+------------+
| version()  |
+------------+
| **5.6**.**10**-log |
+------------+
**1** row in set (**0.00** sec)
mysql> desc t1;
+-------+----------+------+-----+---------+-------+
| Field | Type     | Null | Key | Default | Extra |
+-------+----------+------+-----+---------+-------+
| id    | int(**11**)  | YES  |     | NULL    |       |
| date  | datetime | YES  | MUL | NULL    |       |
+-------+----------+------+-----+---------+-------+
**2** rows in set (**0.00** sec)
mysql> explain select * from t1 where date='2015-05-03 12:10:10';
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
|  **1** | SIMPLE      | t1    | ref  | date          | date | **6**       | const |    **1** | NULL  |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
**1** row in set (**0.00** sec)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
分析：mysql5.6的datetime已经不是存储8个字节了，应该存储5个字节了，允许为Null，所以加一个字节，所以key_len：5+1
2、不允许Null时：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> alter table t1 modify date datetime not null;
Query OK, **0** rows affected (**0.06** sec)
Records: **0**  Duplicates: **0**  Warnings: **0**
mysql> explain select * from t1 where date='2015-05-03 12:10:10';
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
| id | select_type | table | type | possible_keys | key  | key_len | ref   | rows | Extra |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
|  **1** | SIMPLE      | t1    | ref  | date          | date | **5**       | const |    **1** | NULL  |
+----+-------------+-------+------+---------------+------+---------+-------+------+-------+
**1** row in set (**0.00** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
分析：不为Null时，则存储自身的字节大小，所以key_len=5
 
四、联合索引的key_len计算，在说联合索引计算之前，我们先回顾一个索引的限制
InnoDB: INNODB的索引会限制单独Key的最大长度为767字节，超过这个长度必须建立小于等于767字节的前缀索引。
MyISAM: MyISAM存储引擎的表，索引前缀的长度可以达到1000字节长。
前缀索引能提高索引建立速度和检索速度，但是无法使用：索引覆盖扫描和通过索引的排序
 
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> show create table xuanzhi\G
*************************** **1**. row ***************************
       Table: xuanzhi
Create Table: CREATE TABLE `xuanzhi` (
  `id` int(**11**) NOT NULL AUTO_INCREMENT,
  `name` char(**20**) DEFAULT NULL,
  `addr` varchar(**20**) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `name` (`name`,`addr`)
) ENGINE=InnoDB AUTO_INCREMENT=**2** DEFAULT CHARSET=utf8
**1** row in set (**0.00** sec)
mysql> explain select * from xuanzhi where name='xuanzhi' and addr='shanghai';
+----+-------------+---------+------+---------------+------+---------+-------------+------+--------------------------+
| id | select_type | table   | type | possible_keys | key  | key_len | ref         | rows | Extra                    |
+----+-------------+---------+------+---------------+------+---------+-------------+------+--------------------------+
|  **1** | SIMPLE      | xuanzhi | ref  | name          | name | **124**     | const,const |    **1** | Using where; Using index |
+----+-------------+---------+------+---------------+------+---------+-------------+------+--------------------------+
**1** row in set (**0.00** sec)
mysql> 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
可以看到表结构里有一个联合索引name,那么上面的key_len是怎么算出来的呢，相信到现在，同学们都有计算的思路，好吧，我们算一下：
name的key_len计算：utf8:char(20)x3+null:1=61
addr的key_len计算： utf8:varchar(20)x3+null:1+2=63 (如果不明白为什么+2往前面再看一次)
联合索引name('name','addr') key_len：61+63=124
嘻嘻，我想到现在没多少人不会算了吧，通过key_len可以让我们知道它是否有充分利用索引
 
还有一些类型没有说到的，希望同学们自己测试一下，下面我总结一下计算公式：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
char和varchar类型key_len计算公式:
varchr(N)变长字段且允许NULL   =  N * ( character set：utf8=3,gbk=2,latin1=1)+1(NULL)+2(变长字段)
varchr(N)变长字段且不允许NULL =  N * ( character set：utf8=3,gbk=2,latin1=1)+2(变长字段)
char(N)固定字段且允许NULL     =  N * ( character set：utf8=3,gbk=2,latin1=1)+1(NULL)
char(N)固定字段且允许NULL     =  N * ( character set：utf8=3,gbk=2,latin1=1)
 
数值数据的key_len计算公式：
TINYINT允许NULL = 1 + 1(NULL)
TINYINT不允许NULL = 1
SMALLINT允许为NULL = 2+1(NULL)
SMALLINT不允许为NULL = 2
INT允许为NULL = 4+1(NULL)
INT不允许为NULL = 4
日期时间型的key_len计算：（针对mysql5.5及之前版本）
DATETIME允许为NULL =  8 + 1(NULL)
DATETIME不允许为NULL = 8
TIMESTAMP允许为NULL = 4 + 1(NULL)
TIMESTAMP不允许为NULL = 4
![复制代码](http://common.cnblogs.com/images/copycode.gif)
还有一些没写出来，相信大家对key_len有一定的认识了，所以这里就不把所有的都写出来了。
 
总结：
   一、INT型如果不结合可选扩展属性ZEROFILL使用，INT(1)和INT（N），它在数据库里面存储的都是4个字节的长度,当然N最大的上限
   二、从上面的例子可以看到，定义表结构时，如果字段允许为NULL，会有额外的开销，所以建议字段尽量不要使用允许NULL，提高索引的使用效率
   三、INNODB的索引会限制单独Key的最大长度为767字节，MyISAM索引前缀的长度可以达到1000字节长，如果order by也使用了索引则key_len不计算在内
 
 参考资料：
     [http://www.cnblogs.com/gomysql/p/3616366.html](http://www.cnblogs.com/gomysql/p/3616366.html)
    [http://www.cnblogs.com/LMySQL/p/4525867.html](http://www.cnblogs.com/LMySQL/p/4525867.html)
     <<深入浅出MySQL>>
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
