# 【Hive】 报错 FAILED: SemanticException Unable to determine if hdfs:// - Simple 专栏 - CSDN博客
2018年10月08日 23:39:03[Simple_Zz](https://me.csdn.net/love284969214)阅读数：338
```bash
Logging initialized using configuration in file:/soft/softwares/apache-hive-1.2.1-bin/conf/hive-log4j.properties
FAILED: SemanticException Unable to determine if
hdfs://localhost:8020/user/hive/warehouse/student is encrypted: 
java.lang.IllegalArgumentException: Wrong FS: 
hdfs://localhost:8020/user/hive/warehouse/student, expected: hdfs://192.168.25.121:8020
```
### 此错误原因：
mysql 中 **DBS **与 **SDS **两表中设置了 Hive的元数据信息  需要修改此表。
```bash
mysql> use metastore;
mysql> select * from DBS;
mysql> select LOCATION from SDS; 
+-------+-----------------------+-------------------------------------------+---------+------------+------------+
| DB_ID | DESC                  | DB_LOCATION_URI                           | NAME    | OWNER_NAME | OWNER_TYPE |
+-------+-----------------------+-------------------------------------------+---------+------------+------------+
|     1 | Default Hive database | hdfs://localhost:8020/user/hive/warehouse | default | public     | ROLE       |
+-------+-----------------------+-------------------------------------------+---------+------------+------------+
+---------------------------------------------------+
| LOCATION                                          |
+---------------------------------------------------+
| hdfs://localhost:8020/user/hive/warehouse/student |
+---------------------------------------------------+
```
- 
### 这里 DBS 表 DB_LOCATION_URI  需要修改。 
- 
### SDS 表 LOCATION 需要修改。
### update 表名 set 字段名=REPLACE (字段名,'原来的值','要修改的值')
###  实例：
```
update DBS set DB_LOCATION_URI=REPLACE (DB_LOCATION_URI,'hdfs://localhost:8020/user/hive/warehouse','hdfs://192.168.25.121:8020/user/hive/warehouse');
update SDS set LOCATION=REPLACE (LOCATION,'hdfs://localhost:8020/user/hive/warehouse','hdfs://192.168.25.121:8020/user/hive/warehouse');
```
### 修改后查看mysql是否自动commit
```bash
mysql> show variables like 'autocommit';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit    | ON    |
+---------------+-------+
```
