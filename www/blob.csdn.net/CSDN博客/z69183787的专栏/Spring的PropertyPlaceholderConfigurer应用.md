# Spring的PropertyPlaceholderConfigurer应用 - z69183787的专栏 - CSDN博客
2016年05月11日 18:01:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：607
**转载地址：**[http://www.cnblogs.com/yl2755/archive/2012/05/06/2486752.html](http://www.cnblogs.com/yl2755/archive/2012/05/06/2486752.html)
**Spring 利用PropertyPlaceholderConfigurer占位符**
1. PropertyPlaceholderConfigurer是个bean工厂后置处理器的实现，也就是 BeanFactoryPostProcessor接口的一个实现。PropertyPlaceholderConfigurer可以将上下文（配置文 件）中的属性值放在另一个单独的标准java Properties文件中去。在XML文件中用${key}替换指定的properties文件中的值。这样的话，只需要对properties文件进
 行修改，而不用对xml配置文件进行修改。
2.在Spring中，使用PropertyPlaceholderConfigurer可以在XML配置文件中加入外部属性文件，当然也可以指定外部文件的编码，如：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<bean id="propertyConfigurer"class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
   <property name="location">
     <value>conf/sqlmap/jdbc.properties</value>
   </property>
    <property name="fileEncoding">
      <value>UTF-8</value>
    </property>
</bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
当然也可以引入多个属性文件，如：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<bean id="propertyConfigurer"class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
  <property name="locations">
   <list>
    <value>/WEB-INF/mail.properties</value>  
    <value>classpath: conf/sqlmap/jdbc.properties</value>//注意这两种value值的写法
   </list>
  </property>
</bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
3.譬如，jdbc.properties的内容为：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
jdbc.driverClassName=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://localhost/mysqldb?useUnicode=true&characterEncoding=UTF-8&zeroDateTimeBehavior=round;
jdbc.username=root
jdbc.password=123456
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
4.那么在spring配置文件中，我们就可以这样写：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<bean id="propertyConfigurer"class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
  <property name="locations">
   <list>
    <value>classpath: conf/sqlmap/jdbc.properties </value>
   </list>
  </property>
</bean>
<bean id="dataSource"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close">
  <property name="driverClassName"value="${jdbc.driverClassName}" />
  <property name="url" value="${jdbc.url}" />
  <property name="username" value="${jdbc.username}"/>
  <property name="password"value="${jdbc.password}" />
</bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
5.这样，一个简单的数据源就设置完毕了。可以看出：PropertyPlaceholderConfigurer起的作用就是将占位符指向的数据库配置信息放在bean中定义的工具。
6.查看源代码，可以发现，locations属性定义在PropertyPlaceholderConfigurer的祖父类 PropertiesLoaderSupport中，而location只有 setter方法。类似于这样的配置，在spring的源程序中很常见的。
PropertyPlaceholderConfigurer如果在指定的Properties文件中找不到你想使用的属性，它还会在Java的System类属性中查找。
我们可以通过System.setProperty(key, value)或者java中通过-Dnamevalue来给Spring配置文件传递参数。
