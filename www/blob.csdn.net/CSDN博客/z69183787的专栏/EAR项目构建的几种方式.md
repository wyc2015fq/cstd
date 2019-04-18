# EAR项目构建的几种方式 - z69183787的专栏 - CSDN博客
2015年11月24日 13:44:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3370
PS:说实话，在写这篇帖子之前，我也没用过EAR，因此该贴仅是记录学习过程用~有什么不对的地方，或者欠缺的，还请各位看官斧正，先谢过各位了~~
一、基于传统WebProject方式
在eclipse中，右键new > project > Dynamic web project ，
在弹出的对话框中输入项目名称testWeb，注意，在EAR membership中勾选上“Add project to an EAR”，并输入准备创建的EAR的名称，例如TestEAR，
再创建几个Web project，此时可以选择需要加入的EAR的名称，这里，我们都选择TestEAR，
每个web project，我们都可以随意的写些测试页面，以供测试使用。
在TestEAR项目上，右键 > properties 弹出的面板中，进入 project facets条目，可以看到，该项目特征为EAR，且被选中。
在该EAR项目目录下的EarContent文件夹下创建META-INF/application.xml，该文件是设置ear打包时需要包含相关资源的配置文件。样例如下：
**[html]**[view
 plain](http://blog.csdn.net/shangyue1110/article/details/45506829#)[copy](http://blog.csdn.net/shangyue1110/article/details/45506829#)[print](http://blog.csdn.net/shangyue1110/article/details/45506829#)[?](http://blog.csdn.net/shangyue1110/article/details/45506829#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <applicationid="testEAR"version="1.4"
- xmlns="http://java.sun.com/xml/ns/j2ee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee http://java.sun.com/xml/ns/j2ee/application_1_4.xsd">
- <display-name>TestEAR</display-name>
- <moduleid="WebModule_testA">
- <web>
- <web-uri>testA.war</web-uri>
- <context-root>.testA</context-root>
- </web>
- </module>
- 
- <moduleid="WebModule_testB">
- <web>
- <web-uri>testB.war</web-uri>
- <context-root>.testB</context-root>
- </web>
- </module>
- 
- <moduleid="WebModule_testC">
- <web>
- <web-uri>testC.war</web-uri>
- <context-root>.testC</context-root>
- </web>
- </module>
- </application>
display-name设置部署显示的名称，module定义一个被包含的资源，可以是EJB的jar，也可以是一个子系统的war，上面这个例子中都是引入的war，因此用web作为标签。web-uri声明被引入的war路径，context-root声明访问路径（相对于该EAR项目）。
在TestEAR项目上右键 export 选择EAR file，即可导出ear文件，用压缩包程序打开TestEAR.ear文件可以看到该ear包含的war文件。
二、基于maven-ear-plugin插件构建EAR项目
1、创建一个maven webapp，将packaging类型选为ear
 <packaging>ear</packaging>
2、添加包含的war、jar等依赖，如下：
**[html]**[view
 plain](http://blog.csdn.net/shangyue1110/article/details/45506829#)[copy](http://blog.csdn.net/shangyue1110/article/details/45506829#)[print](http://blog.csdn.net/shangyue1110/article/details/45506829#)[?](http://blog.csdn.net/shangyue1110/article/details/45506829#)
- <dependency>
- <groupId>com.test</groupId>
- <artifactId>test</artifactId>
- <version>0.0.1</version>
- <type>war</type>
- </dependency>
3、添加maven-ear-plugin插件，并配置
**[html]**[view
 plain](http://blog.csdn.net/shangyue1110/article/details/45506829#)[copy](http://blog.csdn.net/shangyue1110/article/details/45506829#)[print](http://blog.csdn.net/shangyue1110/article/details/45506829#)[?](http://blog.csdn.net/shangyue1110/article/details/45506829#)
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-ear-plugin</artifactId>
- <version>2.10</version>
- <configuration>
- <!-- 打包指定目录到lib -->
- <defaultLibBundleDir>lib/</defaultLibBundleDir>
- <!-- 将多个war的共享包提取到父级别 -->
- <skinnyWars>true</skinnyWars>
- <modules>
- <webModule>
- <moduleId>webdemo-framework</moduleId>
- <groupId>com.test</groupId>
- <artifactId>framework</artifactId>
- <bundleFileName>test-framework.war</bundleFileName>
- </webModule>
- <webModule>
- <moduleId>webdemo-test</moduleId>
- <groupId>com.test</groupId>
- <artifactId>test</artifactId>
- <bundleFileName>test.war</bundleFileName>
- </webModule>
- </modules>
- </configuration>
- 
- </plugin>
4、很大可能在配置完上面的内容后，pom文件会报错，例如：
**Maven Project Build Lifecycle Mapping Problem**
Plugin execution not covered by lifecycle configuration: org.apache.maven.plugins:maven-ear-plugin:2.10:generate-application-xml (execution: default-generate-application-xml, phase: generate-resources)
解决方案如下：（参考帖子>>[传送门](http://ljhzzyx.blog.163.com/blog/static/383803122013440345857/)）
在plugins同级增加一组pluginManagement标签，在插件管理中，对lifecycle-mapping进行配置
**[html]**[view
 plain](http://blog.csdn.net/shangyue1110/article/details/45506829#)[copy](http://blog.csdn.net/shangyue1110/article/details/45506829#)[print](http://blog.csdn.net/shangyue1110/article/details/45506829#)[?](http://blog.csdn.net/shangyue1110/article/details/45506829#)
- <build>
- 
- <pluginManagement>
- <plugins>
- <plugin>
- <groupId>org.eclipse.m2e</groupId>
- <artifactId>lifecycle-mapping</artifactId>
- <version>1.0.0</version>
- <configuration>
- <lifecycleMappingMetadata>
- <pluginExecutions>
- <pluginExecution>
- <pluginExecutionFilter>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-ear-plugin</artifactId>
- <versionRange>[2.4.0,)</versionRange>
- <goals>
- <goal>generate-application-xml</goal>
- </goals>
- </pluginExecutionFilter>
- <action>
- <ignore/>
- </action>
- </pluginExecution>
- </pluginExecutions>
- </lifecycleMappingMetadata>
- </configuration>
- </plugin>
- </plugins>
- </pluginManagement>
- <plugins>
- 
- </plugins>
- </build>
如此，可解除上述错误~
其他参考：
[maven开发ear](http://blog.csdn.net/conquer0715/article/details/44805141)
[使用Maven构建和部署J2EE应用程序的EAR文件](http://blog.csdn.net/shan9liang/article/details/26396163)
