# MySQL存储引擎与数据类型的选择 - littlely_ll的博客 - CSDN博客





2018年07月28日 15:36:48[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：47








## 存储引擎

### MyISAM

MyISAM不支持事物，不支持外键，但是访问速度快，对事物完整性没有要求或只以SELECT和INSERT为主的应用可以使用MyISAM来创建表。 

MyISAM的表支持3种不同存储格式，分别是：

> - 静态（固定长度）表
- 动态表
- 压缩表


静态表是默认存储格式，静态表中字段都是非变长字段，每个记录都是固定长度，优点是存储迅速，容易缓存，出现故障容易恢复；缺点是占有的空间比动态表多。静态表的数据在存储时会按照列的宽度定义补足空格，但访问的时候并不会得到这些空格，即使保存的内容后面本来就有空格，在返回结果的时候也会被去掉。

### InnoDB

InnoDB存储引擎具有提交、回滚和崩溃恢复能力的事物安全。与MyISAM相比它写的处理效率差一些，会占用更多的磁盘空间以保留数据和索引。InnoDB表具有自动增长列功能，如果某一字段具有自动增长列功能（关键字为auto_increment），如果插入的值为空(null)或为0，则其实际为自动增长后的值。

```
create table auto_demo(
i int not null auto_increment,
name varchar(10),
primary key(i)
)engine=innodb;

insert into auto_demo values(1,'1'),(0,'2'),(null,'3');
select * from auto_demo;
```
i  name
1   1
2   2
3   3

对于InnoDB表，自动增长列必须是索引。 

mysql支持外键的存储引擎只有InnoDB，在创建外键时，要求父表必须有对应的索引，子表在创建外键时也会自动动创建对应的索引。在创建索引时，可以指定在删除，更新父表时对字表进行的相应的操作，包括RESTRICT, CASCADE, SET NULL 和NO ACTION。RESTRICT和NO ACTION一样，都是限制在字表有关联记录的时候父表不能更新；CASECADE表示父表更新或删除的时候更新或删除子表对应的记录；SET NULL表示父表在更新或删除的时候，子表的对应字段被SET NULL。

```
create table country(
country_id smallint unsigned not null auto_increment,
country varchar(50) not null,
primary key(country_id)
)engine=innodb;

create table city(
city_id smallint unsigned not null auto_increment,
city varchar(50) not null,
country_id smallint unsigned not null,
primary key(city_id),
key idx_fk_country_id(country_id),
constraint fk_city_country foreign key (country_id) references country (country_id)
on delete restrict on update cascade
)engine = innodb;
```

在执行LOAD DATA或ALTER TABLE的时候，可以暂时关闭外键约束来加快处理速度，关闭命令是“SET FOREIGN_KEY_CHECK = 0;” ,执行完后，通过执行“SET FOREIGN_KEY_CHECK = 1”改回原来状态。

### MEMORY

MEMORY使用存在于内存中的内容来创建表，每个MEMORY表实际只对应一个磁盘文件，格式为.frm。MEMORY类型的表访问非常快，默认使用hash索引，但是一旦服务关闭，表中的数据就会丢失。服务器要有足够的内存来维持同一时间使用的MEMORY表，当不再使用表的内容时，需要释放其使用的内存。MEMORY类型的存储引擎主要用于那些内容变化不频繁的代码表，或者作为统计操作的中间结果表。

## 数据类型

### char与varchar

都用来存储字符串，char属于固定长度字符类型，而varchar属于可变长度字符类型。char和varchar检索时的值并不总是相同，因为检索时从char列删除了尾部空格。 

不同存储引擎对char和varchar的使用原则：

> - MyISAM：建议使用固定长度的数据列代替可变长度的数据列
- MEMORY：目前都使用固定长度的数据行存储，无论使用char还是varchar都是当做char类型处理。
- InnoDB：建议使用varchar。


### text与blob

保存较大文本时通常选择text或blob，其主要差别是blob能保存二进制数据，而text只能保存字符数据。

### 浮点数与定点数

浮点数一般用于表示含有小数部分的数值，当字段定义为浮点数时，如果插入数据精度超过定义的实际精度，则插入值会四舍五入到实际定义的精度值，然后插入，并不会报错，mysql中表示浮点数的有float，double。 

定点数实际上是以字符串形式存放的，如果实际插入的数值精度大于实际定义的精度，则mysql会进行警告，但数据按照实际精度四舍五入后插入，如果sqlmode是在traditional下，则系统直接报错，数据无法插入。 

关于在浮点数与定点数的应用中，要考虑一下几个原则：
> - 浮点数存在误差问题
- 对货币等对精度敏感的数据，应使用定点数表示或存储
- 要注意浮点数中一些特殊值的处理


### 日期

mysql提供的常用日期类型有DATE, TIME, DATETIME,TIMESTAMP。 

选择日期类型的原则：

> - 根据实际需要选择能够满足应用的最小存储的日期类型
- 如果要记录年月日时分秒，并且记录时间久远，最好使用DATETIME而不使用TIMESTAMP，因为TIMESTAMP表示的日期范围比DATETIME要短的多
- 如果记录的日期需要让不同时区的用户使用，最好使用TIMESTAMP





