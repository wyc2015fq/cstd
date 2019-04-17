# Linux用户和HDFS和Hive权限关系 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年04月28日 17:51:16[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6425
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
1、Linux用户组和用户新建

   1)用户组

     新建：$groupadd hphs

     删除命令：groupdel;

     属性修改：groupmod;

   2)用户：

     新建：#useradd -d /usr/hphs/ -m hphs -g hphs

     删除命令：userdel; 带-r表示删除用户相关文件，带-f 表示强制删除（即便用户仍有登录的）

     修改账号：usermod;

     密码修改：passwd;

   3)用户组和用户查看：$cat /etc/passwd



2、赋予HDFS目录权限：

   1)新建hdfs目录：$hadoop fs -mkdir /user/hphs

   2)赋予hphs权限：$hadoop fs -chown -R hphs:hphs /user/hphs

   HDFS权限管理用户指南参考：

   http://hadoop.apache.org/docs/r1.0.4/cn/hdfs_permissions_guide.html



3、Hive权限

   1)用户级权限：对于操作系统上用户进入Hive服务的权限控制，和HDFS权限控制一致，都是ACL支撑。


```
<property>  
  <name>hive.metastore.execute.setugi</name>  
  <value>true</value>  
  <description>In unsecure mode, setting this property to true will cause the metastore to execute DFS operations using the client's reported user and group permissions. Note that this property must be set on both the client and server sides. Further note that its best effort. If client sets its to true and server sets it to false, client setting will be ignored.简单说就是，hadoop在非安全模式（未使用kerborers认证）时，使用hive客户端进程对应的用户和组权限操作hdfs</description>  
</property>
```



   2)SQL操作权限：Hive数据库自身角色的权限管理，不同数据库和表可赋予具体权限的操作






```
<property>   
   <name>hive.security.authorization.enabled</name>   
   <value>true</value>   
   <description>enable or disable the hive client authorization.开启权限验证</description>   
</property>   
<property>   
   <name>hive.security.authorization.createtable.owner.grants</name>   
   <value>ALL</value>   
   <description>the privileges automatically granted to the owner whenever a table gets created. An example like "select,drop" will grant select and drop privilege to the owner of the table.表的创建者对表拥有所有权限.  
   </description>  
</property>
```





实际上生产遇到的问题是：

1）新建一个linux用户和用户组，Hadoop上如何设置其权限，一个是是否允许执行Hadoop命令；另一个是控制具体HDFS路径；这个应该就是ACL控制了；

2）新建一个linux用户和用户组，Hive上如何设置其权限，一个是否允许自行hive命令；另一个是控制具体hive文件路径（这点应该和Hadoop一致）；再一个就是具体库和表的使用权限；

决定了如何有效管理多用户使用hadoop和Hive（再同一客户端），尤其是hadoop集群设置kerberos安全认证下，有待继续研究。

此前研究的成果还有待继续优化：

http://blog.csdn.net/fjssharpsword/article/details/51280335


http://blog.csdn.net/fjssharpsword/article/details/51312153




