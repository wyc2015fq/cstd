# ROLLUP和CUBE语句 - ljx0305的专栏 - CSDN博客
2009年04月13日 17:24:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：606标签：[manager																[报表																[null																[sql																[oracle																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=报表&t=blog)](https://so.csdn.net/so/search/s.do?q=manager&t=blog)
个人分类：[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)

Oracle的GROUP BY语句除了最基本的语法外，还支持ROLLUP和CUBE语句。如果是ROLLUP(A, B, C)的话，首先会对(A、B、C)进行GROUP BY，然后对(A、B)进行GROUP BY，然后是(A)进行GROUP BY，最后对全表进行GROUP BY操作。如果是GROUP BY CUBE(A, B, C)，则首先会对(A、B、C)进行GROUP BY，然后依次是(A、B)，(A、C)，(A)，(B、C)，(B)，(C)，最后对全表进行GROUP BY操作。 grouping_id()可以美化效果：
Oracle的GROUP BY语句除了最基本的语法外，还支持ROLLUP和CUBE语句。
除本文内容外，你还可参考：
分析函数参考手册： [http://xsb.itpub.net/post/419/33028](http://xsb.itpub.net/post/419/33028)
分析函数使用例子介绍：[http://xsb.itpub.net/post/419/44634](http://xsb.itpub.net/post/419/44634)
SQL> create table t as select * from dba_indexes;
表已创建。
SQL> select index_type, status, count(*) from t group by index_type, status;
INDEX_TYPE STATUS COUNT(*)
--------------------------- -------- ----------
LOB VALID 51
NORMAL N/A 25
NORMAL VALID 479
CLUSTER VALID 11
下面来看看ROLLUP和CUBE语句的执行结果。
SQL> select index_type, status, count(*) from t group by rollup(index_type, status);
INDEX_TYPE STATUS COUNT(*)
--------------------------- -------- ----------
LOB VALID 51
LOB 51
NORMAL N/A 25
NORMAL VALID 479
NORMAL 504
CLUSTER VALID 11
CLUSTER 11
566
已选择8行。
SQL> select index_type, status, count(*) from t group by cube(index_type, status);
INDEX_TYPE STATUS COUNT(*)
--------------------------- -------- ----------
566
N/A 25
VALID 541
LOB 51
LOB VALID 51
NORMAL 504
NORMAL N/A 25
NORMAL VALID 479
CLUSTER 11
CLUSTER VALID 11
已选择10行。
查询结果不是很一目了然，下面通过Oracle提供的函数GROUPING来整理一下查询结果。
SQL> select grouping(index_type) g_ind, grouping(status) g_st, index_type, status, count(*)
2 from t group by rollup(index_type, status) order by 1, 2;
G_IND G_ST INDEX_TYPE STATUS COUNT(*)
---------- ---------- --------------------------- -------- ----------
0 0 LOB VALID 51
0 0 NORMAL N/A 25
0 0 NORMAL VALID 479
0 0 CLUSTER VALID 11
0 1 LOB 51
0 1 NORMAL 504
0 1 CLUSTER 11
1 1 566
已选择8行。
这个查询结果就直观多了，和不带ROLLUP语句的GROUP BY相比，ROLLUP增加了对INDEX_TYPE的GROUP BY统计和对所有记录的GROUP BY统计。
也就是说，如果是ROLLUP(A, B, C)的话，首先会对(A、B、C)进行GROUP BY，然后对(A、B)进行GROUP BY，然后是(A)进行GROUP BY，最后对全表进行GROUP BY操作。
下面看看CUBE语句。
SQL> select grouping(index_type) g_ind, grouping(status) g_st, index_type, status, count(*) 
2 from t group by cube(index_type, status) order by 1, 2;
G_IND G_ST INDEX_TYPE STATUS COUNT(*)
---------- ---------- --------------------------- -------- ----------
0 0 LOB VALID 51
0 0 NORMAL N/A 25
0 0 NORMAL VALID 479
0 0 CLUSTER VALID 11
0 1 LOB 51
0 1 NORMAL 504
0 1 CLUSTER 11
1 0 N/A 25
1 0 VALID 541
1 1 566
已选择10行。
和ROLLUP相比，CUBE又增加了对STATUS列的GROUP BY统计。
如果是GROUP BY CUBE(A, B, C)，则首先会对(A、B、C)进行GROUP BY，然后依次是(A、B)，(A、C)，(A)，(B、C)，(B)，(C)，最后对全表进行GROUP BY操作。
除了使用GROUPING函数，还可以使用GROUPING_ID来标识GROUP BY结果。
SQL> select grouping_id(index_type, status) g_ind, index_type, status, count(*) 
2 from t group by rollup(index_type, status) order by 1;
G_IND INDEX_TYPE STATUS COUNT(*)
---------- --------------------------- -------- ----------
0 LOB VALID 51
0 NORMAL N/A 25
0 NORMAL VALID 479
0 CLUSTER VALID 11
1 LOB 51
1 NORMAL 504
1 CLUSTER 11
3 566
已选择8行。
SQL> select grouping_id(index_type, status) g_ind, index_type, status, count(*) 
2 from t group by cube(index_type, status) order by 1;
G_IND INDEX_TYPE STATUS COUNT(*)
---------- --------------------------- -------- ----------
0 LOB VALID 51
0 NORMAL N/A 25
0 NORMAL VALID 479
0 CLUSTER VALID 11
1 LOB 51
1 NORMAL 504
1 CLUSTER 11
2 N/A 25
2 VALID 541
3 566
已选择10行。
**grouping_id()可以美化效果：**
select DECODE(GROUPING_ID(C1), 1, '合计', C1) D1,
DECODE(GROUPING_ID(C1, C2), 1, '小计', C2) D2,
DECODE(GROUPING_ID(C1, C2, C1 + C2), 1, '小计', C1 + C2) D3,
count(*),
GROUPING_ID(C1, C2, C1 + C2, C1 + 1, C2 + 1),
GROUPING_ID(C1)
from T2
group by rollup(C1, C2, C1 + C2, C1 + 1, C2 + 1);
===========================================================
**1.报表合计专用的Rollup函数**
销售报表
广州1月2000元
广州2月2500元
广州4500元
深圳1月1000元
深圳2月2000元
深圳3000元
所有地区7500元
以往的查询SQL:
Selectarea,month,sum(money) from SaleOrder group by area,month
然后广州，深圳的合计和所有地区合计都需要在程序里自行累计
1.其实可以使用如下SQL:
Select area,month,sum(total_sale) from SaleOrder group by rollup(area,month)
就能产生和报表一模一样的纪录
2.如果year不想累加，可以写成
Select year,month,area,sum(total_sale) from SaleOrder group by year, rollup(month,area)
另外Oracle 9i还支持如下语法:
Select year,month,area,sum(total_sale) from SaleOrder group by rollup((year,month),area)
3.如果使用Cube(area,month)而不是RollUp(area,month)，除了获得每个地区的合计之外，还将获得每个月份的合计，在报表最后显示。
4.Grouping让合计列更好读
RollUp在显示广州合计时，月份列为NULL，但更好的做法应该是显示为"所有月份"
Grouping就是用来判断当前Column是否是一个合计列，1为yes，然后用Decode把它转为"所有月份"
SelectDecode(Grouping(area),1,'所有地区',area) area,Decode(Grouping(month),1,'所有月份',month),sum(money)From SaleOrderGroup by RollUp(area,month);
**2.对多级层次查询的start with.....connect by**
比如人员组织,产品类别,Oracle提供了很经典的方法
SELECT LEVEL, name, emp_id,manager_emp_id FROM employee START WITH manager_emp_id is null CONNECT BY PRIOR emp_id = manager_emp_id;
上面的语句demo了全部的应用,start with指明从哪里开始遍历树,如果从根开始,那么它的manager应该是Null,如果从某个职员开始,可以写成emp_id='11'
CONNECT BY 就是指明父子关系,注意PRIOR位置
另外还有一个LEVEL列,显示节点的层次
**3.更多报表/分析决策功能**
3.1 分析功能的基本结构
分析功能() over( partion子句,order by子句,窗口子句)
概念上很难讲清楚,还是用例子说话比较好.
3.2 Row_Number 和 Rank, DENSE_Rank
用于选出Top 3 sales这样的报表
当两个业务员可能有相同业绩时,就要使用Rank和Dense_Rank
比如
金额RowNumRankDense_Rank
张三 4000元111
李四 3000元222
钱五 2000元333
孙六 2000元433
丁七 1000元554
这时,应该把并列第三的钱五和孙六都选进去,所以用Ranking功能比RowNumber保险.至于Desnse还是Ranking就看具体情况了。
SELECT salesperson_id, SUM(tot_sales) sp_sales,RANK( ) OVER (ORDER BY SUM(tot_sales) DESC) sales_rankFROM ordersGROUP BY salesperson_id
3.3 NTILE 把纪录平分成甲乙丙丁四等
比如我想取得前25%的纪录,或者把25%的纪录当作同一个level平等对待,把另25%当作另一个Level平等对待
SELECT cust_nbr, SUM(tot_sales) cust_sales,NTILE(4) OVER (ORDER BY SUM(tot_sales) DESC) sales_quartileFROM ordersGROUP BY cust_nbrORDER BY 3,2 DESC;
NTITLE(4)把纪录以 SUM(tot_sales)排序分成4份.
3.4 辅助分析列和Windows Function
报表除了基本事实数据外,总希望旁边多些全年总销量,到目前为止的累计销量,前后三个月的平均销量这样的列来参考.
这种前后三个月的平均和到目前为止的累计销量就叫windows function, 见下例
SELECT month, SUM(tot_sales) monthly_sales,SUM(SUM(tot_sales)) OVER (ORDER BY monthROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) max_preceedingFROM ordersGROUP BY monthORDER BY month;
SELECT month, SUM(tot_sales) monthly_sales,AVG(SUM(tot_sales)) OVER (ORDER BY monthROWS BETWEEN 1 PRECEDING AND 1 FOLLOWING) rolling_avgFROM ordersGROUP BY monthORDER BY month;
Windows Function的关键就是Windows子句的几个取值
1 PRECEDING 之前的一条记录
1 FOLLOWING 之后的一条记录
UNBOUNDED PRECEDING 之前的所有记录
CURRENT ROW 当前纪录
**4.SubQuery总结**
SubQuery天天用了,理论上总结一下.SubQuery 分三种
1.Noncorrelated 子查询最普通的样式.
2.Correlated Subqueries把父查询的列拉到子查询里面去,头一回cyt教我的时候理解了半天.
3.Inline View也被当成最普通的样式用了.
然后Noncorrelated 子查询又有三种情况
1.返回一行一列where price < (select max(price) from goods )
2.返回多行一列where price>= ALL (select price from goods where type=2)
or where NOT price< ANY(select price from goods where type=2)
最常用的IN其实就是=ANY()
3.返回多行多列一次返回多列当然就节省了查询时间
UPDATE monthly_ordersSET (tot_orders, max_order_amt) =(SELECT COUNT(*), MAX(sale_price)FROM cust_order)DELETE FROM line_itemWHERE (order_nbr, part_nbr) IN(SELECT order_nbr, part_nbr FROM cust_order c)
========================================
/*--------理解grouping sets
select a, b, c, sum( d ) from t
group by grouping sets ( a, b, c )
等效于
select * from (
select a, null, null, sum( d ) from t group by a
union all
select null, b, null, sum( d ) from t group by b 
union all
select null, null, c, sum( d ) from t group by c 
)
*/
引用：[http://blog.chinaunix.net/u2/70515/showart_1161137.html](http://blog.chinaunix.net/u2/70515/showart_1161137.html)
