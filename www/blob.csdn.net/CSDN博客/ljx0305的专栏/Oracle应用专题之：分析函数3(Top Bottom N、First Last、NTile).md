# Oracle应用专题之：分析函数3(Top/Bottom N、First/Last、NTile)  - ljx0305的专栏 - CSDN博客
2008年07月15日 15:49:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：709
**目录===============================================**
[**1.带空值的排列**](http://www.blogjava.net/pengpenglin/archive/2008/06/27/211019.html#part1)
[**2.Top/Bottom N查询**](http://www.blogjava.net/pengpenglin/archive/2008/06/27/211019.html#part2)
[**3.First/Last排名查询**](http://www.blogjava.net/pengpenglin/archive/2008/06/27/211019.html#part3)
[**4.按层次查询**](http://www.blogjava.net/pengpenglin/archive/2008/06/27/211019.html#part4)
**一、带空值的排列：**
在前面《[Oracle开发专题之：分析函数2(Rank、Dense_rank、row_number)](http://www.blogjava.net/pengpenglin/archive/2008/06/26/210839.html)》一文中，我们已经知道了如何为一批记录进行全排列、分组排列。假如被排列的数据中含有空值呢？
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select region_id, customer_id,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2sum(customer_sales) cust_sales,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3sum(sum(customer_sales)) over(partition by region_id) ran_total,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4         rank() over(partition by region_id
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5orderbysum(customer_sales) desc) rank
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7groupby region_id, customer_id;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID CUSTOMER_ID CUST_SALES  RAN_TOTAL       RANK
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------- ---------- ---------- ---------- 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)103162389011
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1026180894962389012
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1027132274762389013
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1030121685862389014
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)102898696462389015
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)102990338362389016
我们看到这里有一条记录的CUST_TOTAL字段值为NULL，但居然排在第一名了！显然这不符合情理。所以我们重新调整完善一下我们的排名策略，看看下面的语句：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select region_id, customer_id,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2sum(customer_sales) cust_total,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3sum(sum(customer_sales)) over(partition by region_id) reg_total,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4         rank() over(partition by region_id 
orderbysum(customer_sales) desc**NULLS LAST**) rank
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6groupby region_id, customer_id;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID CUSTOMER_ID CUST_TOTAL  REG_TOTAL       RANK
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------- ---------- ---------- ----------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1026180894962389011
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1027132274762389012
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1030121685862389013
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)102898696462389014
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)102990338362389015
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)103162389016
绿色高亮处，NULLS LAST/FIRST告诉Oracle让空值排名最后后第一。
**注意是NULLS，不是NULL。**
**二、Top/Bottom N查询：**
在日常的工作生产中，我们经常碰到这样的查询：找出排名前5位的订单客户、找出排名前10位的销售人员等等。现在这个对我们来说已经是很简单的问题了。下面我们用一个实际的例子来演示：
【1】找出所有订单总额排名前3的大客户：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select*
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>from (select region_id,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>                customer_id,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>sum(customer_sales) cust_total,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL> rank() over(orderbysum(customer_sales) desc NULLS LAST) rank
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>groupby region_id, customer_id)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>where rank <=3;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID CUSTOMER_ID CUST_TOTAL       RANK
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------- ---------- ----------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)92522327031
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)81719442812
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)71419297743
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>
【2】找出每个区域订单总额排名前3的大客户：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select*
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2from (select region_id,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3                 customer_id,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4sum(customer_sales) cust_total,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5sum(sum(customer_sales)) over(partition by region_id) reg_total,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6rank() over(partition by region_id
orderbysum(customer_sales) desc NULLS LAST) rank
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8groupby region_id, customer_id)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9where rank <=3;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID CUSTOMER_ID CUST_TOTAL  REG_TOTAL       RANK
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------- ---------- ---------- ----------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)54187827555856411
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)52122499255856412
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)55116992655856413
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)66178883663077661
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)69120895963077662
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)610119674863077663
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)714192977468684951
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)713131043468684952
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)715125559168684953
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)817194428168547311
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)820141372268547312
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)818125384068547313
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)925223270367393741
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)923122499267393742
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)924122499267393742
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1026180894962389011
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1027132274762389012
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1030121685862389013
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)18 rows selected.
**三、First/Last排名查询：**
想象一下下面的情形：找出订单总额最多、最少的客户。按照前面我们学到的知识，这个至少需要2个查询。第一个查询按照订单总额降序排列以期拿到第一名，第二个查询按照订单总额升序排列以期拿到最后一名。是不是很烦？因为Rank函数只告诉我们排名的结果，却无法自动替我们从中筛选结果。
幸好Oracle为我们在排列函数之外提供了两个额外的函数：first、last函数，专门用来解决这种问题。还是用实例说话： 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>selectmin(customer_id)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2         keep (dense_rank first orderbysum(customer_sales) desc) first,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3min(customer_id)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4keep (dense_rank last orderbysum(customer_sales) desc) last
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6groupby customer_id;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)     FIRST       LAST
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)311
这里有几个看起来比较疑惑的地方：
**①为什么这里要用min函数②Keep这个东西是干什么的③fist/last是干什么的④dense_rank和dense_rank()有什么不同，能换成rank吗？**
首先解答一下第一个问题：min函数的作用是用于当存在多个First/Last情况下保证返回唯一的记录。假如我们去掉会有什么样的后果呢？ 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select keep (dense_rank first orderbysum(customer_sales) desc) first, 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2             keep (dense_rank last orderbysum(customer_sales) desc) last
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4groupby customer_id;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)select keep (dense_rank first orderbysum(customer_sales) desc) first,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)*
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)ERROR at line 1:
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)ORA-00907: missing right parenthesis
接下来看看第2个问题：keep是干什么用的？从上面的结果我们已经知道Oracle对排名的结果只“保留”2条数据，这就是keep的作用。告诉Oracle只保留符合keep条件的记录。
那么什么才是符合条件的记录呢？这就是第3个问题了。dense_rank是告诉Oracle排列的策略，first/last则告诉最终筛选的条件。
第4个问题：如果我们把dense_rank换成rank呢？ 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>selectmin(region_id)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2          keep(rank first orderbysum(customer_sales) desc) first,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3min(region_id)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4          keep(rank last orderbysum(customer_sales) desc) last
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6groupby region_id;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)selectmin(region_id)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)*
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)ERROR at line 1:
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)ORA-02000: missing DENSE_RANK
**四、按层次查询：**
现在我们已经见识了如何通过Oracle的分析函数来获取Top/Bottom N，第一个，最后一个记录。有时我们会收到类似下面这样的需求：找出订单总额排名前1/5的客户。
很熟悉是不？我们马上会想到第二点中提到的方法，可是rank函数只为我们做好了排名，并不知道每个排名在总排名中的相对位置，这时候就引入了另外一个分析函数NTile，下面我们就以上面的需求为例来讲解一下：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select region_id,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2         customer_id,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3ntile(5) over(orderbysum(customer_sales) desc) til
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5groupby region_id, customer_id;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID CUSTOMER_ID       TILE
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------- ----------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)10311
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9251
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)10261
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)661
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8182
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)522
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9233
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)693
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7113
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)534
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)684
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8164
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)675
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)10295
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)515
Ntil函数为各个记录在记录集中的排名计算比例，我们看到所有的记录被分成5个等级，那么假如我们只需要前1/5的记录则只需要截取TILE的值为1的记录就可以了。假如我们需要排名前25%的记录(也就是1/4)那么我们只需要设置ntile(4)就可以了。
参考资料：《Mastering Oracle SQL》(By [Alan Beaulieu](http://www.oreillynet.com/cs/catalog/view/au/789?x-t=book.view), [Sanjay Mishra](http://www.oreillynet.com/cs/catalog/view/au/607?x-t=book.view) O'Reilly June 2004  0-596-00632-2)
引用至:http://www.blogjava.net/pengpenglin/archive/2008/06/27/211019.html
