# 从 "org.apache.hadoop.security.AccessControlException:Permission denied: user=..." 看Hadoop 的用户登陆认证 - 我相信...... - CSDN博客





2016年04月19日 20:36:47[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：18048








如果远程提交任务给Hadoop 可能会遇到 "org.apache.hadoop.security.AccessControlException:Permission denied: user=..." ， 当然，如果是spark over YARN， 也同样会遇到类似的问题，例如：



 An error occurred while calling None.org.apache.spark.api.java.JavaSparkContext.

: org.apache.hadoop.security.AccessControlException: Permission denied: user=abel, access=WRITE, inode="/user/abel/.sparkStaging/application_1460633311001_0032":hdfs:hdfs:drwxr-xr-x




hadoop 的用户鉴权是基于JAAS的，其中hadoop.security.authentication属性 有simple 和kerberos 等方式。如果hadoop.security.authentication等于”kerberos”,那么是“hadoop-user-kerberos”或者“hadoop-keytab-kerberos”，否则是“hadoop-simple”。 当用户登陆的时候，若org.apache.hadoop.security.User为空，那么说明尚未登录过，调用静态方法getLoginUser()创建org.apache.hadoop.security.UserGroupInformatio实例，在getLoginUser（）中又会调用HadoopLoginModule的login()和commit()方法。




在使用了kerberos的情况下，从javax.security.auth.kerberos.KerberosPrincipal的实例获取username。在没有使用kerberos时，首先读取hadoop 的系统环境变量，如果没有的话，对于windows 从com.sun.security.auth.NTUserPrincipal 获取用户名，对于类unix 从com.sun.security.auth.UnixPrincipal 中获得用户名，然后再看该用户属于哪个group，从而完成登陆认证。





基本理解了问题的根源，那么这个“org.apache.hadoop.security.AccessControlException:Permission denied: user=...”异常信息是怎么产生的呢？远程提交，如果没有hadoop 的系统环境变量，就会读取当前主机的用户名，结果Hadoop集群中没有该用户，所以杯具了。




至于问题的解决，以mac 为例， 将

export HADOOP_USER_NAME ＝ hdfs 

添加到 ~/.bash_profile 中，

 $ source  ~/.bash_profile 




接下来，继续提交任务到 hadoop 集群，everything is OK。 






