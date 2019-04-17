# Hive报错：Unable to instantiate org.apache.hadoop.hive.ql.metadata.SessionHiveMetaStoreClient - Machine Learning with Peppa - CSDN博客





2018年07月09日 17:09:09[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：149







`Unable to instantiate org.apache.hadoop.hive.ql.metadata.SessionHiveMetaStoreClient`
## 解决方法

1、Hive版本过高。第一次安装我用的是Hive的1.2.1版本，mysql是5.6.33，hadoop是2.6版本。但是安装之后按照相关步骤进行操作却报如上错误。后来经过不断的查资料发现有网友说有可能是Hive版本太高，所以下载了0.9版本（下载地址：[http://download.csdn.net/download/qq_16633405/10028786](http://download.csdn.net/download/qq_16633405/10028786)）又按照之前的步骤来了一遍就OK了。 

2.需要hive元数据库初始化，执行 schematool -dbType mysql -initSchema

3.HIVE_HOME/lib 下的derby-10.11.1.1.jar问题，，把derby-10.11.1.1.jar 换成derby-10.10.2.0.jar问题成功解决

4.可能相关的配置文件配置错误。 

```
解决办法：

　　进入所安装的Hive的conf目录，找到hive-site.xml，（若没修改，则是hive-default.xml.template）。

<property>

  <name>hive.metastore.schema.verification</name>

  <value>true</value>

   <description>

   Enforce metastore schema version consistency.

   True: Verify that version information stored in metastore matches with one from Hive jars.  Also disable automatic

         schema migration attempt. Users are required to manully migrate schema after Hive upgrade which ensures

         proper metastore schema migration. (Default)

   False: Warn if the version information stored in metastore doesn't match with one from in Hive jars.

   </description>

</property>

 

改为

<property>

  <name>hive.metastore.schema.verification</name>

  <value>false</value>

   <description>

   Enforce metastore schema version consistency.

   True: Verify that version information stored in metastore matches with one from Hive jars.  Also disable automatic

         schema migration attempt. Users are required to manully migrate schema after Hive upgrade which ensures

         proper metastore schema migration. (Default)

   False: Warn if the version information stored in metastore doesn't match with one from in Hive jars.

   </description>

</property>
```


或者  在hive安装目录下的bin目录下，./hive -hiveconf hive.root.logger=DEBUG,console





