# JDK从1.7.x升到1.8.x后Eclipse Maven打包及Tomcat服务启动问题处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月26日 13:49:54[boonya](https://me.csdn.net/boonya)阅读数：2242













# 1      Eclipse JDK从1.7.x升级到1.8.xMaven打包问题解决

[INFO] 1 error

[INFO]-------------------------------------------------------------

[INFO] ------------------------------------------------------------------------

[INFO] BUILD FAILURE

[INFO]------------------------------------------------------------------------

[INFO] Total time: 1.552s

[INFO] Finished at: Wed Apr 2610:17:28 CST 2017

[INFO] Final Memory: 10M/112M

[INFO]------------------------------------------------------------------------

[ERROR] Failed to execute goalorg.apache.maven.plugins:maven-compiler-plugin:2.3.2:compile (default-compile)on project fmcgwms: Compilation failure

[ERROR] Unable to locate the JavacCompiler in:

[ERROR] C:\ProgramFiles\Java\jre1.8.0_92\..\lib\tools.jar

[ERROR] Please ensure you are usingJDK 1.4 or above and

[ERROR] not a JRE (thecom.sun.tools.javac.Main class is required).

[ERROR] In most cases you can changethe location of your Java

[ERROR] installation by setting theJAVA_HOME environment variable.

[ERROR] -> [Help 1]

[ERROR] 

[ERROR] To see the full stack traceof the errors, re-run Maven with the -e switch.

[ERROR] Re-run Maven using the -Xswitch to enable full debug logging.

[ERROR] 

[ERROR] For more information aboutthe errors and possible solutions, please read the following articles:

[ERROR] [Help 1] http://cwiki.apache.org/confluence/display/MAVEN/MojoFailureException



## 1.1  修改Eclipse JRE

![](https://img-blog.csdn.net/20170426134653426)




## 1.2  添加JAVA_HOME\lib\tools.jar

选择1.8版本的JRE：

![](https://img-blog.csdn.net/20170426134705474)




选中行进行编辑添加：



![](https://img-blog.csdn.net/20170426134716724)

## 1.3  Eclipse Maven 打包设置

选择打包项目：



![](https://img-blog.csdn.net/20170426134729973)

设置JRE：

![](https://img-blog.csdn.net/20170426134741130)

## 1.4  迁移到生产及测试环境必须重新设置JAVA_HOME

升级部署环境的JDK版本为1.8.x并设置相应的JAVA_HOME等环境变量。

# 2      Windows JDK升级Tomcat服务问题处理

在JDK1.7.x安装的服务默认使用的是之前的JDK版本，需要卸载并重新安装.

## 2.1  卸载Tomcat服务

cd 进入TOMCAT_HOME\bin目录输入：



TOMCAT_HOME\bin\service unstall TOMCAT-SERVER-NAME

## 2.2  安装Tomcat服务

cd 进入TOMCAT_HOME\bin目录输入：



TOMCAT_HOME\bin\service install TOMCAT-SERVER-NAME

![](https://img-blog.csdn.net/20170426134441379)



由此可见Tomcat服务安装时是找到了JAVA_HOME环境变量来进行配置的，所以JDK升级后应用程序如果依赖JDK8必须要重新安装服务。




