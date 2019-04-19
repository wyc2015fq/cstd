# mysql 主键重复问题解决 Duplicate entry '787192513' for key 'PRIMARY' - fanyun的博客 - CSDN博客
2016年03月18日 13:53:13[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：16974标签：[mysql](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Mysql](https://blog.csdn.net/fanyun_01/article/category/6140890)
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)

   Mysql导入大量数据时，会报如下错误，主键重复，不能再继续执行。
   Query:
INSERT INTO `FNDCN_MNG` VALUES(787390578, 1, '杨明', 95585, '2015-06-08', '招募说明书', 80100, NULL, '1', NULL, '3', '2015-06-29', NULL, '1', '    杨明先生，中央财经大学硕士研究生，14年银行、基金从业经历。曾在上海银行从事信贷员、交易员及风险管理工作。2004年10月加入华安基金管理有限公司，任研究发展部研究员。2013年6月起担任华安策略优选股票型基金的基金经理。2014年6月起担任投资研究部高级总监。',
 '2015-11-09 12:33:58', '2016-01-06 21:34:41', '2015-12-31 12:14:18', 'JY', '488969752640', NULL, NULL, NULL, NULL)
Error occured at:2016-02-16 15:23:41
Line no.:220
Error Code: 1062 - Duplicate entry '95585-80100-2015-06-29 00:00:00-1--1' for key 'IDX_FNDCN_MNG'
解决方案一：
   将主键设置为自动增长。可以在数据库手动将相关表设置ID为自增长。
   如图：![](https://img-blog.csdn.net/20160216174102968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   再继续导入正常。
解决方案二：
  问题描述：单独导入出错sql,会报如下错误：
1 queries executed, 0 success, 1 errors, 0 warnings
查询：INSERT INTO `FNDCN_MNG` VALUES(787192513, 1, '丁进', 150150, '2015-12-28', '招募说明书', 80100, NULL, '1', NULL, '3', '20...
错误代码： 1062
Duplicate entry '787192513' for key 'PRIMARY'
   检查了下，主键生成策略是：@GeneratedValue(strategy=GenerationType.IDENTITY)，也没问题。
   删除导入出错的表；
   新建需要导入数据的表；
我的问题解决是在数据库中：在数据库中，没有将主键设为自动增长。将已经存在的表的主键设为自动增长SQL语句为：
**alter table course change course_id   course_id   int(10)   not null   auto_increment ; **
   然后用sql脚本的方式导入所需数据，这样也可以很容易定位错误。

