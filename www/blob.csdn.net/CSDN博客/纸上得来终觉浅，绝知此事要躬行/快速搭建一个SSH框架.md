# 快速搭建一个SSH框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月26日 21:37:02[boonya](https://me.csdn.net/boonya)阅读数：2624








quick_start_build


Updated Jul 21, 2010 by [bad...@gmail.com](http://code.google.com/u/112380455388073357672/)






- [介绍](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#%E4%BB%8B%E7%BB%8D)
- [环境介绍](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#%E7%8E%AF%E5%A2%83%E4%BB%8B%E7%BB%8D)
- [struts2+spring+hibernate详细搭建介绍](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#struts2+spring+hibernate%E8%AF%A6%E7%BB%86%E6%90%AD%E5%BB%BA%E4%BB%8B%E7%BB%8D)- [1.创建一个 struts2_demo的 web project](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#1.%E5%88%9B%E5%BB%BA%E4%B8%80%E4%B8%AA_struts2_demo%E7%9A%84_web_project)
- [2.拷贝rapid-framework至项目根目录](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#2.%E6%8B%B7%E8%B4%9Drapid-framework%E8%87%B3%E9%A1%B9%E7%9B%AE%E6%A0%B9%E7%9B%AE%E5%BD%95)
- [3.设置项目的编码为utf-8](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#3.%E8%AE%BE%E7%BD%AE%E9%A1%B9%E7%9B%AE%E7%9A%84%E7%BC%96%E7%A0%81%E4%B8%BAutf-8)
- [4.设置源码路径](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#4.%E8%AE%BE%E7%BD%AE%E6%BA%90%E7%A0%81%E8%B7%AF%E5%BE%84)
- [5.将generator/lib的jars加入classpath](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#5.%E5%B0%86generator/lib%E7%9A%84jars%E5%8A%A0%E5%85%A5classpath)
- [6.安装插件 hibernate及struts2](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#6.%E5%AE%89%E8%A3%85%E6%8F%92%E4%BB%B6_hibernate%E5%8F%8Astruts2)
- [7.在mysql数据库中创建表user_info](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#7.%E5%9C%A8mysql%E6%95%B0%E6%8D%AE%E5%BA%93%E4%B8%AD%E5%88%9B%E5%BB%BA%E8%A1%A8user_info)
- [8.修改生成器配置文件 generator.xml 的jdbc数据库连接信息 其它属性暂不修改](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#8.%E4%BF%AE%E6%94%B9%E7%94%9F%E6%88%90%E5%99%A8%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6_generator.xml_%E7%9A%84jdbc%E6%95%B0%E6%8D%AE%E5%BA%93%E8%BF%9E%EF%BF%BD)
- [9.运行代码生成器 GeneratorMain.java](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#9.%E8%BF%90%E8%A1%8C%E4%BB%A3%E7%A0%81%E7%94%9F%E6%88%90%E5%99%A8_GeneratorMain.java)
- [10.拷贝生成的代码至项目根目录,并刷新项目](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#10.%E6%8B%B7%E8%B4%9D%E7%94%9F%E6%88%90%E7%9A%84%E4%BB%A3%E7%A0%81%E8%87%B3%E9%A1%B9%E7%9B%AE%E6%A0%B9%E7%9B%AE%E5%BD%95,%E5%B9%B6%E5%88%B7%E6%96%B0%E9%A1%B9%E7%9B%AE)
- [11.修改java_src/spring/applicationContext-datasource.xml的数据库连接信息](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#11.%E4%BF%AE%E6%94%B9java_src/spring/applicationContext-datasource.xml%E7%9A%84%E6%95%B0)
- [12.部署应用struts2_demo至tomcat](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#12.%E9%83%A8%E7%BD%B2%E5%BA%94%E7%94%A8struts2_demo%E8%87%B3tomcat)
- [13.启动tomcat,访问URL:](http://code.google.com/p/rapid-framework/wiki/quick_start_guild#13.%E5%90%AF%E5%8A%A8tomcat,%E8%AE%BF%E9%97%AEURL:)




# 介绍

以下将详细描述struts2+hibernate的项目搭建方法,其它的项目组合如(springmvc+iBatis,struts2+jdbc)可以参考如下步骤.

注意: 请使用firefox或是IE7阅读本wiki,**IE6显示有问题**

# 环境介绍
- IDE: MyEclipse 6.5
- 数据库: Mysql 5
- JDK: 1.5

# struts2+spring+hibernate详细搭建介绍

## 1.创建一个 struts2_demo的 web project

注意:Web root foler的值为**web**

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/v3.5/1.jpg)

## 2.拷贝rapid-framework至项目根目录

将rapidframework.zip解压出来的内容全部拷贝至项目根目录,拷贝完效果如下.

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/v3.5/2.jpg)

## 3.设置项目的编码为utf-8

右键点击项目,修改项目属性

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/set_resource_encoding.jpg)

## 4.设置源码路径

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/v3.5/4.jpg)

## 5.将generator/lib的jars加入classpath

点击"Add jars"按钮将生成器依赖的jar包需要加入classpath

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/add_generator_jars_to_classpath.jpg)

注：如果使用Eclipse，需要将web/WEB-INF/lib下jars和Tomcat的servlet-api.jar加入。

## 6.安装插件 hibernate及struts2

项目为插件结构,web框架及dao层需要安装(可以安装不同的web框架或是dao层)

打开plugins/build.xml,并打开eclipse的 **Outline** 视图

现运行 **install_dao_hibernate** 及 **install_web_struts2** 任务,请注意安装顺序

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/run_plugin_installer.jpg)

## 7.在mysql数据库中创建表user_info

在test数据库创建表,运行如下sql
CREATE TABLE user_info (
  user_id bigint  PRIMARY KEY AUTO_INCREMENT,
  username varchar(50)notnull,
  password varchar(50),
  birth_date date,
  sex int,
  age int) ENGINE=InnoDB DEFAULT CHARSET=utf8;
注：需要自行将数据库中插入相应的数据。

## 8.修改生成器配置文件generator.xml的jdbc数据库连接信息 其它属性暂不修改
<entrykey="jdbc.username">root</entry><entrykey="jdbc.password">123456</entry><entrykey="jdbc.url">jdbc:mysql://localhost:3306/test?useUnicode=true&characterEncoding=UTF-8</entry><entrykey="jdbc.driver">com.mysql.jdbc.Driver</entry>
## 9.运行代码生成器 GeneratorMain.java

以application的方式运行cn.org.rapid_framework.generator.GeneratorMain,直接修改源码,在便生成不同的表.

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/run_generator.jpg)

## 10.拷贝生成的代码至项目根目录,并刷新项目

生成器生成的文件默认会输出在d:/webapp-generator-output

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/copy_generate_output_to_project.jpg)

注：缺省Hibernate生成代码为注解方式，可以手工选择other目录下生成的XML方式代码进行替换。

## 11.修改java_src/spring/applicationContext-datasource.xml的数据库连接信息

该配置是应用需要连接的数据库,与生成器的数据库连接是分开配置的,与**第8步**的数据库连接配置相同即可
<beanid="dataSource"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close"><propertyname="driverClassName"value="com.mysql.jdbc.Driver"/><propertyname="url"value="jdbc:mysql://localhost:3306/test?useUnicode=true&characterEncoding=UTF-8"/><propertyname="username"value="root"/><propertyname="password"value="123456"/></bean>
注：如果生成代码缺省包不是以com开头，需要修改spring下相关的XML文件。

## 12.部署应用struts2_demo至tomcat

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/deploy_app_to_tomcat.jpg)

## 13.启动tomcat,访问URL:[http://localhost:8080/struts2_demo/pages/UserInfo/list.do](http://localhost:8080/struts2_demo/pages/UserInfo/list.do)

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/struts2_hibernate_step_by_step/view_jsp_list.jpg)

### 如果需要查看不同的组合，请查看[dao及web插件切换指南](http://code.google.com/p/rapid-framework/wiki/rapid_plugin_switch_desc)





