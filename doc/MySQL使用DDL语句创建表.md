# MySQL使用DDL语句创建表



一、使用DDL语句创建表

　　DDL语言全面数据定义语言（Data Define Language）

主要的DDL动词：

　　CREATE（创建）、DROP（删除）、ALTER（修改）

　　TRUNCATE（截断）、RENAME（重命名）

注意：

　　1、使用DDL语言时，必须在动词后跟上数据库对象名词（例如：TABLE、VIEW、INDEX、SCHEMA、TRIGGER等）。

　　2、MySQL的DDL语言创建数据库，如CREATE DATABASE、DROP DATABASE（在Oracle中没有）。

语法：

```
CREATE  [TEMPORARY]  TABLE  [IF NOT EXISTS] [database_name.] <table_name>
(
  <column_name>  <data_type>  [[not] null],…
)
```

　　TEMPORARY：指明创建临时表

　　IF NOT EXISTS：如果要创建的表已经存在，强制不显示错误消息

　　database_name：数据库名

　　table_name：表名

　　column_name：列名

　　data_type：数据类型

------

mysql> help create table　　#查看详细的建表帮助信息

示例：创建一个员工(employee)信息表

```
mysql> create table emp_xinxi
    -> (
    -> 　　id int unsigned not null primary key,
    -> 　　name varchar(20) not null,
    -> 　　sex char(1) not null,
    -> 　　birth date not null,
    -> 　　job varchar(30) null
    -> );

mysql> desc emp_xinxi;
+-------+------------------+------+-----+---------+-------+
| Field | Type             | Null | Key | Default | Extra |
+-------+------------------+------+-----+---------+-------+
| id    | int(10) unsigned | NO   | PRI | NULL    |       |
| name  | varchar(20)      | NO   |     | NULL    |       |
| sex   | char(1)          | NO   |     | NULL    |       |
| birth | date             | NO   |     | NULL    |       |
| job   | varchar(30)      | YES  |     | NULL    |       |
+-------+------------------+------+-----+---------+-------+
```

 

MySQL反引号`的使用：为了区分保留字与普通字符而引入的符号

举个栗子：

　　SELECT `select` FROM `test` WHERE select='字段值';

解析：

　　在test表中，有个select字段，如果不用反引号，MySQL将把select视为保留字而导致出错，所以，有mysql保留字作为字段的，必须加上反引号来区分。

①保留字不能用于表名，比如desc，此时需要加入反引号来区别；但是，使用表名时可忽略反引号

```
mysql> create table desc……报错
mysql> create table `desc`……成功
mysql> create table `test`……成功
mysql> drop table test……成功
```

②保留字不能用于字段名，比如desc，此时也需要加入反引号；并且，insert等使用时也要加上反引号

```
mysql> create table `test`（`desc` varchar(255)）;　　成功
mysql> insert into test(desc) values('fxf');　　失败
mysql> insert into test(`desc`) values('fxf');　　成功
```

所以：在容易因为保留字问题会出现问题的情况下，都使用反引号`进行区分，不容易出错。

 

 

二、建表时列的数据类型

1、整型：(5种)

　　每个整型类型可以指定一个最小显示宽度(注意：这个宽度并不表示存储的值有多大)

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170427200904147-1722750765.png)

基本格式：

　　INT[(M)] [UNSIGNED] [ZEROFILL]

①M：指定显示宽度(默认是M=10)，以实际的取值范围为准，值超过显示宽度，显示宽度自动增加

②unsigned：无符号的，表示只允许正数，所以取值范围取绝对值且最大值*2

　　(例：int unsigned range is 0 to 4294967295)

③zerofill：填充0(如果声明了zerofill，该列会自动设为unsigned)

影响数字的显示方式：

　　如果一个数字的宽度小于所允许的最大宽度，这个值前面会用0填充；

　　如果宽度大于所允许的最大宽度但不超过取值范围，以实际的取值范围为准，不填0；超出取值范围的报错不存储。

```
mysql> create table t1(id int zerofill);    #默认显示宽度10

mysql> insert into t1 values(-1);
ERROR 1264 (22003): Out of range value for column 'id' at row 1
mysql> insert into t1 values(123);
mysql> insert into t1values(12300);
mysql> insert into t1values(1.123);

mysql> select * from t1
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

 

2、小数类型

1）定点小数：decimal(dec)

　　同义词：decimal=dec=numeric=fixed

基本格式：

　　DEC[(M[,D])] [UNSIGNED] [ZEROFILL]

①M：精度，（整数位数+小数位数）不包含小数点的数字总位数(1--30)。不指定默认为10

②D：小数位数，如果不指定，默认为0 

若位数不够，小数末尾补零；若位数超预算(整数位数=M-D)，报错。

```
mysql> create table t2id dec(5,3) unsigned);
##数字总位数5
##整数位数5-3=2(不够没关系；超出报错)
##小数位数3(不够补零；超出四舍五入)

mysql> insert into t2values(12.345);
mysql> insert into t2values(1.23);
mysql> insert into t2values(123);
ERROR 1264 (22003): Out of range value for column 'id' at row 1
mysql> insert into t2values(12.3456);

mysql> select * from t2;
+--------+
| id     |
+--------+
| 12.345 |
|  1.230 |
| 12.346 |
+--------+
```

2）浮点小数：float/double

基本格式：

　　float[(length) | (<display width>,<scale>)]

　　double[(precision) | (<display width>,<scale>)]

两者区别在于值所保留的存储空间数量不同

注意：

　　由于对一个浮点数字来说，可用的存储空间有限，如果一个数字非常大或非常小，将存储这个数字的近似值而不是实际值

①使用一个参数来指定浮点类型

length(precision)：在一个浮点数据类型中可以指定长度，来确定具体的浮点类型

　　0~24：单精度float，从第7位有效位，进行四舍五入存储

　　25~30：双精度double，从第16位有效位，进行四舍五入存储

```
mysql> create table t3(id float(8));
m=8，说明是单精度，存储6位的近似值（第7位四舍五入，后面为0）

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

解析：float的小数位数不是固定的；超出存储范围，近似值进行存储。

 

3、位类型

基本格式：

　　BIT [(LENGTH)]

存储基于位的值，Length取值范围1---64.默认为1

```
mysql> create table an_bit (id bit(8));

mysql> insert into an_bit values(b'11');
#插入数据，用b注释插入的是位类型数据(默认是十进制数据)，要添加的数据用引号引起

mysql> select * from an_bit;    
#注意：位类型数据不能如此查询，会乱码

mysql> select id+0 from an_bit;    #可以看到十进制的值
+------+
| id+0 |
+------+
|    3 |
+------+

mysql> select bin(id+0) from an_bit;    #可以看到二进制的值
+-----------+
| bin(id+0) |
+-----------+
| 11        |
+-----------+

mysql> select hex(id+0) from an_bit;    #可以看到十六进制的值
+-----------+
| hex(id+0) |
+-----------+
| 3         |
+-----------+
```

位类型一般用在记录数据只有两种选择，比如：男1女0、省内1省外0……

 

4、字符串类型

①char[(length)]：固定长度字符串

　　值少于长度则在尾部自动填充空格

　　length的单位是字符，取值范围0---255

②varchar[(length)]：可变长度字符串

　　length的单位是字符，取值范围0---65535

对于char和varchar，如果长度为0，则只能存储null值或者空字符串’’值

③long varchar：可变长度字符串

　　最大长度2的24次方减1个字符

④longtext：可变长度字符串

　　最大长度4G个字符

text是varchar的同义词，mediumtext是long varchar的同义词

 

5、日期类型

date ： 3字节

datetime：  8字节

time ： 3字节

timestamp ： 4字节

year[(2) | (4)] ： 1字节

 

6、二进制大对象类型

binary： 255个字符

varbinary ：65535个字符

blob： 65535字符(图片、文件)

long varbinary： 2的24次方减1个字符

long blob：  2的32次方-1个字符

图片存储方式：

　　①在数据库中定义数据类型为blob，进行图片存储

　　②……varchar，“mysql.PNG“，在数据库中存放图片的路径，指向文件系统该照片对象

 

 

三、添加数据类型选项

1、对于字符串类型，可以添加类型选项：character set和collate

　　character set：设置字符集

　　collate：设置校对规则

mysql> create table t1(id int,name varchar(20)) character set=gbk collate=gbk_bin;   

\#t1建表指定校对规则（区分大小写）

 

2、对于bit之外的所有数字类型，可以添加一个或是几个类型选项：

　　①unsigned：无符号的，表示只允许正数

　　②zerofill：填充0，(自动设unsigned)若数字的宽度小于所允许的最大宽度，在值前面用0填充

　　③auto_increment：自动增长，只适应于整数类型，一个表中只有一个列可以是自动增长的。（一般会用在主键序列自增）

示例：创建一个city_names表，其主键是自动增长的

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170427202927834-461190816.png)

注意：

　　当在insert语句中为auto_increment列，指定了null值或者没有指定值时，MySQL自动为其赋值；

　　MySQL会记住给出的最后一个顺序号码是多少。当要分配一个新的顺序号码时，最后一个顺序号码被找到，并且列中的最大值被确定。新的顺序号码就是这两个值中的最大者加1。

```
mysql> insert into city_names values(10,'武汉');
mysql> insert into city_names(name) values('赣州');

mysql> select * from city_names;
+-------+--------+
| seqno | name   |
+-------+--------+
|     1 | 北京   |
|     2 | 上海   |
|     3 | 深圳   |
|    10 | 武汉   |
|    11 | 赣州   |
+-------+--------+

mysql> delete from city_names;    #删除表中的所有行

mysql> insert into city_names values(null,'天津');
mysql> insert into city_names values(null,'成都');

mysql> select * from city_names;
+-------+--------+
| seqno | name   |
+-------+--------+
|    12 | 天津   |
|    13 | 成都   |
+-------+--------+
```

即使删除了表中的所有行，插入新行时，编号还是从原来的位置继续；要想再次从1开始，必须删除整个表并重新创建。

 

默认情况下，顺序号码从1开始并且每次加1；可以分别使用两个系统变量来改变默认行为：

```
mysql> show variables like '%auto_increment%';
+--------------------------+-------+
| Variable_name            | Value |
+--------------------------+-------+
| auto_increment_increment | 1     |
| auto_increment_offset    | 1     |
+--------------------------+-------+

mysql> set @@auto_increment_offset=10; --起始值
mysql> set @@auto_increment_increment=10; --每次加几

mysql> create table t5(id integer auto_increment not null primary key);
mysql> insert into t5 values(null),(null);
mysql> select * from t5;
+----+
| id |
+----+
| 10 |
| 20 |
+----+
```

@author：http://www.cnblogs.com/geaozhang/