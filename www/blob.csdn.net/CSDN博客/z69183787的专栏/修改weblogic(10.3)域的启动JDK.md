# 修改weblogic(10.3)域的启动JDK - z69183787的专栏 - CSDN博客
2013年02月05日 14:56:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3051
修改weblogic(10.3)域的启动JDK
说明:
1.D:\bea为笔者weblogic安装目录
2.D:\bea\user_projects\domains\base_domain为笔者域创建目录
方法1:
修改文件:D:\bea\wlserver_10.3\common\bin\commEnv.cmd
修改如下行:
@rem Reset JAVA Home
set  JAVA_HOME=d:\bea\jdk160_05
方法2:
修改文件:D:\bea\user_projects\domains\base_domain\bin\setDomainEnv.cmd
在下面行
call "%WL_HOME%\common\bin\commEnv.cmd"
后面增加
set  JAVA_HOME=d:\bea\jdk160_05
方法3:
修改文件:D:\bea\user_projects\domains\base_domain\bin\startWebLogic.cmd
在下面行
%JAVA_HOME%\bin\java %JAVA_VM% -version
前面增加:
set  JAVA_HOME=d:\bea\jdk160_05
