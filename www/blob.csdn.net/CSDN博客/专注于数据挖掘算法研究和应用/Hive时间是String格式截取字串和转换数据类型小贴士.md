# Hive时间是String格式截取字串和转换数据类型小贴士 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月31日 15:55:38[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5467
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









导入hive的时间格式是：2016/8/23 00:00:00




无法直接to_date，只能截取具体子串substr在转换类型到int比较大小。




select count(*) from tmp_fjs_dba where substr(f4,1,9)='2016/8/23' and f28 like 'update%' and int(substr(f4,11,2))>12;
            


