
# Oracle基础：1: 相关组件版本确认 - 知行合一 止于至善 - CSDN博客

2018年11月09日 21:11:18[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2718


![在这里插入图片描述](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这系列文章将介绍一下Oracle的基础知识，将会使用docker下运行的oracle11g的精简版进行说明。这篇文章介绍一下oracle的版本确认方法。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 环境准备
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)使用Oracle的精简版创建docker方式的demo环境，详细可参看：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[https://blog.csdn.net/liumiaocn/article/details/82733272](https://blog.csdn.net/liumiaocn/article/details/82733272)
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# v$version视图
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)通过确认v$version视图的BANNER信息可以确认Oracle各组件的版本信息
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`SQL> select * from v$version;
BANNER
--------------------------------------------------------------------------------
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
PL/SQL Release 11.2.0.2.0 - Production
CORE	11.2.0.2.0	Production
TNS for Linux: Version 11.2.0.2.0 - Production
NLSRTL Version 11.2.0.2.0 - Production
SQL>`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# Oracle版本信息说明
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)关于使用的XE版本的Oracle，这里出现一个版本号，11.2.0.2.0，可以看到其以.为分割分成了5部分，具体各部分代表的意义如下：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|类型|取值|说明|
|---|---|---|
|Major Database Release Number|11|数据库的主版本号，11g或者12c等，主要功能升级|
|Database Maintenance Release Number|2|次版本号，在主版本生命期用于标志特性功能的升级|
|Fusion Middleware Release Number|0|Oracle中间件的版本号（Oracle Fusion Middleware）|
|Component-Specific Release Number|2|组件级别的发布和更新，比如相应的补丁|
|Platform-Specific Release Number|0|平台级别的补丁的发布和更新版本|
# product_component_version
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)另外v$version中的信息也可以从product_component_version获取
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`SQL> desc product_component_version
 Name								   Null?    Type
 ----------------------------------------------------------------- -------- --------------------------------------------
 PRODUCT								    VARCHAR2(320)
 VERSION								    VARCHAR2(320)
 STATUS 								    VARCHAR2(320)
SQL>`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)由于product_component_version的每个column太长，所在确认之前需要设定column，不然一般的屏幕一行肯定显示不完，使用select取得了类似的版本信息
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`SQL> column product format a20
SQL> column version format a20
SQL> column status  format a20
SQL> select * from product_component_version;
PRODUCT 	     VERSION		  STATUS
-------------------- -------------------- --------------------
NLSRTL		     11.2.0.2.0 	  Production
Oracle Database 11g  11.2.0.2.0 	  64bit Production
Express Edition
PL/SQL		     11.2.0.2.0 	  Production
TNS for Linux:	     11.2.0.2.0 	  Production
SQL>`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# v$intance
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)当然如果有示例的信息，也可以通过确认v$intance视图中的Version字段来确认版本信息
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`SQL> desc v$instance
 Name					   Null?    Type
 ----------------------------------------- -------- ----------------------------
 INSTANCE_NUMBER				    NUMBER
 INSTANCE_NAME					    VARCHAR2(16)
 HOST_NAME					    VARCHAR2(64)
 VERSION					    VARCHAR2(17)
 STARTUP_TIME					    DATE
 STATUS 					    VARCHAR2(12)
 PARALLEL					    VARCHAR2(3)
 THREAD#					    NUMBER
 ARCHIVER					    VARCHAR2(7)
 LOG_SWITCH_WAIT				    VARCHAR2(15)
 LOGINS 					    VARCHAR2(10)
 SHUTDOWN_PENDING				    VARCHAR2(3)
 DATABASE_STATUS				    VARCHAR2(17)
 INSTANCE_ROLE					    VARCHAR2(18)
 ACTIVE_STATE					    VARCHAR2(9)
 BLOCKED					    VARCHAR2(3)
 EDITION					    VARCHAR2(7)
SQL> select version from v$instance;
VERSION
--------------------
11.2.0.2.0
SQL>`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# sqlplus -v
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)确认sqlplus客户端的版本信息可以通过sqlplus -v来进行，或者sqlplus的执行结果也能看到。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`oracle@e871d42341c0:~$ sqlplus -v
SQL*Plus: Release 11.2.0.2.0 Production
oracle@e871d42341c0:~$`[
            ](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

