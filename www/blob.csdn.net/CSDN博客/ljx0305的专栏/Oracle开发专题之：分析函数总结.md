# Oracle开发专题之：分析函数总结 - ljx0305的专栏 - CSDN博客
2008年07月15日 16:02:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：544标签：[oracle																[报表](https://so.csdn.net/so/search/s.do?q=报表&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)
这一篇是对前面所有关于分析函数的文章的总结：
**一、统计方面：**
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Sum(![](http://www.blogjava.net/Images/dot.gif)) Over ([Partition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)])
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Sum(![](http://www.blogjava.net/Images/dot.gif)) Over ([Partition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)]
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)     Rows Between![](http://www.blogjava.net/Images/dot.gif) Preceding And![](http://www.blogjava.net/Images/dot.gif) Following)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Sum(![](http://www.blogjava.net/Images/dot.gif)) Over ([Partition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)]
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)     Rows Between![](http://www.blogjava.net/Images/dot.gif) Preceding AndCurrent Row)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Sum(![](http://www.blogjava.net/Images/dot.gif)) Over ([Partition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)]
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)     Range Between Interval '![](http://www.blogjava.net/Images/dot.gif)''Day' Preceding
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)And Interval '![](http://www.blogjava.net/Images/dot.gif)''Day' Following )
具体请参考《[Oracle开发专题之：分析函数(OVER)](http://www.blogjava.net/pengpenglin/archive/2008/06/25/210536.html)》和《[Oracle开发专题之：窗口函数](http://www.blogjava.net/pengpenglin/archive/2008/06/28/211334.html)》
**二、排列方面：**
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Rank() Over ([Partition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)][Nulls First/Last])
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
  Dense_rank() Over ([Patition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)][Nulls First/Last])
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Row_number() Over ([Partitionby ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)][Nulls First/Last])
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Ntile(![](http://www.blogjava.net/Images/dot.gif)) Over ([Partition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)])
具体请参考《[Oracle开发专题之：分析函数2](http://www.blogjava.net/pengpenglin/archive/2008/06/26/210839.html)》
**三、最大值/最小值查找方面：**
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Min(![](http://www.blogjava.net/Images/dot.gif))/Max(![](http://www.blogjava.net/Images/dot.gif)) Keep (Dense_rank First/Last [Partition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)])
具体请参考《[Oracle开发专题之：分析函数3](http://www.blogjava.net/pengpenglin/archive/2008/06/27/211019.html)》
**四、首记录/末记录查找方面：**
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)First_value / Last_value(Sum(![](http://www.blogjava.net/Images/dot.gif)) Over ([Patition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)]
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)       Rows Between![](http://www.blogjava.net/Images/dot.gif) Preceding And![](http://www.blogjava.net/Images/dot.gif) Following  ))
具体请参考《[Oracle开发专题之：窗口函数](http://www.blogjava.net/pengpenglin/archive/2008/06/28/211334.html#part5)》
**五、相邻记录之间比较方面：**
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Lag(Sum(![](http://www.blogjava.net/Images/dot.gif)), 1) Over([Patition by ![](http://www.blogjava.net/Images/dot.gif)][Order by ![](http://www.blogjava.net/Images/dot.gif)])
具体请参考《[Oracle开发专题之：报表函数](http://www.blogjava.net/pengpenglin/archive/2008/06/29/211462.html#part2)》
引用于:http://www.blogjava.net/pengpenglin/archive/2008/06/29/211491.html
