# 配置Tomcat中jvm的内存参数 - 零度的博客专栏 - CSDN博客
2016年12月02日 09:14:20[零度anngle](https://me.csdn.net/zmx729618)阅读数：362
修改 tomcat安装目录\bin\catalina.bat
只要在set JAVA_OPTS=%JAVA_OPTS% %LOGGING_CONFIG%这行前面加上
set JAVA_OPTS= -Xms1024M -Xmx1024M  -XX:MaxNewSize=256M -XX:PermSize=64M -XX:MaxPermSize=128M
set JAVA_OPTS= -server -Xms2048m -Xmx2048m -XX:NewSize=512m -XX:NewRatio=2（-XX:MaxNewSize=1024m -XX:PermSize=128m） -XX:MaxPermSize=256m
-Xloggc:$CATALINA_BASE/logs/gc.log -XX:+PrintGCDetails 
