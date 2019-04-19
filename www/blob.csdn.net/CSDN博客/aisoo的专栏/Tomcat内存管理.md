# Tomcat内存管理 - aisoo的专栏 - CSDN博客
2010年05月12日 23:36:00[aisoo](https://me.csdn.net/aisoo)阅读数：1884
纸上谈兵，我今天才体会到了。没有实践，就没有发言权！关于Tomcat内存溢出的问题算是老生常谈了
   以前听朋友说遇到过这种问题，网上也看到不少相关的资料，感觉就是那么回事。最近做个项目前台felx
   后台java 加上我们现在做的10几个web应用程序，客户最近过来视察，匆忙的配置服务器，把这些应用搭建好，开始简单的
测试，不测不要紧，一测吓一跳啊，1，OutOfMemoryError： Java heap space    2， OutOfMemoryError： PermGen space
这两个溢出时不时就出来了，因为以前知道有这么回事，上网找了一下，在tomcat的catalina.bat里面设置了一下tomcat的JVM内存，不过没什么效果，回来之后，仔细研究了一下，才发现设置的位置设置的有些问题。如何看你设置的内存是否生效呢 ？
  我们可以打开tomcat的Status界面，可以发现里面的JVM
Free memory: 492.91 MB Total memory: 508.06 MB Max memory: 1016.12 MB 说明你的配置已经生效了 我用的是tomcat6
   在catalina.bat中rem ----- Execute The Requested Command ---------------------------------------下面加入
  set JAVA_OPTS=-Xms512m -Xmx1024m -XX:PermSize=128M -XX:MaxPermSize=256M
上面几个参数我就不说了，大家可以查查
这时候要注意 Xmx和MaxPermSize的和 不要超过系统支持最大的jvm堆的大小，否则tomcat也无法启动。可以通过
java -Xmx###M -version 来测试系统支持最大的jvm大小 
  先写到这，睡觉了！
