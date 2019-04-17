# Hive自定义UDF的JAR包加入运行环境的方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年04月20日 16:55:47[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：11289
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
Hive开发udf函数打包jar文件后，需将jar文件放入hive的运行环境，方法有三。



先将http://blog.csdn.net/fjssharpsword/article/details/70265554中重定义的两个类打包成DefTextInputFormat.jar，并放到/home/hdfs目录下。



1、方法一：使用add jar命令




   1）在hive下执行:add jar /home/hdfs/DefTextInputFormat.jar;

   Added [/home/hdfs/DefTextInputFormat.jar] to class path

   Added resources: [/home/hdfs/DefTextInputFormat.jar]

   该方法每次启动Hive的时候都要从新加入，退出hive就会失效。




   2）验证，建表语句如下：

```
ROW FORMAT DELIMITED 
  FIELDS TERMINATED BY '#' 
STORED AS INPUTFORMAT 
  'com.hive.DefTextInputFormat' 
OUTPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat'
LOCATION
  'hdfs://nameservice-ha/pgw/gz';
```



   查询后文件和表字段一致。



2、hive-site.xml文件配置hive.aux.jars.path：




   配置参考如下：

   <property>

       <name>hive.aux.jars.path</name>

       <value>file:///home/hdfs/fangjs/DefTextInputFormat.jar,file:///jarpath/test.jar</value>

   </property>



   该方法不需要每次启动Hive执行命令加入，需要配置文件。



3、根目录${HIVE_HOME}下创建文件夹auxlib，然后将自定义jar文件放入该文件夹中；



   该方法方便快捷，不过对于客户端操作环境就不能执行。
            


