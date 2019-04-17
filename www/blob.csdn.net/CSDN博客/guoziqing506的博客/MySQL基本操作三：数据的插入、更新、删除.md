# MySQL基本操作三：数据的插入、更新、删除 - guoziqing506的博客 - CSDN博客





2018年08月17日 22:14:03[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1466








介绍MySQL中数据的插入，更新，删除操作前，我先建一个表。方便后面举例子。这是一个学生信息表：

```
CREATE TABLE tab_student(
    Name CHAR(20),
    StuID VARCHAR(20),
    Rank INT
);
```

建好以后，长成这样： 


![](https://img-blog.csdn.net/20180817220125733?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


## 1. 插入数据

### 1.1 插入完整数据记录

语法形式如下（写成一行就行，我写两行是为了看起来方便）：

```
INSERT INTO table_name (att_1, att_2, ..., att_n) 
                VALUES (val_1, val_2, ..., val_n);
```

以最初我们创建好的学生信息表为例，现在插入学生Alice，学号为01，排名2：
`INSERT INTO tab_student (Name, StuID, Rank) VALUES ('Alice', '01', 2);`
完成插入后，可以按下面的语句查询插入的数据记录：
`SELECT * FROM tab_student;`
得到的结果如下图所示：



![](https://img-blog.csdn.net/20180817220249853?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


有关数据查询的内容，我会在下一篇博客中详细说，现在有个大概印象就行。

其实，除了上述这种方法外，MySQL还提供了一种更加简洁的插入数据的方式，即省略字段名称，直接插入值。语法形式如下：
`INSERT INTO table_name VALUES (val_1, val_2, ..., val_n);`
但是注意，此时插入的值一定要与表中定义的字段保持顺序上的一致。

### 1.2 插入数据记录的一部分

除了插入完整的记录，也可以在数据库中插入数据记录的一部分。当有些字段已经被设置了默认值或者可以由事先自动增加的约束生成（这部分内容可参考之前的博文 [MySQL基本操作一：数据库和表](https://blog.csdn.net/guoziqing506/article/details/80426545) 中“表的约束”部分）时，我们自然可以省略对这一部分字段的插入。

```
INSERT INTO table_name (att_1, att_2, ..., att_k) 
                VALUES (val_1, val_2, ..., val_k);
```

注：代码写成一行就行，我写两行是为了看起来方便

其实语法上与插入完整的数据记录并没有什么分别，还是要求属性值与属性（字段）相对应，只不过可以只插入一部分字段。还是上面的例子，我现在插入Bob的数据，但是省略他的排名：
`INSERT INTO tab_student (Name, StuID) VALUES ('Bob', '02');`
可以看到，此时，Bob的排名被置为空。如果事先定义了这个字段为自动增加或默认，那就会显示自动增加的值或默认值。



![](https://img-blog.csdn.net/20180817220322865?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 1.3 插入多条数据记录

语法形式如下（写成一行就行，我写3行是为了看起来方便）：

```
INSERT INTO table_name (att_1, att_2, ..., att_n) 
                VALUES (val_11, val_12, ..., val_1n), 
                (val_21, val_22, ..., val_2n), ...;
```

给多个属性值的元组即可，不多解释了。

也可以插入多条数据记录的一部分（写成一行就行，我写3行是为了看起来方便）：

```
INSERT INTO table_name (att_1, att_2, ..., att_k) 
                VALUES (val_11, val_12, ..., val_1k), 
                (val_21, val_22, ..., val_2k), ...;
```

道理与插入单条数据记录的一部分一样，不举例子了。

### 1.4 插入查询结果

MySQL提供了可以将另一个表中的查询结果插入本表，以实现数据记录的复制。语法形式如下：

```
INSERT INTO table_name1 (att_11, att_12, ..., att_1n) 
       SELECT (att_21, att_22, ..., att_2n) FROM table_name2 WHERE ... ;
```

其实从`SELECT`开始后面的就是一个查询语句，我们可以把从`table_name2`中的一条或多条数据（可以看做是属性元组）插入`table_name1`。

## 2. 更新数据

### 2.1 更新特定的数据记录

用`UPDATE`语句更新数据。语法形式如下：
`UPDATE table_name SET att_1 = val_1, att_2 = val_2 WHERE ....;`
`SET`后面跟`属性=属性值`的等式，`WHERE`语句用来设定要被更新的数据记录的条件。

例如，一个学生表格数据如下：



![](https://img-blog.csdn.net/20180817220405294?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


我们现在将`Alice`的排名改为3：
`UPDATE tab_student SET Rank = 3 WHERE Name = 'Alice';`
那表格变成了如下的样子：



![](https://img-blog.csdn.net/20180817220434662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 2.2 更新所有数据记录

语法形式与更新特定的数据记录是一致的，为了能更新所有的数据记录，只需要去掉`WHERE`语句即可，或者令`WHERE`后面的条件满足表中所有的数据记录。

## 3. 删除数据

### 3.1 删除特定的数据记录

用关键字`DELETE`删除数据记录。注意删除数据库或者表我们用的是关键字`DROP`。删除数据记录的语法形式如下：
`DELETE FROM table_name WHERE ....;`
其中，`WHERE`后面跟的是要被删除的数据所满足的条件。比方说上面那个学生表的例子，我现在要删除Bob的数据，那么应该写成这样：
`DELETE FROM tab_student WHERE Name = 'Bob';`
### 3.2 删除所有数据记录

与前面说的更新所有数据记录的道理是一样的，不要`WHERE`语句或者令`WHERE`语句的条件满足所有的数据记录。



