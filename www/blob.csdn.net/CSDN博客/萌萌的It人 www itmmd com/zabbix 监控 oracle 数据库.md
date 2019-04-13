
# zabbix 监控 oracle 数据库 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月14日 16:49:23[Jlins](https://me.csdn.net/dyllove98)阅读数：17218


一、简介
Orabbix是设计用来为zabbix监控Oracle数据库的插件，它提供多层次的监控，包括可用性和服务器性能指标。
它提供了从众多Oracle实例采集数据的有效机制，进而提供此信息的监控和性能指标。然后，您可以利用的zabbix的报告功能为收集的所有数据，并提供分析。目前的发行版中包含了一组预先定义的模板，包括从初始部署警报和图形功能。然而，这些可以进行微调，以满足您的需求和数据/监控要求。

Orabbix监控什么？
数据库版本
归档日志与生产趋势分析
触发器，表/过程等命中率
逻辑I/O性能
物理I/O性能
PGA
SGA
共享池
Sessions
数据库大小

二、安装配置
\# mkdir /opt/orabbix
\# cd /opt/orabbix
\# unzip orabbix-1.2.3.zip
\# mvconf/config.props.sample conf/config.props
\# cp init.d/orabbix /etc/init.d/
\# chmod 755 /etc/init.d/orabbix
\# chkconfig orabbix on

三、修改orabbix配置
\# vim conf/config.props
\#comma separed list of Zabbix servers
ZabbixServerList=ZabbixServer
ZabbixServer.Address=192.168.0.180
ZabbixServer.Port=10051
\#pidFile
OrabbixDaemon.PidFile=./logs/orabbix.pid
\#frequency of item's refresh
OrabbixDaemon.Sleep=300
\#MaxThreadNumber should be >= thanthe number of your databases
OrabbixDaemon.MaxThreadNumber=100
\#put here your databases in a commaseparated list
DatabaseList=192.168.0.70\#名称与该机在zabbix中监控的主机名称保持一致
\#Configuration of Connection pool
\#if not specified Orabbis is going touse default values (hardcoded)
\#Maximum number of active connectioninside pool
DatabaseList.MaxActive=10
\#The maximum number of milliseconds thatthe pool will wait
\#(when there are no availableconnections) for a connection to be returned
\#before throwing an exception, or <=0 to wait indefinitely.
DatabaseList.MaxWait=100
DatabaseList.MaxIdle=1
\#define here your connection string foreach database
192.168.0.70.Url=jdbc:oracle:thin:@192.168.0.70:32778:southdb\#需要 jdk 环境，因为这里是通过JDBC连接的，southdb为数据库实例名称
192.168.0.70.User=ZABBIX\#用来监控oracle数据库的用户名和密码，需要在oracle中创建并赋予一定的权限
192.168.0.70.Password=ZABBIX
\#Those values are optionals if notspecified Orabbix is going to use the general values
192.168.0.70.MaxActive=10
192.168.0.70.MaxWait=100
192.168.0.70.MaxIdle=1
192.168.0.70.QueryListFile=./conf/query.props
\#DB2.Url=jdbc:oracle:thin:@server2.domain.example.com:<LISTENER_PORT>:DB2
\#DB2.User=zabbix
\#DB2.Password=zabbix_password
\#DB2.QueryListFile=./conf/query.props
\#DB3.Url=jdbc:oracle:thin:@server3.domain.example.com:<LISTENER_PORT>:DB3
\#DB3.User=zabbix
\#DB3.Password=zabbix_password
\#DB3.QueryListFile=./conf/query.props


关于jdbc：
Oracle         =       jdbc:oracle:thin:@<host>:<LISTENER_PORT>:<instance>
PostgreSQL    =       jdbc:postgresql://<host>:<port>/<database>
MS Sql Server =       jdbc:jtds:sqlserver://<host>:<port>/<instancename>
MySQL Server  =       jdbc:mysql://[host:port],[host:port].../[database]
DB2           =       jdbc:db2://<servername>:<port>/<installation>


四、创建oracle监控账户
1、登录oracle命令行
su - oracle\#切换到oracle用户
sqlplus /nolog\#不连接任何数据库
conn /as sysdba\#用sysdba登陆
或
conn用户名/密码
select instance_name from v$instance;\#查看实例
2、创建用户
CREATE USER ZABBIX
IDENTIFIED BY ZABBIX
DEFAULT TABLESPACE SYSTEM
TEMPORARY TABLESPACE TEMP
PROFILE DEFAULT
ACCOUNT UNLOCK;
3、赋予角色权限
GRANT CONNECT TO ZABBIX;
GRANT RESOURCE TO ZABBIX;
ALTER USER ZABBIX DEFAULT ROLEALL;
4、赋予系统权限
GRANT SELECT ANY TABLE TOZABBIX;
GRANT CREATE SESSION TO ZABBIX;
GRANT SELECT ANY DICTIONARY TOZABBIX;
GRANT UNLIMITED TABLESPACE TOZABBIX;
GRANT SELECT ANY DICTIONARY TOZABBIX;

五、启动服务
/etc/init.d/orabbix start
或
/opt/orabbix/run.sh
在启动服务过程中通过tail -f /opt/orabbix/logs/orabbix.log日志查看报错信息。

六、zabbix
 server端配置
将该模板Orabbix_export_full.xml下载到本机，差导入zabbix
 servertemplate
\# sz orabbix/template/Orabbix_export_full.xml
![wKiom1Rlv6iQ_IkoAAJEtv07_R8358.jpg](http://s3.51cto.com/wyfs02/M02/53/4B/wKiom1Rlv6iQ_IkoAAJEtv07_R8358.jpg)
将模板绑定到要监控的主机上：
![wKiom1Rlv6iTo3WsAAR3Gp0R1ms142.jpg](http://s3.51cto.com/wyfs02/M01/53/4B/wKiom1Rlv6iTo3WsAAR3Gp0R1ms142.jpg)
![wKioL1RlwBfBGOZIAANPq43h3j8674.jpg](http://s3.51cto.com/wyfs02/M02/53/49/wKioL1RlwBfBGOZIAANPq43h3j8674.jpg)





附：官方文档
http://www.smartmarmot.com/wiki/index.php/Orabbix
Assumptions
Theinstallation instructions have been created based on instructions for most *nixdeployments (i.e. RHEL/CentOS), and is assumed that any alterations requiredfor your specific environment are
 to be taken into account accordingly.
Thisguide is based upon there being two (2) Hosts, 1x Zabbix Server and 1x OracleServer. If you are planning on monitoring an Oracle instance that is running onyour Zabbix Server, the steps are
 the same, with minor adjustments required foryour connection information.
Thesteps also assume you are configuring Orabbix to monitor a new installation orsetup of Oracle. The installation steps will have you GRANT access for theZabbix user to all tables, and this will
 include any USER tables present at thetime of execution. If you do not want Zabbix to have access to specific tablesor resources within your database, you will need to set a DENY to the Zabbixusers access as required. Consult your DBA for details, as this
 is beyond thescope of these instructions.
Stepsfor Installation
Download Orabbix to your Zabbix Server
On your Zabbix server, unzip Orabbix to: /opt/orabbix
Copy file /opt/orabbix/init.d/orabbix to /etc/init.d/orabbix
Grant execute permissions to the following files:
/etc/init.d/orabbix
/opt/orabbix/run.sh
Create a User (ZABBIX) for Orabbix to access your Oracle Database.     You can use the following script:

CREATE USER ZABBIX
IDENTIFIED BY ZABBIX
DEFAULT TABLESPACE SYSTEM
TEMPORARY TABLESPACE TEMP
PROFILE DEFAULT
ACCOUNT UNLOCK;
– 2 Roles for ZABBIX
GRANT CONNECT TO ZABBIX;
GRANT RESOURCE TO ZABBIX;
ALTER USER ZABBIX DEFAULT ROLE ALL;
– 5 System Privileges for ZABBIX
GRANT SELECT ANY TABLE TO ZABBIX;
GRANT CREATE SESSION TO ZABBIX;
GRANT SELECT ANY DICTIONARY TO ZABBIX;
GRANT UNLIMITED TABLESPACE TO ZABBIX;
GRANT SELECT ANY DICTIONARY TO ZABBIX;
NOTEif you need an User (ZABBIX) for Orabbix     with the minimum grants available,
 you can use the following script:
CREATE USER ZABBIX
IDENTIFIED BY ZABBIX
DEFAULT TABLESPACE USERS
TEMPORARY TABLESPACE TEMP
PROFILE DEFAULT
ACCOUNT UNLOCK;
GRANT ALTER SESSION TO ZABBIX;
GRANT CREATE SESSION TO ZABBIX;
GRANT CONNECT TO ZABBIX;
ALTER USER ZABBIX DEFAULT ROLEALL;
GRANT SELECT ON V_$INSTANCE TOZABBIX;
GRANT SELECT ON DBA_USERS TOZABBIX;
GRANT SELECT ON V_$LOG_HISTORYTO ZABBIX;
GRANT SELECT ON V_$PARAMETER TOZABBIX;
GRANT SELECT ON SYS.DBA_AUDIT_SESSIONTO ZABBIX;
GRANT SELECT ON V_$LOCK TOZABBIX;
GRANT SELECT ON DBA_REGISTRY TOZABBIX;
GRANT SELECT ON V_$LIBRARYCACHETO ZABBIX;
GRANT SELECT ON V_$SYSSTAT TOZABBIX;
GRANT SELECT ON V_$PARAMETER TOZABBIX;
GRANT SELECT ON V_$LATCH TOZABBIX;
GRANT SELECT ON V_$PGASTAT TOZABBIX;
GRANT SELECT ON V_$SGASTAT TOZABBIX;
GRANT SELECT ON V_$LIBRARYCACHETO ZABBIX;
GRANT SELECT ON V_$PROCESS TOZABBIX;
GRANT SELECT ON DBA_DATA_FILESTO ZABBIX;
GRANT SELECT ON DBA_TEMP_FILESTO ZABBIX;
GRANT SELECT ON DBA_FREE_SPACETO ZABBIX;
GRANT SELECT ON V_$SYSTEM_EVENTTO ZABBIX;
NOTE: If you are using Oracle 11g, you will need to add the following:
exec dbms_network_acl_admin.create_acl(acl=> 'resolve.xml',description => 'resolve acl', principal =>'ZABBIX',is_grant => true, privilege => 'resolve');
exec dbms_network_acl_admin.assign_acl(acl=> 'resolve.xml', host =>'*');
commit;
Youcan verify the above is correct by running:
select utl_inaddr.get_host_name('127.0.0.1')from dual;
For this example on RedHat, run:
chkconfig -add orabbix
Verify with:
chkconfig -list
Create a Host entry in Zabbix for the DB Instance/s you are planning     to monitor, and import the templates found at: /opt/orabbix/template
Please note, instructions for this step can befound in the Zabbix Manual at
http://www.zabbix.com/documentation.php
With this step,ensure the name of yourhost in Zabbix is the same name of the SID or Oracle instance name.
Configuration
Nowwe need to configure your Orabbix setup. The tags below are listed as they willappear within the respective configuration files. The first you will need tomodify is your config.props file to
 define your connection properties forZabbix and Oracle.
config.props
Theconfig.props file can be found at /opt/orabbix/conf/config.props
ZabbixServerList
\#comma separed list of Zabbix servers
Thesettings under this tag allow you to configure your Zabbix Server information.Orabbix can also be configured to send your Oracle data to multiple Zabbixservers.
This can be beneficial for distributedmonitoring scenarios, server migrations or replicating Orabbix data  to a Disaster Recovery site or server.
Replacewith your Zabbix Server info where appropriate;
ZabbixServerList=ZabbixServer1,ZabbixServer2
ZabbixServer1.Address=192.168.0.1
ZabbixServer1.Port=10051
ZabbixServer2.Address=192.168.0.2
ZabbixServer2.Port=10051
OrabbixDaemon
Entriesunder this tag allow you to set your Orabbix Daemon parameters.
\#MaxThreadNumber should be >= than thenumber of your databases
Setthe number of threads the Orabbix Daemon should have inside his internal poolof DB Jobs. This number should be at least equal to (or more than) then numberof databases monitored by Orabbix,
 now this parameter if not set isautomatically calculated.
e.g.For 50 Databases, we are using 100 threads
OrabbixDaemon.MaxThreadNumber=100
\#pidFile
Nextyou can set the location of the Daemons PID file. Default location is:/opt/orabbix/logs
OrabbixDaemon.PidFile=./logs/orabbix.pid
NOTE:Relative path is permitted here
TIP:With the features of the OrabbixDaemon, it is possible to clone Orabbix and use different configurations.
As such, it is therefore possible to haveone Daemon that checks your TEST databases, another iteration that checks your
DEVELOPMENT databases and a third thatchecks your PRODUCTION databases, each with different timings and check loops!
DatabaseList
Thisis where you define your database instances. You can specify more than oneinstance here, separated using a comma.
DatabaseList=EXAMPLE1,EXAMPLE2
NOTE: The names of the instances must match those you have specified asyour HOST name in Zabbix
\#Configuration of Connection pool
Fromhere, you will configure settings that are specific to the connection pool. Asthe comments in config.props suggest, if you do not specify these values,Orabbix will use default values which
 have been hard-coded.
\#Maximum number of active connection insidepool
Setthe maximum number of connections that can be allocated to this pool at anytime,or alternatively set a negative value for no limit.
DatabaseList.MaxActive=10
\#The maximum number of milliseconds
Hereyou define how long that the pool will wait (when there are no available connections)for a connection to be returned before throwing an exception, or set the value<= 0 to wait indefinitely.
DatabaseList.MaxWait=100
Alsounder the same section, you can define the maximum number of connections thatcan remain idle within the connection pool, without being released.Alternatively, you can set a negative value
 for no limit.
DatabaseList.MaxIdle=1
TIP:You can specify the Database connection parameters for each database youwish
 to monitor individually, underneath your Database Connection Parameters.This allows you to customise your connection settings based on the Databaseconstraints I.e one connection pool for your PRODUCTION DB and another for youTEST DB, for example;
DB1.MaxActive=10
DB1.MaxWait=100
DB1.MaxIdle=1
Notethat any settings defines this way will override the general settings for yourgeneric connection pool.
Database Connection Parameters
Thissection sets your connection string to the Oracle Database. This string invokesa Java Database Connector (JDBC) to your Oracle Databases, and as such does notneed the Oracle Client to be installed.
\#define here your connection string for eachdatabase
Hereyou will define the connection string. These are formatted as: DBName.Url(asspecified in your DatabaseList) followed by the jdbc string and your DatabaseServer information.
EXAMPLE1.Url=jdbc:oracle:thin:@SERVER1.EXAMPLE.COM:1521:DB1
Setyour Database username and password below. e.g.
EXAMPLE1.User=zabbix
EXAMPLE1.Password=zabbix_password
TIP:Setting connections to multiple databases is made easy by adding moreconnection
 strings, with their corresponding credentials. Example given below.
EXAMPLE2.Url=jdbc:oracle:thin:@server2.domain.example.com:<LISTENER_PORT>:EXAMPLE2
EXAMPLE2.User=zabbix
EXAMPLE2.Password=zabbix_password
EXAMPLE3.Url=jdbc:oracle:thin:@server3.domain.example.com:<LISTENER_PORT>:EXAMPLE3
EXAMPLE3.User=zabbix
EXAMPLE3.Password=zabbix_password
NOTE:After these entries, you can set your MaxActive, MaxWait and MaxIdle forthe
 individual database connections if you so wish.
Thereis a pair of default parameter:
DefaultUser = <USERNAME>
DefaultPassword = <PASSWORD>
thisdefault parameter can be overridden using the following pair
<DB_NAME>.User and <DB_NAME>.User
Itis only possible to override Username or Password.
Query List File
Thisdefines where the file containing the Oracle (SQL) queries can be found. Thisis a customisable file. You can find the query parameter file at:
QueryListFile=/opt/orabbix/conf/query.props
youcan also specify :
<DBNAME>.QueryListFile=./confQueryTest.propsthis file specify the query    file of adatabase relative path are allowed
TIP:This is really useful to use different query file for differentdatabases
 or to have a pool of query file one for each
OracleRelease (if you have different release) and/or to have different query filesfor Production environment, Test environment, Developement environment etc..
thefollowing parameter
<DB_NAME>.ExtraQueryListFile==./confQueryTest_2.props
addsanother custom query to the default query file. If there are duplicates theExtraQueryListFile will override them.
query.props
Configurationof query.props file
Thequery.props file can be modified or added to, so you can supply your owncustomised queries through Orabbix against your Oracle instances/databases.Each query created has an associated 'Item'
 or item name that Zabbix will useto identify the query.
NOTE:Item names must be unique.
Theconfigurable items are formatted as follows.
Youhave to set the query name under the QueryList in the query.props file. Eachquery name is comma separated. For example;
QueryList=queryName1,queryName2,queryName3
Youmust identify the query by the unique item name you specified in the QueryList,followed by '.Query' for Orabbix to recognize that this is the query string.Its important to remember NOT to add
 the semi-colon“;”to
 the end of your custom query.
customQueryItemName.Query=yourQueryHere
Nowyou can define what you want Orabbix to return to your Zabbix Server if no datais found for your query.
customQueryItemName.NoDataFound=none
TIP:In the example above, Orabbix would send the string“none”to
 the Zabbix Server.
Youcan specify if you want a different execution period for your query
customQueryItemName.Period=<Express a periodin minute>
TIP:You can change the execution time of dbversion that don’t
 change often
Nextyou can specify a query that will be executed and if return RaceCondiftionValuethe query customQueryItemName is executed otherwise is skipped
customQueryItemName.ACTIVE=[true|false] iftrue query is executed otherwise skipped
archive.Query=select round(A.LOGS*B.AVG/1024/1024/10 ) \
from ( SELECT COUNT (*)  LOGS FROM V$LOG_HISTORYWHERE \
FIRST_TIME >= (sysdate -10/60/24)) A, \
( SELECT Avg(BYTES) AVG,  Count(1), Max(BYTES) Max_Bytes,Min(BYTES)Min_Bytes  FROM  v$log) B
archive.RaceConditionQuery=select value \
from \
v$parameter where name='log_archive_start'
archive.RaceConditionValue=FALSE
TIP:In the example above, Orabbix would execute“archive.Query”only
 if the query“archive.RaceConditionQuery”return
 the value“archive.RaceConditionValue”.
 Pratically the“Archive”query
 is execute only if database is in arhivelogmode (Parameter“log_archive_start”set
 to TRUE)
<QueryName>.Trim=[true|false]
iftrue the resultset is trimmed (default is true)
<QueryName>.AddSpaces=[true|false]
iftrue the add a space between columns of resultset(default is true)
<QueryName>.ExcludeColumnsList=[1,2,3,..n]
excludefrom result set the 1st, the 2nd etc.. columns from resultset
<QueryName>.Period
<QUERY_NAME>.WhenNotAlive =<VALUE>
isused to specify a value or a string to send if a database is not working, doingsuch will "clean" the graphs generated in Zabbix with a"set" value.
Addedthe following parameter on main configuration file:
DefaultUser = <USERNAME>
DefaultPassword = <PASSWORD>
<QueryName>.ExtraQueryListFile =<VALUE>
onquery's properties file I've added the following parameter:
<QUERY_NAME>.WhenNotAlive =<VALUE>
withthese parameter you can define a default username and password for all thedatabase.
DefaultUser = <USERNAME>
DefaultPassword = <PASSWORD>
thisdefault parameter can be overridden using the couple
<DB_NAME>.User and <DB_NAME>.User
Ispossible to override only Username or Password.
thefollowing parameter
<DB_NAME>.ExtraQueryListFile =
addanother custom query to a default query file. if there are duplicate theExtraQueryListFile will override them.
thefollowing parameter
<QUERY_NAME>.WhenNotAlive =<VALUE>
isused to specify a value or a string to send if a database is not working, doingso you are going to "clean" the graphs generated in Zabbix with a"set" value.
Howto Use Orabbix/FAQ
How do I start/stop the Daemon?
Tostart the Orabbix Daemon, simply run:
/etc/init.d/orabbix start
Tostop the Orabbix Daemon, simply run:
/etc/init.d/orabbix start
How does Logging work?
TheOrabbix daemon outputs its log file to the default location of/opt/orabbix/logs/
How to modify Log format?
Loggingproperties can be modified by making your required changes to
/opt/orabbix/conf/log4j.properties
Theproperty setting responsible for defining the output location is:
log4j.appender.Orabbix.File=logs/orabbix.log
TIP:From this properties file, you can modify the location, file name and logformat
 as desired. For additional information, please refer to the officiallog4j documentation, found at:http://logging.apache.org/log4j/1.2/index.html
Is there a way for orabbix to connect to RAC or DataGuard?
Yesis possible.
e.g.
Ifyou have two host RAC1 and RAC2 and one instance RACINST
youshould write the connection string as follow:
RACINST.Url=jdbc:oracle:thin:@(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=RAC1.EXAMPLE.COM)\
(PORT=1521))(ADDRESS=(PROTOCOL=TCP)(HOST=RAC2.EXAMPLE.COM)(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=RACINST)))
Whatcan I modify without restarting the daemon?
Theparameters that are dynamically read at each iteration of“OrabbixDaemon.Sleep”are
 as follows;
Any Query added to query.props
Modifications to config.props
Database List
Currently,the only known items that don't dynamically update are theZabbixDaemon.MaxThreadNumber, and changes to Connection Pool info.
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

