# MySQL基本操作二：索引的操作 - guoziqing506的博客 - CSDN博客





2018年06月29日 11:32:09[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：216








索引创建在数据库的表对象上，由表中的一个或多个字段生成的键组成，这些键被数据结构（例如B树）组织起来，目的是通过索引快速地定位与键值相关的数据对象。MySQL支持6种索引：普通索引；唯一索引；全文索引；单列索引；多列索引；空间索引。本文，我将分别介绍关于这6种索引创建，查看，删除的基本操作。

## 普通索引

### 1. 创建表时创建普通索引

语法形式如下：

```
CREATE TABLE table_name(
    Att_name_1 Data_type_1,
    Att_name_2 Data_type_2,
    ...
    Att_name_n Data_type_n,
    INDEX index_name (Att_name)
);
```

语句`INDEX index_name (Att_name)`用来创建索引，其中关键字`INDEX`也可以用`KEY`替代，`index_name`是创建的索引名，括号里面的`Att_name`表示作为索引的字段。需要注意的是，在`Att_name`后面还可以添加关键字`ASC`用来指定为升序排序或者`DESC`用来指定降序排序。

举个例子，按照下面的代码创建一个学生信息表：

```
CREATE TABLE tab_student(
    ID VARCHAR(20),
    Score INT,
    INDEX index_ID (ID)
);
```

上面的代码中，设置学生的ID为索引。我们可以用语句`SHOW CREATE TABLE tab_student`查看是否将索引设置成功，结果如下图所示：



![](https://img-blog.csdn.net/2018062911242985?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


此外，可以用下面的代码校验表中索引是否被使用：
`EXPLAIN SELECT * FROM tab_student WHERE ID = 1`
运行结果如下图所示：



![](https://img-blog.csdn.net/20180629112535357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


可见，possible keys是所创建的索引名，而key为空，说明该索引存在，但未开始使用。

### 2. 在已经存在的表上创建普通索引

语法形式如下：
`CREATE INDEX index_name ON table_name (Att_name);`
其中，`index_name`为要创建的索引名，`table_name`是要创建索引所在的表名，`Att_name`是作为索引的字段。这种情况是处理对已经存在的表创建索引的情况。

创建好索引后，查看的方法不变。此处略过。

### 3. 通过`ALTER TABLE`语句创建普通索引

通过我的上一篇博客 [MySQL基本操作一：数据库和表](https://blog.csdn.net/guoziqing506/article/details/80426545) 可以知道`ALTER TABLE`是用来修改表的，那当然也可以为存在的表创建索引了。实现方法如下：
`ALTER TABLE table_name ADD INDEX index_name (Att_name_1);`
查看的方法与上面相同，略。

## 唯一索引

创建与查看的方法与普通索引几乎相同，只是多了一个关键字`UNIQUE`。

### 1. 创建表时创建唯一索引

语法形式如下：

```
CREATE TABLE table_name(
    Att_name_1 Data_type_1,
    Att_name_2 Data_type_2,
    ...
    Att_name_n Data_type_n,
    UNIQUE INDEX index_name (Att_name)
);
```

### 2. 在已经存在的表上创建唯一索引

语法形式如下：
`CREATE UNIQUE INDEX index_name ON table_name (Att_name);`
### 3. 通过`ALTER TABLE`语句创建唯一索引

语法形式如下：
`ALTER TABLE table_name ADD UNIQUE INDEX index_name (Att_name_1);`
## 全文索引

创建与查看的方法与普通索引几乎相同，只是多了一个关键字`FULLTEXT`。

### 1. 创建表时创建全文索引

语法形式如下：

```
CREATE TABLE table_name(
    Att_name_1 Data_type_1,
    Att_name_2 Data_type_2,
    ...
    Att_name_n Data_type_n,
    FULLTEXT INDEX index_name (Att_name)
);
```

### 2. 在已经存在的表上创建全文索引

语法形式如下：
`CREATE FULLTEXT INDEX index_name ON table_name (Att_name);`
### 3. 通过`ALTER TABLE`语句创建全文索引

语法形式如下：
`ALTER TABLE table_name ADD FULLTEXT INDEX (Att_name_1);`
## 多列索引

上面这些都是以一个字段作为索引，因此我们也把他们统称为“单列索引”，而有时需要关联多个字段创建索引（关联多列索引的目的就是提升查询效率）。因为与普通索引的创建和查看类似，所以下面我迅速过一下。

### 1. 创建表时创建多列索引

语法形式如下：

```
CREATE TABLE table_name(
    Att_name_1 Data_type_1,
    Att_name_2 Data_type_2,
    ...
    Att_name_n Data_type_n,
    INDEX index_name (Att_name_1, Att_name_2, ..., Att_name_m)
);
```

### 2. 在已经存在的表上创建全文索引

语法形式如下：

```
CREATE INDEX index_name ON 
        table_name (Att_name_1, Att_name_2, ..., Att_name_m);
```

### 3. 通过`ALTER TABLE`语句创建全文索引

语法形式如下：
`ALTER TABLE table_name ADD INDEX index_name (Att_name_1);`
## 空间索引

空间索引是对空间数据类型的字段建立的索引，MYSQL中的空间数据类型有4种，分别是GEOMETRY、POINT、LINESTRING、POLYGON。MYSQL使用SPATIAL关键字进行扩展，使得能够用于创建正规索引类型的语法创建空间索引。需要注意的是，创建空间索引的列，必须将其声明为NOT NULL，而且空间索引只能在存储引擎为MYISAM的表中创建。

### 1. 创建表时创建空间索引

语法形式如下：

```
CREATE TABLE table_name(
    Att_name_1 Data_type_1,
    Att_name_2 Data_type_2,
    ...
    Att_name_n Data_type_n,
    SPATIAL INDEX index_name (Att_name)
) ;
```

一般空间索引关联的是空间位置的字段，我写个例子：

```
CREATE TABLE tab_poi (
    Loc POINT NOT NULL,
    Keyword VARCHAR(20),
    SPATIAL INDEX loc_index (Loc)
) ENGINE = MyISAM;
```

执行上述代码，并查看生成的表，效果如Fig.3所示。其中，位置的数据类型为`POINT`，表示一组(x, y)坐标。关于MySQL中具体的数据类型的定义大家可以参考这篇博文：[MySQL数据类型](http://outofmemory.cn/mysql/mysql-data-types)



![](https://img-blog.csdn.net/20180629112927750?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


同理，在已经存在的表上创建空间索引，以及通过`ALTER TABLE`语句创建空间索引的方法相信大家可以自己总结出来了。

## 删除索引

上面说的是对索引的创建，下面看看如何删除。很简单，一行代码而已：
`DROP INDEX index_name (Att_name) ON table_name`


