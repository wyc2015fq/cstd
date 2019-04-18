# Spring-Boot Not Finding JSP Pages in WAR File - z69183787的专栏 - CSDN博客
2015年06月16日 17:11:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2232
个人分类：[SpringBoot-概述](https://blog.csdn.net/z69183787/article/category/5580807)
官方有个sample，地址是：
https://github.com/spring-projects/spring-boot/tree/v1.1.5.RELEASE
里面有个spring-boot\spring-boot-samples\spring-boot-sample-web-jsp
自己跑一下。
如果是原来的工程应该添加tomcat-jsp解析包
               <dependency>
<groupId>org.apache.tomcat.embed</groupId>
<artifactId>tomcat-embed-jasper</artifactId>
<scope>provided</scope>
</dependency>
另外其他标签库自行添加，应该能运行起来
I had the same issue and in my case it happened because I was missing a library in the classpath.
Spring Boot does not include Jasper as default and therefore JSP rendering doesn't work unless you explicitly include the library:
For Gradle:
compile("org.apache.tomcat.embed:tomcat-embed-jasper")
For Maven:
<dependency>
    <groupId>org.apache.tomcat.embed</groupId>
    <artifactId>tomcat-embed-jasper</artifactId>
    <scope>provided</scope>
    <version>8.0.23</version>
</dependency>
            
