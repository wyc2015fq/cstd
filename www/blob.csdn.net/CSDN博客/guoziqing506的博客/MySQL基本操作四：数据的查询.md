# MySQL基本操作四：数据的查询 - guoziqing506的博客 - CSDN博客





2018年12月02日 21:15:59[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：5429








之前我介绍了MySQL中，数据记录的增、删、改操作。本文我们看查询操作。

为方便后面举例，还是先建立一个表，并插入一些数据。我在这里依旧建立一个学生信息表，建表的代码如下：

```
CREATE TABLE tab_student(
    StuID CHAR(4),
    Name VARCHAR(20),
    Sender CHAR(1),
    Age INT,
    Math INT,
    English INT,
    Nationality VARCHAR(20)
);
```

插入一些数据，然后是这样的：
![](https://img-blog.csdnimg.cn/20181202203832752.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==,size_16,color_FFFFFF,t_70)
表建好了，也有了数据，现在可以展开查询了。

## 1. 简单数据记录查询

### 1.1 查询全部数据记录

数据查询用的是关键字SELECT，语法形式如下：

```
SELECT * FROM table_name;
```

其中，`*`表示表中的所有字段，就上面建立的表来说，查询结果正是上图。

### 1.2 查询指定字段数据记录

语法形式如下：

```
SELECT Att_1, Att_2, ... FROM table_name;
```

其实就是改变一下`*`符号。要查哪些字段，就用字段名称替代`*`。举个例子来看，还是上面这个表，我想查所有学生的姓名和年龄：

```
SELECT Name, Age FROM tab_student;
```

结果如下：
![](https://img-blog.csdnimg.cn/20181202204151417.png)
### 1.3 避免重复数据查询

有时，我们会面临查询的字段有重复的情况，比如上面的学生信息表中，我想知道学生的年龄都有哪些。如果直接查，会得到`20, 20, 18, 19, 21, 20`六项数据，但是我实际上只要知道有`18, 19, 20, 21` 四个数据就行。

实现这种避免重复数据查询，就需要用到关键字DISTINCT。语法形式如下：

```
SELECT DISTINCT Att_1, ATT_2, ...  FROM table_name;
```

我们用DIDTINCT查询一下学生年龄：

```
SELECT DISTINCT Age FROM tab_student;
```
![](https://img-blog.csdnimg.cn/2018120220441328.png)
### 1.3 实现四则运算的数据查询

有时会遇到这种情况，我们需要将字段中的一项或多项经过一定的数学四则运算后，再返回查询结果。比如雇员表中记录了雇员的月薪，而如果我要查年薪，那么最好将月薪数据乘12再返回查询结果，再比如学生信息表中记录了学生的每门课的成绩，我现在要查询他们的总分。

语法形式很简单，只需要在要查询的字段上加上四则运算符号就行。比如，现在要查询学生的两门课的总分。代码如下：

```
SELECT Name, Math + English FROM tab_student;
```

结果如下：
![](https://img-blog.csdnimg.cn/20181202204540378.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==,size_16,color_FFFFFF,t_70)
### 1.4 查询结果的格式优化

我们还可以通过连接字符串的函数`CONCAT()`来优化查询结果。比如还是上面查询总分的例子，我现在希望查询结果显示为“谁的总分是多少”这种形式，同时我还想给查询结果起个名字叫"总分统计".

语法形式如下：

```
SELECT CONCAT(Name, '的总分为', Math + English) 总分统计 FROM tab_student;
```

运行结果如下：
![](https://img-blog.csdnimg.cn/2018120220470683.png)
## 2. 条件数据记录查询

### 2.1 简单的条件数据记录查询

我们用`WHERE + condition`的形式筛选出符合条件的数据元组。语法形式如下：

```
SELECT att_1, att_2, ... FROM table_name WHERE condition;
```

其中，‘condition’可以是一个条件，也可以是多个条件的组合。条件一般用关系运算符（’>’, ‘<’, ‘=’, '!='之类）或者逻辑运算符（‘AND’, ‘OR’, 'NOT’之类）表示。

举个例子，我要查询上面的学生信息表中Bob的两门课的成绩，代码如下：

```
SELECT Math, English FROM tab_student WHERE Name = 'Bob';
```

结果如下图所示：
![](https://img-blog.csdnimg.cn/20181202204822552.png)
写个更复杂一点的例子，我要查询总分在250分以上的男生的全部信息：

```
SELECT * FROM tab_student WHERE Sender = 'M' AND Math + English > 250;
```

结果如下图所示：
![](https://img-blog.csdnimg.cn/2018120220495760.png)
下面有几种比较常用的条件中带关键字的查询，我大致介绍一下：
- 
`BETWEEN AND`关键字。大于号、小于号的存在实际上是为了控制要查询的字段数值的范围。如果我们要查询某个字段在区间[minVal, maxVal]之间，也可以用形如`BETWEEN minVal AND maxVal`的语句实现。同理，若要查询某属性值不在某区间内的元组，只需要将代码写成`NOT BETWEEN minVal AND maxVal`即可。

- 
`IS NULL`关键字。`WHERE att IS NULL`用来筛选出属性值’att’为空的数据元组，同理，可以用`WHERE att IS NOT NULL`筛选出属性值’att’不为空的数据元组

- 
`IN`关键字。`WHERE att IN set`用来筛选出属性值’att’在某个集合`set`中的数据元组，同理，`NOT IN set`表示对应属性不在set中的元组。需要注意的是，如果`IN`后面跟的集合中有`NULL`，则查询结果是不受影响的，但是如果是`NOT IN`后面的集合中有`NULL`则不会查到任何结果。


### 2.2 模糊条件数据记录查询

很多时候我们的查询条件并不是一个精确的值，而是具备某种特征的值的集合。比如在一张雇员表中，我要查询姓"ZHANG"的雇员名单。由于雇员的姓名是写在一起的，所以用`=`肯定不行了。这个时候就要用到关键字`LIKE`进行模糊查询。语法形式如下：

```
SELECT att_1, att_2 FROM table_name WHERE att LIKE value;
```

后面的`WHERE att LIKE value`为筛选条件，其中`value`一般为存在通配符的表达式。通配符大致有’%‘和"_"两种，’%‘表示匹配任意长度的字符串，而’_'表示匹配任意单个字符。比如，`value`是`A%`，则表示以A开头的字符串；`_A%`表示以A为第二个字符开头的字符串，而`%A%`表示任意含`A`的字符串。

其实，`LIKE`关键字做模糊匹配在一些复杂情况下并不好用。不过好在MySQL也支持正则表达式，直接在`WHERE`后面跟正则表达式即可。正则表达式的语法我在这里省略了。感兴趣的话，可以参考我之前写的一篇关于Python中正则表达式的用法：[Python–正则表达式](https://blog.csdn.net/guoziqing506/article/details/52996462)

### 2.3 排序数据记录查询

可以用关键字`ORDER BY`对查询出来的数组元组排序。语法形式如下：

```
SELECT att, ... FROM table_name ORDER BY att_1 ASC/DESC;
```

最后的`ASC/DESC`表示是以升序还是降序排列。其中，`ASC`表升序，而`DESC`表降序，默认的排序方式为升序。

有时，排序的规则会比较复杂。例如，我们需要先以att_1升序或降序排序，而对于任意att_1的值相等的元组则再以att_2升序或降序排序。语法形式也很简单：

```
SELECT att, ... FROM table_name ORDER BY att_1 ASC/DESC, att_2 ASC/DESC;
```

### 2.4 限制数据记录查询数量

查询数据结果的数量用关键字`LIMIT`限制。语法形式如下：

```
SELECT att, ... FROM table_name WHERE condition LIMIT offset_start, row_count;
```

其中，`offset_start`表示数据记录起始的偏移量，默认值是0；而`row_count`表示数量限制。

### 2.5 统计函数的应用

统计函数能够直接在查询的过程中，帮助我们得到对满足条件的元组的相关计算。常用的统计函数有以下5种：
- `COUNT()`：计数功能，语法形式如下，其中`COUNT(att)`表示满足条件的数据元组中属性att非空的元组个数：

```
SELECT COUNT(att) FROM table_name WHERE condition;
```

如果不关心att是否为空，只是想统计全表中满足条件的元组个数，可以使用`COUNT(*)`的形式。
- `AVG()`：求平均值，语法形式如下：

```
SELECT AVG(att) FROM table_name WHERE condition;
```

计算平均值时，将忽略att为NULL的情况，也就是说，用所有att不为空的满足条件的元组中att的数值加和后，再除att不为空的满足条件的元组个数。
- `SUM()`：求和，语法形式如下。相关事项与`AVG()`函数一致，不多说了：

```
SELECT SUM(att) FROM table_name WHERE condition;
```
- `MAX()`：求最大值；
- `MIN()`：求最小值；

这两个函数更简单了，给出语法形式如下，其实这个形式可以同时查询到最大最小值：

```
SELECT MAX(att), MIN(att) FROM table_name WHERE condition;
```

## 3. 分组数据记录查询

### 3.1 简单分组查询

以学生信息表为例，假设每个学生有他们的国籍信息，我想要查询出每个国家学生的平均分，这样就得用到分组查询了。分组查询用关键字`GROUP BY`实现。语法形式如下：

```
SELECT * FROM table_name GROUP BY att_1;
```

查询时，先根据att_1将所有的数据元组分组，然后从每个分组中展示一个元组出来。也就是说，显示的结果一共有k个，k为分组数。举个例子，以国籍分组，查询代码如下：

```
SELECT * FROM tab_student GROUP BY Nationality;
```

显示的结果如下：
![](https://img-blog.csdnimg.cn/20181202211007901.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==,size_16,color_FFFFFF,t_70)
### 3.2 实现统计功能的分组查询

其实上面这种分组实际应用中并没有多大意义。但是如果和统计函数结合起来就有用多了。比如学生信息表中，我想统计来自不同国家的学生人数，平均分，总分等等。而如果我想知道来自各个国家的学生的姓名，也可以用函数`GROUP_CONCAT()`实现。语法形式如下:

```
SELECT GROUP_CONCAT(att_1) FROM table_name GROUP BY att_2;
```

表达的意思是以属性`att_2`分组后，展示出每个分组的att_1的值。以上面表格为例，我要查询来自每个国家的学生的姓名：

```
SELECT GROUP_CONCAT(Name) FROM tab_student GROUP BY Nationality;
```

查询的结果如下：
![](https://img-blog.csdnimg.cn/20181202211128873.png)
### 3.3 多字段分组查询

除了使用单字段进行分组查询之外，还可以使用多字段。比如：

```
SELECT att, ... FROM table_name GROUP BY att_1, att_2, ...;
```

这个语句实现的功能是先按att_1分组，在每个分组内部，再按属性att_2分组，。。。以此类推。

### 3.4 有条件限制的分组查询

如果要对分组进行条件限制，就要用到`HAVING`关键字了。注意，之前用的`WHERE`关键字是对数据元组进行条件限制的，而`HAVING`限制的是分组。语法形式如下：

```
SELECT att, ... FROM table_name WHERE condition_1 GROUP BY att_1 HAVING condition_2;
```

这个语句的意思是先筛选出满足condition_1的元组，并根据att_1进行分组，最后找出这些分组中满足条件condition_2的分组。

举个例子，我要查出每个国家的学生信息，并且要求这些国家的学生的数学平均分大于120：

```
SELECT * FROM tab_student GROUP BY Nationality HAVING AVG(Math) > 120;
```
![](https://img-blog.csdnimg.cn/20181202211320172.png)


