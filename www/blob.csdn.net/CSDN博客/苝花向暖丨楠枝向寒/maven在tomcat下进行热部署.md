# maven在tomcat下进行热部署 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年06月29日 20:53:34[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：32


转自：[点击打开链接](https://blog.csdn.net/feng_qi_1984/article/details/54711060#comments)

1  tomcat 必须配置用户名和密码   打开tomcat目录下的conf/tomcat-users.xml

2 增加节点

 <role rolename="manager-gui"/>
    <role rolename="manager-script"/>
    <user username="tomcat" password="123456" roles="manager-gui, manager-script"/>（username和password可以自己写）

3 启动tomcat

4 在项目中的pom文件中增加tomcat插件 并且配置要部署的tomcat地址  用户名 密码

<plugins>
<plugin>
<groupId>org.apache.tomcat.maven</groupId>
<artifactId>tomcat7-maven-plugin</artifactId>
<version>2.2</version>
<configuration>
<url>http://tomcat地址/manager/text</url>
<username>tomcat</username>
<password>123456</password>
</configuration>
</plugin>
    </plugins>

5 执行maven命令  tomcat7:deploy 

6 如果工程已经存在则执行 tomcat7:redeploy

