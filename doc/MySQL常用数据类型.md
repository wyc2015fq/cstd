# MySQL常用数据类型



MySQL-data_type数据类型

1、查看数据类型

mysql> help data type    //通过help对数据进行查看，以及使用的方法

2、MySQL常见的数据类型

　　[整数int](https://www.cnblogs.com/geaozhang/p/6718773.html#zhengshu)、[定点小数dec](https://www.cnblogs.com/geaozhang/p/6718773.html#dingdianxiaoshu)、[浮点数float](https://www.cnblogs.com/geaozhang/p/6718773.html#fudianshu)、[字符串varchar](https://www.cnblogs.com/geaozhang/p/6718773.html#zifuchuan)、[时间-时区](https://www.cnblogs.com/geaozhang/p/6718773.html#riqishijainshiqu)、[布尔bool](https://www.cnblogs.com/geaozhang/p/6718773.html#buer)、[位](https://www.cnblogs.com/geaozhang/p/6718773.html#wei)

 

前言========================================

　　在MySQL数据库中，进行数据存储时，难免会出现数据类型转换，其中就包括：隐式类型转换、显示类型转换

1、隐式类型转换：有害、无害

①对于新建表列

　　数字：int、decimal

　　字符串：varchar

　　日期：date

②查select、增insert时，对于常量

　　数字：不要加引号(非一定)

　　字符串：一定要加上引号

　　日期：可以加上引号，也可以不加---最好是加上

2、显式类型转换：调用cast函数

格式：

　　cast(expr as type)

　　　　---将任意类型的表达式expr转换成指定类型type的值

```
mysql> select cast(19950101 as date);
+------------------------+
| cast(19950101 as date) |
+------------------------+
| 1995-01-01             |
+------------------------+
```

常见的类型转换：

　　字符串char--->数字、日期

　　数字--->字符串、日期

　　日期date--->数字、字符串

===========================================

 

常用的数据类型：

一、整数：

　　每个整型类型可以指定一个最小显示宽度(注意：这个宽度并不表示存储的值有多大)

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170416135326618-34205524.png)

**基本格式：**

　　INT[(M)] [UNSIGNED] [ZEROFILL]

①M：指定显示宽度(默认是M=10)，以实际的取值范围为准，值超过显示宽度，显示宽度自动增加

②unsigned：无符号的，表示只允许正数，所以取值范围取绝对值且最大值*2

　　　　(例：int unsigned range is 0 to 4294967295)

③zerofill: 填充0(如果声明了zerofill，该列会自动设为unsigned)

 

影响数字的显示方式：

　　如果一个数字的宽度小于所允许的最大宽度，这个值前面会用0填充

　　如果宽度大于所允许的最大宽度但不超过取值范围，以实际的取值范围为准，不填0；超出取值范围的报错不存储。

示例：

```
mysql> create table t1(id int zerofill);　　#默认显示宽度10
mysql> insert into t1 values(-1);　　#取值范围：0--4294967295
ERROR 1264 (22003): Out of range value for column 'id' at row 1

mysql> insert into t1 values(123);
mysql> insert into t1 values(12300);
mysql> insert into t1 values(1.123);

mysql> select * from t1;
+------------+
| id         |
+------------+
| 0000000123 |
| 0000012300 |
| 0123456789 |
| 0000000001 |
+------------+
```

解析：因为建表的设置是int整数，小数点后的数会四舍五入

 

 

二、(定点)小数：　　decimal(dec)精确小数类型---精确数的存储

　　同义词：decimal=dec=numeric=fixed

**基本格式：**

　　DEC[(M[,D])] [UNSIGNED] [ZEROFILL]

①M：精度，（整数位数+小数位数）不包含小数点的数字总位数(1--30)，不指定默认为10

②D：小数位数，如果不指定，默认为0 

　　若位数不够，小数末尾补零；若位数超预算(整数位数=M-D)，报错。

示例：

```
mysql> create table t2(id dec(5,3) unsigned);
##数字总位数5
##整数位数5-3=2(不够没关系；超出报错)
##小数位数3(不够补零；超出四舍五入)

mysql> insert into t2 values(12.345);
mysql> insert into t2 values(1.23);
mysql> insert into t2 values(123);
ERROR 1264 (22003): Out of range value for column 'id' at row 1
mysql> insert into t2 values(12.3456);

mysql> select * from t2;
+--------+
| id     |
+--------+
| 12.345 |
|  1.230 |
| 12.346 |
+--------+
```

dec的小数位数是个定数；

 

 

三、浮点数：　　float/double单精度、双精度浮点类型

**基本格式：**

　　float[(length) | (<display width>,<scale>)]

　　double[(precision) | (<display width>,<scale>)]

两者区别在于值所保留的存储空间数量不同

注意：

　　由于对一个浮点数字来说，可用的存储空间有限，如果一个数字非常大或非常小，将存储这个数字的近似值而不是实际值。

①使用一个参数来指定浮点类型

length(precision)：在一个浮点数据类型中可以指定长度，来确定具体的浮点类型

　　0~24：单精度float，从第7位有效位，进行四舍五入存储

　　25~30：双精度double，从第16位有效位，进行四舍五入存储

```
mysql> create table t3(id float(8));

　　//m=8，说明是单精度，存储6位的近似值（第7位四舍五入，后面为0）

mysql> insert into t3 values(123);
mysql> insert into t3 values(1234567);
mysql> insert into t3 values(123456789);
mysql> insert into t3 values(123456789.123456789);

mysql> select * from t3;
+-----------+
| id        |
+-----------+
| 123       |
| 1234570   |
| 123457000 |
| 123457000 |
+-----------+
```

 

②使用两个参数来指定浮点类型

　　单精度float(m,d)：m表示精度（0~24），d表示小数位数

　　双精度double(m,d)：m表示精度（25~30），d表示小数位数

具有宽度和小数位数的float的同义词：real和float4；指定double=float(30)

 

示例：设一个字段定义为float(5,3)---单精度浮点，精度5，小数位3（和dec一样）

```
mysql> create table t_3(id float(5,3) unsigned);
##数字总位数5
##整数位数5-3=2(不够没关系；超出报错)
##小数位数3(不够补零；超出四舍五入)

mysql> insert into t_3 values(123);
ERROR 1264 (22003): Out of range value for column 'id' at row 1
mysql> insert into t_3 values(12.3);
mysql> insert into t_3 values(12.3456);
mysql> insert into t_3 values(1.23456);

mysql> select * from t_3;
+--------+
| id     |
+--------+
| 12.300 |
| 12.346 |
| 1.235  |
+--------+
```

float的小数位数不是固定的；

超出存储范围，近似值进行存储。 

 

 

四、字符串：　　varchar可变长字符串类型

　　1.char(n)：固定长度，最多255个字符

　　2.varchar(n)：固定长度，最多65535个字符

　　3.tinytext：可变长度，最多255个字符

　　4.text：可变长度，最多65535个字符

　　5.mediumtext：可变长度，最多2的24次方-1个字符

　　6.longtext：可变长度，最多2的32次方-1个字符

示例：

```
mysql> create table t4(name varchar(21));

mysql> insert into t4 values(123);
mysql> insert into t4 values(xiaoming);
ERROR 1054 (42S22): Unknown column 'xiaoming' in 'field list'　　#字符串要用引号引起来
mysql> insert into t4 values('xiaoming');
mysql> insert into t4 values('zhang'san');
    '> ^C
！！！字符串中的单引号用单引号进行转义
mysql> insert into t4 values('zhang''san');

mysql> select * from t4;
+-----------+
| name      |
+-----------+
| 123       |
| xiaoming  |
| zhang'san |
+-----------+
```

若要存储中文字符串，需要进行字符设置

create table tableName(列名 varchar(20) character set utf8);

create table tableName(列名 varchar(20) character set gpk);

 

1、char和varchar的区别：

　　①char(n) 若存入字符数小于n，则以空格补于其后，查询之时再将空格去掉。所以char类型存储的字符串末尾不能有空格，varchar不限于此。

　　②char(n) 固定长度，char(4)不管是存入几个字符，都将占用4个字节，varchar是存入的实际字符数+1个字节（n<=255）或2个字节(n>255)，所以varchar(4),存入3个字符将占用4个字节。

　　③char类型的字符串检索速度要比varchar类型的快。

 

2、varchar和text的区别：

　　①varchar可指定n，text不能指定，内部存储varchar是存入的实际字符数+1个字节（n<=255）或2个字节(n>255)，text是实际字符数+2个字节。

　　②text类型不能有默认值。

　　③varchar可直接创建索引，text创建索引要指定前多少个字符。varchar查询速度快于text,在都创建索引的情况下，text的索引似乎不起作用。

 

 

五、日期时间时区

　　date：日期 '2008-12-02'

　　time：时间 '12:25:36'

　　datetime：日期时间 '2008-12-12 22:06:44'

　　timestamp：自动存储记录修改时间（如果数据库里面有timestamp数据类型，就应该考虑时区问题）

示例：

```
mysql> create table t5(d1 date);

mysql> insert into t5 values(19901101);　　#推荐的正统写法
mysql> insert into t5 values(901101);
mysql> insert into t5 values(1990111);　　#有问题的写法

mysql> select * from t5;
+------------+
| d1         |
+------------+
| 1990-11-01 |
| 1990-11-01 |
| 0199-01-11 |
+------------+
```

 

*1**、获得当前日期时间的函数*

①获得当前日期+时间(date+time)函数：now()、sysdate()

区别：now()在执行开始时值就得到了，sysdate()在函数执行时动态得到值

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170416141606634-1746958484.png) 

 

②获得当前日期curdate()、获得当前时间curtime()

```
mysql> select curdate(),curtime();
+------------+-----------+
| curdate()  | curtime() |
+------------+-----------+
| 2017-03-21 | 11:00:25  |
+------------+-----------+ 
```

========================================

1）日期字符串转换函数format：

　　date_format(date,format)

---将日期date按照给定的模式format转换成字符串

　　time_format(time,format)

---将时间time按照给定的模式format转换成字符串，format中只可以使用时、分、秒、微秒模式元素

format中可使用的模式元素可查官方文档

 ![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170416141805743-200315695.png)

2）日期字符串转换函数

　　str_to_date(str,format)

---将字符串str以指定的模式format转换成日期

mysql> select str_to_date('01,5,2013','%d,%m,%Y');

mysql> select str_to_date('May 1, 2013','%M %d,%Y');

如果在会话变量@@sql_mode设置中包含no_zero_date和no_zero_in_date时，上述转换将会失败，没有包含这些设置时则转换成功。

======================================== 

 

③获得当前 UTC 日期时间函数：utc_date(), utc_time(), utc_timestamp()

 ![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170416142024368-1340404150.png)

 

*2**、MySQL时区问题：重点！！！*

　　注意：当数据库里面有timestamp数据类型的列，就应该考虑时区参数

 

问：如何判断是否有timestamp数据类型的列？

答：

mysql> select table_name,column_name,data_type

　　-> from information_schema.columns

　　-> where

　　-> data_type='timestamp';

　　……

 

　　MySQL是安装在Linux操作系统下面，因此MySQL时区跟Linux的时区有很多关联的地方，MySQL里有一个时区叫做系统时区，MySQL系统时区的存在是为MySQL安装后读取Linux操作系统的时区，将自己的系统时区设置为与Linux操作系统一致 的时区：

mysql> show variables like "system_time%";    #查看系统时区

+------------------+-------+

| Variable_name    | Value |

+------------------+-------+

| system_time_zone | CST   |

+------------------+-------+　

 

Linux 操作系统的时区（有很多选择）与 mysql 中的系统时区是一致的：

　　Linux操作系统的时区

　　　　shell> cat /etc/sysconfig/clock

　　区域信息目录：/usr/share/zoneinfo

 

系统时区的选择对后续 mysql 的操作影响不大，但是，MySQL中的time_zone的全局时区，会对我们的MySQL产生影响：

mysql> show variables like "time_zone%";　　#查看数据库时区

+------------------+--------+

| Variable_name    | Value  |

+------------------+--------+

| time_zone        | SYSTEM |

+------------------+--------+

问：为什么说time_zone正确与否会对MySQL产生影响？（也就是timestamp列的工作过程）

答：

　　①当有timestamp列是，一个会话连接上来，这个会话默认的时区就等于time_zone的时区，当然会话时区也可以被修改。

　　②往timestamp列里存数据的时候，mysql会先查询存数据的会话所在的时区，mysql把会话对应的时区与存储的时间结合换算出时间所对应的0时区，再把这个时间存储到timestamp列中，所以timestamp列存储都是相对于0时区的时间。

　　③当用户查询数据的时候，mysql会将timestamp列中相对应于0时区的数据转换成会话对应的时区，反应给用户。

　　④当会话对应的时区与存储时间的时区不对应时，存储的时间仍然会被转换为0时区对应的时间进行存储，那么这时我们存储的时间就是错误的，所以我们必须保证会话时区与存储时间的时区一致。

 

mysql> select @@global.time_zone;    #查看全局时区

mysql> select @@session.time_zone;    #查看会话时区(默认等于全局时区)

 

1.临时设置时区(全局)：

①'+8:00'格式

　　mysql> set @@global.time_zone='+8:00';

②'Asia/Shanghai'格式

　　mysql> set @@global.time_zone='Asia/Shanghai';

　　ERROR 1298 (HY000): Unknown or incorrect time zone: 'Asia/Shanghai'

加载系统时区：

　　shell> mysql_tzinfo_to_sql /usr/share/zoneinfo |mysql -uroot -p123 mysql

　　mysql> set @@global.time_zone='Asia/Shanghai';

 

2.永久设置时区：

修改配置文件vim /etc/my.cnf

[mysqld]区域中添加default_time_zone=Asia/Shanghai

 

时区的正确实践（只有timestamp数据类型的才有意义）：

　　①insert以前，你的values对应的时间到底是哪个时区，然后设置set @@session.time_zone为对应的时区

　　②select获取以前，你想得到什么时区的时间，就设置set @@session.time_zone为对应的时区。

示例：

会话1：当前会话

```
mysql> select @@global.time_zone;
+--------------------+
| @@global.time_zone |
+--------------------+
| +08:00             |
+--------------------+

mysql> create table t9(tm timestamp);

mysql> insert into t9 values('2017-3-22 10:10');

mysql> select * from t9;
+---------------------+
| tm                  |
+---------------------+
| 2017-03-22 10:10:00 |
+---------------------+
```

会话2：客户端登录会话(东10区用户)

设置会话时区

```
mysql> set @@session.time_zone='+10:00';

mysql> use test; select * from t9;
Database changed

+---------------------+
| tm                  |
+---------------------+
| 2017-03-22 12:10:00 |
+---------------------+ 
```

 

 

六、布尔bool或boolean=tinyint(1)

　　A value of zero is considered false,nonzero values are considered true.

示例：

```
mysql> create table t7(num bool);

mysql> insert into t7 values(true);
mysql> insert into t7 values(false);
mysql> insert into t7 values(123);
mysql> insert into t7 values(1);
mysql> insert into t7 values(0);
mysql> insert into t7 values(128);    #范围在-128~127
ERROR 1264 (22003): Out of range value for column 'num' at row 1

mysql> select * from t7;
+------+
| num  |
+------+
|    1 |
|    0 |
|  123 |
|    1 |
|    0 |
+------+
```

 

 

七、位

　　位主要是用来存放二进制数，select查询默认显示10进制，可以使用bin()、hex()函数来进行查询

bin()---显示二进制格式

hex()---显示十六进制格式

示例：

```
mysql> create table t7(id bit(4));

mysql> insert into t7 values(2);
mysql> insert into t7 values(13);

mysql> select bin(id),hex(id) from t7;
+---------+---------+
| bin(id) | hex(id) |
+---------+---------+
| 10      | 2       |
| 1101    | D       |
+---------+---------+ 
```

@author：http://www.cnblogs.com/geaozhang/