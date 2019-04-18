# Hibernate实例--XML配置表映射 - 深之JohnChen的专栏 - CSDN博客

2017年08月12日 11:36:42[byxdaz](https://me.csdn.net/byxdaz)阅读数：446


本实例使用*.hbm.xml的方式实现一个实体类与数据库表持久化操作。

一、xxx.hbm.xml介绍

<?xml version="1.0"?>  

<!DOCTYPE hibernate-mapping PUBLIC   

   "-//Hibernate/Hibernate Mapping DTD 3.0//EN"  

   "http://www.hibernate.org/dtd/hibernate-mapping-3.0.dtd">  

<!--  

   <hibernate-mapping>一般不去配置，采用默认即可。  

   default-cascade="none"：默认的级联风格，表与表联动。  

   default-lazy="true"：默认延迟加载  

 -->  

<hibernate-mapping>  

   <!--   

       <class>：使用class元素定义一个持久化类。  

       name="cn.javass.user.vo.UserModel"：持久化类的java全限定名；  

       table="tbl_user"：对应数据库表名；  

       mutable="true"：默认为true，设置为false时则不可以被应用程序更新或删除；  

       dynamic-insert="false"：默认为false，动态修改那些有改变过的字段，而不用修改所有字段；  

       dynamic-update="false"：默认为false，动态插入非空值字段；  

       select-before-update="false"：默认为false，在修改之前先做一次查询，与用户的值进行对比，有变化都会真正更新；  

       optimistic-lock="version"：默认为version(检查version/timestamp字段)，取值：all(检查全部字段)、dirty(只检查修改过的字段)、 

                                   none(不使用乐观锁定)，此参数主要用来处理并发，每条值都有固定且唯一的版本，版本为最新时才能执行操作；  

    -->  

   <class name="cn.javass.user.vo.UserModel"table="tbl_user" dynamic-insert="true"dynamic-update="true" optimistic-lock="version">  

       <!--   

           <id>：定义了该属性到数据库表主键字段的映射。  

           name="userId"：标识属性的名字；  

           column="userId"：表主键字段的名字，如果不填写与name一样；  

        -->  

       <id name="userId"> 

           <!-- <generator>：指定主键由什么生成，推荐使用uuid（随机生成唯一通用的表示符，实体类的ID必须是String），

                                                    native（让数据库自动选择用什么生成（根据底层数据库的能力选择identity，sequence或hilo中的一种）），

                                                    assigned（指用户手工填入，默认）。 -->  

           <generatorclass="uuid"/>  

       </id>  

       <!--   

           <version/>：使用版本控制来处理并发，要开启optimistic-lock="version"和dynamic-update="true"。  

           name="version"：持久化类的属性名，column="version"：指定持有版本号的字段名；  

        -->  

       <version name="version"column="version"/>  

       <!--   

           <property>：为类定义一个持久化的javaBean风格的属性。  

           name="name"：标识属性的名字，以小写字母开头；  

           column="name"：表主键字段的名字，如果不填写与name一样；  

           update="true"/insert="true"：默认为true，表示可以被更新或插入；  

        -->  

       <property name="name" column="name" />  

       <property name="sex" column="sex"/>  

       <property name="age" column="age"/>  

       <!--   

           组件映射：把多个属性打包在一起当一个属性使用，用来把类的粒度变小。  

            <component name="属性，这里指对象">  

                <propertyname="name1"></property> 

                <propertyname="name2"></property> 

           </component>  

        -->  

        <!--   

           <join>:一个对象映射多个表，该元素必须放在所有<property>之后。  

           <join table="tbl_test：子表名">  

                <key column="uuid：子表主键"></key>  

           <property name="name1：对象属性" column="name：子表字段"></property>  

        </join>  

         -->  

   </class>  

</hibernate-mapping>  

二、Hibernate开发的环境搭建

1、Hibernate的环境搭建非常简单，只需要引入Hibernate核心包以及Hibernate依赖包即可。

Jar包下载地址：https://sourceforge.net/projects/hibernate/files/hibernate-orm/

2、加入数据库驱动。下面的例子中主要是采用Mysql数据库来演示的，所以在这里引入MysqL的JDBC驱动jar包。

Jar包下载地址：

https://dev.mysql.com/downloads/connector/j/3.0.html

3、提供核心配置文件hibernate.cfg.xml文件，将文件放在src文件夹下即可。其中的配置如下（针对mysql）：

<!DOCTYPEhibernate-configurationPUBLIC

"-//Hibernate/Hibernate Configuration DTD 3.0//EN"

"http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd">

<hibernate-configuration>

<session-factory>

<!-- mysql数据库驱动 -->

<propertyname=*"hibernate.connection.driver_class"*>com.mysql.jdbc.Driver</property>

<!-- mysql数据库名称 -->

<propertyname=*"hibernate.connection.url"*>jdbc:mysql://localhost:3306/test?characterEncoding=utf8</property>

<!-- 数据库的登陆用户名 -->

<propertyname=*"hibernate.connection.username"*>root</property>

<!-- 数据库的登陆密码 -->

<propertyname=*"hibernate.connection.password"*>root</property>

<!-- 方言：为每一种数据库提供适配器，方便转换 -->

<propertyname=*"hibernate.dialect"*>org.hibernate.dialect.MySQLDialect</property>

<propertyname=*"hibernate.show_sql"*>true</property><!-- 设置是否显示生成sql语句 -->

<propertyname=*"hibernate.format_sql"*>true</property><!-- 设置是否格式化sql语句-->

</session-factory>

</hibernate-configuration>

4、提供log4j.properties文件，支持日志操作，将文件放在src文件夹下即可。其中的配置如下：

###set log levels ###

log4j.rootLogger= INFO,console,debug,error

###console ###

log4j.appender.console= org.apache.log4j.ConsoleAppender

log4j.appender.console.Target= System.out

log4j.appender.console.layout= org.apache.log4j.PatternLayout

log4j.appender.console.layout.ConversionPattern= %-d{yyyy-MM-ddHH\:mm\:ss}[%p]-[%c]%m%n

###log file ###

log4j.appender.debug= org.apache.log4j.DailyRollingFileAppender

log4j.appender.debug.File= ./logs/demo.log

log4j.appender.debug.Append= true

log4j.appender.debug.Threshold= INFO

log4j.appender.debug.layout= org.apache.log4j.PatternLayout

log4j.appender.debug.layout.ConversionPattern= %-d{yyyy-MM-ddHH\:mm\:ss}[%p]-[%c]%m%n

###exception ###

log4j.appender.error= org.apache.log4j.DailyRollingFileAppender

log4j.appender.error.File= ./logs/demo_error.log

log4j.appender.error.Append= true

log4j.appender.error.Threshold= ERROR

log4j.appender.error.layout= org.apache.log4j.PatternLayout

log4j.appender.error.layout.ConversionPattern= %-d{yyyy-MM-ddHH\:mm\:ss}[%p]-[%c]%m%n

###\u9700\u8981\u58F0\u660E\uFF0C\u7136\u540E\u4E0B\u65B9\u624D\u53EF\u4EE5\u4F7Fdruid sql\u8F93\u51FA\uFF0C\u5426\u5219\u4F1A\u629B\u51FAlog4j.error.keynot found

log4j.appender.stdout=org.apache.log4j.ConsoleAppender

log4j.appender.stdout.Target=System.out

log4j.appender.stdout.layout=org.apache.log4j.PatternLayout

log4j.appender.stdout.layout.ConversionPattern=%d{ISO8601}%l%c%n%p:%m%n

### druidsql ###

log4j.logger.druid.sql=warn,stdout

log4j.logger.druid.sql.DataSource=warn,stdout

log4j.logger.druid.sql.Connection=warn,stdout

log4j.logger.druid.sql.Statement=warn,stdout

log4j.logger.druid.sql.ResultSet=warn,stdout

三、HIbernate第一个实例

1、新建一个普通的java项目，添加hibernate相关jar包。

2、建立User实体类

import java.util.Date;  

public class User {  

    private String id;  

    private String username;  

    private String password;  

    private Date createTime;  

    private Date expireTime;  

    public String getId() {  

        return id;  

    }  

    public void setId(String id) {  

        this.id = id;  

    }  

    public String getUsername() {  

        return username;  

    }  

    public void setUsername(String userName) {  

        this.username = userName;  

    }  

    public String getPassword() {  

        return password;  

    }  

    public void setPassword(String password) {  

        this.password = password;  

    }  

    public Date getCreateTime() {  

        return createTime;  

    }  

    public void setCreateTime(Date createTime) {  

        this.createTime = createTime;  

    }  

    public Date getExpireTime() {  

        return expireTime;  

    }  

    public void setExpireTime(Date expireTime) {  

        this.expireTime = expireTime;  

    }  

}  

3、提供User.hbm.xml文件，将文件放到User类同一目录下，完成实体类的映射。

<?xmlversion=*"1.0"*?>

<!DOCTYPEhibernate-mappingPUBLIC

"-//Hibernate/HibernateMapping DTD 3.0//EN"

"http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd">

<hibernate-mapping>

<classname=*"com.zychen.www.User"*>

<idname=*"id"*>

<generatorclass=*"uuid"*/>

</id>

<propertyname=*"username"*/>

<propertyname=*"password"*/>

<propertyname=*"createTime"*/>

<propertyname=*"expireTime"*/>

</class>

</hibernate-mapping>

4、配置hibernate.cfg.xml文件。

<!DOCTYPEhibernate-configurationPUBLIC

"-//Hibernate/Hibernate Configuration DTD 3.0//EN"

"http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd">

<hibernate-configuration>

<session-factory>

<!-- mysql数据库驱动 -->

<propertyname=*"hibernate.connection.driver_class"*>com.mysql.jdbc.Driver</property>

<!-- mysql数据库名称 -->

<propertyname=*"hibernate.connection.url"*>jdbc:mysql://localhost:3306/test?characterEncoding=utf8</property>

<!-- 数据库的登陆用户名 -->

<propertyname=*"hibernate.connection.username"*>root</property>

<!-- 数据库的登陆密码 -->

<propertyname=*"hibernate.connection.password"*>root</property>

<!-- 方言：为每一种数据库提供适配器，方便转换 -->

<propertyname=*"hibernate.dialect"*>org.hibernate.dialect.MySQLDialect</property>

<mappingresource=*"com/zychen/www/User.hbm.xml"*></mapping>

</session-factory>

</hibernate-configuration>

5、生成表：编写工具类ExoprtDB.java,将hbm生成ddl

**package**com.zychen.test;

**import**org.apache.log4j.Logger;

**import**org.hibernate.cfg.Configuration;

**import**org.hibernate.tool.hbm2ddl.SchemaExport;

**publicclass** ExoprtDB {

/**

     * **@param** args

     */

**publicstaticvoid** main(String[] args) {

//默认读取hibernate.cfg.xml文件

        Logger.*getRootLogger*().info("测试，生成并输出sql到文件（当前目录）和数据库 begin");

        Configuration cfg = **new** Configuration().configure();

//生成并输出sql到文件（当前目录）和数据库

        SchemaExport export = **new** SchemaExport(cfg);

        export.create(**true**, **true**); 

        Logger.*getRootLogger*().info("测试，生成并输出sql到文件（当前目录）和数据库 end"); 

    }

}

在运行ExoprtDB.java文件之前确保mysql数据库中建立了test数据库实例。直接运行ExoprtDB.java就可以生成User表了。

6、向表中添加数据。

**package**com.zychen.test;

**import**java.util.Date;

**import**org.apache.log4j.Logger;

**import**org.hibernate.Session;

**import**org.hibernate.SessionFactory;

**import**org.hibernate.cfg.Configuration;

**import**com.zychen.www.User;

**publicclass** Client {

/**

     * **@param** args

     */

**publicstaticvoid** main(String[] args) {

        Logger.*getRootLogger*().info("测试，添加表记录  begin");

//读取配置文件

        Configuration cfg = **new** Configuration().configure();  

        SessionFactory factory =cfg.buildSessionFactory();  

        Session session = **null**;  

**try**{  

            session = factory.openSession();  

//开启事务

            session.beginTransaction();  

            User user = **new** User(); 

            user.setUsername("用户名");  

            user.setPassword("123"); 

            user.setCreateTime(**new** Date()); 

            user.setExpireTime(**new** Date()); 

            session.save(user);  

//提交事务

           session.getTransaction().commit(); 

        }**catch**(Exception e){  

            e.printStackTrace();  

//回滚事务

           session.getTransaction().rollback(); 

        }**finally**{  

**if**(session != **null**){  

**if**(session.isOpen()){  

//关闭session  

                    session.close();  

               }  

            } 

        } 

        Logger.*getRootLogger*().info("测试，添加表记录  end"); 

    }

}

执行该java文件就可以完成向表中增加数据。

编译运行过程遇到的问题：

1、运行错误：Caused by: java.lang.ClassNotFoundException:javax.persistence.EntityListener

Caused by: Java.lang.ClassNotFoundException:javax.persistence.EntityListene

我用的是Hibernate3.6.10.final,出现这个问题的可能原因是没有导入hibernate-distribution-3.6.10.Final\lib\jpa\hibernate-jpa-2.0-api-1.0.1.Final.jar这个包

2、 org.hibernate.exception.GenericJDBCException: Could notexecute JDBC batch update

错误提示：org.hibernate.exception.GenericJDBCException: Could not execute JDBC batch update
错误原因：

1）htm.xml映射名字有误。

2）HQL语句中的字段与对象属性不相同包括大小写。

3）对象属性出现了大写字母。

4）最难发现的错误就客户端传过来的数据长度如果没有验证长度超过了数据库表段的长度限制。

5）应该是中文问题吧，你看下你的连接URL后面有没有加字符编码设置。

如：jdbc:mysql://localhost:3306/test?characterEncoding=utf8

[完整代码下载](http://download.csdn.net/download/byxdaz/9930511)

