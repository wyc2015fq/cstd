# Oracle开发专题之：分析函数(OVER)  - ljx0305的专栏 - CSDN博客
2008年07月15日 14:56:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：916
**目录：===============================================**[**1.Oracle分析函数简介**](http://www.blogjava.net/pengpenglin/archive/2008/06/25/210536.html#part1)
[**2. Oracle分析函数简单实例**](http://www.blogjava.net/pengpenglin/archive/2008/06/25/210536.html#part2)
[**3.分析函数OVER解析**](http://www.blogjava.net/pengpenglin/archive/2008/06/25/210536.html#part3)
**一、Oracle分析函数简介：**
在日常的生产环境中，我们接触得比较多的是OLTP系统(即Online Transaction Process)，这些系统的特点是具备实时要求，或者至少说对响应的时间多长有一定的要求；其次这些系统的业务逻辑一般比较复杂，可能需要经过多次的运算。比如我们经常接触到的电子商城。
在这些系统之外，还有一种称之为OLAP的系统(即Online Aanalyse Process)，这些系统一般用于系统决策使用。通常和数据仓库、数据分析、数据挖掘等概念联系在一起。这些系统的特点是数据量大，对实时响应的要求不高或者根本不关注这方面的要求，以查询、统计操作为主。
我们来看看下面的几个典型例子：
①查找上一年度各个销售区域排名前10的员工
②按区域查找上一年度订单总额占区域订单总额20%以上的客户
③查找上一年度销售最差的部门所在的区域
④查找上一年度销售最好和最差的产品
我们看看上面的几个例子就可以感觉到这几个查询和我们日常遇到的查询有些不同，具体有：
**①需要对同样的数据进行不同级别的聚合操作②需要在表内将多条数据和同一条数据进行多次的比较③需要在排序完的结果集上进行额外的过滤操作**
**二、Oracle分析函数简单实例：**
下面我们通过一个实际的例子：按区域查找上一年度订单总额占区域订单总额20%以上的客户，来看看分析函数的应用。
【1】测试环境：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>desc orders_tmp;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) Name                           Null?    Type
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)----------------------- -------- ----------------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) CUST_NBR                    NOTNULLNUMBER(5)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID                   NOTNULLNUMBER(5)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) SALESPERSON_ID      NOTNULLNUMBER(5)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)YEARNOTNULLNUMBER(4)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)MONTHNOTNULLNUMBER(2)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) TOT_ORDERS              NOTNULLNUMBER(7)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) TOT_SALES                 NOTNULLNUMBER(11,2)
【2】测试数据：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select*from orders_tmp;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)  CUST_NBR  REGION_ID SALESPERSON_ID       YEARMONTH TOT_ORDERS  TOT_SALES
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ---------- -------------- ---------- ---------- ---------- ----------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1171120017212204
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)454200110237802
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7672001233750
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)106820011221691
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)106720012342624
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1571220005624
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)127920006250658
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)15220003244494
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)15120009274864
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)25420003235060
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2542000446454
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)251200010435580
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)454200012239190
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)13 rows selected.
【3】测试语句： 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select o.cust_nbr customer,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2         o.region_id region,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3sum(o.tot_sales) cust_sales,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4sum(sum(o.tot_sales)) over(partition by o.region_id) region_sales
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5from orders_tmp o
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6where o.year=2001
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7groupby o.region_id, o.cust_nbr;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)  CUSTOMER     REGION CUST_SALES REGION_SALES
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ---------- ---------- ------------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)453780237802
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)76375068065
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1066431568065
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1171220412204
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
**三、分析函数OVER解析：**
请注意上面的绿色高亮部分，group by的意图很明显：将数据按区域ID，客户进行分组，那么Over这一部分有什么用呢？假如我们只需要统计每个区域每个客户的订单总额，那么我们只需要group by o.region_id,o.cust_nbr就够了。但我们还想在每一行显示该客户所在区域的订单总额，这一点和前面的不同：需要在前面分组的基础上按区域累加。很显然group by和sum是无法做到这一点的(因为聚集操作的级别不一样，前者是对一个客户，后者是对一批客户)。
**这就是over函数的作用了！它的作用是告诉SQL引擎：按区域对数据进行分区，然后累积每个区域每个客户的订单总额(sum(sum(o.tot_sales)))。**
现在我们已经知道2001年度每个客户及其对应区域的订单总额，那么下面就是筛选那些个人订单总额占到区域订单总额20%以上的大客户了
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select*
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2from (select o.cust_nbr customer,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3                 o.region_id region,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4sum(o.tot_sales) cust_sales,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5sum(sum(o.tot_sales)) over(partition by o.region_id) region_sales
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6from orders_tmp o
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7where o.year=2001
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8groupby o.region_id, o.cust_nbr) all_sales
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9where all_sales.cust_sales > all_sales.region_sales *0.2;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)  CUSTOMER     REGION CUST_SALES REGION_SALES
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ---------- ---------- ------------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)453780237802
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1066431568065
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1171220412204
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>
现在我们已经知道这些大客户是谁了！哦，不过这还不够，如果我们想要知道每个大客户所占的订单比例呢？看看下面的SQL语句，只需要一个简单的Round函数就搞定了。 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select all_sales.*,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2100*round(cust_sales / region_sales, 2) ||'%'Percent
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3from (select o.cust_nbr customer,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4                 o.region_id region,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5sum(o.tot_sales) cust_sales,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6sum(sum(o.tot_sales)) over(partition by o.region_id) region_sales
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7from orders_tmp o
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8where o.year=2001
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9groupby o.region_id, o.cust_nbr) all_sales
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)10where all_sales.cust_sales > all_sales.region_sales *0.2;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)  CUSTOMER     REGION CUST_SALES REGION_SALES PERCENT
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ---------- ---------- ------------ ----------------------------------------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)453780237802100%
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)106643156806594%
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1171220412204100%
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>
**总结：**
**①Over函数指明在那些字段上做分析，其内跟Partition by表示对数据进行分组。注意Partition by可以有多个字段。②Over函数可以和其它聚集函数、分析函数搭配，起到不同的作用。例如这里的SUM，还有诸如Rank，Dense_rank等。**参考资料：《Mastering Oracle SQL》(By [Alan Beaulieu](http://www.oreillynet.com/cs/catalog/view/au/789?x-t=book.view), [Sanjay Mishra](http://www.oreillynet.com/cs/catalog/view/au/607?x-t=book.view) O'Reilly June 2004  0-596-00632-2)
引用自：[http://www.blogjava.net/pengpenglin/archive/2008/06/25/210536.html](http://www.blogjava.net/pengpenglin/archive/2008/06/25/210536.html)
