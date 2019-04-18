# Tomcat 7.0 命令行方式启动 - z69183787的专栏 - CSDN博客
2014年04月18日 13:37:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6949
第一种 命令行形式，以下各种都是基于命令行的，只不过IDE，配置文件帮我们封装了一下过程。
命令形式：
java -classpath D:\tomcat5\bin\bootstrap.jar;D:\jdk15\lib\tools.jar -Dcatalina.home=D:\tomcat5 -Dcatalina.base=E:\webwork1_base  org.apache.catalina.startup.Bootstrap
 -config E:\webwork1_base\conf\tomcat_server.xml start
**org.apache.catalina.startup.Bootstrap 为启动main类**
第二种在 IDE 中 如 Eclipse中 启动：
查看 Run configurations -> arguments 参数，classpath 参数 指定了jdk编译命令的 参数 及 main 类
org.apache.catalina.startup.Bootstrap，该类的main函数 根据 传入的 args参数 指定 start或者stop。
VM arguments 指定了编译过程中的环境变量及一些目录参数 -D参数指定系统变量，在java中可使用
System.gerProperty(XXX)来获取，[http://kanpiaoxue.iteye.com/blog/1313925](http://kanpiaoxue.iteye.com/blog/1313925)
-classpath 指定编译引用的jar包
![](https://img-blog.csdn.net/20140418131619031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第三种，使用tomcat bin目录下的 startup.bat 启动 tomcat:
startup.bat 中 调用了如下：
set "EXECUTABLE=%CATALINA_HOME%\bin\catalina.bat"
set CMD_LINE_ARGS=%CMD_LINE_ARGS% %1
call "%EXECUTABLE%" start %CMD_LINE_ARGS%
[http://virgoooos.iteye.com/blog/222585](http://virgoooos.iteye.com/blog/222585)
catalina.bat 中 使用了setclasspath.bat 设置 "%JAVA_HOME%\bin\java.exe"
if ""%1"" == ""start"" goto doStart
:doStart 之后调用了一系列方法，最后至
:doneSetArgs
rem Execute Java with the applicable properties
if not "%JPDA%" == "" goto doJpda
if not "%SECURITY_POLICY_FILE%" == "" goto doSecurity
%_EXECJAVA% %JAVA_OPTS% %CATALINA_OPTS% %DEBUG_OPTS% -Djava.endorsed.dirs="%JAVA_ENDORSED_DIRS%" -classpath "%CLASSPATH%" -Dcatalina.base="%CATALINA_BASE%" -Dcatalina.home="%CATALINA_HOME%" -Djava.io.tmpdir="%CATALINA_TMPDIR%" %MAINCLASS% %CMD_LINE_ARGS% %ACTION%
goto end
命令与第一种方式类同。
启动过程分析：[http://book.51cto.com/art/201104/257120.htm](http://book.51cto.com/art/201104/257120.htm)
