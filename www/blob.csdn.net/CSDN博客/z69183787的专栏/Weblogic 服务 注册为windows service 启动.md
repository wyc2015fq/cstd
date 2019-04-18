# Weblogic 服务 注册为windows service 启动 - z69183787的专栏 - CSDN博客
2012年11月15日 11:04:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3768
项目中需要将weblogic注册时 win service 来启动，所以研究了一下。
第一步：
进入 C:\Oracle\Middleware\wlserver_10.3\server\bin 目录 （C盘为安装盘）
找到  installSvc.cmd 及 uninstallSvc.cmd 文件 顾名思义，1个是注册服务，1个是卸载服务。
第二步：
编辑  installSvc.cmd文件
找到 set WL_HOME=C:\Oracle\Middleware\wlserver_10.3
call "%WL_HOME%\common\bin\commEnv.cmd"
此行（我是将代码加至此行的上面，因为需要放置在一些配置文件的上面）
其实可以看出，此文件中还是调用其他cmd文件来进行一些加载参数的设置。
增加代码如下：
这里有几个参数：
DOMAIN_NAME ：域名
SERVER_NAME ：安装时的server名称 ，我这里没有修改 是默认的
USERDOMAIN_HOME：该域的安装目录
MEM_ARGS：内存设置
WLS_USER WLS_PW ：weblogic 后台的账号密码
PRODUCTION_MOD：生产模式 false 为开发模式
LIB_PATH：因为我的应用用到了个公共包，所以设置了一个临时变量
LIB_CLASSPATH：公共包目录下的所有包
SET CLASSPATH=%LIB_CLASSPATH%;%CLASSPATH%：
将公共包目录加入 classpath中。
@rem *************************************************************************
set DOMAIN_NAME=portal.shmetro.com
set SERVER_NAME=AdminServer
set USERDOMAIN_HOME=C:\WeblogicApp\portal.shmetro.com\portal.shmetro.com
set MEM_ARGS=-Xms1024m -Xmx1024m
set WLS_USER=wonders_app
set WLS_PW=Wonders2012!
set PRODUCTION_MODE=true
SET LIB_PATH=C:/WeblogicApp/portal.shmetro.com/common-libs
SET LIB_CLASSPATH=%LIB_PATH%/slf4j-log4j12-1.5.2.jar;%LIB_PATH%/cglib-nodep-2.1_3.jar;%LIB_PATH%/commons-collections-3.2.jar;%LIB_PATH%/commons-fileupload-1.2.1.jar;%LIB_PATH%/xwork-core-2.2.1.1.jar;%LIB_PATH%/struts2-spring-plugin-2.2.1.1.jar;%LIB_PATH%/struts2-convention-plugin-2.2.1.1.jar;%LIB_PATH%/struts2-core-2.2.1.1.jar;%LIB_PATH%/org.springframework.expression-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.core-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.jdbc-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.orm-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.transaction-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.aop-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.asm-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.aspects-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.beans-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.context-3.1.1.RELEASE.jar;%LIB_PATH%/org.springframework.web-3.1.1.RELEASE.jar;%LIB_PATH%/antlr-2.7.6.jar;%LIB_PATH%/commons-io-1.4.jar;%LIB_PATH%/jxcell-2.4.2.jar;%LIB_PATH%/aspectjrt-1.6.1.jar;%LIB_PATH%/aopalliance-1.0.jar;%LIB_PATH%/persistence-api-1.0.jar;%LIB_PATH%/hibernate-core-3.3.1.GA.jar;%LIB_PATH%/cuteframework-security-3.10.jar;%LIB_PATH%/commons-validator-1.3.1.jar;%LIB_PATH%/concurrent-1.3.2.jar;%LIB_PATH%/javassist-3.4.GA.jar;%LIB_PATH%/hibernate-commons-annotations-3.1.0.GA.jar;%LIB_PATH%/commons-logging-1.1.1.jar;%LIB_PATH%/json-lib-2.2-jdk13.jar;%LIB_PATH%/cuteframework-appmgr-3.10.jar;%LIB_PATH%/ehcache-1.2.3.jar;%LIB_PATH%/cuteframework-widget-3.10.jar;%LIB_PATH%/commons-lang-2.3.jar;%LIB_PATH%/aspectjweaver-1.6.1.jar;%LIB_PATH%/dom4j-1.6.1.jar;%LIB_PATH%/commons-beanutils-1.7.0.jar;%LIB_PATH%/log4j-1.2.14.jar;%LIB_PATH%/hibernate-oscache-3.3.1.GA.jar;%LIB_PATH%/ognl-2.6.11.jar;%LIB_PATH%/ezmorph-1.0.4.jar;%LIB_PATH%/hibernate-annotations-3.4.0.GA.jar;%LIB_PATH%/commons-codec-1.3.jar;%LIB_PATH%/freemarker-2.3.8.jar;%LIB_PATH%/jxl.jar;%LIB_PATH%/hibernate-entitymanager-3.4.0.GA.jar;%LIB_PATH%/cuteframework-core-3.10.jar;%LIB_PATH%/ojdbc14.jar;%LIB_PATH%/poi-3.0.2-FINAL.jar;%LIB_PATH%/standard-1.1.jar;%LIB_PATH%/servlet-api-2.4.jar;%LIB_PATH%/jaxen-1.1.1.jar;%LIB_PATH%/oscache-2.1.jar;%LIB_PATH%/jdom-1.0.jar;%LIB_PATH%/slf4j-api-1.5.2.jar;%LIB_PATH%/ejb3-persistence-1.0.2.GA.jar;%LIB_PATH%/jta-1.1.jar;%LIB_PATH%/commons-digester-1.5.jar;%LIB_PATH%/jakarta-regexp-1.4.jar;%LIB_PATH%/junit-4.1.jar;
SET CLASSPATH=%LIB_CLASSPATH%;%CLASSPATH%
@rem *************************************************************************
这样再执行installSvc.cmd，服务就注册成功了。
**若想更改启动线程数，增加如下语句**
**set JAVA_OPTIONS=%JAVA_OPTIONS% -Dweblogic.threadpool.MinPoolSize=2000 set JAVA_OPTIONS=%JAVA_OPTIONS% -Dweblogic.threadpool.MaxPoolSize=4000**
接下来说一下 uninstallSvc.cmd ，卸载服务。
因为第一次注册服务肯定有些小问题，比如参数会一再更改，所以必然需要用到卸载服务的命令。
这里我也发现了一个问题，若服务已经注册了，修改了install文件后，需要先卸载服务，才能再次注册服务；不然会提示注册失败。
unstall文件中只需要加入：
SETLOCAL
set DOMAIN_NAME=portal.shmetro.com
set SERVER_NAME=AdminServer
set WL_HOME=C:\Oracle\Middleware\wlserver_10.3
指明 域名，服务名即可。
此外：在install文件中的最后几行：
rem *** Install the service
"%WL_HOME%\server\bin\beasvc" -install -svcname:"beasvc %DOMAIN_NAME%_%SERVER_NAME%" -javahome:"%JAVA_HOME%" -execdir:"%USERDOMAIN_HOME%" -maxconnectretries:"%MAX_CONNECT_RETRIES%" -host:"%HOST%" -port:"%PORT%" -extrapath:"%EXTRAPATH%"
 -password:"%WLS_PW%" -cmdline:%CMDLINE%
发现注册在winservice中的服务名 为 ：beasvc portal.shmetro.com_AdminServer
即红色标示，大家可以自行修改。
其他方法及参数，可以自行baidu和google，在这里就不赘述了。。
