# 在Tomcat配置JNDI数据源的三种方式 - z69183787的专栏 - CSDN博客
2014年04月02日 11:27:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3066
在我过去工作的过程中,开发用服务器一般都是Tomcat
数据源的配置往往都是在applicationContext.xml中配置一个dataSource的bean
然后在部署时再修改JNDI配置
我猜是因为Tomcat的配置需要改配置文件
不像JBoss,Weblogic等服务器在管理界面可以直接添加JNDI数据源
也很少人去研究它的配置
最近做了个小项目,发布版本的时候都是通过ant编译成jar包之后丢给测试
测试是大爷,教他修改数据源了还是当没听到
周末闲来无聊,看了一些Tomcat配置的教程,下面做一些总结
注:如果你的工程是直接丢在webapps下面的,server.xml中就没有工程对应的Context节点
更新:由于之前有些配置来自网络,不太好用,作了一些更新
对每种方法作了一些个人评价
PS:以下配置在apache-tomcat-6.0.35下测试通过,可以访问数据库
# 第一种,单个应用独享数据源
### 就一步,找到Tomcat的server.xml找到工程的Context节点,添加一个私有数据源
Xml代码  ![收藏代码](http://136589219.iteye.com/images/icon_star.png)
- <ContextdocBase="WebApp"path="/WebApp"reloadable="true"source="org.eclipse.jst.jee.server:WebApp">
- <Resource
- name="jdbc/mysql"
- scope="Shareable"
- type="javax.sql.DataSource"
- factory="org.apache.tomcat.dbcp.dbcp.BasicDataSourceFactory"
- url="jdbc:mysql://localhost:3306/test"
- driverClassName ="com.mysql.jdbc.Driver"
- username="root"
- password="root"
- />
- </Context>
优点:简单
缺点:重用性差
# 第二种,配置全局JNDI数据源,应用到单个应用
分两步
### 第一步, 找到Tomcat的server.xml中GlobalNamingResources节点,在节点下加一个全局数据源
Xml代码  ![收藏代码](http://136589219.iteye.com/images/icon_star.png)
- <Resource
- name="jdbc/mysql"
- scope="Shareable"
- type="javax.sql.DataSource"
- factory="org.apache.tomcat.dbcp.dbcp.BasicDataSourceFactory"
- url="jdbc:mysql://localhost:3306/test"
- driverClassName ="com.mysql.jdbc.Driver"
- username="root"
- password="root"
- />
### 第二步,找到要应用此JNDI数据源的工程Context节点,增加对全局数据源的引用ResourceLink 
Xml代码  ![收藏代码](http://136589219.iteye.com/images/icon_star.png)
- <ContextdocBase="WebApp"path="/WebApp"reloadable="true">
- <ResourceLinkglobal="jdbc/mysql"name="jdbc/mysql"type="javax.sql.DataSource"/>
- </Context>
优点:重用性,可控性
缺点:配置相对第三种方法要繁琐一点,每个工程都得配
# 第三种,配置全局JNDI数据源,应用到所有Tomcat下部署的应用
也分两步
### 第一步
    参考第二种的第一步
### 第二步,找到Tomcat的context.xml,在Context节点下加一个ResourceLink节点对第一步配置的数据源进行引用
这个XML配置文件的根节点就是<Context>
Xml代码  ![收藏代码](http://136589219.iteye.com/images/icon_star.png)
- <Context>
- <ResourceLinkglobal="jdbc/mysql"name="jdbc/mysql"type="javax.sql.DataSource"/>
- <WatchedResource>WEB-INF/web.xml</WatchedResource>
- <Context>
## 优点:重用性,一次性到位
缺点:没有可控性
## Spring对JNDI数据源的引用
在applicationContext.xml中加一个bean,替代原来的dataSource
Xml代码  ![收藏代码](http://136589219.iteye.com/images/icon_star.png)
- <jee:jndi-lookupid="dataSource"jndi-name="jdbc/mysql"/>
## C3P0数据源的配置
type和factory的值发生变化
username=>user
url=>jdbcUrl
driverClassName=>driverClass
Xml代码  ![收藏代码](http://136589219.iteye.com/images/icon_star.png)
- <Resourcename="jdbc/mysql_c3p0"scope="Shareable"
- type="com.mchange.v2.c3p0.ComboPooledDataSource"
- factory="org.apache.naming.factory.BeanFactory"
- jdbcUrl="jdbc:mysql://localhost:3306/test"driverClass="com.mysql.jdbc.Driver"
- user="root"password="root"/>
 更多详细内容请参考：[java进阶网](http://www.javady.com/)[www.javady.com](http://www.javady.com/)
