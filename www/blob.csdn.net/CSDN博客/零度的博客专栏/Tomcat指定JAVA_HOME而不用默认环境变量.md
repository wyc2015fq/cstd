# Tomcat指定JAVA_HOME而不用默认环境变量 - 零度的博客专栏 - CSDN博客
2016年05月13日 10:01:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：3386
      Tomcat默认情况下会用系统的环境变量中找到JAVA_HOME和JRE_HOME。但是有的时候我们需要不同版本的JDK共存。
可以在${TOMCAT_HOME}/bin/setclasspath.bat最前面设置JAVA_HOME和JRE_HOME。 例如：  
Js代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- rem set user jdk and jre home  
- set JAVA_HOME=E:\javadev\jdk5  
- set JRE_HOME=E:\javadev\jdk5\jre  
      另外，有时需要设置Tomcat的最大内存，方法如下：在catalina.bat的echo Using CATALINA_BASE:   %CATALINA_BASE%上一行中加入： 
Js代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- rem set max meroy  
- set JAVA_OPTS= -Xms800m -Xmx1000m    
