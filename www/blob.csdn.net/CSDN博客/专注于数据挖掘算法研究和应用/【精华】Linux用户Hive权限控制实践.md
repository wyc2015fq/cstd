# 【精华】Linux用户Hive权限控制实践 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月11日 16:19:18[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2517
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
1、困惑：

   hadoop和hive通过客户机接入到集群生产，客户机是linux系统，那么linux用户和hive角色之间是什么关系呢？

   或者说，怎么控制linux系统用户可以细粒度访问hive的数据库和表。




2、新建linux用户和用户组

   1）#groupadd hphs;

   2）#useradd -d /home/hphs/ -m hphs -g hphs -G hadoop

     指定用户所属的附加组hadoop；

   3）#passwd hphs  

   设置密码为：123!@#

   4）ssh进入hphs，查看所属群组：

      #groups

      显示：hphs hadoop



3、回顾下Linux的ACL控制体系

   1)3个权限组：owner、group和other；

   2)3个权限类：r(读）、w(写）和x(执行）；

     r=4，w=2，x=1 

     若要rwx属性则4+2+1=7； 

     若要rw-属性则4+2=6； 

     若要r-x属性则4+1=5。 

   3)在hadoop下有个文件test.txt，用hdfs用户进入：

     目前 

     -rw-r--r--   3 hdfs  supergroup          6 2017-04-05 16:24 /test.txt

     第一：将组赋予hadoop

     hadoop fs -chown hdfs:hadoop /test.txt

     第二：权限修改为组可读其他用户不可读

     hadoop fs -chmod 740 /test.txt

     修改后：

     -rwxr-----   3 hdfs hadoop          6 2017-04-05 16:24 /test.txt





4、HDFS访问控制实践

   集群没有kerberos认证，所以客户机的任何linux用户进入都可以执行hadoop命令。

   如果加入kerberos认证，那linux用户要认证后才能执行hadoop命令。

   1)目录不允许其他用户读写的情形

     —权限：

    drwxrwx---   - hbase hadoop              0 2016-04-14 23:14 /hbase

     —执行：[hphs@appnode142 hphs]$ hadoop fs -ls /hbase

       提示：ls: Permission denied: user=hphs, access=READ_EXECUTE, inode="/hbase":hbase:hadoop:drwxrwx---

   2)目录允许其他用户读的情形

     —权限：

　　drwxr-xr-x   - hdfs  hadoop              0 2017-03-15 10:44 /flume 

     —执行：[hphs@appnode142 hphs]$ hadoop fs -ls /flume

     可以看到目录

   3)目录不允许其他用户写的情形

      —权限：

　　drwxr-xr-x   - hdfs  hadoop              0 2017-03-15 10:44 /flume 

     —执行：[hphs@appnode142 hphs]$ hadoop fs -mkdir /flume/test

     提示：mkdir: Permission denied: user=hphs, access=WRITE, inode="/flume":hdfs:hadoop:drwxr-xr-x

   4)文件允许组读但不允许其他用户读的情形

     [hphs@appnode142 hdfs]$ hadoop fs -ls /test.txt

     -rwxr-----   3 hdfs hadoop          6 2017-04-05 16:24 /test.txt

     因为hphs也属于hadoop组，所以可以读取，但若要执行，则：

    [hphs@appnode142 hdfs]$ hadoop fs -text /test.txt

    text: Permission denied: user=hphs, access=READ, inode="/test.txt":hdfs:hadoop:-rwxr-----

   如此可以总结：

   linux用户在hdfs上访问权限是通过ACL控制的。





5、总结Hive权限控制体系

   1)Hive授权的核心就是用户(User)、组(Group)、角色(Role)；

     Hive中的角色可以理解为一部分有一些相同“属性”的用户或组或角色的集合。

     用户和组使用的是Linux机器上的用户和组，而角色是Hive本身创建。

   2)例子：

     用户     组            

     db1      group_db1

     db2      group_db2

     dball    group_all

    有三个用户分别属于group_db1、group_db2、group_all；

    group _db1、group _db2、group_all分别表示该组用户可以访问数据库1、数据库2和可以访问1、2两个数据库；

    创建role_db1和role_db2，分别并授予访问数据库1和数据库2的权限，再将role_db1赋给group_db1（或者该组的所有用户），将role_db2赋给group_db2，就可以是实现指定用户访问指定数据库。

    最后创建role_all指向role_db1、role_db2（role_all不需要指定访问那个数据库），然后role_all授予group_all，则group_all中的用户可以访问两个数据库。




6、Hive权限控制启用需要配置hive-site.xml文件，如下：




```
<property>
   <name>hive.metastore.authorization.storage.checks</name>
    <value>true</value>
</property>
<property>
    <name>hive.metastore.execute.setugi</name>
    <value>false</value>
</property>
<property>
    <name>hive.security.authorization.enabled</name>
    <value>true</value>
</property>
<property>
   <name>hive.security.authorization.createtable.owner.grants</name>
    <value>ALL</value>
</property>
```


其中hive.security.authorization.createtable.owner.grants设置成ALL表示用户对自己创建的表是有所有权限。

  开启了权限控制，却发现任何用户都可以Grant/Revoke操作，需要设置hive管理员账户。




7、Hive管理员权限

   1)自定义权限控制类




```java
package com.hive;

import org.apache.hadoop.hive.ql.parse.ASTNode;  
import org.apache.hadoop.hive.ql.parse.AbstractSemanticAnalyzerHook;  
import org.apache.hadoop.hive.ql.parse.HiveParser;  
import org.apache.hadoop.hive.ql.parse.HiveSemanticAnalyzerHookContext;  
import org.apache.hadoop.hive.ql.parse.SemanticException;  
import org.apache.hadoop.hive.ql.session.SessionState;  

public class  HiveAdmin extends AbstractSemanticAnalyzerHook {
	private static String admin = "hdfs";
	@Override
	public ASTNode preAnalyze(HiveSemanticAnalyzerHookContext context, ASTNode ast) throws SemanticException {  
		switch (ast.getToken().getType()) {  
			case HiveParser.TOK_CREATEDATABASE:
			case HiveParser.TOK_DROPDATABASE:
			case HiveParser.TOK_CREATEROLE:
			case HiveParser.TOK_DROPROLE:
			case HiveParser.TOK_GRANT:
			case HiveParser.TOK_REVOKE:
			case HiveParser.TOK_GRANT_ROLE:
			case HiveParser.TOK_REVOKE_ROLE:
				String userName = null;
				if (SessionState.get() != null && SessionState.get().getAuthenticator() != null){
					userName=SessionState.get().getAuthenticator().getUserName();
				}
				if (!admin.equalsIgnoreCase(userName)) {
					throw new SemanticException(userName + " can't use ADMIN options, except " + admin +".");
				}
				break;
			default:break;
        }
		return ast;
    }
}
```


2)打包上传HiveAdmin.jar

     hive-env.sh配置对hive的shell终端生效

     export HIVE_AUX_JARS_PATH=file:///home/hdfs/fangjs/HiveAdmin.jar 

     hive-site.xml配置对hiveserver2生效  

     <property>  

        <name>hive.aux.jars.path</name>  

        <value>file:///home/hdfs/fangjs/HiveAdmin.jar</value>  

    </property>  



   3)添加控制类HiveAdmin到Hive的配置文件(hive-site.xml)中

    <property> 

    <name>hive.semantic.analyzer.hook</name> 

    <value>com.hive.HiveAdmin</value> 

    </property>



   4)添加了配置文件以后，重启Hive Server生效。

     至此，只有hdfs用户是hive管理员，可以进行Grant/Revoke操作。



8、Hive权限控制实践

   1)现在有两个linux用户和用户组，如下：

     hdfs:hdfs  -具有hive管理员权限

     hphs:hphs

     hive中有一个数据库cardb和数据表car_hosturl_m；


   2)为不修改hive-site.xm文件和重启Hive Server，这里直接在hive打开的shell上启用权限控制功能，相当于第6步作用。

     set hive.security.authorization.enabled=true;

     set hive.security.authorization.createtable.owner.grants = ALL;

     set hive.security.authorization.task.factory = org.apache.hadoop.hive.ql.parse.authorization.HiveAuthorizationTaskFactoryImpl;


     第三个设置是为避免如出现FAILED: SemanticException The current builtin authorization in Hive is incomplete and disabled.这个异常。


   3)为不修改hive-site.xm文件和重启Hive Server，这里直接hive打开的shell上上传HiveAdmin.jar包并设置参数，相当于第7步作用；  

     set hive.semantic.analyzer.hook=com.hive.HiveAdmin;

     add jar /home/hdfs/fangjs/HiveAdmin.jar;


   4)以下面两个角色操作为例

     hive>create role role_cardb;

     hive>GRANT SELECT ON DATABASE cardb TO USER hdfs;

     hdfs和hphs都可以能使用权限操作命令；

     配置com.hive.HiveAdmin类后发现没生效，怀疑和环境有关，或者要重启hiveserver才有效。

     查看hive版本#hive --version   执行结果：Hive 1.1.0-cdh5.5.0

     hive版本Hive 1.2.1000.2.5.3.0-37上类org.apache.hadoop.hive.ql.exec.DDLTask实现管理员权限。

     Hive管理员权限待继续研究新版本的Hive。



9、HIVE支持以下权限：

   权限名称 含义

   ALL      :  所有权限

   ALTER  :  允许修改元数据（modify metadata data of object）---表信息数据

   UPDATE  :  允许修改物理数据（modify physical data of object）---实际数据

   CREATE  :  允许进行Create操作

   DROP  :  允许进行DROP操作

   INDEX  :  允许建索引（目前还没有实现）

   LOCK  :  当出现并发的使用允许用户进行LOCK和UNLOCK操作

   SELECT  :  允许用户进行SELECT操作

   SHOW_DATABASE : 允许用户查看可用的数据库





10、登录hive元数据库，可以发现以下表:

    Db_privs:记录了User/Role在DB上的权限

    Tbl_privs:记录了User/Role在table上的权限

    Tbl_col_privs：记录了User/Role在table column上的权限

    Roles：记录了所有创建的role

    Role_map：记录了User与Role的对应关系


11、参考资料：

   http://blog.csdn.net/fjssharpsword/article/details/51312153

   http://www.aboutyun.com/thread-12549-1-1.html

   http://blog.csdn.net/kwu_ganymede/article/details/52728623

   http://blog.csdn.net/kwu_ganymede/article/details/52733021
            


