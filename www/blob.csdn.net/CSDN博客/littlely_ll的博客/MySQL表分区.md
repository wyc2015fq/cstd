# MySQL表分区 - littlely_ll的博客 - CSDN博客





2018年09月09日 11:06:29[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：62








**mysql分区优点：**

> - 和单个磁盘或文件系统分区相比，可以存储更多数据
- 优化查询
- 对于过期或不需要保存的数据，通过删除与数据有关的分区来提高效率
- 跨多个磁盘分散数据查询，以获得更大的查询吞吐量


**mysql支持的存储引擎：**

> - MyISAM, InnoDB, Memory


同一个分区表的所有分区必须使用同一个存储引擎，但是可以在同一个mysql服务器中，甚至是同一个数据库中，对于不同的分区表使用不同存储引擎。

**分区类型：**

> - range分区：基于一个给定的连续区间范围，把数据分配到不同的分区
- list分区：类似于range分区，但list分区基于枚举出的所有的值列表进行分区
- hash分区：基于给定的分区个数把数据分配到不同的分区
- key分区：类似于hash分区


无论哪种分区类型，不能使用主键/唯一键字段之外的其他字段分区，即要么分区表上没有主键/唯一键，要么分区表的主键/唯一键都必须包含分区键。 

例如：

```
create table emp(
id int not null,
ename varchar(30),
hired date not null default '1970-01-01',
separated date not null default '9999-12-31',
job varchar(30) not null,
primary key (id)
store_id int not null)
partition by range(store_id)(
partition p0 values less than(10),
partition p1 values less than(20),
partition p2 values less than (30));
```

这个`emp`表存在主键，而分区键不是主键，则出现错误，把`primary key(id)`去掉就可以了。

## RANGE分区

```
create table emp(
id int not null,
ename varchar(30),
hired date not null default '1970-01-01',
separated date not null default '9999-12-31',
job varchar(30) not null,
store_id int not null)
partition by range(store_id)(
partition p0 values less than(10),
partition p1 values less than(20),
partition p2 values less than (30));
```

如果以日期作为range分区：

```
create table emp(
id int not null,
ename varchar(30),
hired date not null default '1970-01-01',
separated date not null default '9999-12-31',
job varchar(30) not null,
store_id int not null)
partition by range(YEAR(separated))(
partition p0 values less than(1996),
partition p1 values less than(2001),
partition p2 values less than (2006);
```

但是这样查询如果不是有函数转换，就无法利用range分区特性提高查询性能，不过`range column`分区支持非整数分区：

```
create table emp(
id int not null,
ename varchar(30),
hired date not null default '1970-01-01',
separated date not null default '9999-12-31',
job varchar(30) not null,
store_id int not null)
partition by range COLUMNS(separated)(
partition p0 values less than('1996-01-01'),
partition p1 values less than('2001-01-01'),
partition p2 values less than ('2006-01-01'));
```

这样创建日期分区就不需要通过函数进行转换了

## LIST分区

list分区通过使用`partition by list(expr)`来实现，`expr`是某列值或一个基于某列值返回的一个整数值的表达式，与range分区不同，list分区不必声明任何特定的顺序。**list分区不存在类似`VALUES LESS THAN MAVALUE`这样的定义方式**。LIST分区和RANGE分区一样，在mysql5.5之前只支持整数分区从而导致额外的函数计算得到整数，不过在5.5之后引入了COLUMNS分区解决了它们只支持整数分区的问题。

## COLUMNS分区

COLUMNS分区可细分为RANGE COLUMNS分区和LIST COLUMNS分区，它们都支持整数、日期时间、字符串三大数据类型。

> - 所有整数类型：tinyint, smallint, mediumint, int和bigint，其他数值类型都不支持
- 日期时间类型：date和datetime
- 字符类型：char,varchar,binary和varbinary;不支持text和blob类型。


## HASH分区

mysql支持两种HASH分区：常规HASH分区和线性HASH分区。创建HASH分区表时，使用`PARTITION BY HASH(expr) PARTITIONS num`子句，其中，`expr`是某列值或一个基于某列值返回一个整数值的表达式，`num`为一个非负的整数，表示分割成分区的数量。 

例：

```
CREATE TABLE emp_hash(
id INT NOT NULL,
store_id INT NOT NULL
)
PARTITION BY HASH (store_id) PARTITIONS 4;
```

线性HASH分区与常规分区的唯一区别就是在`PARTITION BY`后加上`LINEAR`. 
**线性HASH分区的优缺点**：在分区维护（增加，删除，合并，拆分）时，mysql能够处理更加迅速；缺点是与常规HASH分区相比，线性HASH分区之间的数据不太均衡。

## KEY分区

KEY分区和HASH分区类似，同样可以用`PARTITION BY KEY(expr) PARTITIONS num`来构建一个key分区表，**`expr`是零个或多个字段名的列表**。 
**与HASH分区比较：**

> - HASH分区允许用户使用自定义的表达式，而KEY分区不允许，只能使用mysql服务器提供的HASH函数
- key分区支持除了BLOB和text以外的其他类型作为分区键
- 创建key分区表时可以不指定分区键，默认会选择主键作为分区键；在没有主键的情况下会选择非空唯一键作为分区键；在没有主键和非空唯一键时，就不能指定分区键了。


## mysql分区对NULL值的处理

**MYSQL不禁止在分区键值上使用NULL，在RANGE分区中，NULL值会被当做最小值来处理；LIST分区中，NULL值必须出现在枚举列表中，否则不被接受；HASH/KEY分区中，NULL值会被当做零值来处理。**



