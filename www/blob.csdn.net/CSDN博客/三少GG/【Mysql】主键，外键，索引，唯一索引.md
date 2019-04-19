# 【Mysql】主键，外键，索引，唯一索引 - 三少GG - CSDN博客
2013年07月25日 15:31:27[三少GG](https://me.csdn.net/scut1135)阅读数：1536

### [数据库的几个概念：主键，外键，索引，唯一索引](http://blog.csdn.net/xrt95050/article/details/5556411)
分类： [mysql学习](http://blog.csdn.net/xrt95050/article/category/368714)[数据库](http://blog.csdn.net/tag/details.html?tag=%E6%95%B0%E6%8D%AE%E5%BA%93)[mysql](http://blog.csdn.net/tag/details.html?tag=mysql)[table](http://blog.csdn.net/tag/details.html?tag=table)[null](http://blog.csdn.net/tag/details.html?tag=null)[documentation](http://blog.csdn.net/tag/details.html?tag=documentation)[全文检索](http://blog.csdn.net/tag/details.html?tag=%E5%85%A8%E6%96%87%E6%A3%80%E7%B4%A2)
**主键：**
      主键是数据表的唯一索引，比如学生表里有学号和姓名，姓名可能有重名的，但学号确是唯一的，你要从学生表中搜索一条纪录如查找一个人，就只能根据学号去查找，这才能找出唯一的一个，这就是主键;如：id int(10) not null primary key auto_increment ；自增长的类型 ；
**外键：**
      定义数据表
　　假如某个电脑生产商，它的数据库中保存着整机和配件的产品信息。用来保存整机产品信息的表叫做 Pc；用来保存配件供货信息的表叫做Parts。
　　在Pc表中有一个字段，用来描述这款电脑所使用的CPU型号；
　　在Parts 表中相应有一个字段，描述的正是CPU的型号，我们可以把它想成是全部CPU的型号列表。
　　很显然，这个厂家生产的电脑，其使用的CPU一定是供货信息表(parts)中存在的型号。这时，两个表中就存在一种约束关系(constraint)——Pc表中的CPU型号受到Parts 表中型号的约束。
　　首先我们来创建 parts 表：
```
CREATE TABLE parts (
... 字段定义 ...,
model VARCHAR(20) NOT NULL,
... 字段定义 ...
);
```
　　接下来是Pc表：
```
CREATE TABLE pc (
... 字段定义 ...,
cpumodel VARCHAR(20) NOT NULL,
... 字段定义 ...
};
```
　　设置索引
　　若要设置外键，在参照表(referencing table，即Pc表) 和被参照表 (referenced table，即parts表) 中，**相对应的两个字段必须都设置索引(index)。**
　　对Parts表：
**　ALTER TABLE parts ADD INDEX idx_model (model);**
**　　这句话的意思是，为 parts 表增加一个索引，索引建立在 model 字段上，给这个索引起个名字叫idx_model。**
　　对Pc表也类似：
　　ALTER TABLE pc ADD INDEX idx_cpumodel (cpumodel);
　　事实上这两个索引可以在创建表的时候就设置。这里只是为了突出其必要性。
　　定义外键
　　下面为两张表之间建立前面所述的那种“约束”。因为pc的CPU型号必须参照parts表中的相应型号，所以我们将Pc表的cpumodel字段设置为“外键”(FOREIGN KEY)，即这个键的参照值来自于其他表。
```
ALTER TABLE pc ADD CONSTRAINT fk_cpu_model
FOREIGN KEY (cpumodel)
REFERENCES parts(model);
```
　　第一行是说要为Pc表设置外键，给这个外键起一个名字叫做fk_cpu_model；第二行是说将本表的cpumodel字段设置为外键；第三行是说这个外键受到的约束来自于Parts表的model字段。
　　这样，我们的外键就可以了。如果我们试着CREATE一台Pc，它所使用的CPU的型号是Parts表中不存在的，那么MySQL会禁止这台PC被CREATE出来。
　　级联操作
　　考虑以下这种情况：
　　技术人员发现，一个月之前输入到 parts 表中的某个系列的 cpu （可能有很多款）的型号全都输错了一个字母，现在需要改正。我们希望的是，当 parts 表中那些 Referenced Column 有所变化时，相应表中的 Referencing Column 也能自动更正。
　　可以在定义外键的时候，在最后加入这样的关键字：
　　ON UPDATE CASCADE; 即在主表更新时，子表（们）产生连锁更新动作，似乎有些人喜欢把这个叫“级联”操作。:)
　　如果把这语句完整的写出来，就是：
```
ALTER TABLE pc ADD CONSTRAINT fk_cpu_model
FOREIGN KEY (cpumodel)
REFERENCES parts(model)
ON UPDATE CASCADE;
```
　　除了 CASCADE 外，还有 RESTRICT(禁止主表变更)、SET NULL(子表相应字段设置为空)等操作
**索引：**
      索引用来快速地寻找那些具有特定值的记录，所有MySQL索引都以B-树的形式保存。如果没有索引，执行查询时MySQL必须从第一个记录开始扫描整个表的所有记录，直至找到符合要求的记录。表里面的记录数量越多，这个操作的代价就越高。如果作为搜索条件的列上已经创建了索引，MySQL无需扫描任何记录即可迅速得到目标记录所在的位置。如果表有1000个记录，通过索引查找记录至少要比顺序扫描记录快100倍。
       假设我们创建了一个名为people的表： 
CREATE TABLE people ( peopleid SMALLINT NOT NULL, name CHAR(50) NOT NULL );
然后，我们完全随机把1000个不同name值插入到people表。下图显示了people表所在数据文件的一小部分： 
![](http://tech.ccidnet.com/pub/attachment/2002/12/46297.gif)可以看到，在数据文件中name列没有任何明确的次序。如果我们创建了name列的索引，MySQL将在索引中排序name列： 
![](http://tech.ccidnet.com/pub/attachment/2002/12/46298.gif)对于索引中的每一项，MySQL在内部为它保存一个数据文件中实际记录所在位置的“指针”。因此，如果我们要查找name等于“Mike”记录的peopleid（SQL命令为“SELECT peopleid FROM people
 WHERE name='Mike';”），MySQL能够在name的索引中查找“Mike”值，然后直接转到数据文件中相应的行，准确地返回该行的peopleid（999）。在这个过程中，MySQL只需处理一个行就可以返回结果。如果没有“name”列的索引，MySQL要扫描数据文件中的所有记录，即1000个记录！显然，需要MySQL处理的记录数量越少，则它完成任务的速度就越快。
**索引的类型**
MySQL提供多种索引类型供选择： 
**普通索引**
这是最基本的索引类型，而且它没有唯一性之类的限制。普通索引可以通过以下几种方式创建： 
创建索引，例如CREATE INDEX <索引的名字> ON tablename (列的列表);
修改表，例如ALTER TABLE tablename ADD INDEX [索引的名字] (列的列表);
创建表的时候指定索引，例如CREATE TABLE tablename ( [...], INDEX [索引的名字] (列的列表) );
**唯一性索引**
这种索引和前面的“普通索引”基本相同，但有一个区别：索引列的所有值都只能出现一次，即必须唯一。唯一性索引可以用以下几种方式创建： 
创建索引，例如CREATE UNIQUE INDEX <索引的名字> ON tablename (列的列表);
修改表，例如ALTER TABLE tablename ADD UNIQUE [索引的名字] (列的列表);
创建表的时候指定索引，例如CREATE TABLE tablename ( [...], UNIQUE [索引的名字] (列的列表) 
);
**主键**
**主键是一种唯一性索引**，但它必须指定为“[PRIMARY
 KEY](http://whatis.ctocio.com.cn/searchwhatis/287/6026287.shtml)”。如果你曾经用过AUTO_[INCREMENT](http://whatis.ctocio.com.cn/searchwhatis/487/6025487.shtml)类型的列，你可能已经熟悉主键之类的概念了。主键一般在创建表的时候指定，例如“CREATE
 TABLE tablename ( [...], PRIMARY [KEY](http://whatis.ctocio.com.cn/searchwhatis/25/5948525.shtml) (列的列表)
 ); ”。但是，我们也可以通过修改表的方式加入主键，例如“ALTER TABLE tablename ADD PRIMARY KEY (列的列表); ”。每个表只能有一个主键。 
**全文索引**
MySQL从3.23.23版开始支持全文索引和全文检索。在MySQL中，全文索引的索引类型为FULLTEXT。全文索引可以在VARCHAR或者[TEXT](http://whatis.ctocio.com.cn/searchwhatis/162/6092662.shtml)类型的列上创建。它可以通过CREATE
 TABLE命令创建，也可以通过ALTER TABLE或CREATE INDEX命令创建。对于大规模的数据集，通过ALTER TABLE（或者CREATE INDEX）命令创建全文索引要比把记录插入带有全文索引的空表更快。本文下面的讨论不再涉及全文索引，要了解更多信息，请参见MySQL documentation。 
**单列索引与多列索引**
索引可以是单列索引，也可以是多列索引。下面我们通过具体的例子来说明这两种索引的区别。假设有这样一个people表： 
CREATE TABLE people ( peopleid SMALLINT NOT NULL AUTO_INCREMENT, firstname CHAR(50) 
NOT NULL, lastname CHAR(50) NOT NULL, age SMALLINT NOT NULL, townid SMALLINT NOT 
NULL, PRIMARY KEY (peopleid) );
下面是我们插入到这个people表的数据： 
![](http://tech.ccidnet.com/pub/attachment/2002/12/46299.gif)
这个数据片段中有四个名字为“Mikes”的人（其中两个姓Sullivans，两个姓McConnells），有两个年龄为17岁的人，还有一个名字与众不同的[Joe](http://whatis.ctocio.com.cn/searchwhatis/459/5948459.shtml) Smith。 
这个表的主要用途是根据指定的用户姓、名以及年龄返回相应的peopleid。例如，我们可能需要查找姓名为Mike Sullivan、年龄17岁用户的peopleid（SQL命令为SELECT peopleid FROM people WHERE firstname='Mike' AND lastname='Sullivan' AND age=17;）。由于我们不想让MySQL每次执行查询就去扫描整个表，这里需要考虑运用索引。 
首先，我们可以考虑在单个列上创建索引，比如firstname、lastname或者age列。如果我们创建firstname列的索引（ALTER TABLE people ADD INDEX firstname (firstname);），MySQL将通过这个索引迅速把搜索范围限制到那些firstname='Mike'的记录，然后再在这个“中间结果集”上进行其他条件的搜索：它首先排除那些lastname不等于“Sullivan”的记录，然后排除那些age不等于17的记录。当记录满足所有搜索条件之后，MySQL就返回最终的搜索结果。 
由于建立了firstname列的索引，与执行表的完全扫描相比，MySQL的效率提高了很多，但我们要求MySQL扫描的记录数量仍旧远远超过了实际所需要的。虽然我们可以删除firstname列上的索引，再创建lastname或者age列的索引，但总地看来，不论在哪个列上创建索引搜索效率仍旧相似。 
为了提高搜索效率，**我们需要考虑运用多列索引。如果为firstname、lastname和age这三个列创建一个多列索引，MySQL只需一次检索就能够找出正确的结果！下面是创建这个多列索引的SQL命令： **
**ALTER TABLE people ADD INDEX fname_lname_age (firstname,lastname,age);**
由于索引文件以B-树格式保存，MySQL能够立即转到合适的firstname，然后再转到合适的lastname，最后转到合适的age。在没有扫描数据文件任何一个记录的情况下，MySQL就正确地找出了搜索的目标记录！ 
那么，如果在firstname、lastname、age这三个列上分别创建单列索引，效果是否和创建一个firstname、lastname、age的多列索引一样呢？答案是否定的，两者完全不同。当我们执行查询的时候，MySQL只能使用一个索引。如果你有三个单列的索引，MySQL会试图选择一个限制最严格的索引。但是，即使是限制最严格的单列索引，它的限制能力也肯定远远低于firstname、lastname、age这三个列上的多列索引。 
**最左前缀**
多列索引还有另外一个优点，它通过称为最左前缀（Leftmost Prefixing）的概念体现出来。继续考虑前面的例子，现在我们有一个firstname、lastname、age列上的多列索引，我们称这个索引为fname_lname_age。当搜索条件是以下各种列的组合时，MySQL将使用fname_lname_age索引： 
firstname，lastname，age
firstname，lastname
firstname
从另一方面理解，它相当于我们创建了(firstname，lastname，age)、(firstname，lastname)以及(firstname)这些列组合上的索引。下面这些查询都能够使用这个fname_lname_age索引： 
SELECT peopleid FROM people WHERE firstname='Mike' AND lastname='Sullivan' AND 
age='17'; SELECT peopleid FROM people WHERE firstname='Mike' AND 
lastname='Sullivan'; SELECT peopleid FROM people WHERE firstname='Mike'; The 
following queries cannot use the index at all: SELECT peopleid FROM people WHERE 
lastname='Sullivan'; SELECT peopleid FROM people WHERE age='17'; SELECT peopleid 
FROM people WHERE lastname='Sullivan' AND age='17';
**选择索引列**
在性能优化过程中，选择在哪些列上创建索引是最重要的步骤之一。可以考虑使用索引的主要有两种类型的列：在WHERE子句中出现的列，在join子句中出现的列。请看下面这个查询： 
SELECT age ## 不使用索引 FROM people WHERE firstname='Mike' ## 考虑使用索引 AND 
lastname='Sullivan' ## 考虑使用索引
这个查询与前面的查询略有不同，但仍属于简单查询。由于age是在SELECT部分被引用，MySQL不会用它来限制列选择操作。因此，对于这个查询来说，创建age列的索引没有什么必要。下面是一个更复杂的例子： 
SELECT people.age, ##不使用索引 town.name ##不使用索引 FROM people LEFT JOIN town ON 
people.townid=town.townid ##考虑使用索引 WHERE firstname='Mike' ##考虑使用索引 AND 
lastname='Sullivan' ##考虑使用索引
与前面的例子一样，由于firstname和lastname出现在WHERE子句中，因此这两个列仍旧有创建索引的必要。除此之外，由于town表的townid列出现在join子句中，因此我们需要考虑创建该列的索引。
唯一索引：
          通索引允许被索引的数据列包含重复的值。比如说，因为人有可能同名，所以同一个姓名在同一个“员工个人资料”数据表里可能出现两次或更多次。
　　如果能确定某个数据列将只包含彼此各不相同的值，在为这个数据列创建索引的时候就应该用关键字UNIQUE把它定义为一个唯一索引。这么做的好处：一是简化了MySQL对这个索引的管理工作，这个索引也因此而变得更有效率；二是MySQL会在有新记录插入数据表时，自动检查新记录的这个字段的值是否已经在某个记录的这个字段里出现过了；如果是，MySQL将拒绝插入那条新记录。也就是说，唯一索引可以保证数据记录的唯一性。事实上，在许多场合，人们创建唯一索引的目的往往不是为了提高访问速度，而只是为了避免数据出现重复。
