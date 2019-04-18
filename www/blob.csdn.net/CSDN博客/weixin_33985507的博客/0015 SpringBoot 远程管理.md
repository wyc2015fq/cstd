# 0015 SpringBoot 远程管理 - weixin_33985507的博客 - CSDN博客
2017年07月13日 18:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
0015 SpringBoot 远程管理
配置：
1.开启devtools 配置
<build>
<plugins>
<plugin>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-maven-plugin</artifactId>
<configuration>
<excludeDevtools>false</excludeDevtools>
</configuration>
</plugin>
</plugins>
</build>
2.配置属性
spring.devtools.remote.secret=mysecret
3.启动client端配置：
使用STS IDE
1.选择run
2.选择java Application
3.选择  my-app 项目
4.使用 org.springframework.boot.devtools.RemoteSpringApplication作为启动类
5.添加远程服务URL地址
远程更新：
客户端的任何改动都会同步到远程服务端，并引起响应的服务重启操作。
然而这貌似并没有什么用，生产环境肯定不会这样用的哈。
远程Debug
首先确认远程服务开启了debug模式。
例如在配置文件中开启配置
```
env:
    JAVA_OPTS: "-Xdebug -Xrunjdwp:server=y,transport=dt_socket,suspend=n"
```
RemoteClient 在本地服务开了一个8000端口用于debug远程服务。
可以使用spring.devtools.remote.debug.local-port 修改端口配置
