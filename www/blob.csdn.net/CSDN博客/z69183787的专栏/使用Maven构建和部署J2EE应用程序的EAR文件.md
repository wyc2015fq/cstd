# 使用Maven构建和部署J2EE应用程序的EAR文件 - z69183787的专栏 - CSDN博客
2015年11月24日 13:46:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：779
个人分类：[EJB-Ear项目](https://blog.csdn.net/z69183787/article/category/5977003)

## 这篇文章，主要是技术上的整理，用来mark一下，用的时候参考。
## 一、新建项目
新建一个空的Maven Project项目
![](https://img-blog.csdn.net/20140520203719359)
## 二、放入依赖
注：ear部署时如果里面有entity，会发生错误，所以不要把entity放到依赖项中，部署的时候先部实体，然后再部ear。
将需要打入EAR中的jar、war以dependencies-dependency的形式写到pom.xml文件中，如：
![](https://img-blog.csdn.net/20140520203822828)
## 三、配置加载顺序
现在打出来的ear包中的application中只有war的配置信息，而没有jar的配置信息，而我们如果想控制这些jar的配置信息，就必须把jar的信息配置到application.xml中，而application.xml是可以通过此插件生成的，所以在pom.xml文件中配置即可，配置方式如下：
![](https://img-blog.csdn.net/20140520203923984)
## 四、运行
如果现在打包，会报一个没有发现application.xml的错误，这个文件需要我们自己创建么？答案是不需要：
1.运行ear:
 generate-application-xml，会生成相应的application.xml文件，下面是我的pom生成的application.xml文件（格式有改动）：
![](https://img-blog.csdn.net/20140520204057500)
2.同时，此命令还生成一个jboss-app.xml的文件，里面简单一一句话，是说要JBoss按照application.xml里面的声明顺序加载
![](https://img-blog.csdn.net/20140520204134187)
3.现在可以执行生成ear的命令了：ear:ear，坐等成功吧~~
## 五、更改pom的packaging
现在如果你直接Install的话，会发现并不是打成的ear，而是打了jar，需要将packaging改为ear即可。
但改后会发现pom上面报错，你在eclipse应用广场搜一下m2e-wtp，然后安装即可。
## 注：我的完整pom.xml
**[html]**[view
 plain](http://blog.csdn.net/shan9liang/article/details/26396163#)[copy](http://blog.csdn.net/shan9liang/article/details/26396163#)[print](http://blog.csdn.net/shan9liang/article/details/26396163#)[?](http://blog.csdn.net/shan9liang/article/details/26396163#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/355521/fork)
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- <parent>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <relativePath>../gxpt/pom.xml</relativePath>
- </parent>
- <artifactId>gxpt_a_ear</artifactId>
- <packaging>ear</packaging>
- 
- <dependencies>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_entity_qx</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_tool</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_eao</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_eao_impl</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_mgr_qx_module</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_mgr_qx_module_impl</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_web_qx_module</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>war</type>
- </dependency>
- </dependencies>
- 
- <build>
- <plugins>
- <plugin>
- <artifactId>maven-ear-plugin</artifactId>
- <version>2.9</version>
- <configuration>
- <packagingIncludes>META-INF/**,**/gxpt_*.jar,**/gxpt_*.war</packagingIncludes>
- <jboss>
- <version>5</version>
- <module-order>strict</module-order>
- </jboss>
- <modules>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_entity_qx</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_tool</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_eao</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_eao_impl</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_mgr_qx_module</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_mgr_qx_module_impl</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <webModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_web_qx_module</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </webModule>
- </modules>
- </configuration>
- </plugin>
- </plugins>
- </build>
- </project>
**六、部署到JBoss**
1、首先确认你连接的仓库，默认的中央仓库上面是没有JBoss的包的，需要配置一下开源中国的私服库。详细请查看：[http://maven.oschina.net/help.html](http://maven.oschina.net/help.html)，官方给的配置很详细。
2、配置Cargo
**[html]**[view
 plain](http://blog.csdn.net/shan9liang/article/details/26396163#)[copy](http://blog.csdn.net/shan9liang/article/details/26396163#)[print](http://blog.csdn.net/shan9liang/article/details/26396163#)[?](http://blog.csdn.net/shan9liang/article/details/26396163#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/355521/fork)
- <plugin>
- <groupId>org.codehaus.cargo</groupId>
- <artifactId>cargo-maven2-plugin</artifactId>
- <version>1.4.5</version>
- <configuration>
- <container>
- <containerId>jboss51x</containerId>
- <type>remote</type>
- </container>
- <configuration>
- <type>runtime</type>
- <properties>
- <cargo.remote.username>admin</cargo.remote.username>
- <cargo.remote.password>admin</cargo.remote.password>
- <cargo.hostname>192.168.24.48</cargo.hostname>
- <cargo.rmi.port>1099</cargo.rmi.port>
- </properties>
- </configuration>
- </configuration>
- <dependencies>
- <dependency>
- <groupId>org.jboss.integration</groupId>
- <artifactId>jboss-profileservice-spi</artifactId>
- <version>5.1.0.GA</version>
- </dependency>
- <dependency>
- <groupId>org.jboss.jbossas</groupId>
- <artifactId>jboss-as-client</artifactId>
- <version>5.1.0.GA</version>
- <type>pom</type>
- </dependency>
- </dependencies>
- </plugin>
3、将此项目执行clean install后，然后就可以执行cargo:deploy了，通过此命令就可以将项目部署到远程JBoss中了。
需要注意的一点，如果用到了spring，通过jndi查找ejb，那么
由于EJB在EAR中被部署后，绑定的JNDI前面会加上EAR的包名，如：
之前的CommonEao部署之后的JNDI名字为：common_EaoBean/remote，而现在会变成：gxpt_a_ear/commonEaoBean/remote；
**七、总结：**
做j2ee企业级开发，构建EAR是一项基本功。EAR只是一种技术规范，根据不同的项目环境，会有不同的用法，在我们的项目中，适当的时候，会采用EAR作为作为组件的部署单位，让EAR包含一组EJB颗粒，起到分类的作用，同时也用于打包与组件相关联的辅助资源。配合Jenkins自动部署，使用maven
 cargo插件非常便利的就可以将项目部署到远程的jboss中。
## 这篇文章，主要是技术上的整理，用来mark一下，用的时候参考。
## 一、新建项目
新建一个空的Maven Project项目
![](https://img-blog.csdn.net/20140520203719359)
## 二、放入依赖
注：ear部署时如果里面有entity，会发生错误，所以不要把entity放到依赖项中，部署的时候先部实体，然后再部ear。
将需要打入EAR中的jar、war以dependencies-dependency的形式写到pom.xml文件中，如：
![](https://img-blog.csdn.net/20140520203822828)
## 三、配置加载顺序
现在打出来的ear包中的application中只有war的配置信息，而没有jar的配置信息，而我们如果想控制这些jar的配置信息，就必须把jar的信息配置到application.xml中，而application.xml是可以通过此插件生成的，所以在pom.xml文件中配置即可，配置方式如下：
![](https://img-blog.csdn.net/20140520203923984)
## 四、运行
如果现在打包，会报一个没有发现application.xml的错误，这个文件需要我们自己创建么？答案是不需要：
1.运行ear:
 generate-application-xml，会生成相应的application.xml文件，下面是我的pom生成的application.xml文件（格式有改动）：
![](https://img-blog.csdn.net/20140520204057500)
2.同时，此命令还生成一个jboss-app.xml的文件，里面简单一一句话，是说要JBoss按照application.xml里面的声明顺序加载
![](https://img-blog.csdn.net/20140520204134187)
3.现在可以执行生成ear的命令了：ear:ear，坐等成功吧~~
## 五、更改pom的packaging
现在如果你直接Install的话，会发现并不是打成的ear，而是打了jar，需要将packaging改为ear即可。
但改后会发现pom上面报错，你在eclipse应用广场搜一下m2e-wtp，然后安装即可。
## 注：我的完整pom.xml
**[html]**[view
 plain](http://blog.csdn.net/shan9liang/article/details/26396163#)[copy](http://blog.csdn.net/shan9liang/article/details/26396163#)[print](http://blog.csdn.net/shan9liang/article/details/26396163#)[?](http://blog.csdn.net/shan9liang/article/details/26396163#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/355521/fork)
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- <parent>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <relativePath>../gxpt/pom.xml</relativePath>
- </parent>
- <artifactId>gxpt_a_ear</artifactId>
- <packaging>ear</packaging>
- 
- <dependencies>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_entity_qx</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_tool</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_eao</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_eao_impl</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_mgr_qx_module</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_mgr_qx_module_impl</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>jar</type>
- </dependency>
- <dependency>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_web_qx_module</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <type>war</type>
- </dependency>
- </dependencies>
- 
- <build>
- <plugins>
- <plugin>
- <artifactId>maven-ear-plugin</artifactId>
- <version>2.9</version>
- <configuration>
- <packagingIncludes>META-INF/**,**/gxpt_*.jar,**/gxpt_*.war</packagingIncludes>
- <jboss>
- <version>5</version>
- <module-order>strict</module-order>
- </jboss>
- <modules>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_entity_qx</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_tool</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_eao</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_common_eao_impl</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_mgr_qx_module</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <jarModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_mgr_qx_module_impl</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </jarModule>
- <webModule>
- <groupId>com.tgb</groupId>
- <artifactId>gxpt_web_qx_module</artifactId>
- <includeInApplicationXml>true</includeInApplicationXml>
- </webModule>
- </modules>
- </configuration>
- </plugin>
- </plugins>
- </build>
- </project>
**六、部署到JBoss**
1、首先确认你连接的仓库，默认的中央仓库上面是没有JBoss的包的，需要配置一下开源中国的私服库。详细请查看：[http://maven.oschina.net/help.html](http://maven.oschina.net/help.html)，官方给的配置很详细。
2、配置Cargo
**[html]**[view
 plain](http://blog.csdn.net/shan9liang/article/details/26396163#)[copy](http://blog.csdn.net/shan9liang/article/details/26396163#)[print](http://blog.csdn.net/shan9liang/article/details/26396163#)[?](http://blog.csdn.net/shan9liang/article/details/26396163#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/355521/fork)
- <plugin>
- <groupId>org.codehaus.cargo</groupId>
- <artifactId>cargo-maven2-plugin</artifactId>
- <version>1.4.5</version>
- <configuration>
- <container>
- <containerId>jboss51x</containerId>
- <type>remote</type>
- </container>
- <configuration>
- <type>runtime</type>
- <properties>
- <cargo.remote.username>admin</cargo.remote.username>
- <cargo.remote.password>admin</cargo.remote.password>
- <cargo.hostname>192.168.24.48</cargo.hostname>
- <cargo.rmi.port>1099</cargo.rmi.port>
- </properties>
- </configuration>
- </configuration>
- <dependencies>
- <dependency>
- <groupId>org.jboss.integration</groupId>
- <artifactId>jboss-profileservice-spi</artifactId>
- <version>5.1.0.GA</version>
- </dependency>
- <dependency>
- <groupId>org.jboss.jbossas</groupId>
- <artifactId>jboss-as-client</artifactId>
- <version>5.1.0.GA</version>
- <type>pom</type>
- </dependency>
- </dependencies>
- </plugin>
3、将此项目执行clean install后，然后就可以执行cargo:deploy了，通过此命令就可以将项目部署到远程JBoss中了。
需要注意的一点，如果用到了spring，通过jndi查找ejb，那么
由于EJB在EAR中被部署后，绑定的JNDI前面会加上EAR的包名，如：
之前的CommonEao部署之后的JNDI名字为：common_EaoBean/remote，而现在会变成：gxpt_a_ear/commonEaoBean/remote；
**七、总结：**
做j2ee企业级开发，构建EAR是一项基本功。EAR只是一种技术规范，根据不同的项目环境，会有不同的用法，在我们的项目中，适当的时候，会采用EAR作为作为组件的部署单位，让EAR包含一组EJB颗粒，起到分类的作用，同时也用于打包与组件相关联的辅助资源。配合Jenkins自动部署，使用maven
 cargo插件非常便利的就可以将项目部署到远程的jboss中。

