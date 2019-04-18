# 理解JNDI中 java:comp/env/jdbc/datasource 与 jdbc/datasource 的不同之处。 - z69183787的专栏 - CSDN博客
2012年10月22日 14:08:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16934
在描述JNDI，例如获得数据源时，JNDI地址有两种写法，例如同是  jdbc/testDS 数据源：
A:        java:comp/env/jdbc/testDS
B:        jdbc/testDS
这两种写法，配置的方式也不尽相同，第一种方法应该算是一种利于程序移植或迁移的方法，它的实现与“映射”的概念相同，而B方法，则是一个硬引用。
java:comp/env 是环境命名上下文（environment naming context（ENC）），是在EJB规范1.1以后引入的，引入这个是为了解决原来JNDI查找所引起的冲突问题，也是为了提高EJB或者J2EE应用的移植性。
在J2EE中的引用常用的有：
        JDBC 数据源引用在java:comp/env/jdbc 子上下文中声明 
        JMS 连接工厂在java:comp/env/jms 子上下文中声明 
        JavaMail 连接工厂在java:comp/env/mail 子上下文中声明 
        URL 连接工厂在 java:comp/env/url子上下文中声明
可以通过下面的结构示意来发现这两种描述的不同之处：
A:       java:comp/env/jdbc/testDS(虚地址)   ------>    映射描述符   ------>        jdbc/testDS (实际的地址)
B:       jdbc/testDS (实际的地址)
从这种结构上来看，A的确是便于移植的。
再来看一个例子：
假如你需要获取datasource，例如：dataSource = (DataSource) ctx.lookup("java:comp/env/jdbc/testDS");
那么在配置文件中进行资源映射时，在web.xml中,
      <resource-ref>
        <res-ref-name>jdbc/testDS</res-ref-name>
        <res-type>javax.sql.DataSource</res-type>
        <res-auth>Container</res-auth>
      </resource-ref>
在相应的资源配置xml中（不同的应用服务器均不同，WSAD中，可以进行可视化的设置），
    <reference-descriptor>
      <resource-description>
        <res-ref-name>jdbc/testDS</res-ref-name>
        <jndi-name>OraDataSource</jndi-name>
      </resource-description>
    </reference-descriptor>
实际服务器中的JNDI名字是OraDataSource，逻辑名jdbc/DBPool只是用来和它作映射的，这样做的好处是为了提高可移植性，移植的时候只需要把配置文件改一下就可以，而应用程序可不用改动。
假如你写了一个一般的应用程序，想直接通过JNDI来获取数据源，那么直接lookup(“mytest”)就可以了（假如服务器上的JNDI为mytest），用第一种写法反而会报错的。
-----------------------------------------------------
java:comp/env和JNDI是不同的，很多人都有一些混淆，甚至认为这个就是JNDI名称。其实，java:comp/env 是环境命名上下文（environment naming context（ENC），是在EJB规范1.1以后引入的，引入这个是为了解决原来JNDI查找所引起的冲突问题。比如你要把一个EJB的Jar包部署到两台Server，而这两台Server共享一台JNDI名字空间，此时问题就出来了，因为JNDI名字空间要求JNDI名字必须唯一。使用ENC查找，将可以避免这个冲突，EJB或者J2EE应用的移植性也提高了。
 ENC是一个引用，引用是用于定位企业应用程序的外部资源的逻辑名。引用是在应用程序部署描述符文件中定义的。在部署时，引用被绑定到目标可操作环境中资源的物理位置（ JNDI 名）。使用ENC是把对其它资源的JNDI查找的硬编码解脱出来，通过配置这个引用可以在不修改代码的情况下，将引用指向不同的EJB(JNDI)。
---------------------------------------------------
举个例子： 
用weblogic10的控制台定了了一个oracle数据源，这个数据源的JNDI名称是：adsl,那么只要正确连接上了weblogic（当然需传递URL，用户名和密码还有weblogic的JNDI工厂对象，这不属于该文的讨论范围）在任何地方都能用context.lookup("xxx")得到这个数据源了，但是想通过context.lookup("java:comp/env/jdbc/adsl")访问到这个数据源，就不行了。如果想这样写的话需要做两件事情： 
1、确保你的调用程序是一个web项目或者EJB项目，并部署到weblogic上。（例子使用web项目） 
2、在web项目的web.xml里面加上如下配置： 
<resource-ref> 
  <res-ref-name>jdbc/adsl</res-ref-name> 
  <res-auth>Container</res-auth> 
  <mapped-name>adsl</mapped-name> <!-- 这个必须和你的全局JNDI数据源名称一样 -->  
</resource-ref> 
那么你就能在你的web程序里通过context.lookup("java:comp/env/jdbc/adsl")访问到这个数据源了。 
关于EJB的访问也类似这样，可以直接访问全局EJB的JNDI名，和可以映射之后从java:comp/env/下进行访问。
以上的讲解有错。。。。。 
上面的情况只适用于weblogic和J2EE 2.5的规范下。 
一：如果不是J2EE 2.5的规范：那么在你的web项目的web.xml内将根本不会存在<mapped-name>这个标签，那么怎样才能映射全局JNDI资源到你的项目呢？ 
以weblogic为例，你需要在和web.xml同级的目录下建立：weblogic.xml文件，在该文件里面写上： 
<weblogic-web-app> 
<resource-description> 
<res-ref-name>jdbc/adsl</res-ref-name> 
<jndi-name>adsl</jndi-name><!-- 这个就是全局JNDI资源名 --> 
</resource-description> 
</weblogic-web-app> 
然后你仍然需要在web.xml里面配置上: 
<resource-ref> 
  <res-ref-name>jdbc/adsl</res-ref-name> 
  <res-auth>Container</res-auth> 
  </resource-ref> 
这样你才可以在你的程序里面通过context.lookup("java:comp/env/jdbc/adsl")访问到这个数据源。 
这个是weblogic.xml的官方帮助文档：http://edocs.weblogicfans.net/wls/docs92/webapp/weblogic_xml.html 
二：如果不是使用weblogic，可参见这篇文章：http://blog.csdn.net/lovingprince/article/details/6577920 
其实各种不同的J2EE容器，都用不同的配置方式，以我目前的测试来看，weblogic似乎不能配置“私有的JNDI资源”，就是不能配置自己项目单独的JNDI资源（这只是我的观点，我目前没发现怎么在weblogic里面配置私有JNDI资源） 
但是研究tomcat6之后发现，tomcat可以配置 全局JNDI 和私有JNDI（注意这里说的Tomcat6）： 
！！！首先想要说明的是Tomcat的配置不需要修改web.xml里面的任何内容！！！！！ 
！！！Tomcat的全局JNDI资源不能直接访问，必须有java:comp/env/前缀！！！！！！ 
    全局的JNDI配置在server.xml里面的<GlobalNamingResources>标签里面添加如下配置： 
    <Resource name="jdbc/test" 
  auth="Container" 
              type="javax.sql.DataSource" 
              driverClassName="com.mysql.jdbc.Driver" 
              url="jdbc:mysql://127.0.0.1/test" 
              username="root" 
              password="root" 
              maxActive="20" 
              maxIdle="10" 
              maxWait="-1"/> 
    然后某一个项目想要引用这个全局的JNDI，就需要在项目的META-INF下面建立context.xml文件，在里面写上： 
<?xml version="1.0" encoding="UTF-8"?> 
<Context> 
    <ResourceLink name="jdbc/test" global="jdbc/test" type="javax.sql.DataSource"/> 
</Context> 
这样就可以在程序里面通过context.lookup("java:comp/env/jdbc/test")进行访问了。 
    私有的JNDI有三种方式可以配置： 
      1、可以直接在server.xml里面的<Host>节点下添加如下配置： 
<Context path="/test_tomcat6_jndi"> 
<Resource name="jdbc/test" 
      auth="Container" 
              type="javax.sql.DataSource" 
              driverClassName="com.mysql.jdbc.Driver" 
              url="jdbc:mysql://127.0.0.1/test" 
              username="root" 
              password="root" 
              maxActive="20" 
              maxIdle="10" 
              maxWait="-1"/> 
</Context> 
这样就可以直接在程序中通过context.lookup("java:comp/env/jdbc/test")访问了,需要注意的是path="/test_tomcat6_jndi"，这个名字必须和你的项目名称相同，而且不能少了那个斜杠，而且你的项目是通过拷贝文件夹到webapps下面的方式进行的部署。 
     2、 也可以在conf/context.xml里面增加如下配置： 
     <Resource name="jdbc/test" 
      auth="Container" 
              type="javax.sql.DataSource" 
              driverClassName="com.mysql.jdbc.Driver" 
              url="jdbc:mysql://127.0.0.1/test" 
              username="root" 
              password="root" 
              maxActive="20" 
              maxIdle="10" 
              maxWait="-1"/> 
这样就可以直接在程序中通过context.lookup("java:comp/env/jdbc/test")访问了 
      3、还可以在项目的WebRoot下面的META-INF文件夹下面创建context.xml文件，再在context.xml文件里面写上 
<?xml version="1.0" encoding="UTF-8"?> 
<Context> 
    <Resource name="jdbc/test" 
  auth="Container" 
              type="javax.sql.DataSource" 
              driverClassName="com.mysql.jdbc.Driver" 
              url="jdbc:mysql://127.0.0.1/test" 
              username="root" 
              password="root" 
              maxActive="20" 
              maxIdle="10" 
              maxWait="-1"/> 
</Context> 
这样就可以直接在程序中通过context.lookup("java:comp/env/jdbc/test")访问了 
你可以发现的是：以上的Tomcat6中的配置不管是全局还是局部，都没有修改项目的web.xml文件，但是仍然建议在web.xml中进行引用，主要是为了项目的迁移，因为有的服务器需要在web.xml中进行声明！ 
总之：各种服务器有时候确实有比较大的出入，特别是感觉J2EE中类似<resource-ref>这样的一些标签仍然不是很理解。 
