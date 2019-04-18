# ORACLE进阶之一：HINT - ljx0305的专栏 - CSDN博客
2008年10月08日 14:11:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：724标签：[oracle																[table																[join																[sql](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)
最近由于需要经常统计数据，需要经常用到hint，又不好意思每次都去麻烦DBA，所以在与DBA交流之后，整理了一份关于我们写ORACLE SQL语句时最常用到的hint使用方法：<?XML:NAMESPACE PREFIX = O />
另外，我们以前的流程都是等应用发布之前，统一提交DBA check，然后再由开发人员修改；
但是如果我们开发人员在写这些语句的时候可以知道这些hint的常规用法，这样在和dba check的时候会大大降低返工率，而且不止会**知其然，还会知其所以然**！
呵呵，何乐而不为呢！
hint中我们最常用到的有**ordered****、****use_nl****、****use_hash****、****index****、****full**这五种；
下面就介绍下这5种hint适用的主要场合：
1）use_nl主要用于在多表join的时候，nl的意思是nest loop，就是嵌套查询；
规则为，根据参数中指定表名的顺序，根据where子句中的查询条件把所有记录都查出来，然后再循环这些记录与另外的表join；
适应场合为：某表的查询结果集很小，而其他表的查询结果集很大；
比如：select a.* from table<?XML:NAMESPACE PREFIX = ST1 />1 a,table2 b where a.xx='1' and b.yy = '2' and a.x=b.x
若根据a.xx='1'从table1中查询出来的结果集比根据b.yy = '2'从table2中查询出来的结果集小得多，则可以加上use_nl(a b)；
使用的时候将小结果集放前面，大结果集放后面；
2）use_hash可以看作是use_nl的兄弟，也是用于在多表join的时候，但是他不使用嵌套，而是用hash join；
其规则和use_nl完全一样；
适应场合为：各表的查询结果集都比较大，而且大小差不多；
比如：select a.* from table1 a,table2 b where a.xx='1' and b.yy = '2' and a.x=b.x
若根据a.xx='1'从table1中查询出来的结果集和根据b.yy = '2'从table2中查询出来的结果集都比较大，而且大小差不多，则可以加上use_hash(a b)；
3）ordered用于强制指定DB按照FROM子句中各表的先后顺序进行数据查询；
比如说SELECT A.* FROM TABLE1 A, TABLE2 B WHERE A.X=B.X ……中添加了/*+ORDERED */，则标识oracle会强制先查询table1中的记录，然后再与table2 join；
一般来说这个hint都是与use_nl和use_hash联合使用的；
比如1）中的语句可以为select /*+ORDERED use_nl(a b) */ a.* from table1 a,table2 b where a.xx='1' and b.yy = '2' and a.x=b.x
4）index主要用于强制指定使用某个索引，一般选择效率更高的索引;
适应场合为：where子句中有几个索引可供选择、或者是用到了联合索引中的第一个字段；
比如select a.* from table1 where column1 = 'xx' and column2 = 'yy'
若table1中column1和column2两个字段都有索引，则可以强制指定使用哪个索引，比如index(table1 table1_ind_column1)
5）full用于指定全表扫描，这个实际中应用的较少；
按照DBA的说法，如果使用index查出来的结果集>=全表记录的25%，那么使用index的效率并不高，还不如用全表扫描；
引用:http://blog.csdn.net/sfdev/archive/2008/01/10/2034288.aspx
