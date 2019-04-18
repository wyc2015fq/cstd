# MySQL中merge表存储引擎用法 - z69183787的专栏 - CSDN博客
2017年03月24日 19:10:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：452
在Mysql数据库中，Merge表有点类似于视图。mysql的merge引擎类型允许你把许多结构相同的表合并为一个表。之后，你可以执行查询，从多个表返回的结果就像从一个表返回的结果一样。每一个合并的表必须有完全相同表的定义和结构。
**Mysql Merge表的优点：**
A. 分离静态的和动态的数据
B. 利用结构接近的的数据来优化查询
C. 查询时可以访问更少的数据
D. 更容易维护大数据集
E. 可以通过修改.mrg文件来修改Merge表，当然也可以用alter进行修改，修改后要通过FLUSH TABLES刷新表缓存，此法可以动态增加减少子表
merge表存储引擎在如下这种使用场合会最为有用:
如果需要把日志记录不停的录入MySQL数据库，并且每天、每周或者每个月都创建一个单一的表，而且要制作来自多个表的合计查询，MERGE表这时会非常有效。然而，这项功能有局限性。你只能合并MyISAM表而且必须严格遵守相同的表定义的限制。虽然这看起来好像是一个大问题，但是，如果你使用另外一种表类型(例如InnoDB)，这种合并可能就不需要.
下面定义如下几个表:
**基本表:**
CREATE TABLE TEST_MERGE_1(
ID INT(5) NOT NULL,
VALUE VARCHAR(100) NOT NULL,
PRIMARY KEY(ID)
);
CREATE TABLE TEST_MERGE_2(
ID INT(5) NOT NULL,
VALUE VARCHAR(100) NOT NULL,
PRIMARY KEY(ID)
);
MERGE表：
CREATE TABLE TEST_MERGE(
ID INT(5) NOT NULL,
VALUE VARCHAR(100) NOT NULL,
PRIMARY KEY(ID)
) TYPE=MRG_MyISAM INSERT_METHOD=LAST UNION=(TEST_MERGE_1,TEST_MERGE_2);
**说明:**
1. 此表结构必须与基本表完全一致，包括列名、顺序。UNION表必须同属一个DATABASE。
2. 此表类似于SQL中的union机制。
3. 基本表类型必须是MyISAM的。
4. 可以通过修改.mrg文件来修改MERGE表，每个基本表的名字占一行。注意：修改后要通过FLUSH TABLES刷新表缓存。
5. 对基本表的更改可以直接反映在此表上。
6. INSERT_METHOD的取值可以是： 0 不允许插入 FIRST 插入到UNION中的第一个表 LAST 插入到UNION中的最后一个表。(4.0之后可用)
7. 定义在它上面的约束没有任何作用，约束是由基本表控制的，例如两个基本表中存在着同样的一个Key值，那么在MERGE表中会有两个一样的Key值。
**注意:**
1.如果是通过修改.mrg文件的方式来修改MERGE表，那么一定要修改后要通过FLUSH TABLES刷新表缓存，否则修改不会生效。最近犯过一次这样的错误。
2.在数据量、查询量较大的情况下，不要试图使用Merge表来达到类似于[Oracle](http://www.linuxidc.com/topicnews.aspx?tid=12)的表分区的功能，会很影响性能。我的感觉是和union几乎等价。
3.查询结果及顺序与创建Merge表时联合表的顺序有关。
假设有这样两条个语句：
INSERT INTO TEST_MERGE_1(ID,VALUE) VALUES(1,'ciray');
INSERT INTO TEST_MERGE_2(ID,VALUE) VALUES(1,'blog.csdn.net/ciray');
然后，这个查询：
SELECT * FROM TEST_MERGE WHERE ID=1;
将只会得到一条记录(1,'ciray')，并不是两条记录，也不会是(1,'blog.csdn.net/ciray')。这是因为ID是 PRIMARY KEY，如果在第一个表中查询到记录，则不在后面的表中记录查。如果ID并没有定义唯一性约束，则这个查询会得到两条记录。
