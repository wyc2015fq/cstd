# ORACLE进阶之三：分析函数 - ljx0305的专栏 - CSDN博客
2008年10月08日 14:02:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：642标签：[oracle																[function																[sql																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)
有时候我们需要从DB中提取一些很复杂的数据，而标准SQL却对此无能为力，或者是执行效率非常的低；比如我们需要提取如下数据：
    逐行显示各个部门的累计工资，每行包括部门内前面所有人的工资总和；
    查找各个部门工资最高的前N个人；
    ……
**语法****Function名称([参数]) OVER ([partition 子句][ order 子句] [window 子句])**  OVER为分析函数的关键字，用于区别普通的聚合函数；从语法格式上区分的话，没加over()即时聚合函数，加了over()就是分析函数。
**Partition 子句：Partition by exp1[ ,exp2]...；**  主要用于分组，可以理解成select中的group by；不过它跟select语句后跟的group by 子句并不冲突；指定该子句之后，前面的函数起效范围就是该分组内，若不指定，则Function的起效范围是全部结果集。
**Order 子句：Order by exp1[asc|desc] [ ,exp2 [asc|desc]]... [nulls first|last]；**  其参数基本与select中的order by相同；Nulls first|last是用来限定nulls在分组序列中的所在位置的，我们知道oracle中对于null的定义是未知，所以默认order by的时候nulls总会被排在最前面。如果想控制值为null的行显示位置，nulls first|last参数就能派上用场了。
**Window 子句**：该子句的语法比较复杂，具体可以见下图；
![](https://p-blog.csdn.net/images/p_blog_csdn_net/sfdev/windowClause.jpg)
  该子句给出了一个定义变化或者固定的数据窗口方法，分析函数将对这些数据进行操作；默认情况下，一般用不上该子句，分析函数产生一个固定的窗口，影响的数据范围是从第一行到当前行，其效果和RANGE BETWEEN UNDOUNDED PRECEDING AND CURRENT ROW一样；若需要指定操作数据为当前行及其前两行，则可以用ROWS 2 PRECEDING来实现其效果；
 其中用[]标注的子句都可以为空，一个最简单的分析函数可能是COUNT(*) OVER ()；
**样例**
逐行显示各个部门的累计工资，每行包括部门内前面所有人的工资总和：
SELECT EMP_NO,
       NAME,
       DEPT_NO,
       SUM(SAL) OVER(PARTITION BY DEPT_NO ORDER BY EMP_NO) DEPT_SAL_SUM
  FROM EMP
 ORDER BY DEPT_NO, EMP_NO;
查找各个部门工资最高的前N个人：
SELECT *
  FROM (SELECT DEPT_NO,
               NAME,
               SAL,
               DENSE_RANK() OVER(PARTITION BY DEPT_NO ORDER BY SAL DESC) DR
          FROM EMP)
 WHERE DR <= 3
 ORDER BY DEPT_NO, SAL DESC;
**注意点**
1、分析函数与聚合函数非常相似，不同于聚合函数的地方在于它们每个分组序列均返回多行，而聚合函数返回一行；
2、带有分析函数的SQL列表中，除了order by子句之外，分析函数将在SQL语句中最后执行；因此，分析函数只能用于select的列或order by子句，而不能用于where、group by、having之类的语句中；
3、当分析函数中使用了distinct参数时，则只能使用partition子句，而不能指定order by子句；
4、SELECT语句中的ORDER BY子句与分析函数中的order by子句是互不影响的，但一般来说两者一致比较好，若两者不一致，则意味着分析函数需要对结果集进行多次排序，这将严重降低分析函数的执行效率；
5、dense_rank在做排序时如果遇到列有重复值，则重复值所在行的序列值相同，而其后的序列值依旧递增，rank则是重复值所在行的序列值相同，但其后的序列值从+重复行数开始递增，而row_number则不管是否有重复行，序列值始终递增；
**函数列表**
大致有26个函数可用，其中很多都是和聚合函数同名的，比如SUM、AVG、MIN、MAX……；其他是一些提供新功能的新函数；具体的函数列表如下：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/sfdev/functionList.jpg)
引用于:http://blog.csdn.net/sfdev/archive/2008/02/23/2115244.aspx
