# Linux下Tomcat的安装配置 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年04月11日 10:45:41[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：10
[http://tomcat.apache.org/](http://tomcat.apache.org/)
[http://mirror.bit.edu.cn/apache/tomcat/tomcat-7/v7.0.93/bin/apache-tomcat-7.0.93.tar.gz](http://mirror.bit.edu.cn/apache/tomcat/tomcat-7/v7.0.93/bin/apache-tomcat-7.0.93.tar.gz)
启动
cd /usr/local/tomcat/bin ./startup.sh
关闭
shutdown.sh
配置端口
修改conf/server.xml里面的Connector元素(默认端口8080)，需要重新启动t服务生效。
添加应用
- cd /usr/local/tomcat/webapps
- 把war加入到webapps中,tomcat会自动解压war包webaapps目录下面主要是放应用包的.war在tomcat重启访问后会自动解压
webapps目录结构
webapps -- web应用所在目录
|--- html, jsp, css, js文件等 -- 这些文件一般在web应用根目录下，根目录下的文件外界可以直接访问.
|--- WEB-INF 目录 -- java类、jar包、web配置文件存在这个目录下，外界无法直接访问，由web服务器负责调用.
|--- classes 目录 -- java类
|--- lib 目录 -- java类运行所需要的jar包
|--- web.xml 文件 -- web应用的配置文件
---------------------
