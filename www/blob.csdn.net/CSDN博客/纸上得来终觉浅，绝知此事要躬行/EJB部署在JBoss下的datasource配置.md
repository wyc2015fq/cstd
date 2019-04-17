# EJB部署在JBoss下的datasource配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年06月24日 16:20:31[boonya](https://me.csdn.net/boonya)阅读数：907








作者: [Michael](http://www.micmiu.com/author/michael/) 日期:
 2013 年 9 月 25 日    原文地址：[http://www.micmiu.com/j2ee/ejb/ejb-deploy-jboss-ds-config/](http://www.micmiu.com/j2ee/ejb/ejb-deploy-jboss-ds-config/)





本文以JBoss 5.0.1.GA 、Oracle 数据库为例，介绍JBoss下部署EJB如何配置datasource。


以 JBOSS_HOME 表示JBoss 本地的根目录 ，默认配置部署在 JBOSS_HOME/server/default/下。
- 创建配置文件：oracle-ds.xml

- 
配置数据库驱动
- 
配置 datasource 名称
- 
部署EJB工程

1. 创建配置文件：oracle-ds.xml


从 JBOSS_HOEM/docs/examples/jca/ 目录下找到Oracle数据源的配置模板文件：oracle-ds.xml ，copy一份到目录 JBOSS_HOME/server/default/deploy/ 目录下，并根据具体的Oracle参数修改配置文件如下：



|`1`|`<?``xml``version``=``"1.0"``encoding``=``"UTF-8"``?>`|
|----|----|


|`2`|`<``datasources``>`|
|----|----|


|`3`|`  ``<``local-tx-datasource``>`|
|----|----|


|`4`|`    ``<``jndi-name``>micmiuOracleDS</``jndi-name``>`|
|----|----|


|`5`|`    ``<``connection-url``>jdbc:oracle:thin:@192.168.1.8:1521:orcl</``connection-url``>`|
|----|----|


|`6`||
|----|----|


|`7`|`    ``<``driver-class``>oracle.jdbc.driver.OracleDriver</``driver-class``>`|
|----|----|


|`8`|`    ``<``user-name``>micmiu</``user-name``>`|
|----|----|


|`9`|`    ``<``password``>111111</``password``>`|
|----|----|


|`10`||
|----|----|


|`11`|`    ``<``exception-sorter-class-name``>org.jboss.resource.adapter.jdbc.vendor.OracleExceptionSorter</``exception-sorter-class-name``>`|
|----|----|


|`12`||
|----|----|


|`13`|`      ``<``metadata``>`|
|----|----|


|`14`|`         ``<``type-mapping``>Oracle9i</``type-mapping``>`|
|----|----|


|`15`|`      ``</``metadata``>`|
|----|----|


|`16`|`  ``</``local-tx-datasource``>`|
|----|----|


|`17`||
|----|----|


|`18`|`</``datasources``>`|
|----|----|




2. 配置数据库驱动 


把数据库对应驱动的lib包（比如：ojdbc6-11.2.0.3.0.jar）copy 到目录 JBOSS_HOME/server/default/lib 下。

3.配置 datasource 名称


把JBOSS_HOME/server/default/conf/tandardjbosscmp-jdbc.xml 中的



|`1`|`<``jbosscmp-jdbc``>`|
|----|----|


|`2`|`   ``<``defaults``>`|
|----|----|


|`3`|`      ``<``datasource``>java:/DefaultDS</``datasource``>`|
|----|----|


|`4`|`      ``...`|
|----|----|


|`5`|`   ``</``defaults``>`|
|----|----|


|`6`|`   ``...`|
|----|----|


|`7`|`</``jbosscmp-jdbc``>`|
|----|----|





修改成：



|`1`|`<``jbosscmp-jdbc``>`|
|----|----|


|`2`|`   ``<``defaults``>`|
|----|----|


|`3`|`      ``<``datasource``>java:/micmiuOracleDS</``datasource``>`|
|----|----|


|`4`|`      ``...`|
|----|----|


|`5`|`   ``</``defaults``>`|
|----|----|


|`6`|`   ``...`|
|----|----|


|`7`|`</``jbosscmp-jdbc``>`|
|----|----|





注意：<datasource>java:/micmiuOracleDS</datasource> 的值 java:/micmiuOracleDS 要和EJB工程中persistence.xml 配置文件中的<jta-data-source>java:/micmiuOracleDS</jta-data-source>
 配置要一致。

4. 部署EJB工程


把EJB工程打包后，copy到目录 JBOSS_HOME/server/default/deploy 下发布即可。


本文介绍到此结束@[Michael Sun](http://www.micmiu.com/).




