# (转载)详解Hive配置Kerberos认证 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月11日 16:59:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：8205
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)










Hive提供了运行SQL语句查询存储在HDFS上数据的能力，Hive提供的查询引擎，可以将SQL语句转化成MapReduce任务，提交到Hadoop集群上执行。MapReduce任务运行的结果会存在HDFS上。下面的图表示了一个用户运行Hive查询的Hadoop内部交互。

![a_1.png](http://www.cognoschina.net/home/attachment/201504/16/58961_1429166379tBW9.png)



有多种和Hive交互的方法，最常用的是CLI，不过，CLI的设计使其不便于通过编程的方式进行访问。还有可以使用HIveServer，用Java和其它语言和Hive进行交互。


Hiveserver2支持Kerberos和LDAP的用户认证。推荐在生产环境中，我们设置Hiveserver2和Kerberos拥有用户提交Hive查询的seamless授权和访问控制。


设置安全的Hive我们使用下面的步骤：

![a_2.png](http://www.cognoschina.net/home/attachment/201504/16/58961_1429166392AuW0.png)



步骤描述为：1.在KDC中创建Hive的Principal；2.创建Hive的keytab文件；3.拷贝keytab文件到部署了HiveServer2的机器上；4.设置合适的keytab权限；5.更新Hive和Hadoop相应的设置。6.设置metastore安全性。




下面根据我实际安装的情况来详细描述相关步骤：


步骤一：在KDC中创建Hive的Principal
|Kadmin.local –q “addprinc –randkey hive/master5.hadoop@MYREALM.COM ”Kadmin.local –q “addprinc –randkey hive/slave6.hadoop@MYREALM.COM”Kadmin.local –q “addprinc –randkey hive/slave78.hadoop@MYREALM.COM”|
|----|




步骤二：创建Hive的keytab文件
|Kadmin.local –q “xst –norandkey –k hive.keytab hive/master5.hadoop@MYREALM.COM”Kadmin.local –q “xst –norandkey –k hive.keytab hive/slave6.hadoop@MYREALM.COM”Kadmin.local –q “xst –norandkey –k hive.keytab hive/slave78.hadoop@MYREALM.COM”|
|----|




步骤三：拷贝keytab文件到部署了HiveServer2的机器上；
|scp hive.keytab master5.hadoop:/etc/hive/confscp hive.keytab slave6.hadoop:/etc/hive/confscp hive.keytab slave78.hadoop:/etc/hive/conf|
|----|




步骤四：设置合适的keytab权限
|Chown hive:Hadoop hive.keytab;chmod 400 hive.keytab;|
|----|


注：由于 keytab 相当于有了永久凭证，不需要提供密码(如果修改 kdc 中的 principal 的密码，则该 keytab 就会失效)，所以其他用户如果对该文件有读权限，就可以冒充 keytab 中指定的用户身份访问 hadoop，所以 keytab 文件需要确保只对 owner 有读权限(0400)




步骤五、六：更新Hive和Hadoop相应的设置，设置metastore安全性


修改/etc/hive/conf/hive-site.xml，添加下面配置：
|<property>  <name>hive.server2.authentication</name>  <value>KERBEROS</value></property><property>  <name>hive.server2.authentication.kerberos.principal</name>  <value>hive/_HOST@MYREALM.COM</value></property><property>  <name>hive.server2.authentication.kerberos.keytab</name>  <value>/etc/hive/conf/hive.keytab</value></property><property>  <name>hive.metastore.sasl.enabled</name>  <value>true</value></property><property>  <name>hive.metastore.kerberos.keytab.file</name>  <value>/etc/hive/conf/hive.keytab</value></property><property>  <name>hive.metastore.kerberos.principal</name>  <value>hive/_HOST@MYREALM.COM</value></property>|
|----|


在 core-site.xml 中添加：
|<property>  <name>hadoop.proxyuser.hive.hosts</name>  <value>*</value></property><property>  <name>hadoop.proxyuser.hive.groups</name>  <value>*</value></property><property>  <name>hadoop.proxyuser.hdfs.hosts</name>  <value>*</value></property><property>  <name>hadoop.proxyuser.hdfs.groups</name>  <value>*</value></property><property>  <name>hadoop.proxyuser.HTTP.hosts</name>  <value>*</value></property><property>  <name>hadoop.proxyuser.HTTP.groups</name>  <value>*</value></property>|
|----|


将修改的上面文件同步到其他节点
|$ scp /etc/hive/conf/hive-site.xml slave6.hadoop:/etc/hive/conf/$ scp /etc/hive/conf/hive-site.xml master5.hadoop:/etc/hive/conf/|
|----|




启动服务
|service hive-metastore startservice hive-server2 start|
|----|


 转载网址：http://www.cognoschina.net/Article/120457



