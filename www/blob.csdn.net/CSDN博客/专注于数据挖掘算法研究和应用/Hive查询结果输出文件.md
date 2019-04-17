# Hive查询结果输出文件 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月05日 14:52:45[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5879
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









进入hive执行：

insert overwrite local directory '/tmp/ftp0803/' select * from tmp_ftp where userno='XX' order by starttime; 

在/tmp/ftp0803/目录下看到文件。




或者直接执行：

hive -e 'select f28 from tmp_fjs_dba' >> /tmp/dbasql.txt





具体分隔符等定义再参考相关命令。



