# MyEclipse下安装MyBatis Generator代码反向生成工具 - 深之JohnChen的专栏 - CSDN博客

2017年11月08日 22:54:37[byxdaz](https://me.csdn.net/byxdaz)阅读数：2948


一、安装MyBatisGenerator代码反向生成工具

1、离线方式：

在[https://github.com/mybatis/generator/tree/master/eclipse/UpdateSite/](http://mybatis.googlecode.com/svn/sub-projects/generator/trunk/eclipse/UpdateSite/)下载

[features/](http://mybatis.googlecode.com/svn/sub-projects/generator/trunk/eclipse/UpdateSite/features/)

[plugins/](http://mybatis.googlecode.com/svn/sub-projects/generator/trunk/eclipse/UpdateSite/plugins/)

里面所有的jar包，新建一个mybatis-generator文件夹，把features跟plugins都丢到mybatis-generator文件夹中，把mybatis-generator文件夹移到D:\MyEclipse10_7\MyEclipse10\dropins中，新建一个mbg.link,内容如下：

path=D:\MyEclipse10_7\MyEclipse10\dropins\mybatis-generator

重启MyEclipse

![](https://img-blog.csdn.net/20171108225810852)

2、在线方式

既然离线安装不成功，可以选择在线安装

1。选择 Help->Install from site...

2。在弹出的对话框中点击右上角的Add按钮。

3。在弹出的对话框中输入

Name:mybatis

Location:https://dl.bintray.com/mybatis/mybatis-generator

(location中可以输入以上地址，也可以点击Archive，选择本地已经下载好的jar、zip压缩文件：org.mybatis.generator.eclipse.site-1.3.5.201609070108.zip)

点击OK

4。在Work with 选择mybatis - https://dl.bintray.com/mybatis/mybatis-generator或者自己选择的本地路径(个人建议先下载好，选择本地安装，这样速度快,在线安装太慢)

5。勾选MyBatis Generator点击next

6。之后安装完成，重启myeclipse即可。

二、MyBatisGenerator工具使用

1、Eclipse方式

1）、新建工程、将组件和将配置文件config.xml放到对应的目录 

![](https://img-blog.csdn.net/20171108225828224)

2）、在main函数中写代码运行

public static void main(String[] args)throws Exception {

   List<String> warnings = new ArrayList<String>();

   boolean overwrite = true;

   //指向逆向工程配置文件

   File configFile = new File("generatorConfig.xml"); 

   ConfigurationParser cp = new ConfigurationParser(warnings);

   Configuration config = cp.parseConfiguration(configFile);

   DefaultShellCallback callback = new DefaultShellCallback(overwrite);

   MyBatisGenerator myBatisGenerator = new MyBatisGenerator(config,

           callback, warnings);

   myBatisGenerator.generate(null);

}

3）、以application的方式运行就可以了

2、MyEclipse方式

1）、用myeclipse创建java工程． 

![](https://img-blog.csdn.net/20171108225846474)

将mysql-connector-java-5.1.18.jar放到指定的目录项目，并且配置generatorConfig.xml．

<?xml version="1.0"encoding="UTF-8" ?>

<!DOCTYPE generatorConfiguration PUBLIC"-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN""http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd" >

<generatorConfiguration>

   <classPathEntry

       location="/opt/app-w/gitresp/testcore/src/main/webapp/WEB-INF/lib/mysql-connector-java-5.1.18.jar"/>

   <context id="context1">

       <jdbcConnection driverClass="com.mysql.jdbc.Driver"

           connectionURL="jdbc:mysql://ip:3306/xx?useUnicode=true&characterEncoding=UTF-8"

           userId="root"password="123456" />

       <javaModelGenerator targetPackage="model"

           targetProject="testcore" />

       <sqlMapGenerator targetPackage="mapper"targetProject="testcore" />

       <!--    数据库　表名　生成名 -->

       <javaClientGenerator targetPackage="client"

           targetProject="testcore" type="XMLMAPPER" />

       <table schema="ｘｘ" tableName="ｘｘ"

           domainObjectName="ｘｘ">

       </table>

   </context>

</generatorConfiguration>

2）、到github上下载mybatis/generator插件，把目录generator/eclipse/UpdateSite下面的features和plugins拷贝到myeclipse相应的目录下面。

3）、这样就可以右击在myeclipse中的generatorConfig.xml,找到生成mybatis artifacts选项,就可以生成mapper,model,client,如下图: 
![](https://img-blog.csdn.net/20171108225910382)

如果出现错误:Unexpectederror while running MyBatis Generator. Exception getting JDBC Driver 

这个就说明是<classPathEntry 

location="/opt/app-w/gitresp/testcore/src/main/webapp/WEB-INF/lib/mysql-connector-java-5.1.18.jar"/> 

这里路径有问题,配置好就可以解决!

