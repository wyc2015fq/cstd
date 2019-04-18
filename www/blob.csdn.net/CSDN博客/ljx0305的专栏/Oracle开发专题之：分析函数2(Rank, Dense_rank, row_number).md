# Oracle开发专题之：分析函数2(Rank, Dense_rank, row_number)  - ljx0305的专栏 - CSDN博客
2008年07月15日 15:19:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：538标签：[oracle																[测试																[sql](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)
**目录===============================================**[**1.使用rownum为记录排名**](http://www.blogjava.net/pengpenglin/archive/2008/06/26/210839.html#part1)
[**2.使用分析函数来为记录排名**](http://www.blogjava.net/pengpenglin/archive/2008/06/26/210839.html#part2)
[**3.使用分析函数为记录进行分组排名**](http://www.blogjava.net/pengpenglin/archive/2008/06/26/210839.html#part3)
**一、使用rownum为记录排名：**
在前面一篇《Oracle开发专题之：分析函数》，我们认识了分析函数的基本应用，现在我们再来考虑下面几个问题：
**①对所有客户按订单总额进行排名②按区域和客户订单总额进行排名③找出订单总额排名前13位的客户④找出订单总额最高、最低的客户⑤找出订单总额排名前25%的客户**
按照前面第一篇文章的思路，我们只能做到对各个分组的数据进行统计，如果需要排名的话那么只需要简单地加上rownum不就行了吗？事实情况是否如此想象般简单，我们来实践一下。
【1】测试环境：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>desc user_order;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) Name                                      Null?    Type
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)----------------------------------------- -------- ----------------------------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID                                          NUMBER(2)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) CUSTOMER_ID                                  NUMBER(2)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) CUSTOMER_SALES                          NUMBER
【2】测试数据：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select*from user_order orderby customer_sales;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID CUSTOMER_ID CUSTOMER_SALES
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------- --------------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)51151162
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1029903383
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)67971585
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)1028986964
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9211020541
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9221036146
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8161068467
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)681141638
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)531161286
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)551169926
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8191174421
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7121182275
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7111190421
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6101196748
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)691208959
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)10301216858
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)521224992
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9241224992
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9231224992
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8181253840
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7151255591
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7131310434
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)10271322747
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8201413722
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)661788836
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)10261808949
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)541878275
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)7141929774
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8171944281
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9252232703
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)30 rows selected.
注意这里有3条记录的订单总额是一样的。假如我们现在需要筛选排名前12位的客户，如果使用rownum会有什么样的后果呢？ 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select rownum, t.*
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2from (select*
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4orderby customer_sales desc) t
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5where rownum <=12
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6orderby customer_sales desc;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    ROWNUM  REGION_ID CUSTOMER_ID CUSTOMER_SALES
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ---------- ----------- --------------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)19252232703
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)28171944281
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)37141929774
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4541878275
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)510261808949
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6661788836
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)78201413722
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)810271322747
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)97131310434
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)107151255591
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)118181253840
12521224992
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)12 rows selected.
很明显假如只是简单地按rownum进行排序的话，我们漏掉了另外两条记录(参考上面的结果)。
**二、使用分析函数来为记录排名：**
针对上面的情况，Oracle从8i开始就提供了3个分析函数：rand，dense_rank，row_number来解决诸如此类的问题，下面我们来看看这3个分析函数的作用以及彼此之间的区别：
Rank，Dense_rank，Row_number函数为每条记录产生一个从1开始至N的自然数，N的值可能小于等于记录的总数。这3个函数的唯一区别在于当碰到相同数据时的排名策略。
**①ROW_NUMBER：**
Row_number函数返回一个唯一的值，当碰到相同数据时，排名按照记录集中记录的顺序依次递增。 
**②DENSE_RANK：**Dense_rank函数返回一个唯一的值，除非当碰到相同数据时，此时所有相同数据的排名都是一样的。 
**③RANK：**Rank函数返回一个唯一的值，除非遇到相同的数据时，此时所有相同数据的排名是一样的，同时会在最后一条相同记录和下一条不同记录的排名之间空出排名。
这样的介绍有点难懂，我们还是通过实例来说明吧，下面的例子演示了3个不同函数在遇到相同数据时不同排名策略：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select region_id, customer_id, sum(customer_sales) total,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2         rank() over(orderbysum(customer_sales) desc) rank,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3         dense_rank() over(orderbysum(customer_sales) desc) dense_rank,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4         row_number() over(orderbysum(customer_sales) desc) row_number
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6groupby region_id, customer_id;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID CUSTOMER_ID      TOTAL       RANK DENSE_RANK ROW_NUMBER
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------- ---------- ---------- ---------- ----------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)![](http://www.blogjava.net/Images/dot.gif)![](http://www.blogjava.net/Images/dot.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)8181253840111111
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)521224992121212
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9231224992121213
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)9241224992121214
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)10301216858151315
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)![](http://www.blogjava.net/Images/dot.gif)![](http://www.blogjava.net/Images/dot.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)30 rows selected.
请注意上面的绿色高亮部分，这里生动的演示了3种不同的排名策略：
①对于第一条相同的记录，3种函数的排名都是一样的：12
②当出现第二条相同的记录时，Rank和Dense_rank依然给出同样的排名12；而row_number则顺延递增为13，依次类推至第三条相同的记录
③当排名进行到下一条不同的记录时，可以看到Rank函数在12和15之间空出了13,14的排名，因为这2个排名实际上已经被第二、三条相同的记录占了。而Dense_rank则顺序递增。row_number函数也是顺序递增
比较上面3种不同的策略，我们在选择的时候就要根据客户的需求来定夺了：
**①假如客户就只需要指定数目的记录，那么采用row_number是最简单的，但有漏掉的记录的危险②假如客户需要所有达到排名水平的记录，那么采用rank或dense_rank是不错的选择。至于选择哪一种则看客户的需要，选择dense_rank或得到最大的记录**
**三、使用分析函数为记录进行分组排名：**
上面的排名是按订单总额来进行排列的，现在跟进一步：假如是为各个地区的订单总额进行排名呢？这意味着又多了一次分组操作：对记录按地区分组然后进行排名。幸亏Oracle也提供了这样的支持，我们所要做的仅仅是在over函数中order by的前面增加一个分组子句：partition by region_id。
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)SQL>select region_id, customer_id, 
sum(customer_sales) total,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)2         rank() over(partition by region_id
orderbysum(customer_sales) desc) rank,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)3         dense_rank() over(partition by region_id
orderbysum(customer_sales) desc) dense_rank,
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)4         row_number() over(partition by region_id
orderbysum(customer_sales) desc) row_number
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)5from user_order
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6groupby region_id, customer_id;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif) REGION_ID CUSTOMER_ID      TOTAL       RANK DENSE_RANK ROW_NUMBER
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)---------- ----------- ---------- ---------- ---------- ----------
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)541878275111
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)521224992222
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)551169926333
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)661788836111
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)691208959222
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)6101196748333
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)![](http://www.blogjava.net/Images/dot.gif)![](http://www.blogjava.net/Images/dot.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)30 rows selected.
现在我们看到的排名将是基于各个地区的，而非所有区域的了！Partition by 子句在排列函数中的作用是将一个结果集划分成几个部分，这样排列函数就能够应用于这各个子集。
前面我们提到的5个问题已经解决了2个了(第1,2)，剩下的3个问题(Top/Bottom N，First/Last, NTile)会在下一篇讲解。
参考资料：《Mastering Oracle SQL》(By [Alan Beaulieu](http://www.oreillynet.com/cs/catalog/view/au/789?x-t=book.view), [Sanjay Mishra](http://www.oreillynet.com/cs/catalog/view/au/607?x-t=book.view) O'Reilly June 2004  0-596-00632-2)
引用至:http://www.blogjava.net/pengpenglin/archive/2008/06/26/210839.html
