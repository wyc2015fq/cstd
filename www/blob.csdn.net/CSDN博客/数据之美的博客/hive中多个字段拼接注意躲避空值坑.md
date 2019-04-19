# hive中多个字段拼接注意躲避空值坑 - 数据之美的博客 - CSDN博客
2019年03月14日 15:08:13[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：33
个人分类：[hive/sql](https://blog.csdn.net/oppo62258801/article/category/6961227)
select concat(leix01,leix02,leix03) from dim_ivr_dictionary where ivr_table like 'zj%' and bm='40102'；
select concat(nvl(leix01,''),nvl(leix02,''),nvl(leix03,'')) from dim_ivr_dictionary where ivr_table like 'zj%' and bm='40102'
如果三个字段都不为空，可以直接使用第一个语句获取拼接字段
若三个字段中包含空值，则使用第一个语句获取的拼接字段为空值，使用第二个语句才可获取正确拼接字段。
切记拼接字段的时候使用nvl预防空值
--------------------- 
作者：Sunshine~L&H 
来源：CSDN 
原文：https://blog.csdn.net/LH0912666/article/details/80427623 
版权声明：本文为博主原创文章，转载请附上博文链接！
