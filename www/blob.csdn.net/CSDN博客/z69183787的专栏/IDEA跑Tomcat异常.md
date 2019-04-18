# IDEA跑Tomcat异常 - z69183787的专栏 - CSDN博客
2014年11月08日 10:10:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4992
具体异常如下
intellij idea Server is not connected.
 Deploy is not available
unable to ping localhost 1099
根据别人的[回答](http://stackoverflow.com/questions/20163910/tomcat-in-idea-war-exploded-server-is-not-connected-deploy-is-not-available),去掉tomcat/bin下的setenv.*文件即可。
原因是该脚本里含有
set
 JAVA_OPTS= -Xms512M -Xmx512M -XX:PermSize=256M -XX:MaxNewSize=256M -XX:MaxPermSize=256M
将IDEA的JAVA_OPTS覆盖了。
但是我用的tomcat7没有这个文件，看了下tomcat的启动脚本start.bat和catalina.bat,发现在catalina.bat里有设置JAVA_OPTS，去掉即可。
