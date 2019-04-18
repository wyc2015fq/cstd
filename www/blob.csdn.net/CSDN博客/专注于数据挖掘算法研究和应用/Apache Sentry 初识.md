# Apache Sentry 初识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月12日 11:06:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6317
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、Apache Sentry是什么？

     一句话：提供了细粒度级、基于角色的授权以及多租户的管理模式。

     对于Hadoop和Hive来说，引入Sentry提升数据安全是非常必要的。




2、Apache Sentry有什么作用？

     1）安全授权 ：Sentry可以控制数据访问，并对已通过验证的用户提供数据访问特权。

     2）细粒度访问控制 ：Sentry支持细粒度的Hadoop数据和元数据访问控制。Sentry在服务器、数据库、表和视图范围提供了不同特权级别的访问控制，包括查找、插入等，允许管理员使用视图限制对行或列的访问。管理员也可以通过Sentry和带选择语句的视图或UDF，根据需要在文件内屏蔽数据。

     3）基于角色的管理 ：Sentry通过基于角色的授权简化了管理，将访问同一数据集的不同特权级别授予多个组。

     4）多租户管理 ：Sentry允许为委派给不同管理员的不同数据集设置权限。Sentry可以在数据库/schema级别进行权限管理。

     5）统一平台 ：Sentry为确保数据安全，提供了一个统一平台，使用现有的Hadoop Kerberos实现安全认证。

     这里顺提Hadoop Kerberos安全认证：解决了主机和用户级的安全认证

    1）解决服务器到服务器的认证

           由于kerberos对集群里的所有机器都分发了keytab，相互之间使用密钥进行通信，确保不会冒充服务器的情况。集群中的机器就是它们所宣称的，是可靠的。

           防止了用户伪装成Datanode，Tasktracker，去接受JobTracker，Namenode的任务指派。

    2）解决client到服务器的认证

          Kerberos对可信任的客户端提供认证，确保他们可以执行作业的相关操作。防止用户恶意冒充client提交作业的情况。

          用户无法伪装成其他用户入侵到一个HDFS 或者MapReduce集群上

          用户即使知道datanode的相关信息，也无法读取HDFS上的数据

          用户无法发送对于作业的操作到JobTracker上




3、Apache Sentry怎么组成呢？

![](http://www.cloudera.com/content/www/en-us/documentation/enterprise/latest/images/sentry_components.png)

       1）Sentry Server: The Sentry RPC server manages the authorization metadata. It supports interface to securely retrieve and manipulate the metadata;

             主脑，管理着认证元数据。

        2）Data Engine: This is a data processing application such as Hive or Impala that needs to authorize access to data or metadata resources. The data engine loads the Sentry plugin and all client requests for accessing resources are intercepted and routed
 to the Sentry plugin for validation;

               引擎，验证访问权限。   

        3）Sentry Plugin: The Sentry plugin runs in the data engine. It offers interfaces to manipulate authorization metadata stored in the Sentry server, and includes the authorization policy engine that evaluates access requests using the authorization metadata
 retrieved from the server.

                集成接口。




4、Apache Sentry的关键概念

      1）Authentication - Verifying credentials to reliably identify a user

      2）Authorization - Limiting the user’s access to a given resource

      3）User - Individual identified by underlying authentication system

     4）Group - A set of users, maintained by the authentication system

     5）Privilege - An instruction or rule that allows access to an object

     6）Role - A set of privileges; a template to combine multiple access rules

     7）Authorization models - Defines the objects to be subject to authorization rules and the granularity of actions allowed. For example, in the SQL model, the objects can be databases or tables, and the actions are SELECT, INSERT, CREATE and so on. For the
 Search model, the objects are indexes, collections and documents; the access modes are query, update and so on.




5、Apache Sentry和Hadoop生态的集成

![](http://www.cloudera.com/content/www/en-us/documentation/enterprise/latest/images/sentry_hadoop_ecosystem.png)

     具体不一一扩展。




6、Apache Sentry部署

     由于环境暂不具备，后续部署后单独发文说明。

     参考：http://www.360doc.com/content/16/1026/09/37466175_601432990.shtml




官网：http://sentry.apache.org/



