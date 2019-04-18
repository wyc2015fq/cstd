# Oracle中merge into的使用 - z69183787的专栏 - CSDN博客
2014年06月13日 11:43:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：600
个人分类：[Oracle](https://blog.csdn.net/z69183787/article/category/2175483)
http://blog.csdn.net/yuzhic/article/details/1896878
http://blog.csdn.net/macle2010/article/details/5980965
该命令使用一条语句从一个或者多个数据源中完成对表的更新和插入数据. ORACLE 9i 中，使用此命令必须同时指定UPDATE 和INSERT 关键词,ORACLE 10g 做了如下改动。
1，insert 和update是可选的 2，UPDATE 和INSERT 后面可以跟WHERE 子句 3,在ON条件中可以使用常量来insert 所有的行到目标表中，不需要连接到源表和目标表 4,UPDATE 子句后面可以跟delete 来去除一些不需要的行。
举例：
- createtable PRODUCTS
   
-     (   
-     PRODUCT_ID INTEGER,
   
-     PRODUCT_NAME VARCHAR2(60),   
-     CATEGORY VARCHAR2(60)   
-     );   
- 
- insertinto PRODUCTS values (1501, 'VIVITAR 35MM', 'ELECTRNCS');
   
- insertinto PRODUCTS values (1502, 'OLYMPUS IS50', 'ELECTRNCS');
   
- insertinto PRODUCTS values (1600, 'PLAY GYM', 'TOYS');
   
- insertinto PRODUCTS values (1601, 'LAMAZE', 'TOYS');
   
- insertinto PRODUCTS values (1666, 'HARRY POTTER', 'DVD');
   
- commit;
   
- 
- createtable NEWPRODUCTS
   
-     (   
-     PRODUCT_ID INTEGER,
   
-     PRODUCT_NAME VARCHAR2(60),   
-     CATEGORY VARCHAR2(60)   
-     );   
- 
- insertinto NEWPRODUCTS values (1502, 'OLYMPUS CAMERA', 'ELECTRNCS');
   
- insertinto NEWPRODUCTS values (1601, 'LAMAZE', 'TOYS');
   
- insertinto NEWPRODUCTS values (1666, 'HARRY POTTER', 'TOYS');
   
- insertinto NEWPRODUCTS values (1700, 'WAIT INTERFACE', 'BOOKS');
   
- commit;   
- 
- 1，可省略的update 或者insert
-    MERGE INTO products p
   
-     2 USING newproducts np   
-     3 ON (p.product_id = np.product_id)
   
-     4 WHEN MATCHED THEN
-     5 UPDATE
-     6 SET p.product_name = np.product_name,
   
-     7 p.category = np.category;
  使用表newproducts中的product_name 和category字段来更新表products 中相同product_id的product_name 和category.
2，当条件不满足的时候把newproducts表中的数据INSERT 到表products中。
- MERGE INTO products p
   
-     USING newproducts np   
- ON (p.product_id = np.product_id)
   
- WHENNOT MATCHED THEN
- INSERT
- VALUES (np.product_id, np.product_name,
   
-      np.category);   
3，带条件的insert 和update
- MERGE INTO products p
   
-  USING newproducts np   
- ON (p.product_id = np.product_id)
   
- WHEN MATCHED THEN
- UPDATE
- SET p.product_name = np.product_name
   
- WHERE p.category = np.category;
 insert 和update　都带有where 字句
- MERGE INTO products p   
-  USING newproducts np   
- ON (p.product_id = np.product_id)
   
- WHEN MATCHED THEN
- UPDATE
- SET p.product_name = np.product_name,
   
-      p.category = np.category   
- WHERE p.category = 'DVD'
- WHENNOT MATCHED THEN
- INSERT
- VALUES (np.product_id, np.product_name, np.category)
   
- WHERE np.category != 'BOOKS'
4,无条件的insert
- MERGE INTO products p
   
-  USING newproducts np   
- ON (1=0)
   
- WHENNOT MATCHED THEN
- INSERT
- VALUES (np.product_id, np.product_name, np.category)
   
- WHERE np.category = 'BOOKS'
5，delete 子句
１  merge into products p
  2  using newproducts np
  3  on(p.product_id = np.product_id)
  4  when matched then
  5  update
  6  set p.product_name = np.product_name
  7  delete where category = 'macle1_cate';
select *
from products;
 PRODUCT_ID PRODUCT_NAME         CATEGORY
--------------------------------------- -------------------- --------------------
                                   1502 macle22              macle2_cate
                                   1503 macle3                macle2_cate
                                   1504 macle                  macle1_cate
                                   1505 macle5                macle5_cate
1504 中的macle1_cate 满足delete where，但是不满足 on 中的条件，所以没有被删除。！！！！！！重点
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
动机：
想在Oracle中用一条SQL语句直接进行Insert/Update的操作。
说明：
在进行SQL语句编写时，我们经常会遇到大量的同时进行Insert/Update的语句 ，也就是说当存在记录时，就更新(Update)，不存在数据时，就插入(Insert)。
实战：
接下来我们有一个任务，有一个表T，有两个字段a,b，我们想在表T中做Insert/Update,如果存在，则更新T中b的值，如果不存在，则插入一条记录。在Microsoft的SQL语法中，很简单的一句判断就可以了，SQL Server中的语法如下：
if exists(select 1 from T where T.a='1001' ) update T set T.b=2 Where T.a='1001' else insert into T(a,b) values('1001',2);
以上语句表明当T表中如果存在a='1001' 的记录的话，就把b的值设为2，否则就Insert一条a='100'，b=2的记录到T中。
但是接下来在Oracle中就遇到麻烦了，记得在Oracle 9i之后就有一条Merge into 的语句可以同时进行Insert 和Update的吗，Merge的语法如下：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)MERGE INTO table_name alias1 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)USING (table|view|sub_query) alias2
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ON (join condition) 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)WHEN MATCHED THEN
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)UPDATE table_name 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)SET col1 = col_val1, 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        col2     = col2_val 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)WHENNOT MATCHED THEN
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)INSERT (column_list) VALUES (column_values); 
上面的语法大家应该都容易懂吧，那我们按照以上的逻辑再写一次。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)MERGE INTO T T1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)USING (SELECT a,b FROM T WHERE t.a='1001') T2
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ON ( T1.a=T2.a)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)WHEN MATCHED THEN
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)UPDATESET T1.b =**2**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)WHENNOT MATCHED THEN
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)INSERT (a,b) VALUES('1001',**2**);
以上的语句貌似很对是吧![](http://blog.csdn.net/Editor/FCKeditor/editor/images/smiley/msn/tounge_smile.gif)，实际上，该语句只能进行更新，而无法进行Insert，错误在哪里呢？
其实在Oracle中Merge语句原先是用来进行整表的更新用的，也就是ETL工具比较常用的语法，重点是在Using上。
用中文来解释Merge语法，就是：
在alias2中Select出来的数据，每一条都跟alias1进行 ON (join condition)的比较，如果匹配，就进行更新的操作(Update),如果不匹配，就进行插入操作(Insert)。
因此，严格意义上讲，”在一个同时存在Insert和Update语法的Merge语句中，总共Insert/Update的记录数，就是Using语句中alias2的记录数。”
以上这句话也就很好的解释了在上面写的语句为何只能进行Update，而不能进行Insert了，因为都Select不到数据，如何能进行Insert呢:)
接下来要改成正确的语句就容易多了，如下：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)MERGE INTO T T1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)USING (SELECT'1001'AS a,2AS b FROM dual) T2
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ON ( T1.a=T2.a)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)WHEN MATCHED THEN
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)UPDATESET T1.b = T2.b
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)WHENNOT MATCHED THEN
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)INSERT (a,b) VALUES(T2.a,T2.b);
查询结果，OK!
注意：
如果不懂Merge语句的原理，Merge语句是一条比较危险的语句，特别是在您只想更新一条记录的时候，因为不经意间，你可能就把整表的数据都Update了一遍.....汗!!!
我曾经犯过的一个错误如下所示，大家看出来是什么问题了吗？
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)MERGE INTO T T1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)USING (SELECTCount(*) cnt FROM T WHERE T.a='1001') T2
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)ON (T2.cnt>**0**)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)WHEN MATCHED THEN
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)UPDATESET T1.b = T2.b
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)WHENNOT MATCHED THEN
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)INSERT (a,b) VALUES(T2.a,T2.b);
