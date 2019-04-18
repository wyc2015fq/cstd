# 【数据平台】sqoop支持hive和mysql互导的基本语句格式 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月25日 15:05:46[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：315
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)









1)从mysql导入到hive的命令格式：


`sqoop import --connect jdbc:mysql://127.0.0.1:3306/dbname --username mysql(mysql用户名) --password 123456(密码) --table student(mysql上的表) --hive-import -m 1`

导入到hive默认的default库，指定数据库可以设置--hive-database 参数






2)从hive导入到mysql的命令格式：



全部导入：


```
Sqoop export --connect  jdbc:mysql://127.0.0.1:3306/dbname  --username mysql(mysql用户名) --password 123456(密码) --table  student(mysql上的表) --hcatalog-database sopdm(hive上的schema) --hcatalog-table student(hive上的表)
```






部分导入，指定导入的具体列名：
`Sqoop export --connect  jdbc:mysql://127.0.0.1:3306/dbname  --username mysql(mysql用户名) --password 123456(密码) --table  student(mysql上的表) --columns "id,name,age"  --hcatalog-database sopdm(hive上的schema) --hcatalog-table student(hive上的表)`

3）分享一个案例，花了一天时间摸索的，比较坑爹，希望对后来者有参考作用

第一步：在mysql建表，字段中包含：



```
`from` varchar(255),
  `target` varchar(255),
  `click_event` varchar(255),
  `sessionid` varchar(255),
```


第二：在集群上执行sqoop


`sqoop export --connect "jdbc:mysql://127.0.0.1:3306/dbname?useUnicode=true&characterEncoding=utf-8"  --username mysql用户名 --password mysql密码 --table mysql表  -fields-terminated-by '\001'  --input-null-string "\\\\N" --input-null-non-string "\\\\N" --export-dir /user/hive/warehouse/db/hive表hdfs路径 --update-key resourceid --update-mode allowinsert`

然后一直提示：


`com.mysql.jdbc.exceptions.jdbc4.MySQLSyntaxErrorException: You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near 'from, target, click_event' at line 1`

然后一直不断建表（核对字段、分隔符、数据类型等），研究sqoop的export参数设置，想想各种空值处理，一直不成功。




第三步：终极方案，想当郁闷，from字段改名即可，如下：



```
`upfrom` varchar(255),
  `target` varchar(255),
  `click_event` varchar(255),
  `sessionid` varchar(255),
```







