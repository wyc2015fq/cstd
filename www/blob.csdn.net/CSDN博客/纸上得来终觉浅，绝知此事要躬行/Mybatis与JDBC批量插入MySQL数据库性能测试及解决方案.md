# Mybatis与JDBC批量插入MySQL数据库性能测试及解决方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月13日 14:37:38[boonya](https://me.csdn.net/boonya)阅读数：12905








Mybatis与JDBC批量插入MySQL数据库性能测试





Author:boonya 



Date:2017-04-13



# 1      背景

系统中需要批量生成单据数据到数据库表，所以采用批量插入数据库的方式。由于系统中ORM操作集成使用的是Mybatis来完成的。



**在Mybatis中操作一般使用批量插入的方式如下：**

<insertid=*"insertBatch"*parameterType=*"java.util.List"*>

insert into userinfo (uid, uname, uphone, uaddress)

values 

<foreachcollection=*"list"*item=*"item"*index=*"index"*separator=*","*>

(#{item.uid,jdbcType=INTEGER}, 

#{item.uname,jdbcType=VARCHAR},

#{item.uphone,jdbcType=VARCHAR}, 

#{item.uaddress,jdbcType=VARCHAR}

)

</foreach>

</insert>

在实际生产中发现，这样调用的效率并不高，于是我们迫切地需要寻找一种处理批量插入性能较高的方式——回归原生数据库JDBC操作。我们要分析Mybatis和JDBC的插入性能，来决策适合我们生产系统的批量插入方式。



参考资料中有一篇文章是对Spring Mybatis和Spring JDBC插入效率的测试，由于测试的数据量不多不予置评。

# 2      Mybatis

## 2.1  Mybatis测试样本1K

测试10组数据：
|时间（ms）|
|----|
|923|
|412|
|426|
|408|
|405|
|353|
|365|
|344|
|316|
|493|



## 2.2  Mybatis测试样本2K

测试10组数据：
|时间（ms）|
|----|
|11031|
|3340|
|3571|
|2327|
|7273|
|1353|
|2676|
|1249|
|1245|
|1155|



## 2.3  Mybatis测试样本4K

测试10组数据：
|时间（ms）|
|----|
|6070|
|5565|
|5731|
|5400|
|5830|
|5543|
|5469|
|5697|
|5528|
|5399|



## 2.4  Mybatis测试样本6K

测试10组数据：
|时间（ms）|
|----|
|13383|
|12672|
|13030|
|13484|
|13841|
|12952|
|13331|
|13275|
|13000|
|13236|



## 2.5  Mybatis测试样本8K

测试10组数据：
|时间（ms）|
|----|
|25312|
|24702|
|27065|
|25921|
|25156|
|24686|
|25314|
|33947|
|25304|
|25853|



## 2.6  Mybatis测试样本10K

测试10组数据：
|时间（ms）|
|----|
|42148|
|39209|
|38548|
|40109|
|37820|
|37728|
|38178|
|38481|
|38157|
|39032|



## 2.7  Mybatis测试样本15K

测试10组数据：
|时间（ms）|
|----|
|98250|
|88585|
|87438|
|89547|
|88427|
|89522|
|83261|
|80842|
|87163|
|84804|



## 2.8  Mybatis测试样本20K

测试10组数据：
|时间（ms）|
|----|
|145481|
|146618|
|147098|
|145578|
|144947|
|145614|
|142014|
|142315|
|141984|
|143625|



## 2.9  性能测试样本平均值

### 2.9.1  测试样本SQL

#### 2.9.1.1       毫秒级SQL【time(ms)】

-- DELETE FROMprocess_spend_time_result  WHEREpmethod='mybatis';



**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=1000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=2000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=4000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=6000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=8000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=15000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=20000 **AND** p.pmethod='mybatis';

#### 2.9.1.2       秒级SQL【time(s)】

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=1000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=2000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=4000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=6000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=8000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=15000 **AND** p.pmethod='mybatis';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=20000 **AND** p.pmethod='mybatis';

### 2.9.2  测试数据汇总

数据4舍5入保留3位小数
|测试样例|AVG（s）|
|----|----|
|1K|0.445|
|2K|3.522|
|4K|5.623|
|6K|13.221|
|8K|26.326|
|10K|38.941|
|15K|87.784|
|20K|144.527|



## 2.10    Mybatis批量插入问题

### 2.10.1             问题一：处理性能低下

处理10000条耗时：34292ms

![](https://img-blog.csdn.net/20170413143455798)




### 2.10.2             问题二：批量处理数据量大小受限

![](https://img-blog.csdn.net/20170413143408507)

Windows下需要修改MySQL的my.ini文件加入如下配置内容：
max_allowed_packet=500M
Linux下是在/my.cnf修改添加如上内容。



也可以直接这样设置：
SET GLOBAL max_allowed_packet=1073741824;
但MySQL重启后就不起作用了。

### 2.10.3             问题三：线程处理锁表

在通过多线程并发处理的时，很容易导致数据库表锁表，使得后续的操作无法进行。

### 2.10.4             问题四：导致对象回收GC问题

对象超出GC对象回收阀值，导致程序中断。

![](https://img-blog.csdn.net/20170413143524477)




# 3      JDBC

## 3.1  JDBC测试数据1W

测试10组数据：


|普通插入(ms)|普通+事务(ms)|普通批量（ms）|批量+事务(MS)|
|----|----|----|----|
|28489|801|31287|3494|
|30536|3042|35547|1899|
|25571|2041|31022|3501|
|27954|2733|28927|2547|
|29620|1261|34408|1449|
|27125|819|29318|923|
|28993|1079|31099|939|
|27594|2547|33504|3410|
|27967|781|31646|3587|
|33145|1293|37030|1912|



## 3.2  JDBC测试数据5W

测试10组数据：


|普通插入(ms)|普通+事务(ms)|普通批量（ms）|批量+事务(MS)|
|----|----|----|----|
|131427|11568|168623|6926|
|132271|19313|231526|9915|
|192176|5238|227724|10978|
|185640|18955|227497|41959|
|211777|11238|184970|9461|
|208446|5019|263636|23394|
|253351|14265|227391|24870|
|225268|17009|229871|5583|
|163739|9719|230719|16657|
|215033|15802|238018|5330|



## 3.3  JDBC测试数据10W

测试10组数据：


|普通插入(ms)|普通+事务(ms)|普通批量（ms）|批量+事务(MS)|
|----|----|----|----|
|308773|21389|360510|16432|
|352773|23487|372343|25545|
|378805|24034|368416|12507|
|384189|30119|392974|23742|
|369975|30651|378634|26180|
|368659|11902|416932|21321|
|388453|12644|411571|18138|
|391155|11287|396363|11678|
|368055|30987|399078|12212|
|363375|22576|361478|18544|



## 3.4  JDBC测试数据25W

测试10组数据：


|普通插入(ms)|普通+事务(ms)|普通批量（ms）|批量+事务(MS)|
|----|----|----|----|
|942067|51343|990800|70103|
|1070688|28737|1051132|35536|
|1002076|38065|1222409|89644|
|1073114|57050|1312620|82354|
|960697|51733|1338932|33428|
|1025890|37666|1273338|76934|
|1017361|50916|1115627|92790|
|1077821|78650|1175512|52427|
|1038000|23290|1247797|91801|
|1200532|75494|1262051|72087|



## 3.5  JDBC测试数据50W

测试10组数据：


|普通插入(ms)|普通+事务(ms)|普通批量（ms）|批量+事务(MS)|
|----|----|----|----|
|1914920|166575|2059826|146472|
|2111596|62807|1897888|125075|
|2174029|147265|1891542|166921|
|1948838|61284|2129791|93167|
|1909861|167575|1856811|56286|
|1990816|141381|1980060|148012|
|1896793|48087|2065937|56832|
|2130856|174388|2019914|113289|
|2073636|117462|2045715|102792|
|1966828|141319|1857867|116854|



## 3.6  JDBC测试样本均值

### 3.6.1  测试样本SQL

#### 3.6.1.1       毫秒级SQL【time(ms)】



**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **and** p.pmethod='batchInsertWithTransaction2';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=50000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=50000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=50000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=50000 **and** p.pmethod='batchInsertWithTransaction2';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=100000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=100000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=100000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=100000 **and** p.pmethod='batchInsertWithTransaction2';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=250000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=250000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=250000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=250000 **and** p.pmethod='batchInsertWithTransaction2';

**SELECT**AVG(p.ptime) **FROM** process_spend_time_resultp **WHERE**p.plimit=500000 **and**p.pmethod='batchInsert';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=500000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=500000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime) **FROM**process_spend_time_result p **WHERE** p.plimit=500000 **and** p.pmethod='batchInsertWithTransaction2';



#### 3.6.1.2       秒级SQL【time(s)】

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=10000 **and** p.pmethod='batchInsertWithTransaction2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=50000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=50000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=50000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime)/1000 **FROM** process_spend_time_resultp **WHERE**p.plimit=50000 **and**p.pmethod='batchInsertWithTransaction2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=100000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=100000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=100000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=100000 **and** p.pmethod='batchInsertWithTransaction2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=250000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=250000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=250000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=250000 **and** p.pmethod='batchInsertWithTransaction2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=500000 **and** p.pmethod='batchInsert';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=500000 **and** p.pmethod='batchInsert2';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=500000 **and** p.pmethod='batchInsertWithTransaction';

**SELECT**AVG(p.ptime)/1000 **FROM**process_spend_time_result p **WHERE** p.plimit=500000 **and** p.pmethod='batchInsertWithTransaction2';



### 3.6.2  测试数据汇总

数据4舍5入保留2位小数
|测试样例|1W（s）|5W(s)|10W(s)|25W(s)|50W(s)|
|----|----|----|----|----|----|
|普通插入|28.70|191.91|367.42|1040.82|2011.82|
|普通+事务|1.64|12.81|21.91|49.29|122.81|
|批量插入|32.38|223.00|385.83|1199.02|1980.54|
|批量+事务|2.37|15.51|18.63|69.71|112.57|

## 3.7  JDBC测试规律总结

### 3.7.1  规律一：批量插入尽量使用事务控制

数据4舍5入保留2位小数
|测试样例|1W（s）|5W(s)|10W(s)|25W(s)|50W(s)|
|----|----|----|----|----|----|
|普通插入|28.70|191.91|367.42|1040.82|2011.82|
|普通+事务|1.64|12.81|21.91|49.29|122.81|
|批量插入|32.38|223.00|385.83|1199.02|1980.54|
|批量+事务|2.37|15.51|18.63|69.71|112.57|



### 3.7.2  规律二：事务批量处理的数据量不要太大

数据4舍5入保留2位小数
|测试样例|1W（s）|5W(s)|10W(s)|25W(s)|50W(s)|
|----|----|----|----|----|----|
|普通插入|28.70|191.91|367.42|1040.82|2011.82|
|普通+事务|1.64|12.81|21.91|49.29|122.81|
|批量插入|32.38|223.00|385.83|1199.02|1980.54|
|批量+事务|2.37|15.51|18.63|69.71|112.57|

### 3.7.3  规律三：适当地设置MySQL参数可以提高数据库性能

数据4舍5入保留2位小数
|测试样例|1W（s）|5W(s)|10W(s)|25W(s)|50W(s)|
|----|----|----|----|----|----|
|普通插入|28.70|191.91|367.42|1040.82|2011.82|
|普通+事务|1.64|12.81|21.91|49.29|122.81|
|批量插入|32.38|223.00|385.83|1199.02|1980.54|
|批量+事务|2.37|15.51|18.63|69.71|112.57|

### 3.7.4  规律四：处理数据量和时间不是成倍增长

数据4舍5入保留2位小数
|测试样例|1W（s）|5W(s)|10W(s)|25W(s)|50W(s)|
|----|----|----|----|----|----|
|普通插入|28.70|191.91|367.42|1040.82|2011.82|
|普通+事务|1.64|12.81|21.91|49.29|122.81|
|批量插入|32.38|223.00|385.83|1199.02|1980.54|
|批量+事务|2.37|15.51|18.63|69.71|112.57|

### 3.7.5  规律五：数据库性能随数据量的不断加大而降低

数据4舍5入保留2位小数
|测试样例|1W（s）|5W(s)|10W(s)|25W(s)|50W(s)|
|----|----|----|----|----|----|
|普通插入|28.70|191.91|367.42|1040.82|2011.82|
|普通+事务|1.64|12.81|21.91|49.29|122.81|
|批量插入|32.38|223.00|385.83|1199.02|1980.54|
|批量+事务|2.37|15.51|18.63|69.71|112.57|



# 4      结论

经过以上测试得出结论：Mybatis的批量适合处理少了数据的批量处理，而JDBC适合大数据量的批量处理。据此，采用JDBC批量+事务处理大数据量的表插入操作是最合适的。

# 5      方案

因为要考虑JVM的GC所以数据应该限制一下，但鉴于Mybatis大数据量的批量插入效率不高，所以根据数据大小分段治理。



## 5.1  小于1W使用：Mybatis批量插入方案

对JVM进行调优，但主要的性能瓶颈在批量插入操作。鉴于mybatis在项目开发方面的优势，数据量很小的情况下还是建议使用Mybatis。



## 5.2  大于1W小于10W使用：JDBC批量+事务处理

对JVM进行调优（设置Stack和GC等）。一般操作30秒以内是可以容忍的性能耗时。

## 5.3  10W以上数据使用：数据分批+JDBC批量+事务处理

对JVM进行调优（设置Stack和GC等），通过数据分批处理。对于分批处理需要借鉴前面的测试数据来定义分批量的大小，主要是对操作时间调优。



如果是100W、1000W级别的数据量，分批处理可以很大程度地提升插入效率，具体的分批需要通过实践去分配，数据量太大这里就不做实验了。

# 6      参考资料

JDBC实现往MySQL数据库插入百万数据：[http://www.cnblogs.com/fnz0/p/5713102.html](http://www.cnblogs.com/fnz0/p/5713102.html)





MySQL Max_allowed_packet: [http://stackoverflow.com/questions/8062496/how-to-change-max-allowed-packet-size](http://stackoverflow.com/questions/8062496/how-to-change-max-allowed-packet-size)



Spring Mybatis和Spring JDBC的插入效率比较：[http://toplchx.iteye.com/blog/1988254](http://toplchx.iteye.com/blog/1988254)



**注：另外一种比较高效的导入方式是生成一个文本文件使用MySQL的JDBC LOAD DATA LOCAL INFILE；**参考示例：

[MySQL使用JDBC LOAD DATA LOCAL INFILE导入注意事项](http://www.cnblogs.com/obullxl/archive/2012/06/11/jdbc-mysql-load-data-infile.html)


[MySQL使用LOAD DATA LOCAL INFILE数据3-5秒导入40W数据](http://www.jb51.net/article/33365.htm)


[Java不写文件，LOAD DATA LOCAL INFILE大批量导入数据到MySQL的实现](http://blog.itpub.net/29254281/viewspace-1839492/)


# 7.实战演练

注意：事务处理的数据量不能太多。



## 7.1  JDBC工具类





package com.wlyd.fmcgwms.util.jdbc;



import java.io.IOException;

import java.sql.Connection;

import java.sql.DriverManager;

import java.sql.SQLException;

import java.util.Properties;

/**

 *JDBC连接工具类

 * 

 *@package com.wlyd.fmcgwms.util.jdbc.JdbcConnection

 *@date   2017年4月17日  下午3:32:15

 *@author pengjunlin

 *@comment   

 *@update

 */

public class JdbcConnection {



         privatestatic Connection conn = null;  



   private static Properties props = null; 



   static {  

       props = new Properties();  

       try {  

           props.load(JdbcConnection.class.getResourceAsStream("/jdbc.properties"));  

       } catch (IOException e1) {  

           e1.printStackTrace();  

       }  

       try {  

           Class.forName(props.getProperty("jdbc.driverClass"));  

       } catch (ClassNotFoundException e) { 

           e.printStackTrace();  

       }  

    }



   public static Connection getConn(){ 

       try {  

           conn = DriverManager.getConnection(props.getProperty("jdbc.jdbcUrl"),props.getProperty("jdbc.user"),props.getProperty("jdbc.password")); 

           conn.setAutoCommit(false);  

       } catch (SQLException e) {  

           e.printStackTrace();  

       }  

       return conn;  

   }  



   public void closeConn(){  

       try {  

           if (conn != null)  

                conn.close();  

       } catch (SQLException e) {  

           e.printStackTrace();

       }  



   }  



}



## 7.2  批量事务插入

### 7.2.1  接口

/**

* jdbc批量插入

* 

* **@MethodName**: jdbcBatchInsert 

* **@Description**: 

* **@param** user

* **@param** items

* **@return**

* **@throws** Exception

* **@throws**

*/

**int** jdbcBatchInsert(EsUser user,List<WmElectronicSheetItem> items) **throws** Exception;



/**

 * jdbc批量插入

 * 

 * **@MethodName**: jdbcPerBatchInsert 

 * **@Description**: 

 * **@param** user

 * **@param** items

 * **@return**

 * **@throws** Exception

 * **@throws**

 */

**int**jdbcPerBatchInsert(EsUser user, List<WmElectronicSheetItem> items)  **throws** Exception;

### 7.2.2  实现

@Override

**publicint** jdbcBatchInsert(EsUser user, List<WmElectronicSheetItem> items) **throws** Exception {

**int**flag = 0;



**int**perCount = 100, index = 0;



**int**times = items.size() / perCount;

**long**stime=System.*currentTimeMillis*();

**try** {

**do** {

// 休眠50ms

Thread.*sleep*(50);

List<WmElectronicSheetItem>listTemp= **null**;

**if** (items.size() >= perCount) {

listTemp = items.subList(0, perCount);// listTemp是分段处理逻辑的参数

}**else**{

listTemp = items.subList(0, items.size());// listTemp是分段处理逻辑的参数



}

// 遍历当前的值是否正确

Stringresult= "";

**for** (**int**i = 0; i < listTemp.size(); i++) {

result += listTemp.get(i) + ",";

}

Log.*getLogger*(getClass()).info("第" + (index+1)+ "轮:>>" + result);



// 事务单元执行个数==尽量在事务里面处理少一点(事务尽量小一点)

jdbcPerBatchInsert(user, listTemp);



items.removeAll(listTemp);



Log.*getLogger*(getClass()).info("当前剩余集合长度:>>" + items.size());



index++;

}**while**(index<= times);

// 计算时间

**long**etime=System.*currentTimeMillis*();

Log.*getLogger*(getClass()).info(">>封装面单号批量事务插入总共耗时-----------------------:"+(etime-stime)+"ms!");

}**catch**(Exception e) {

e.printStackTrace();

flag=2;

Log.*getLogger*(getClass()).error("JDBC批量执行插入异常:>>" + items.size());

**thrownew** RuntimeException();

}



**return**flag;

}





@Override

**publicint**jdbcPerBatchInsert(EsUser user, List<WmElectronicSheetItem> items)

**throws** Exception {

**int**flag=0;

Connectionconn=JdbcConnection.*getConn*();

PreparedStatementpstm= **null**;

**try** {

Stringsql= "insert intowm_electronic_sheet_item_?  ("

+"WESI_WESB_ID,WESI_CARRIER_CODE,WESI_START_CHARACTER,WESI_SEQUENCE,"

+"WESI_WAREHOUSE_ID, WESI_CODE,WESI_STATE, "

+"CREATOR, CREATE_TIME, MODIFIER,MODIFY_TIME) values"

+"(?, ?, ?,  ?, ?, ?, ?, ?, ?, ?, ?)";

conn.setAutoCommit(**false**);

Log.*getLogger*(getClass()).info(">>>>>>验证JDBC连接："+(conn!=**null**));

**for** (**int**i = 0, j = items.size(); i < j; i++) {

WmElectronicSheetItemitem= items.get(i);

pstm = conn.prepareStatement(sql);

pstm.setInt(1, Integer.*valueOf*(user.getEsCorCode())); // tableName

pstm.setInt(2, item.getWesiWesbId());// WESI_WESB_ID

pstm.setString(3, item.getWesiCarrierCode());// WESI_CARRIER_CODE

pstm.setString(4, item.getWesiStartCharacter());// WESI_START_CHARACTER

pstm.setString(5, item.getWesiSequence());// WESI_SEQUENCE

pstm.setInt(6, item.getWesiWarehouseId());// WESI_WAREHOUSE_ID

pstm.setString(7, item.getWesiCode());// WESI_CODE

pstm.setInt(8, item.getWesiState());// WESI_STATE

pstm.setInt(9, user.getEsId());// CREATOR

pstm.setTimestamp(10,**new** java.sql.Timestamp(**new** Date().getTime()));// CREATE_TIME

pstm.setInt(11, -1);// MODIFIER

pstm.setTimestamp(12, **null**);// MODIFY_TIME

pstm.executeUpdate();

}

conn.commit();// 手动提交事务

}**catch**(Exception e) {

e.printStackTrace();

flag=2;

Log.*getLogger*(getClass()).error("JDBC批量分配事务单元执行插入异常:>>" + items.size());

**thrownew** RuntimeException();

}**finally**{

**if** (pstm != **null**) {

**try** {

pstm.close();

}**catch**(SQLException e) {

e.printStackTrace();

}

}

**if** (conn != **null**) {

conn.close();

}

}

**return**flag;

}



### 7.3测试一组数据
测试50000条数据耗时统计：



|数据样本事务大小 （每个事务处理的数量）|耗时(ms)|
|----|----|
|100|100258|
|500|75041|
|1000|68850|
|3000|78354|

通过这种方式提交比较安全不会出现线程锁表问题，事务处理尽量少，根据每次事务提交执行的量可以实现时间上的优化。




本文测试文档代码和数据库均已上传：[http://download.csdn.net/detail/boonya/9812860](http://download.csdn.net/detail/boonya/9812860)




