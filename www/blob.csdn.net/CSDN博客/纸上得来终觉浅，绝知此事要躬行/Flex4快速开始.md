# Flex4快速开始 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月26日 21:44:40[boonya](https://me.csdn.net/boonya)阅读数：1488标签：[flex4																[web框架																[插件																[flash builder																[源代码](https://so.csdn.net/so/search/s.do?q=源代码&t=blog)
个人分类：[Flex/Flash](https://blog.csdn.net/boonya/article/category/1768619)





转自：[http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild)



flex4_quick_start_guild


Updated Sep 11, 2010 by [hhlai1...@gmail.com](http://code.google.com/u/104071195869840892242/)






- [介绍](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#%E4%BB%8B%E7%BB%8D)
- [环境介绍](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#%E7%8E%AF%E5%A2%83%E4%BB%8B%E7%BB%8D)
- [flex4+hibernate详细搭建介绍](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#flex4+hibernate%E8%AF%A6%E7%BB%86%E6%90%AD%E5%BB%BA%E4%BB%8B%E7%BB%8D)- [一.初始项目配置](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#%E4%B8%80.%E5%88%9D%E5%A7%8B%E9%A1%B9%E7%9B%AE%E9%85%8D%E7%BD%AE)- [1.1 创建名称为flex4demo的web project](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.1_%E5%88%9B%E5%BB%BA%E5%90%8D%E7%A7%B0%E4%B8%BAflex4demo%E7%9A%84web_project)
- [1.2 拷贝rapid-framework至项目根目录](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.2_%E6%8B%B7%E8%B4%9Drapid-framework%E8%87%B3%E9%A1%B9%E7%9B%AE%E6%A0%B9%E7%9B%AE%E5%BD%95)
- [1.3 设置项目的编码为utf-8](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.3_%E8%AE%BE%E7%BD%AE%E9%A1%B9%E7%9B%AE%E7%9A%84%E7%BC%96%E7%A0%81%E4%B8%BAutf-8)
- [1.4 设置源码路径](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.4_%E8%AE%BE%E7%BD%AE%E6%BA%90%E7%A0%81%E8%B7%AF%E5%BE%84)
- [1.5 将generator/lib的jars加入classpath](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.5_%E5%B0%86generator/lib%E7%9A%84jars%E5%8A%A0%E5%85%A5classpath)
- [1.6 安装插件flex4_cairngorm及dao_hibernate](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.6_%E5%AE%89%E8%A3%85%E6%8F%92%E4%BB%B6flex4_cairngorm%E5%8F%8Adao_hibernate)
- [1.7 在mysql数据库中创建表user_info](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.7_%E5%9C%A8mysql%E6%95%B0%E6%8D%AE%E5%BA%93%E4%B8%AD%E5%88%9B%E5%BB%BA%E8%A1%A8user_info)
- [1.8 修改生成器配置文件generator.properties的jdbc数据库连接信息
 其它属性暂不修改](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.8_%E4%BF%AE%E6%94%B9%E7%94%9F%E6%88%90%E5%99%A8%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6generator.properties%E7%9A%84jdbc%E6%95%B0%E6%8D%AE)
- [1.9 运行代码生成器 GeneratorMain.java](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.9_%E8%BF%90%E8%A1%8C%E4%BB%A3%E7%A0%81%E7%94%9F%E6%88%90%E5%99%A8_GeneratorMain.java)
- [1.10 拷贝生成的代码至项目根目录,并刷新项目](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#1.10_%E6%8B%B7%E8%B4%9D%E7%94%9F%E6%88%90%E7%9A%84%E4%BB%A3%E7%A0%81%E8%87%B3%E9%A1%B9%E7%9B%AE%E6%A0%B9%E7%9B%AE%E5%BD%95,%E5%B9%B6%E5%88%B7%E6%96%B0%E9%A1%B9%E7%9B%AE)

- [二.flex相关操作](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#%E4%BA%8C.flex%E7%9B%B8%E5%85%B3%E6%93%8D%E4%BD%9C)- [2.1 增加Flex Project Nature(1)](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#2.1_%E5%A2%9E%E5%8A%A0Flex_Project_Nature(1))
- [2.2 增加Flex Project Nature(2)](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#2.2_%E5%A2%9E%E5%8A%A0Flex_Project_Nature(2))
- [2.3 增加Flex Project Nature(3)](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#2.3_%E5%A2%9E%E5%8A%A0Flex_Project_Nature(3))
- [2.4 设置Flex Build Path](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#2.4_%E8%AE%BE%E7%BD%AEFlex_Build_Path)
- [2.5 设置Flex Applications](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#2.5_%E8%AE%BE%E7%BD%AEFlex_Applications)
- [2.6 flex源代码的相关配置修改](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#2.6_flex%E6%BA%90%E4%BB%A3%E7%A0%81%E7%9A%84%E7%9B%B8%E5%85%B3%E9%85%8D%E7%BD%AE%E4%BF%AE%E6%94%B9)

- [三.部署](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#%E4%B8%89.%E9%83%A8%E7%BD%B2)- [3.1 修改java_src/spring/applicationContext-datasource.xml的数据库连接信息](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#3.1_%E4%BF%AE%E6%94%B9java_src/spring/applicationContext-datasource.xml%E7%9A%84%EF%BF%BD%EF%BF%BD)
- [3.2 部署应用flex4demo至tomcat](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#3.2_%E9%83%A8%E7%BD%B2%E5%BA%94%E7%94%A8flex4demo%E8%87%B3tomcat)
- [3.3 启动tomcat,访问URL:](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#3.3_%E5%90%AF%E5%8A%A8tomcat,%E8%AE%BF%E9%97%AEURL:)
- [3.49（附加）改动main.mxml的源码，再部署,访问URL:](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#3.49%EF%BC%88%E9%99%84%E5%8A%A0%EF%BC%89%E6%94%B9%E5%8A%A8main.mxml%E7%9A%84%E6%BA%90%E7%A0%81%EF%BC%8C%E5%86%8D%E9%83%A8%E7%BD%B2,%E8%AE%BF%E9%97%AEURL:)

- [四.flex4强大的功能介绍，IDE内置代码生成器](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#%E5%9B%9B.flex4%E5%BC%BA%E5%A4%A7%E7%9A%84%E5%8A%9F%E8%83%BD%E4%BB%8B%E7%BB%8D%EF%BC%8CIDE%E5%86%85%E7%BD%AE%E4%BB%A3%E7%A0%81%E7%94%9F%E6%88%90%E5%99%A8)- [选择数据服务](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#%E9%80%89%E6%8B%A9%E6%95%B0%E6%8D%AE%E6%9C%8D%E5%8A%A1)
- [Tips:](http://code.google.com/p/rapid-framework/wiki/flex4_quick_start_guild#Tips:)





# 介绍

以下将详细描述flex4+hibernate的项目搭建方法(本插件由 [*随变混混*](http://code.google.com/u/hhlai1990/) 贡献)

# 环境介绍
- IDE: MyEclipse 8.5(flex4插件版安装需至少Myeclipse7.0以上)
- 数据库: Mysql 5
- JDK: 1.6
- flash builder4 eclipse插件版本
- flex sdk 4.0

# flex4+hibernate详细搭建介绍

优酷视频教程[http://v.youku.com/v_show/id_XMTk5NTYwODcy.html](http://v.youku.com/v_show/id_XMTk5NTYwODcy.html)

## 一.初始项目配置

### 1.1 创建名称为flex4demo的web project

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/1.PNG)

### 1.2 拷贝rapid-framework至项目根目录

将rapidframework.zip解压出来的内容全部拷贝至项目根目录,拷贝文件如下.

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/2.PNG)

### 1.3 设置项目的编码为utf-8

右键点击项目,修改项目属性

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/3.PNG)

### 1.4 设置源码路径

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/4.PNG)

### 1.5 将generator/lib的jars加入classpath

点击"Add jars"按钮将生成器依赖的jar包需要加入classpath

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/5.PNG)

项目文件结构

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/6.PNG)

### 1.6 安装插件flex4_cairngorm及dao_hibernate

项目为插件结构,web框架及dao层需要安装(可以安装不同的web框架或是dao层)

打开plugins/build.xml,并打开MyEclipse的 Outline 视图

现运行 install_flex4_cairngorm及 install_dao_hibernate 任务

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/7.PNG)

### 1.7 在mysql数据库中创建表user_info

在test数据库创建表,运行如下sql
CREATE TABLE user_info (
  user_id bigint  PRIMARY KEY AUTO_INCREMENT,
  username varchar(50)notnull,
  password varchar(50),
  birth_date date,
  sex int,
  age int) ENGINE=InnoDB DEFAULT CHARSET=utf8;
### 1.8 修改生成器配置文件generator.properties的jdbc数据库连接信息 其它属性暂不修改
jdbc.url=jdbc:mysql://localhost:3306/test?useUnicode=true&characterEncoding=UTF-8 
jdbc.driver=com.mysql.jdbc.Driver 
jdbc.username=root 
jdbc.password=0000
### 1.9 运行代码生成器GeneratorMain.java

以application的方式运行cn.org.rapid_framework.generator.GeneratorMain,直接修改源码,在便生成不同的表.
publicclassGeneratorMain{/**
         * 请直接修改以下代码调用不同的方法以执行相关生成任务.
         */publicstaticvoid main(String[] args)throwsException{GeneratorFacade g =newGeneratorFacade();//              g.printAllTableNames();                         //打印数据库中的表名称
                
                g.clean();//删除生成器的输出目录
                g.generateByTable("user_info","template");//通过数据库表生成文件,注意: oracle 需要指定schema及注意表名的大小写.//打开文件夹Runtime.getRuntime().exec("cmd.exe /c start D:\\webapp-generator-output");}}
![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/9.PNG)

### 1.10 拷贝生成的代码至项目根目录,并刷新项目

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/10.PNG)

最好删除不需要的文件夹 ![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/11.PNG)

## 二.flex相关操作

### 2.1 增加Flex Project Nature(1)

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/12.PNG)

### 2.2 增加Flex Project Nature(2)

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/13.PNG)

### 2.3 增加Flex Project Nature(3)

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/14.PNG)

### 2.4 设置Flex Build Path

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/15.PNG) 改变flex4的主题为Halo,以便兼容flex3的组件特性 ![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/16.PNG)

### 2.5 设置Flex Applications

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/17.PNG)

### 2.6 flex源代码的相关配置修改

根据生成器生成的flex_src/readme.txt执行如下相关操作.
//copy follow code to appcommon.flex.GlobalControler.as: cairngorm中event与command的映射import com.company.project.user_info.UserInfoRegister;// UserInfo event and command mappingUserInfoRegister.initialiseCommands(this)；//copy follow code to appcommon.flex.Services.mxml<!--userInfoService--><mx:RemoteObject endpoint="../messagebroker/amf"
                                         id="userInfoService"
                                         destination="userInfoService"
                                         showBusyCursor="true"/>
![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/18.PNG)

## 三.部署

### 3.1 修改java_src/spring/applicationContext-datasource.xml的数据库连接信息

该配置是应用需要连接的数据库,与生成器的数据库连接是分开配置的,与第1.8步的数据库连接配置相同即可
<beanid="dataSource"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close"><propertyname="driverClassName"value="com.mysql.jdbc.Driver"/><propertyname="url"value="jdbc:mysql://localhost:3306/test?useUnicode=true&characterEncoding=UTF-8"/><propertyname="username"value="root"/><propertyname="password"value="123456"/></bean>
### 3.2 部署应用flex4demo至tomcat

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/20.PNG)

### 3.3 启动tomcat,访问URL:[http://localhost:8899/flex4demo/flex/UserInfoIndex.html](http://localhost:8899/flex4demo/flex/UserInfoIndex.html)

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/21.PNG)

### 3.49（附加）改动main.mxml的源码，再部署,访问URL:[http://localhost:8899/flex4demo/flex/main.html](http://localhost:8899/flex4demo/flex/main.html)

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/22.PNG)

## 四.flex4强大的功能介绍，IDE内置代码生成器

### 选择数据服务

![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/23.PNG)![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/24.PNG)![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/25.PNG)![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/26.PNG)![](http://rapid-framework.googlecode.com/svn/trunk/images/doc/flex4_step_by_step/27.PNG)

### Tips:

1.由于官方的Blazeds4.0会出现Nan=>0的错误，rapid修改了blazeds的flex-core.jar包,修正为Nan=>null.

2.若需要将flex4与spring的结合开发,建议不要使用IDE自动生成代码功能,此功能有BUG，官方正在修复中.](https://so.csdn.net/so/search/s.do?q=flash builder&t=blog)](https://so.csdn.net/so/search/s.do?q=插件&t=blog)](https://so.csdn.net/so/search/s.do?q=web框架&t=blog)](https://so.csdn.net/so/search/s.do?q=flex4&t=blog)




