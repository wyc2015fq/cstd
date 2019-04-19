# mysql数据类型 - 建建的博客 - CSDN博客
2017年03月15日 11:09:24[纪建](https://me.csdn.net/u013898698)阅读数：172
个人分类：[Mysql](https://blog.csdn.net/u013898698/article/category/6790249)

整形
- INT                      4bytes      0-2^(4*8)
- SMALLINT            2bytes      0-2^(2*8)
- MEDIUMINT         3bytes      0-2^(3*8)
- BIGINT                8bytes      0-2^(8*8)
- TINYNIT              1bytes      0-2(1*8)
经典问题：
int(11) VS int(21)
存储空间，还是存储范围是没有任何区别的。
那么在mysql中两者实际存储会是怎样的一种情况呢？
```
create table t (a int(**11**) zerofill,b int(**21**) zerofill);
insert into t values (**1**,**1**);
mysql> select * from t;
+-------------+-----------------------+
| a           | b                     |
+-------------+-----------------------+
| **00000000001** | **000000000000000000001** |
+-------------+-----------------------+
**1** row in set (**0.00** sec)
```
如果不加zerofill：
```
mysql> create table t (a int(**11**),b int(**21**));
mysql> insert into t values(**1**,**1**);
mysql> SELECT * FROM t;
+------+------+
| a    | b    |
+------+------+
|    **1** |    **1** |
+------+------+
**1** row in set (**0.00** sec)
```
则两者没有任何区别；所以int（11），int（21）本质上没有任何区别，仅仅是在某些场景的显示上有一些区别；
浮点型
- FLOAT(M,D)      4byte   单精度         非精确
- DOUBLE(M,D)   8byte    双精度        非精确（比float精度高）
M,代表小数点左右两边总的位数
D,小数点右边我们成为标度的位数
精度丢失问题：
- 精度丢失：如果公司工资用了float类型，可能会出现工资四舍五入问题
例子：
```
mysql> CREATE TABLE t ( a int(**11**) DEFAULT NULL, b float(**7**,**4**) DEFAULT NULL ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
mysql> INSERT INTO t VALUES (**2**,**123.12345**);
Query OK, **1** row affected (**0.00** sec)
mysql> SELECT * FROM t ；
+------+----------+
| a    | b        |
+------+----------+
|    **2** | **123.1235** |
+------+----------+
**1** row in set (**0.00** sec)
```
此时float便出现了四舍五入的情况；
DECIMAL定点数：更精确的数字类型
- 高精度数据类型，常用来存储交易相关的数据
- DECIMAL(M,N),M代表总精度，N代表小数点右侧的位数（标度）
- 1<M<254,0<N<60
- 存储空间变长
例子
```
mysql> show create table aa;
 aa    | CREATE TABLE `aa` (
  `a` decimal(**18**,**9**) DEFAULT NULL,
  `b` decimal(**9**,**4**) DEFAULT NULL,
  `c` decimal(**3**,**2**) DEFAULT NULL,
  `f` float(**3**,**2**) DEFAULT NULL,
  `d` double(**3**,**2**) DEFAULT NULL
```
注意：根据存取的数据，decimal的存储空间自动变换；
```
mysql> select * from aa ;
+-------------+--------+------+------+------+
| a           | b      |  c   | f    | d    |
+-------------+--------+------+------+------+
| **5.500000000** | **5.5000** | **5.50** | **5.50** | **5.50** |
+-------------+--------+------+------+------+
**2** rows in set (**0.00** sec)
```
mysql> select length(a),length(b),length(c),length(f),length(d) from aa;
+-----------+-----------+-----------+-----------+-----------+
| length(a) | length(b) | length(c) | length(f) | length(d) |
+-----------+-----------+-----------+-----------+-----------+
|    **11**     |       **6**   |     **4**     |     **4**     |       **4**   |
+-----------+-----------+-----------+-----------+-----------+
#每一个数字（包括标点）都占一个字符；小数部分没有达到位数的用0补齐，整数部分有多少算多少；
```
```
```
mysql> insert into aa values(**5.5**,**5.5**,**55.5**,**55.5**,**55.5**);
mysql> select * from aa;
+-------------+--------+------+------+------+
| a           | b      | c    | f    | d    |
+-------------+--------+------+------+------+
| **5.500000000** | **5.5000** | **9.99** | **9.99** | **9.99** |
#超出范围取范围中最大的值，比如总个3位，小数有2两位，则他们最大的值就是9.99；
```
经验之谈：
- 存储性别，省份，类型等分类信息的时候选择TINIYINT或者ENUM(占字节少)
- BIGINT存储空间更大，INT和BIGINT之间通常选择BIGINT
- 交易等高精度数据时选择使用DECIMAL
字符型
存储用户名的属性：
- CHAR        字符
- VARCHAR  字符
- TEXT  
字符和字节的区别：
|编码\输入的字符串|网易|netease|
|----|----|----|
|gbk(双字节)|varchar（2）/4字节|varchar（7）/7字节|
|utf8(三字节)|varchar（2）/6字节|varchar（7）/7字节|
|utf8mb4(四字节)|varchar（2）/？|varchar（7）/7字节|
utf8mb4虽然说每个汉字占4个字节，但是内部有规定，如果utf8可以存的下的则
还使用utf8标准，这里存储网易两个字显然utf8可以搞定，所以此时？中应该是6个
字节，如果说存储的多了utf8搞不定，则用每一个汉字4个字节来存储，就是8个字节。
emoji表情：
- mysql>5.5.3
- 库和表的字符集设为utf8mb4
CAHR与VARCHAR区别：
- char存储定长，容易造成空间浪费
- varchar存储变长，节省空间
- char的处理速度比varchar快因为char是固定长度
- char会对行尾空格进行处理
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160718195753326-2054267478.png)
例子：
```
mysql> show create table vc;
| vc    | CREATE TABLE `vc` (
  `v` varchar(**5**) DEFAULT NULL,
  `c` char(**5**) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 |
**1** row in set (**0.00** sec)
#注意AB后面都有一个空格
mysql> INSERT INTO vc VALUES('AB ','AB ');
Query OK, **1** row affected (**0.00** sec)
mysql> SELECT * from vc;
+------+------+
| v    | c    |
+------+------+
| AB   | AB   |
+------+------+
**1** row in set (**0.00** sec)
#可以看到char会对行尾空格删掉，而varchar不会；
mysql> select concat(v,'+'),concat(c,'+') from vc;
+---------------+---------------+
| concat(v,'+') | concat(c,'+') |
+---------------+---------------+
| AB +          | AB+           |
+---------------+---------------+
**1** row in set (**0.00** sec)
```
TEXT与CHAR和varchar的区别：
- CHAR和VARCHAR存储单位为字符
- TEXT存储单位为字节，总大小为65535字节，约64kb
- CHAR数据类型最大为255字符
- VARCHAR存储类型为变长；
- TEXT在数据库内部大多存储格式为溢出页，效率不如CHAR
存储头像：
- BLOB
- BINARY
虽然可以，但是非常不推荐；不推荐在数据库里直接存二进制文件；效率会非常低；那二进制如何存取？
blob和text值会引起一些性能问题，特别是在执行了大量的删除操作时，会产生”空洞“
所谓空洞，即虽然已经删除，但是系统还没有回收，要使用optimize table tbl 来整理碎片。
删除操作会在数据表中留下很大的空洞，以后填入这些空洞的记录在插入的性能上会有影响。建议定期使用 OPTIMIZE TABLE tbl 来整理碎片
经验之谈：
- CHAR与VARCHAR定义的长度是字符长度不是字节长度
- 存储字符串推荐选择使用VARCHAR(N),N尽量小
- 虽然数据库可以存储二进制数据，但是性能低下，不要使用数据库存储文件音频等二进制数据
存储生日信息：
- DATE                三字节，如2015-05-01
- TIME                三字节，如11:12:00
- DATETIME        八字节，如2015-05-01 11:12:00
- TIMESTAMP      四字节，如2015-05-01 11:12:00
- BIGINT
TIMESTAMP vs DATETIME
   存储范围的区别 ：
   TIMESTAMP         1970-01-01 00:00:01 to 2038-01-19 03:14:07
   DATETIME           1000-01-01 00:00:00 to 9999-12-31 23:59:59
   字段类型与时区的关联：
   TIMESTAMP会根据系统时区进行转换，DATETIME则不会
把现在的时间转换成了BIGINT类型的数据来存储；
```
mysql> select UNIX_TIMESTAMP (now());
+------------------------+
| UNIX_TIMESTAMP (now()) |
+------------------------+
|             **1461728217** |
+------------------------+
**1** row in set (**0.00** sec)
 
mysql> select from_unixtime(**1461728217**);
+---------------------------+
| from_unixtime(**1461728217**) |
+---------------------------+
| **2016**-**04**-**27** **11**:**36**:**57**       |
+---------------------------+
**1** row in set (**0.00** sec)
```
 总结：
-   整形经典问题--int(11),int(21)　
-   浮点型精度丢失
-   DECIMAL变长存储空间
-   varchar，char区别
-   mysql，concat(v,'+')连接字段  
- BIGINT存时间：
- select UNIX_TIMESTAMP (now());
- select FROM_UNIXTIME (1468842905);
