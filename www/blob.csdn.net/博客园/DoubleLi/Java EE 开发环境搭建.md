# Java EE 开发环境搭建 - DoubleLi - 博客园






**下载安装****J****ava EE SDK**



版本：Java Platform,Enterprise Edition 7 SDK (with JDK 7u45)



下载页面：

[http://www.oracle.com/technetwork/java/javaee/downloads/java-ee-7-sdk-with-jdk-u45-2066865.html](http://www.oracle.com/technetwork/java/javaee/downloads/java-ee-7-sdk-with-jdk-u45-2066865.html)



文件名：java_ee_sdk-7-jdk7-windows.exe





安装路径：D:\glassfish4



环境变量设置

JAVA_HOME=D:\glassfish4\jdk7

JRE_HOME=D:\glassfish4\jdk7\jre

Path +=D:\glassfish4\jdk7\bin

classpath=D:\glassfish4\jdk7\lib\dt.jar;D:\glassfish4\jdk7\lib\tools.jar



下载安装Apache Maven



版本：Apache Maven 3.0.5



下载页面：

[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)



下载链接：

[http://mirrors.cnnic.cn/apache/maven/maven-3/3.0.5/binaries/apache-maven-3.0.5-bin.zip](http://mirrors.cnnic.cn/apache/maven/maven-3/3.0.5/binaries/apache-maven-3.0.5-bin.zip)



文件名：apache-maven-3.0.5-bin.zip



安装路径：D:\apache-maven-3.0.5



环境变量设置

Path +=D:\apache-maven-3.0.5\bin



下载安装 Eclipse IDE for Java EE Developers



版本：Eclipse Kepler(4.3.1) SR1



下载页面：

[http://www.eclipse.org/downloads/packages/eclipse-ide-java-ee-developers/keplersr1](http://www.eclipse.org/downloads/packages/eclipse-ide-java-ee-developers/keplersr1)



下载链接：

[http://www.eclipse.org/downloads/download.php?file=/technology/epp/downloads/release/kepler/SR1/eclipse-jee-kepler-SR1-win32.zip](http://www.eclipse.org/downloads/download.php?file=/technology/epp/downloads/release/kepler/SR1/eclipse-jee-kepler-SR1-win32.zip)



文件名：eclipse-jee-kepler-SR1-win32.zip



安装路径：D:\eclipse-jee-kepler-SR1-win32\eclipse



Eclipse工作目录：E:\workspace\eclipse-jee





下载安装Tomcat



版本：Tomcat 8.0.3



下载页面：[http://tomcat.apache.org/download-80.cgi](http://tomcat.apache.org/download-80.cgi)



下载链接：[http://mirror.esocc.com/apache/tomcat/tomcat-8/v8.0.3/bin/apache-tomcat-8.0.3-windows-x86.zip](http://mirror.esocc.com/apache/tomcat/tomcat-8/v8.0.3/bin/apache-tomcat-8.0.3-windows-x86.zip)



安装目录：E:\apache-tomcat-8.0.3



环境变量设置：

CATALINA_HOME=E:\apache-tomcat-8.0.3





**例程演示**** -****The Echo WebSocket Sample Application**



D:\glassfish4\samples下是用来演示 Java EE 7各种新特性的示例

将改文件夹复制到工作目录

E:\workspace\eclipse-jee\javaee7\samples



打开命令提示符窗口，进入到websocket echo例程目录

D:\glassfish4\samples\websocket\echo>





执行命令mvn cleancompile编译构建例程

命令成功执行后生成war包：E:\workspace\eclipse-jee\javaee7\samples\websocket\echo\target\websocket-echo.war



部署war包到tomcat：

将war文件websocket-echo.war拷贝到tomcat目录\webapps\ 下，即：E:\apache-tomcat-8.0.3\webapps



启动tomcat

执行BAT脚本：E:\apache-tomcat-8.0.3\bin\startup.bat



在浏览器（支持HTML5）打开前台页面：http://localhost:8080/websocket-echo

如下图所示：



![](http://img.blog.csdn.net/20140219111634578)

点击 "Press me"按钮

浏览器将会通过WebSocket发送编辑框中的文本到后台Web Socket服务器，服务器收到后将会把文本

发回到浏览器，浏览器在页面下方显示收到的文本。



![](http://img.blog.csdn.net/20140219111653375)

![](http://img.blog.csdn.net/20140219111700171)











