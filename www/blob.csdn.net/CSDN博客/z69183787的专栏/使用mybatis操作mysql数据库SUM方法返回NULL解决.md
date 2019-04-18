# 使用mybatis操作mysql数据库SUM方法返回NULL解决 - z69183787的专栏 - CSDN博客
2016年06月27日 12:56:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3686
使用SQL语句用函数SUM叠加的时候，默认查询没有值的情况下返回的是ＮＵＬＬ，而实际可能我们要用的是返回0
解决：
  SELECT SUM(total)   FROM test_table
改成：
 SELECT COALESCE(SUM(total),0)  FROM test_table
解释：
coalesce()解释：返回参数中的第一个非空表达式（从左向右）
