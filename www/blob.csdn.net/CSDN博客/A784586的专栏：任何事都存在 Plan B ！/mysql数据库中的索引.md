# mysql数据库中的索引 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月05日 16:04:50[QuJack](https://me.csdn.net/A784586)阅读数：388标签：[mysql数据库中的索引																[索引](https://so.csdn.net/so/search/s.do?q=索引&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql数据库中的索引&t=blog)
个人分类：[数据库](https://blog.csdn.net/A784586/article/category/6777843)








**mysql数据库中的索引**

**1.索引概叙 MySQL 5.0**

1.1 对相关列使用索引，提高select操作的最佳途径；

1.2 每种存储引擎对每个表至少支持16个索引，总索引长度至少为256字节；

1.3 MyISAM和InnoDB默认都是BTREE索引；

1.4 MySQL不支持函数索引，支持前缀索引【对索引字段的钱N个字符创建索引（MyISAM
 N=1000B,InnoDB N=767B;创建表的前缀长度是以字符为单位的，使用多字节字符集的时候需要注意】；

1.5 MyISAM支持全文本索引（FULLTEXT），只限于char,varchar,text列，对整个列进行，不支持部分索引（前缀索引）；

1.6 MyISAM支持空间列类型创建索引，要求非空；

1.7 MEMORY存储引擎使用HASH索引，也支持BTREE索引；

1.8 创建：

**create****[unique | fulltext | spatial ]****index ****index_name **

**[using index_type ]**

**on****tablename (index_col_name(10));**

**也可以用 alter table;**

**drop index index_name on table_name;**




**2.设计索引的原则**

2.1 搜索的列（出现在select后面的列）不一定是所要选择的列：最适合索引的列是where子句中，连接子句指定的列；

2.2 使用唯一索引：考虑列中值的分布，索引的列的基数越大，索引效果越好；用出生日期比用性别做索引合理；

2.3 使用短索引，前缀索引；节省索引空间+较少磁盘ＩＯ＋缓存中存放更多的键值；

2.4 利用最左前缀；创建一个n列的索引，实际创建了MySQL可利用的n个索引；

2.5 不要过度索引；

2.6 InnoDB尽量自己指定主键，主键尽可能的短；长作为访问条件；




**3.BTREE和HASH索引**

3.1 hash索引特性：

只能使用的比较：=或者<=>操作符的等式比较；

优化器不能使用hash索引加速order by操作；

MySQL不能确定两个值之间大约有多少行；

只能使用整个关键字来搜索一行；

3.2 btree索引特性;

当使用>,<,>=,<=,bettween,!=,<>,或者like 'pattern'(pattern不以通配符开始)，都可以使用相关列上的索引；

下列适合两种索引：

select * from t1 where key_col = 1 or key key_col in (15,18,20);

适用入btree索引：

select * from t1 where key_col > 1 and key_col <10 ;

select * from t1 where key_col like 'ab%' and key_col ='lisa' and key_col ='limon' ;

创建一个表为memory表，默认为hash索引：

查询explain select * from t2 where key_col > 1 and key_col <10 ;

查询explain select * from t1 where key_col > 1 and key_col <10 ;

//key_col 为索引列

比较,可以看到 t2表为hash索引，key：null， t1表为btree索引，key：key_col ; hash索引的表t2全表扫描了；

总结：

当使用memory表时，默认是hash索引，注意sql语句的编写，如果一定要使用范围查询，那么应创建btree索引；

索引用于快速找出在某个列中有一特定行，如果不使用索引，MySQL必须从第一行记录开始然后读完整个表；

------------------------------------------------------------------

**MySQL优化中的索引：**

1.MyISAM：数据和索引自动分开存储，各自是独立的一个文件；

InnoDB：数据和索引是存储在同一个表空间里面，但可以有多个文件组成；

MyISAM和InnoDB值支持btree,memory和heap存储引擎可以支持hash和BTREE；

MySQL不支持函数索引；支持前缀索引；




2.使用索引：

创建一个多列索引，复合索引：

create index index_name on sales2(company_id, moneys);

由于索引的前缀特性，where company_id=2006;的条件查询会用到索引；

where moneys=1;的条件查询会不会用到索引；

where name like '%3' 与 where name like '3%' ，%不在第一位开始的会用到索引；【name为索引列】

where
name is null的会用到索引；【name为索引列】




3.存在索引但不使用索引：

3.1 估计使用索引比扫描全表慢，则不会用索引：

select * from t1 where key_col > 1 and key_col <90 ;当key_col均匀分布在1---100之间；

3.2 memory/heap表在使用条件中不使用=进行索引列的时候；

3.3 条件中or前面的列有索引，后面的列没有索引；

where year=2006 or country='China';year是索引列，country不是索引列；

3.4 如果不是索引列的第一部分；

3.5 like中%位置；

3.6 where name=294；如果name是字符串类型，需要引号。否则不会用到索引；where name=‘294’；




查看索引使用情况：

show status like 'Handler_read%';

其中：

Handler_read_key:被索引读的次数，越高，索引效果好；

Handler_read_rnd_next:在数据文件中查下一行的的请求数，越高说明正在大量表扫描，索引效果不好，需要改善；































