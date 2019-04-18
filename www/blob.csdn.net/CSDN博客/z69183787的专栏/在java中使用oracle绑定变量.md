# 在java中使用oracle绑定变量 - z69183787的专栏 - CSDN博客
2014年04月15日 12:43:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：971
**为什么要使用绑定变量：**
在JAVA中的SQL 语句的编写方面，没有使用ORACLE 绑定变量，很大程度上降低了数据库的性能，表现在两个方面：
1、SQL语句硬分析(Hard Parse)太多，严重消耗CPU资源，延长了SQL语句总的执行时间。
SQL语句的执行过程分几个步骤：语法检查、分析、执行、返回结果。其中分析又分为硬分析(Hard
 Parse)和软分析(Soft Parse)。
一条SQL语句通过语法检查后，Oracle 会先去shared pool 中找是否有相同的sql，如果找着了，就叫软分析，然后执行SQL语句。
硬分析主要是检查该sql所涉及到的所有对象是否有效以及权限等关系，然后根据RBO或CBO模式生成执行计划，然后才执行SQL语句。
可以看出，硬分析比软分析多了很多动作，而这里面的关键是“在shared pool 中是否有相同的sql”，而这就取决于是否使用绑定变量。
2、共享池中SQL语句数量太多，重用性极低，加速了SQL语句的老化，导致共享池碎片过多。
共享池中不同的SQL语句数量巨大，根据LRU原则，一些语句逐渐老化，最终被清理出共享池；这样就导致shared_pool_size 里面命中率
下降，共享池碎片增多，可用内存空间不足。而为了维护共享池内部结构，需要使用latch，一种内部生命周期很短的lock，这将使用大量
的cpu 资源，使得性能急剧下降。
不使用绑定变量违背了oracle 的shared pool 的设计的原则，违背了这个设计用来共享的思想。
**如何正确在java中使用绑定变量：**
编写java 程序时，我们习惯都是定义JAVA 的程序变量，放入SQL 语句中，如
String v_id = 'xxxxx';
String v_sql = 'select name from table_a where id = ' + v_id ; 
以上代码，看起来是使用了变量v_id ，但这却是java 的程序变量，而不是oracle 的绑定变量，语句传递到数据库后，此java 的程序变量
已经被替换成具体的常量值，变成：
select * from table_a where name = 'xxxxx' ;
假定这个语句第一次执行，会进行硬分析。后来，同一段java 代码中v_id 值发现变化（v_id = 'yyyyyy'），数据库又接收到这样的语句：
select * from table_a where name = 'yyyyyy' ;
ORACLE 并不认为以上两条语句是相同的语句，因此对第二条语句会又做一次硬分析。这两条语句的执行计划可是一样的！
其实，只需将以上java 代码改成以下这样，就使用了oracle 的绑定变量：
String v_id = 'xxxxx';
String v_sql = 'select name from table_a where id = ? '; //嵌入绑定变量
stmt = con.prepareStatement(
 v_sql );
stmt.setString(1, v_id ); //为绑定变量赋值
stmt.executeQuery();
在Java中，结合使用setXXX 系列方法，可以为不同数据类型的绑定变量进行赋值，从而大大优化了SQL 语句的性能。
