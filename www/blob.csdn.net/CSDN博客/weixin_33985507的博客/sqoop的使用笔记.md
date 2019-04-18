# sqoop的使用笔记 - weixin_33985507的博客 - CSDN博客
2017年11月22日 20:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 一、将数据从mysql导入到hdfs上。
`sqoop import /`
`--connect jdbc:mysql://10.110.22.238/test_sqoop / # mysql数据库所在得到节点ip及数据库名`
`--username root / # 数据库用户名`
`--password bigdata / # 数据库密码`
`--table books / # 所要导出数据的表名`
`--target-dir /user/root/user_data / # 指定导入到hdfs上的哪个路径，sqoop自动创建此目录`
`--columns "stud_id,stud_name" / # 抽取数据表中的指定列`
`--m 1`
![6070341-ba38a27b2f7e83ca.png](https://upload-images.jianshu.io/upload_images/6070341-ba38a27b2f7e83ca.png)
命令
- 结果出现错误：
ERROR tool.ImportTool: Error during import: No primary key could be found for table books. Please specify one with --split-by or perform a sequential import with '-m 1'.
![6070341-21c52359cc7cdb93.png](https://upload-images.jianshu.io/upload_images/6070341-21c52359cc7cdb93.png)
错误
- 结果还是有错误：
ERROR tool.ImportTool: Encountered IOException running import job: org.apache.hadoop.security.AccessControlException: Permission denied: user=root, access=WRITE, inode="/user/root/.staging":hdfs:hdfs:drwxr-xr-x
> 
原来是在hdfs下没有/user/root这个目录，自己新建目录，并且必须将此目录的所属用户和用户组设为root:root。
`hadoop fs -mkdir /user/root`
`hadoop fs -chown -R root:root /user/root`
修改完后在执行即可成功。
![6070341-5443d549513d7302.png](https://upload-images.jianshu.io/upload_images/6070341-5443d549513d7302.png)
导入到hdfs上的数据
![6070341-d74d827b35e62f06.png](https://upload-images.jianshu.io/upload_images/6070341-d74d827b35e62f06.png)
mysql中的数据
# 二、将mysql中的数据导入到Hive中
