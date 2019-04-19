# tomcat内存设置及jvm参数调优 - 零度的博客专栏 - CSDN博客
2016年05月12日 16:42:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：690
**设置tomcat的内存**
1、修改 start.bat 中的
call "%EXECUTABLE%" start %CMD_LINE_ARGS%
为 call "%EXECUTABLE%" run %CMD_LINE_ARGS% 
2、 catalina.bat 首行加入
set JAVA_OPTS=-server -Xms512m -Xmx512m -XX:NewSize=64m -XX:MaxNewSize=512m -XX:PermSize=256m 
-XX:MaxPermSize=256m -XX:+UseParallelGC -XX:ParallelGCThreads=2
-server 以server的模式启动
-Xms   初始化堆内存大小（initial heap size）
-Xmx   最大堆内存大小（maximum heap size）
-XX:NewSize   新生代内存大小
-XX:MaxNewSize 最大新生代内存大小
-XX:PermSize   持久带内存初始化大小
-XX:MaxPermSize持久带内存最大大小
