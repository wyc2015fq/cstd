# Mysql支持哪几种索引 - z69183787的专栏 - CSDN博客
2016年11月29日 10:53:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：559
### 从数据结构角度
1、B+树索引(O(log(n)))：关于B+树索引，可以参考 [MySQL索引背后的数据结构及算法原理](http://blog.codinglabs.org/articles/theory-of-mysql-index.html)
2、hash索引：
a 仅仅能满足"=","IN"和"<=>"查询，不能使用范围查询
b 其检索效率非常高，索引的检索可以一次定位，不像B-Tree 索引需要从根节点到枝节点，最后才能访问到页节点这样多次的IO访问，所以 Hash 索引的查询效率要远高于 B-Tree 索引
c 只有Memory存储引擎显示支持hash索引
3、FULLTEXT索引（现在MyISAM和InnoDB引擎都支持了）
4、R-Tree索引（用于对GIS数据类型创建SPATIAL索引）
### 从物理存储角度
1、聚集索引（clustered index）
2、非聚集索引（non-clustered index）
### 从逻辑角度
1、主键索引：主键索引是一种特殊的唯一索引，不允许有空值
2、普通索引或者单列索引
3、多列索引（复合索引）：复合索引指多个字段上创建的索引，只有在查询条件中使用了创建索引时的第一个字段，索引才会被使用。使用复合索引时遵循最左前缀集合
4、唯一索引或者非唯一索引
5、空间索引：空间索引是对空间数据类型的字段建立的索引，MYSQL中的空间数据类型有4种，分别是GEOMETRY、POINT、LINESTRING、POLYGON。MYSQL使用SPATIAL关键字进行扩展，使得能够用于创建正规索引类型的语法创建空间索引。创建空间索引的列，必须将其声明为NOT NULL，空间索引只能在存储引擎为MYISAM的表中创建
```
CREATE TABLE table_name[col_name data type]
[unique|fulltext|spatial][index|key][index_name](col_name[length])[asc|desc]
```
1、unique|fulltext|spatial为可选参数，分别表示唯一索引、全文索引和空间索引；
2、index和key为同义词，两者作用相同，用来指定创建索引
3、col_name为需要创建索引的字段列，该列必须从数据表中该定义的多个列中选择；
4、index_name指定索引的名称，为可选参数，如果不指定，MYSQL默认col_name为索引值；
5、length为可选参数，表示索引的长度，只有字符串类型的字段才能指定索引长度；
6、asc或desc指定升序或降序的索引值存储
