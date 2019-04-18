# SpringBoot 项目如何在tomcat容器中运行 - z69183787的专栏 - CSDN博客
2019年01月21日 10:14:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：51
个人分类：[SpringBoot-概述](https://blog.csdn.net/z69183787/article/category/5580807)
[https://blog.csdn.net/u010598360/article/details/78789197](https://blog.csdn.net/u010598360/article/details/78789197)
一. SpringBoot内嵌容器的部署方式
SpringBoot内部默认提供内嵌的tomcat容器，所以可以直接打成jar包，丢到服务器上的任何一个目录，然后在当前目录下执行java -jar demo.jar即可运行，但是这种方式的运行退出进程就结束了。如果想在后台可以运行，则需要执行 java -jar demo.jar  > log_demo.file 2>&1 & 即可在后台运行该服务了，log_demo.file是日志文件。如需停止该进程 执行ps -ef|grep java 找到对应的进程 kill 进程ID 即可
二. 使用外部的tomcat部署方式
pom.xml文件，dependencies中添加
<dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-tomcat</artifactId>
        <scope>provided</scope>
</dependency>12345
在pom.xml文件中，project下面packaging标签改为
<packaging>war</packaging>
12
将项目的启动类Application.java继承SpringBootServletInitializer并重写configure方法
@SpringBootApplication
public class Application extends SpringBootServletInitializer {
    @Override
    protected SpringApplicationBuilder configure(SpringApplicationBuilder application) {
        return application.sources(Application.class);
    }
    public static void main(String[] args) throws Exception {
        SpringApplication.run(Application.class, args);
    }
}12345678910111213
这样，只需要以上3步就可以打包成war包，并且部署到tomcat中了。需要注意的是这样部署的request url需要在端口后加上项目的名字才能正常访问。spring-boot更加强大的一点就是：即便项目是以上配置，依然可以用内嵌的tomcat来调试，启动命令和以前没变，还是：mvn spring-boot:run。
如果需要在springboot中加上request前缀，需要在application.properties中添加server.contextPath=/prefix/即可。其中prefix为前缀名。这个前缀会在war包中失效，取而代之的是war包名称，如果war包名称和prefix相同的话，那么调试环境和正式部署环境就是一个request地址了。 
如果是application.yml 文件 则为
server:
  port: 8080
  context-path: /prefix
--------------------- 
