# Linux Ubuntu server安装Tomcat-7.0.63 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年08月25日 18:45:45[boonya](https://me.csdn.net/boonya)阅读数：2171









﻿﻿


###        0、下载地址

          wget  [http://mirror.bit.edu.cn/apache/tomcat/tomcat-7/v7.0.63/bin/apache-tomcat-7.0.63.tar.gz](http://mirror.bit.edu.cn/apache/tomcat/tomcat-7/v7.0.63/bin/apache-tomcat-7.0.63.tar.gz)

### 　　1、解压

　　tar –xzf apache-tomcat-7.0.63.tar.gz

　　得到apache-tomcat-7.0.63文件夹

         注意事项：如果使用tar命令不能解压说明包没有下全，我之前在另外一个镜像站点去下载很快就下完了，实际上是错误包，大小不对；

![](https://img-blog.csdn.net/20150825183821291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 正确的Tomcat包至少是8M左右；



![](https://img-blog.csdn.net/20150825184004245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 如看到如下问题，应该是下载的包不全或不正确：

![](https://img-blog.csdn.net/20150825184236333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      如果是正确的，你可以看到如下所示的图像：

![](https://img-blog.csdn.net/20150825184448857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 　　2、重命名

　　mv apache-tomcat-7.0.63 tomcat



### 　　3、放入/usr/local/

　　sudo mv tomcat /usr/local/



### 　　4、进入/usr/local/tomcat

　　cd /usr/local/tomcat



### 　　5、加环境变量

　　sudo su -

　　vi /etc/profile

　　最下方加入：

　　CATALINA_HOME=/usr/local/tomcat

　　export CATALINA_HOME

　　保存退出

### 　　6、进入$CATALINA_HOME/bin

　　cd $CATALINA_HOME/bin



### 　　7、修改catalina.sh

　　vi catalina.sh , 找到

　　# OS specific support. $var _must_ be set to either true or false.

　　在这行上面再定义一次CATALINA_HOME以及JAVA_HOME：

　　CATALINA_HOME=/usr/local/tomcat

　　JAVA_HOME=/usr/local/java/jdk1.7.0_21

　　保存退出



### 　　8、启动tomcat

　　logout

　　sudo ./bin/startup.sh

　　如果显示如下且无任何异常，则表示启动成功：

　　Using CATALINA_BASE: /usr/local/tomcat

　　Using CATALINA_HOME: /usr/local/tomcat

　　Using CATALINA_TMPDIR: /usr/local/tomcat

　　Using JRE_HOME: /usr/local/java/jdk

　　Using CLASSPATH: /usr/local/tomcat/bin/bootstrap.jar:/usr/local/tomcat/bin/tomcat-juli.jar

![](https://img-blog.csdn.net/20150825183557936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 　　9、停止tomcat：

        sudo ./bin/shutdown.sh



### 　　10、安装tomcat服务：

　　sudo cp bin/catalina.sh /etc/init.d/tomcat

　　查看服务：

　　sudo service tomcat status

　　列出一堆操作选项，即表示服务安装正常。



### 　　11、让tomcat开机启动

　　sudo update-rc.d –f tomcat defaults



### 　　12、通过服务启动

　　sudo service tomcat start






