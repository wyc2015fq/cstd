# Jenkins:项目持续集成管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月21日 10:57:02[boonya](https://me.csdn.net/boonya)阅读数：2065








目前国内很多项目都是采用敏捷开发的方式，敏捷不是对开发人员要求高，不是对文档没要求，也不是只有敏捷开发方式才可取。敏捷开发以用户的需求进化为核心，采用迭代、循序渐进的方法进行软件开发。在敏捷开发中，软件项目在构建初期被切分成多个子项目，各个子项目的成果都经过测试，具备可视、可集成和可运行使用的特征。换言之，就是把一个大项目分为多个相互联系，但也可独立运行的小项目，并分别完成，在此过程中软件一直处于可使用状态。本文将做一个github项目的持续集成工具Jekins管理讲解。

# Jenkins介绍

Jenkins是一个开源软件项目，旨在提供一个开放易用的软件平台，使软件的持续集成变成可能。Jenkins官方网站：[https://jenkins.io/index.html](https://jenkins.io/index.html)


Jenkins是Continous Integration Server，是Hadson的继续。 类似的产品： CruiseControl，BuildBot，TeamCity，BuildForge, ElectricCommander等。


## 特点

Jenkins 的主要特点：

- 容易安装，只需要执行 Java -jar jenkins.war， 或者直接部署到一个 servlet container 中，例如 tomcat。不需要安装，不需要数据库的支持。
- 容易配置， jenkins 可以完全地通过友好的 web GUI 来配置，且配置页面支持配置项的错误检查和很好的在线帮助。不需要手动地编辑 xml 的配置文件，但是 jenkins 也支持手动修改 xml 配置文件。
- 项目源码修改的检测， jenkins 能够从项目的 Subversion/CVS 生成最近修改的集合列表，且改方式非常有效，不会增加 Subversion/CVS Repository 的负载。
- 可读的永久的链接生成， jenkins 对于大部分 pages 都生成清楚的可读的永久的链接，例如''latest build"/"latest successful build",因此可以容易地在其他的地方引用 jenkins 的生成的pages。
- RSS/EMail/IM 集成，可以通过 RSS， EMail 或 IM 来实时地监视 build 的失败。
- Build 完成后仍然可以 tag，支持在 build 完成后 tag 或重 tag。
- Junit/TestNG 测试报告，能够很好地显示各种测试的报告，且可以生成失败的趋向图。
- 分布式 build， jenkins 能够分发 build/test 的负载到多台机器，能够更好地利用硬件资源，提高 build 的时间。
- 文件标识， jenkins 可以标识 build 产生的文件，例如 jars。
- 插件支持， jenkins 可以通过第三方的插件来扩展。
- 跨平台，支持几乎所有的平台，例如 Windows,Ubuntu/Debian,RedHat/Fedora/CentOS,Mac OS X,openSUSE,FreeBSD,OpenBSD,Solaris/OpenIndiana.Gentoo。

## 功能
Jenkins是基于Java开发的一种持续集成工具，用于监控持续重复的工作，功能包括：


1、持续的软件版本发布/测试项目。

2、监控外部调用执行的工作。


## 启动
首先保证系统中已经安装了jdk，最好是jdk1.5以上。


**第一种启动方法，切换到jenkins.war存放的目录，输入如下命令：**

`$ java -jar jenkins.war`
命令执行完成后输出：



```
*************************************************************
*************************************************************
*************************************************************

Jenkins initial setup is required. An admin user has been created and a password generated.
Please use the following password to proceed to installation:

861576ad42044407a29c89aba60c324f

This may also be found at: /root/.jenkins/secrets/initialAdminPassword

*************************************************************
*************************************************************
*************************************************************
```
注：这里输出的是Jenkins管理密码。无论是Linux还是Windows都可以使用此命令来启动。
然后在浏览器中（推荐用火狐）输入localhost:8080，localhost可以是本机的ip，也可以是计算机名就可以打开jenkins。

![](https://img-blog.csdn.net/20170521152654664)



**第二种方法是放于tomcat容器打开**

将jenkins.war文件放入tomcat下的webapps目录下，启动jenkins时，会自动在webapps目录下建立jenkins目录，启动后访问方式如下：

![](https://img-blog.csdn.net/20170521153229604)


# 环境配置



在Linux下安装JDK、Maven。主要是解压并修改/etc/profle文件，配置环境变量：

## java环境配置



```
export JAVA_HOME=/usr/local/jdk/jdk1.8.0_21  
export JAVA_OPTS="-Xms1024m-Xmx1024m"  
export CLASSPATH=.:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/lib/dt.jar:$CLASSPATH  
export PATH=$JAVA_HOME/bin:$PATH
```

## maven环境配置





```
export MAVEN_HOME=/usr/local/maven/apache-maven-3.3.9  
export PATH=$PATH:$MAVEN_HOME/bin
```



Linux下配好之后记得让资源生效：


`source /etc/profile`


参考：[http://blog.csdn.net/boonya/article/details/55506386](http://blog.csdn.net/boonya/article/details/55506386)

# Jekins使用管理

## 下载Jenkins war包

到[http://mirrors.jenkins.io/war/](http://mirrors.jenkins.io/war/)下载Jekins的war包。





## 部署Jenkins 

将Jenkins放在任意路径下执行：

`$ java -jar jenkins.war`注：执行后获取到管理密码，可以保存起来待后续使用。

## 部署jenkins到tomcatJenkins的部署过程很简单，只需要把下载的war包复制到tomcat的webapps目录下即可。




![](https://img-blog.csdn.net/20170521144956895)


注：它的war包自带Jetty服务器，剩下的工作我们全部在浏览器中进行。


访问jenkins启动tomcat，访问[http://localhost:8080/jenkins](http://localhost:8080/jenkins)



## 首次启动插件及用户设置

选择推荐的插件进行安装：

![](https://img-blog.csdn.net/20170521144456893)








![](https://img-blog.csdn.net/20170521145106693)


注：如果安装插件失败此处可以重试安装。

![](https://img-blog.csdn.net/20170521145206053)


设置用户：

![](https://img-blog.csdn.net/20170521145233585)


进入到系统主界面：

![](https://img-blog.csdn.net/20170521145256383)

## 管理Jenkins

![](https://img-blog.csdn.net/20170523141530692)





## 全局工具配置

系统设置，配置jdk与maven的环境

![](https://img-blog.csdn.net/20170523141327115)


## 插件管理

![](https://img-blog.csdn.net/20170523141845913)


## 构建项目

注意：项目名称不能与构建过的项目重复。


![](https://img-blog.csdn.net/20170523142304368)


配置相关的项目参数：

![](https://img-blog.csdn.net/20170523142510319)





## 构建项目注意事项

### 保证环境已安装git

git没有下载对应的代码：


![](https://img-blog.csdn.net/20170523142819761)

处理方法就是安装git.

![](https://img-blog.csdn.net/20170523142926340)

![](https://img-blog.csdn.net/20170523142945528)


### 项目首次构建需要耐心

项目首次构建比较慢因为会将远程仓库的jar包下载到本地maven仓库。

![](https://img-blog.csdn.net/20170523143008044)

项目第二次构建时就比较快了：

![](https://img-blog.csdn.net/20170523143118624)


## 部署项目

新增构建后的操作选项Deploy war/ear to a container：

![](https://img-blog.csdn.net/20170523151606425)


设置相关的Tomcat参数值，并且需要修改Tomcat的conf/tomcat-users.xml的管理登录权限：



```
<?xml version='1.0' encoding='utf-8'?>
<!--
  Licensed to the Apache Software Foundation (ASF) under one or more
  contributor license agreements.  See the NOTICE file distributed with
  this work for additional information regarding copyright ownership.
  The ASF licenses this file to You under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with
  the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
-->
<tomcat-users>
<!--
  NOTE:  By default, no user is included in the "manager-gui" role required
  to operate the "/manager/html" web application.  If you wish to use this app,
  you must define such a user - the username and password are arbitrary.
-->
<!--
  NOTE:  The sample user and role entries below are wrapped in a comment
  and thus are ignored when reading this file. Do not forget to remove
  <!.. ..> that surrounds them.
-->
<!--
  <role rolename="tomcat"/>
  <role rolename="role1"/>
  <user username="tomcat" password="tomcat" roles="tomcat"/>
  <user username="both" password="tomcat" roles="tomcat,role1"/>
  <user username="role1" password="tomcat" roles="role1"/>
-->
<role rolename="manager-script"/>
<user username="tomcat" password="password" roles="manager-script"/>
</tomcat-users>
```

注意：Tomcat的用户名和密码必须对应，保存后重新构建，项目也同时进行了部署。
构建日志：




```
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 9.252 s
[INFO] Finished at: 2017-05-23T15:32:12+08:00
[INFO] Final Memory: 24M/187M
[INFO] ------------------------------------------------------------------------
Deploying /root/.jenkins/workspace/SpringVelocity/target/SpringVelocity-0.0.1-SNAPSHOT.war to container Tomcat 7.x Remote
  Redeploying [/root/.jenkins/workspace/SpringVelocity/target/SpringVelocity-0.0.1-SNAPSHOT.war]
  Undeploying [/root/.jenkins/workspace/SpringVelocity/target/SpringVelocity-0.0.1-SNAPSHOT.war]
  Deploying [/root/.jenkins/workspace/SpringVelocity/target/SpringVelocity-0.0.1-SNAPSHOT.war]
Finished: SUCCESS
```



上面显示已经构建并部署，我们访问：http://192.168.200.122:8888/SpringVelocity/

![](https://img-blog.csdn.net/20170523151953819)

![](https://img-blog.csdn.net/20170523153404357)


有此可见：大工告成！！！





# 参考资料

Jenkins百科：[http://baike.baidu.com/link?url=2ORQzmgFijbepqqPRK1qaV9O7n6wFVTsd6pxk8gLhsQu1ep061MEM0PDH-9hKkqrclp2z_sqEUFRxSl9Ebcqlx7tVDO3P8NgsCOZtX6u2Va](http://baike.baidu.com/link?url=2ORQzmgFijbepqqPRK1qaV9O7n6wFVTsd6pxk8gLhsQu1ep061MEM0PDH-9hKkqrclp2z_sqEUFRxSl9Ebcqlx7tVDO3P8NgsCOZtX6u2Va)





轻松搭建持续集成工具Jenkins：[https://my.oschina.net/u/3080373/blog/873081](http://baike.baidu.com/link?url=2ORQzmgFijbepqqPRK1qaV9O7n6wFVTsd6pxk8gLhsQu1ep061MEM0PDH-9hKkqrclp2z_sqEUFRxSl9Ebcqlx7tVDO3P8NgsCOZtX6u2Va)




Jenkins+maven+SVN+Tomcat持续集成发布环境安装和配置 ：[http://blog.csdn.net/luckykapok918/article/details/70142165](http://blog.csdn.net/luckykapok918/article/details/70142165)




JenkinsPDF的下载：[http://files.cnblogs.com/itech/Jenkins%E5%85%A5%E9%97%A8.pdf](http://files.cnblogs.com/itech/Jenkins%E5%85%A5%E9%97%A8.pdf)




