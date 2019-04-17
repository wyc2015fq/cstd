# MySQL基本操作一：数据库和表 - guoziqing506的博客 - CSDN博客





2018年05月23日 21:55:49[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：3871








本章，主要就数据库和表的基本操作作以总结。

需要事先说明的一点是，我写的所有MySQL的代码将采用大写形式。虽然说至少在MySQL环境下，SQL语句的大小写并不会影响代码运行结果，但是，有同行讲到，SQL语句大写比是一个更好地习惯。因为在执行程序的时候，如果你用的是小写，会先转化为大写字母然后执行，而如果大写，那就可以省一个转化的时间。在大型的ERP系统开发时，往往会这么要求。所以，我也就尽量规范一些了。算是帮自己养成好的习惯。

## 数据库的操作

MySQL中，数据库的操作基本有：创建，查看，选择，以及删除4种。
- 创建数据库：`CREATE DATABASE database_name;`，后面的database_name是你要创建的数据库的名称；
- 查看数据库：`SHOW DATABASES;`，查看已经创建的所有数据库
- 选择数据库：`USE database_name;`，选择名为database_name的数据库，很多数据库的操作都需要先选中一个数据库，再执行进一步的操作；
- 删除数据库：`DROP DATABASE database_name;`，删除名为database_name的数据库；

## 创建表

建好了数据库，我们选择一个数据库，来对它进行表的相关操作。表是数据在数据库中一种逻辑上的存储形式，和常见的电子表格类似，其中每一行代表一个记录（可以理解成一个数据对象），而记录的每一列代表一个字段（其实你可以理解成数据对象的属性）。

表的基本操作有创建，查看，删除，修改4种基本操作。我先从创建表说起。

```
CREATE TABLE table_name(
    Att_name_1 Data_type_1,
    Att_name_2 Data_type_2,
    ...
    Att_name_n Data_type_n,
);
```

其中，`table_name`表示要创建的表的名称；`Att_name_i`表示第i个字段（也就是属性）的名称，`Data_type_i`表示第i个属性所对应的值的数据类型。这里多说一句，Mysql中的数据类型大致上分为“整数”，“浮点数”，“定点数”，“位”，“日期”，“时间”，“字符串”这么几种。东西太杂，也没必要具体介绍，简单知道就行，具体用到时，可以再查。

举个例子，现在如下创建一个表"tab_student"（最好养成良好的编程习惯，这里为了表明在创建表，最好以 “tab_”+ 的形式设置表名）：

```
CREATE TABLE tab_student(
    ID VARCHAR(20),
    Score INT
);
```

表"tab_student"中有两个字段，学生的身份`ID`以及对应的成绩`Score`，其中，`ID`是一个VARCHAR(20)型的字符变量，里面的20是参数；`Score`则是一个整数类型的变量。

另外，如果想要查看当前数据库中有哪些表，可以先选择当前数据库，再通过语句`SHOW TABLES`查看。而具体查看已经创建的某个表的信息，则通过下面讲的方式了。

## 查看表

先说说如何查看数据库当中有哪些表。方法是选择数据库后，用语句`SHOW TABLES`：

```
USE test; # 选择数据库test
SHOW TABLES; # 查看test中的表tab_student
```

比如我们创建了数据库`test`，然后在`test`中创建了表`tab_student`，运行结果如下图所示：
![](https://img-blog.csdn.net/201806290939252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到系统给出了一个列表，里面是现在数据库`test`中所有的表。

那么如何查看某个表的具体信息呢？还是先选择数据库，然后用`DESCRIBE`语句查看。

```
USE test; # 选择数据库test
DESCRIBE tab_student; # 查看test中的表tab_student
```

运行结果如下图所示：
![](https://img-blog.csdn.net/20180523214705147?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果想要查看表结构更详细的定义，可以采用`SHOW CREATE TABLE`语句，例如：

```
SHOW CREATE TABLE tab_student;
```

需要注意的是，在显示表的详细信息时，可以用`;`, `\g`或者`\G`来结束语句（我上面的代码用的是`;`）。其实，如果用`\G`可以让表的详细信息显示得更加美观。运行结果你自己跑一下就能发现区别，我这里不给图了。

## 删除表

`DROP TABLE table_name`语句用来删除表，其中"table_name"是表名。

## 修改表

修改表主要分为修改表名和修改表中的字段。

### 1. 修改表名

`ALTER TABLE old_table_name RENAME new_table_name`语句用来修改表名，其中"old_table_name"是旧表名，"new_table_name"是新表名，当然，这里所要操作的对象一定得是存在的，不然会报错。

例如，还是上面的例子：

```
ALTER TABLE tab_student RENAME tab_student1;
```

我分两行写，是为了看起来更清楚。现在将表格tab_student更名为tab_student1。然后我们在用上面说的查看表格的命令，可以看到以新名命名的表格，同时查看旧的名字时，报错。

### 2. 增加字段
- `ADD`语句在表格末尾加字段：

```
ALTER TABLE table_name ADD Att_name_1 Data_type_1;
```

在表"table_name"末尾，增加属性名为`Att_name_1`，数据类型为`Data_type_1`的字段
- `ADD ... FIRST`语句在表格开头加字段：

```
ALTER TABLE table_name ADD Att_name_1 Data_type_1 FIRST;
```

在表"table_name"开头，增加属性名为`Att_name_1`，数据类型为`Data_type_1`的字段
- `ADD ... AFTER ...`语句在指定字段之后添加字段：

```
ALTER TABLE table_name ADD Att_name_1 Data_type_1 AFTER Att_name_2;
```

在属性名为`Att_name_2`的字段后，增加属性名为`Att_name_1`，数据类型为`Data_type_1`的字段

就上边那个"tab_student"表为例，我现在在字段`ID`的后面增加一个属性名为`Gender`，数据类型为`CHAR`的字段。

```
ALTER TABLE tab_student ADD Gender CHAR AFTER ID;
```

可以看到一开始如左图的表变成了右图的样子。其他添加字段的操作类似，这里不举例了。
![](https://img-blog.csdn.net/20180523214826296?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 3. 删除字段

语法形式如下：

```
ALTER TABLE table_name DROP Att_name;
```

将属性名为"Att_name"的字段删除。

### 4. 修改字段
- 修改字段的数据类型：

```
ALTER TABLE table_name MODIFY Att_name New_data_type;
```

将属性名为"Att_name"的字段的数据类型新设置为"New_data_type"。
- 修改字段名：

```
ALTER TABLE table_name CHANGE Old_att_name New_att_name Old_data_type;
```

将属性名为"Old_att_name"，数据类型为"Old_data_type"的字段名修改为"New_att_name"。注意此处用的是`CHANGE`而非`MODIFY`（用混了会报错），再者，三个参数在代码中的顺序不要搞错。以表tab_student为例，我们现在想要将字段名"ID"改为"IDs"，代码如下：

```
ALTER TABLE tab_student CHANGE ID IDs CHAR;
```

这样，将表的形式由下面的左图变成了右图：
![](https://img-blog.csdn.net/2018052321483718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 同时修改字段名和数据类型：

```
ALTER TABLE table_name CHANGE Old_att_name New_att_name New_data_type;
```

将属性名为"Old_att_name"的字段名修改为"New_att_name"，数据类型为"New_data_type"的字段。基本语法结构与上面的修改字段名是一样的。
- 修改字段顺序：

```
ALTER TABLE table_name MODIFY Att_name_1 Data_type_1 FIRST (AFTER Att_name_2);
```

我解释一下，括号里面是另一种形式，先不看，括号外头的意思是将属性名为"Att_name_1"，数据类型为"Data_type_1"的字段放置于表的开头。而括号里面的是另一种修改次序的方法，表示将属性名为"Att_name_1"，数据类型为"Data_type_1"的字段放置于属性名为"Att_name_2"的字段后面。

以表tab_student为例，现在将字段"Gender"放置到字段"Score"之后，代码如下：

```
ALTER TABLE tab_student MODIFY Gender CHAR AFTER Score;
```

tab_student表的形式由下面的左图变为右图：
![](https://img-blog.csdn.net/20180523214850326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 表的约束

表的约束是用来检验表中数据的完整性的，说的直白一些，就是检验表中的数据是否满足了规定的条件。以tab_student表举例来说，如果我不愿意令字段ID为空，就可以在建表时定义相关的约束。如果用户在插入数据时，违反了这样的约束条件，就会报错。当然可以定义的约束条件分很多种，下面会一一讲解。

### 1. 设置非空约束

关键字`NOT NULL`用来约束某字段不能为空，语法形式如下：

```
CREATE TABLE table_name(
    Att_name Data_type NOT NULL,
    ...
);
```

上述代码将字段"Att_name"设置为非空。

### 2. 设置字段的默认值

关键字`DEFAULT`用来设置字段的默认值。如果在插入数据时，没有为某个字段赋值，则Mysql会自动为这个字段赋值为默认值。其语法形式如下：

```
CREATE TABLE table_name(
    Att_name Data_type DEFAULT deVal,
    ...
);
```

上述代码将字段"Att_name"的默认值设置为"deVal"。

### 3. 设置唯一约束

关键字`UNIQUE`用来设置对某个字段的唯一约束，唯一约束表示，插入的数据在这个字段上的值得是唯一的，不能重复（你可以类比一般编程语言中“键值对”的“键”）。以学生的信息系统为例，每个人的学号信息应该是唯一的，那么在录入数据时，如果这一项出现了重复，那必然是出错了。所以我们最好设置唯一值约束，如果出现重复，则报错。其语法形式如下：

```
CREATE TABLE table_name(
    Att_name Data_type UNIQUE,
    ...
);
```

上述代码将字段"Att_name"设置为了唯一约束。

现在以上面的tab_student为例，我对tab_student中的属性设置上面说的“非空”，“默认值”，“唯一”的约束：

```
CREATE TABLE tab_student(
    ID VARCHAR(20) NULL UNIQUE, # 设置学生ID非空，且唯一
    Score INT DEFAULT 0, # 设置Score默认值为0
    Gender CHAR
);
```

我们看到表会变成下面的样子：
![](https://img-blog.csdn.net/20180523214907326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 4. 设置主键约束

主键的作用在于用某个字段唯一地标识所有记录，以便在数据库中快速地查找数据。比如我们一般用索引号来唯一标识每一条数据记录。由于主键可以是单一字段，也可以是多个字段，所以，下面分两种情况讨论：
- 单字段主键：用关键字`PRIMARY KEY`设置主键。比如，还是tab_student的例子，现在将学号设置为主键。

```
CREATE TABLE tab_student(
    ID VARCHAR(20) PRIMARY KEY, # 设置学生ID为主键
    Score INT,
    Gender CHAR
);
```

我们查看刚才建的表：
![](https://img-blog.csdn.net/20180523214923143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
发现ID字段也是非空的，其实，设置了主键，则主键字段必然是非空且唯一的。
- 多字段主键：当主键由多个字段组合而成时，则需要关键字`CONSTRAINT`和`PRIMARY KEY`配合设置。其语法结构如下：

```
CREATE TABLE tab_student(
    ID VARCHAR(20),
    Name VARCHAR(20),
    Score INT,
    CONSTRAINT ID_Name PRIMARY KEY (ID, Name)
);
```

在上面代码展示的例子中，语句`CONSTRAINT ID_Name PRIMARY KEY (ID, Name)`，将学生的ID和Name组合起来设置为主键。最后括号内表示的是主键涵盖的字段，而CONSTRAINT后面的`ID_Name`则是为这个约束起的名字（就设置主键约束来说，这个名字不要也没什么影响）。

例子最后生成的表信息如下：
![](https://img-blog.csdn.net/20180523214934941?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 5. 设置字段值自动增加

往表中插入记录时，往往会遇到需要递增的字段。例如商品编号啊，等等。Mysql支持对这类字段设置字段值自动增加的约束。默认情况下，设置了自动增加的字段值为整数，从1开始，每次加1。又因为这种自动增加的字段肯定是唯一的，所以一般我们也把它设置为主键。

设置自动增加的关键字为`AUTO_INCREMENT`

```
CREATE TABLE tab_student(
    ID INT PRIMARY KEY AUTO_INCREMENT, # 将ID设置为整数类型，并且设置字段值自动增加
    Name VARCHAR(20),
    Score INT
);
```

现在这个表示这样的：
![](https://img-blog.csdn.net/20180523214943346?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意，如果要将某个字段设置为自动增加，那么必须将这个字段设置“唯一”的属性，即设置为主键或者唯一约束，否则会报错。

### 6. 设置外键约束

上面的5种约束都是对于单表，而外键约束则是**保证多个表之间的参照完整性**。

设置外键约束的两个表之间具备父子关系，即子表中某个字段的取值依赖于父表。举例来说，现在有两个表：部门表和雇员表，部门表的主键为部门编号`depNo`，而雇员表中表示雇员所在部门编号的字段`depno`就成为了雇员表（子表）的外键。外键设置时有以下几点要求：
- 外键所代表的字段必须依赖于已经存在的父表的主键；
- 外键与相应的父表的主键可以不同名；
- 外键可以为空（null）

设置外键的语法用`FOREIGN KEY (attribute) + REFERENCES father_table_name (attribute)`的形式：

```
CREATE TABLE child_table_name(
    Att_name_1, DataType_1,
    Att_name_2, DataType_2,
    CONSTRAINT FK_name FOREIGN KEY (Att_name_1) REFERENCES father_table_name (Att_name)
);
```



