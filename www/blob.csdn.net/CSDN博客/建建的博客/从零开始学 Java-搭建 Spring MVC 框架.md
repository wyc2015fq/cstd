# 从零开始学 Java - 搭建 Spring MVC 框架 - 建建的博客 - CSDN博客
2017年09月06日 15:12:19[纪建](https://me.csdn.net/u013898698)阅读数：283
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
# 没有什么比一个时代的没落更令人伤感的了
整个社会和人都在追求创新、进步、成长，没有人愿意停步不前，一个个老事物慢慢从我们生活中消失掉真的令人那么伤感么？或者说被取代？我想有些是的，但有些东西其实并不是这样。
游天鸣和蓝玉不正说明了两种截然不同的人生么。随着社会的进步，那些消失的艺术或者说民间艺术可能现实中我们再也见不到了，这个社会永远都是优胜劣汰，消失以为着不再需要，只不过这些老物件有更多的技艺和精神层面的东西罢了。
《百鸟朝凤》这部电影我最近才在网络上看，看完后我久久不能动弹，它并没有强烈的戏剧冲突，画面也不够细腻，人物刻画并不突出，但它讲明了唢呐这个事儿，不脑残，令人动容，这才是有思考的电影。
# 如果创建一个 Spring 项目
Spring MVC 框架在 Java 的 Web 项目中应该是无人不知的吧，你不会搭建一个 Spring 框架？作为身为一个刚刚学习Java的我都会，如果你不会的话，那可真令人忧伤。
1.在 MyEclipse 创建项目后，可以以选择的方式去`配置`一个
 Spring 项目，这里不在讨论。因为我只用 Eclipse。
2.手动搭建。就是动手。
# 新建一个 Java Web 项目
1.打开 Eclipse ，在
```
Project
 Explorer
```
选项卡下面点击右键，选择
```
Web
 - Dynamic Web Prodect
```
（这一步应该都知道阿！！！）。
![newProject.png](http://o8t4sfm1g.bkt.clouddn.com/2016/08/3935284619.png)
2.点击`Next`。起一个你认为还不错的项目名，注意：命名很重要，把每一次命名都当做给自己孩子起名字一样庄严神圣。
![SpringDemo.png](http://o8t4sfm1g.bkt.clouddn.com/2016/08/477824978.png)
3.没有了，完成。
![demoMenu.png](http://o8t4sfm1g.bkt.clouddn.com/2016/08/3246737662.png)
# 搞到 Spring 框架的 jar 包
无论你用坑蒙拐骗，还是死皮赖脸，只要你搞到 Spring 框架的 jar 包就行。我这里给你个地址，你可以体面的去下载就行了。地址：[http://projects.spring.io/spring-framework/](http://projects.spring.io/spring-framework/)
找到适合自己的版本，下载下来保存到合适的位置就可以了，就这么简单。解压后，应该是这样的：
![spring4.2.6.png](http://o8t4sfm1g.bkt.clouddn.com/2016/08/493357410.png)
你看包的命名，你可能就大致明白了这个 jar 包是干嘛的了，接下来就是引入你需要的了。
然后，你要你需要的 jar 包，复制到项目的`/WebContent/WEB-INF/lib`下，为什么要这么做，下面会说的。
# 导入 jar 包
记得当年一个学 Java 的朋友抱怨说: Java 每天都在导包，不如 .Net 爽。我现在并不这么认为。
在项目名上，点击右键，
```
Build
 Path - Configure Bulid Path... - Libraries - Add JARs...
```
，在弹出的框里边找到项目的`/WebContent/WEB-INF/lib`，这样就看到刚刚你复制过来的
 jar 包了。
![add-jars.png](http://o8t4sfm1g.bkt.clouddn.com/2016/08/3803729409.png)
# 配置配置配置
搭建 Spring 框架最重要的步骤应该就是配置了。官网对框架的解释说明如下：
> 
Spring MVC 框架是围绕一个 DispatcherServlet 来设计的，这个 Servlet 会把请求分发给各个处理器，并支持可配置的处理器映射、视图渲染、本地化、时区与主题渲染等，甚至还能支持文件上传。处理器是你的应用中注解了 @Controller 和 @RequestMapping 的类和方法，Spring 为处理器方法提供了极其多样灵活的配置。
所以，首先我们应该在`/WebContent/WEB-INF/`下新建`web.xml`文件，接下来在这个文件中配置
 DispatcherServlet。
```
<servlet>
    <servlet-name>springMVC</servlet-name>
    <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
    <load-on-startup>1</load-on-startup>
</servlet>
<servlet-mapping>
    <servlet-name>springMVC</servlet-name>
    <url-pattern>/</url-pattern>
</servlet-mapping>
<context-param>
    <param-name>contextConfigLocation</param-name>
    <param-value>/WEB-INF/applicationContext.xml</param-value>
</context-param>
```
还可以配置字符编码，默认启动页面什么的，这里不在配置，具体见示例项目：[https://github.com/mafly/SpringDemo/blob/master/WebContent/WEB-INF/web.xml](https://github.com/mafly/SpringDemo/blob/master/WebContent/WEB-INF/web.xml)，因为这里是把
 DispatcherServlet 命名为`springMVC`，并且让它在
 Web 项目一启动就加载。接下来我们需要在`/WebContent/WEB-INF/`目录下创建一个`springMVC-servlet.xml`的Spring配置文件。Spring官方文档上推荐的默认的文件名是`[servlet-name]-servlet.xml`文件，这里
 servlet-name 叫 springMVC ，因此，我新建了一个`springMVC-servlet.xml`。
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"               xmlns:p="http://www.springframework.org/schema/p"
xmlns:context="http://www.springframework.org/schema/context"
xmlns:util="http://www.springframework.org/schema/util" xmlns:mvc="http://www.springframework.org/schema/mvc"
xsi:schemaLocation="
  http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.2.xsd
  http://www.springframework.org/schema/util
  http://www.springframework.org/schema/util/spring-util-4.2.xsd
  http://www.springframework.org/schema/context 
  http://www.springframework.org/schema/context/spring-context-4.2.xsd
  http://www.springframework.org/schema/mvc
  http://www.springframework.org/schema/mvc/spring-mvc-4.2.xsd">
<!-- 使用默认的注解映射 -->
<mvc:annotation-driven />
<mvc:resources location="/" mapping="/index.html" />
<!-- 自动扫描controller包中的控制器 -->
<context:component-scan base-package="cn.mayongfa.api.controller" />
<context:component-scan base-package="cn.mayongfa.controller" />
<!-- 上传文件拦截，设置最大上传文件大小 30M=30*1024*1024(B)=31457280 bytes -->
<bean id="multipartResolver"
    class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
    <property name="maxUploadSize" value="31457280" />
</bean>
```
具体详见：[https://github.com/mafly/SpringDemo/blob/master/WebContent/WEB-INF/springMVC-servlet.xml](https://github.com/mafly/SpringDemo/blob/master/WebContent/WEB-INF/springMVC-servlet.xml)
我们在`web.xml`文件中定义的`contextConfigLocation`,指定要装入的
 Spring 配置文件，一般文件都命名为`applicationContext.xml`，这个文件中我们可以进行扫描类包、读取配置文件、数据源管理、AOP配置、缓存以及消息队列等配置，所以，接下来就新建`applicationContext.xml`文件。
```
<?xml version="1.0" encoding="UTF-8" ?>
<beans xmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
xmlns:context="http://www.springframework.org/schema/context"
xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
xsi:schemaLocation="http://www.springframework.org/schema/beans
   http://www.springframework.org/schema/beans/spring-beans.xsd
   http://www.springframework.org/schema/context
   http://www.springframework.org/schema/context/spring-context.xsd
   http://www.springframework.org/schema/tx
   http://www.springframework.org/schema/tx/spring-tx.xsd
   http://www.springframework.org/schema/aop
   http://www.springframework.org/schema/aop/spring-aop.xsd">
<!-- 将多个配置文件读取到容器中，交给Spring管理 -->
<bean id="propertyConfigurer"
    class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
    <property name="locations">
        <list>
            <value>classpath:global.properties</value>
            <value>classpath:jdbc.properties</value>
        </list>
    </property>
</bean>
<!-- 扫描类包，将标注Spring注解的类自动转化Bean，同时完成Bean的注入 -->
<context:component-scan base-package="cn.mayongfa.common" />
<context:component-scan base-package="cn.mayongfa.service" />
<context:component-scan base-package="cn.mayongfa.dao" />
<!--master 配置数据源 -->
<bean id="masterDataSource" class="com.alibaba.druid.pool.DruidDataSource"
    init-method="init" destroy-method="close">
    <property name="driverClassName">
        <value>${master.jdbc.driverClassName}</value>
    </property>
    <property name="url">
        <value>${master.jdbc.url}</value>
    </property>
    <property name="username">
        <value>${master.jdbc.username}</value>
    </property>
    <property name="password">
        <value>${master.jdbc.password}</value>
    </property>
    ...
</bean>
<!--slave 配置数据源 -->
<bean id="slaveDataSource" class="com.alibaba.druid.pool.DruidDataSource"
    init-method="init" destroy-method="close">
    
            ...
</bean>
<bean id="dataSource" class="cn.mayongfa.service.imp.DynamicDataSource">
    <property name="targetDataSources">
        <map>
            <entry key="slave" value-ref="slaveDataSource" />
        </map>
    </property>
    <property name="defaultTargetDataSource" ref="masterDataSource" />
</bean>
<!-- 配置Jdbc模板 -->
<bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
    <property name="dataSource" ref="dataSource"></property>
</bean>
<!-- 配置事务管理器 -->
...
<!-- 通过AOP配置提供事务增强，让service包下所有Bean的所有方法拥有事务 -->
...
```
上面只是简单的配置，文件并不完整，具体完整项目示例见GitHub:[https://github.com/mafly/SpringDemo](https://github.com/mafly/SpringDemo)
到这里，其实我们已经配置完成了，接下来就是新建我们需要的`Package`包，来实现不同包来完成不同的事儿的。
## 新增 Package 包
分层的意义及优缺点我这里不在唠叨，按照正常的分层架构一般都会分为 View 层、Action 层、Service 层、Dao 层，这里我们也是这样做的，下面就开始新建包，.Net 下面是叫类库。
![package.png](http://o8t4sfm1g.bkt.clouddn.com/2016/08/4065750617.png)
按照这样的方式新建就可以了，具体的架构如下图：
![demoLastMenu.png](http://o8t4sfm1g.bkt.clouddn.com/2016/08/402552663.png)
到这里，搭建 Spring MVC 框架的工作算是完成了。接下来就是配置具体的数据源、缓存、AOP、JMS 这些东西了。祝你好运！
下一篇[从零开始学 Java - Spring AOP 拦截器的基本实现](http://blog.mayongfa.cn/86.html)
所有代码我同步到了 GitHub , 示例项目地址：[https://github.com/mafly/SpringDemo](https://github.com/mafly/SpringDemo)
