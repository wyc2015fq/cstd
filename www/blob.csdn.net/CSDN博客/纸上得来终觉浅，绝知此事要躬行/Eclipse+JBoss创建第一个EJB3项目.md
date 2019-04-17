# Eclipse+JBoss创建第一个EJB3项目 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年06月24日 15:58:58[boonya](https://me.csdn.net/boonya)阅读数：1910








作者: [Michael](http://www.micmiu.com/author/michael/) 日期:
 2013 年 9 月 21 日  原文地址：[http://www.micmiu.com/j2ee/ejb/eclipse-jboss-ejb-demo/](http://www.micmiu.com/j2ee/ejb/eclipse-jboss-ejb-demo/)




目录
- 演示环境
- 
JBoss 配置
- 
创建EJB工程
- 
部署
- 
创建客户端工程
- 
测试

[一]、演示环境
- Eclipse Juno（4.2）
- 
Java 1.7.0_25
- 
JBoss AS (5/6/7)

[二]、JBoss 配置


eclipse默认支持到JBoss 5.0，如果需要更高版本JBoss AS，可以去官方下载Eclipse对应版本的压缩包离线安装即可，当然也支持在线安装插件, JBoss Tools 下载地址：[http://www.jboss.org/tools/download.html](http://www.jboss.org/tools/download.html)


本文就以自带的JBoss5.0 为例图文演示EJB3.0工程构建过程。

1. 下载JBoss AS


JBoss官网 [http://www.jboss.org/jbossas/downloads](http://www.jboss.org/jbossas/downloads)下载
 JBoss AS 5.0.1的版本，下载后解压缩到目录：/Users/micmiu/workspace_eclipse/jboss-5.0.1.GA

2.配置 Server Runtime Environments


eclipse 依次打开 Preference → Server → Runtime Environments  → Add…  配置新环境，如下图：

![eclipse-jboss-ejb-01](http://www.micmiu.com/wp-content/uploads/2013/09/eclipse-jboss-ejb-01.png)

3.创建Server


eclipse  依次打开 File → New → Other → Server → JBoss v5.0

![eclipse-jboss-ejb-02](http://www.micmiu.com/wp-content/uploads/2013/09/eclipse-jboss-ejb-02.png)


Server runtime environment 选择 上面第二步中配置的环境即可，【Next】配置端口

![eclipse-jboss-ejb-03](http://www.micmiu.com/wp-content/uploads/2013/09/eclipse-jboss-ejb-03.png)


默认即可。

[三]、创建EJB工程


依次点击 File → New → Other … → EJB → EJB Project 如下图

![eclipse-jboss-ejb-05](http://www.micmiu.com/wp-content/uploads/2013/09/eclipse-jboss-ejb-05.png)


【Next】→【Next】（选中 Generate ejb-jar.xml deployment descriptor）→【Finish】。

![eclipse-jboss-ejb-06](http://www.micmiu.com/wp-content/uploads/2013/09/eclipse-jboss-ejb-06.png)


如上图中生成的目录结构中，ejbModule目录是编写JAVA源码的目录（即EJB代码）。

在ejbModule 目录上右键【New】→【Other…】→【EJB】→ Session Bean(EJB 3.x)

![eclipse-jboss-ejb-07](http://www.micmiu.com/wp-content/uploads/2013/09/eclipse-jboss-ejb-07.png)


→【Next】输入包名:com.micmiu.ejb、EJB类名:HelloWorld ,选中 Remote选项  →【Finish】.

![eclipse-jboss-ejb-08](http://www.micmiu.com/wp-content/uploads/2013/09/eclipse-jboss-ejb-08.png)


自动生成两个类：HelloWorldRemote.java



|`1`|`package``com.micmiu.ejb;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``javax.ejb.Remote;`|
|----|----|


|`4`||
|----|----|


|`5`|`/**`|
|----|----|


|`6`|` ``*`|
|----|----|


|`7`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`8`|` ````* @time Create on 2013-9-21 下午1:55:51```|
|----|----|


|`9`|` ````* @version 1.0```|
|----|----|


|`10`|` ``*/`|
|----|----|


|`11`|`@Remote`|
|----|----|


|`12`|`public``interface````HelloWorldRemote {```|
|----|----|


|`13`||
|----|----|


|`14`|`}`|
|----|----|




HelloWorld.java



|`1`|`package``com.micmiu.ejb;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``javax.ejb.Stateless;`|
|----|----|


|`4`|`/**`|
|----|----|


|`5`|` ````* Session Bean implementation class HelloWorld```|
|----|----|


|`6`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`7`|` ````* @time Create on 2013-9-21 下午1:55:40```|
|----|----|


|`8`|` ````* @version 1.0```|
|----|----|


|`9`|` ``*/`|
|----|----|


|`10`|`@Stateless`|
|----|----|


|`11`|`public``class``HelloWorld ``implements````HelloWorldRemote {```|
|----|----|


|`12`||
|----|----|


|`13`|`    ``/**`|
|----|----|


|`14`|`     ````* Default constructor.```|
|----|----|


|`15`|`     ``*/`|
|----|----|


|`16`|`    ``public````HelloWorld() {```|
|----|----|


|`17`|`        ````// TODO Auto-generated constructor stub```|
|----|----|


|`18`|`    ``}`|
|----|----|


|`19`||
|----|----|


|`20`|`}`|
|----|----|





修改接口 HelloWorldRemote.java 添加sayHello(String username) 方法：



|`1`|`package``com.micmiu.ejb;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``javax.ejb.Remote;`|
|----|----|


|`4`||
|----|----|


|`5`|`/**`|
|----|----|


|`6`|` ``*`|
|----|----|


|`7`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`8`|` ````* @time Create on 2013-9-21 下午1:55:51```|
|----|----|


|`9`|` ````* @version 1.0```|
|----|----|


|`10`|` ``*/`|
|----|----|


|`11`|`@Remote`|
|----|----|


|`12`|`public``interface````HelloWorldRemote {```|
|----|----|


|`13`||
|----|----|


|`14`|`    ``String``sayHello(``String``username);`|
|----|----|


|`15`|`}`|
|----|----|





修改实现类 HelloWorld.java ，实现sayHello(String username)方法：



|`1`|`package``com.micmiu.ejb;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``javax.ejb.Stateless;`|
|----|----|


|`4`||
|----|----|


|`5`|`/**`|
|----|----|


|`6`|` ````* Session Bean implementation class HelloWorld```|
|----|----|


|`7`|` ``*`|
|----|----|


|`8`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`9`|` ````* @time Create on 2013-9-21 下午1:55:40```|
|----|----|


|`10`|` ````* @version 1.0```|
|----|----|


|`11`|` ``*/`|
|----|----|


|`12`|`@Stateless`|
|----|----|


|`13`|`public``class``HelloWorld ``implements````HelloWorldRemote {```|
|----|----|


|`14`||
|----|----|


|`15`|`    ``/**`|
|----|----|


|`16`|`     ````* Default constructor.```|
|----|----|


|`17`|`     ``*/`|
|----|----|


|`18`|`    ``public````HelloWorld() {```|
|----|----|


|`19`|`        ````// TODO Auto-generated constructor stub```|
|----|----|


|`20`|`    ``}`|
|----|----|


|`21`||
|----|----|


|`22`|`    ``@Override`|
|----|----|


|`23`|`    ``public````String sayHello(String username) {```|
|----|----|


|`24`|`        ``return``"Hello,"````+ username +``````" welcome to EJB."````;`|
|----|----|


|`25`|`    ``}`|
|----|----|


|`26`|`}`|
|----|----|




[四]、部署


部署的办法有两种：

1.在eclipse中发布


在之前创建好的Server中，右击  Add and Remove … 添加这个项目，然后 Start即可

2.打包后再部署到指定server下


在工程上右键→【Export】→【EJB JAR file】 Destination处：→【Browse…】→ 选择JBoss服务器的部署目录：JBOSS_HOME/server/default/deploy （以JBoss 5.0.1GA为例）→【完成】即可

[五]、创建客户端工程

1. 点击 File → New → Other … → Java Project


创建一个 EJB-HelloWorld-Client 的工程。

2. 把EJB-HelloWorld 工程中的接口类 【Export】 jar包，添加到当前客户端工程classpath中。

3. 添加Jboss Client的Jar包（$JBOSS_HOME/clent/jbossall-clent.jar）到classpath中，如下图

![eclipse-jboss-ejb-09](http://www.micmiu.com/wp-content/uploads/2013/09/eclipse-jboss-ejb-09.png)

4.创建客户端测试类

HelloWorldClientTest.java



|`1`|`package``com.micmiu.ejb.demo;`|
|----|----|


|`2`||
|----|----|


|`3`|`import``java.util.Properties;`|
|----|----|


|`4`||
|----|----|


|`5`|`import``javax.naming.InitialContext;`|
|----|----|


|`6`|`import``javax.naming.NamingException;`|
|----|----|


|`7`||
|----|----|


|`8`|`import``com.micmiu.ejb.HelloWorldRemote;`|
|----|----|


|`9`||
|----|----|


|`10`|`/**`|
|----|----|


|`11`|` ``*`|
|----|----|


|`12`|` ````* @author <a href="[http://www.micmiu.com](http://www.micmiu.com/)">Michael</a>```|
|----|----|


|`13`|` ````* @time Create on 2013-9-21 下午2:51:59```|
|----|----|


|`14`|` ````* @version 1.0```|
|----|----|


|`15`|` ``*/`|
|----|----|


|`16`|`public``class````HelloWorldClientTest {```|
|----|----|


|`17`||
|----|----|


|`18`|`    ``/**`|
|----|----|


|`19`|`     ````* @param args```|
|----|----|


|`20`|`     ``*/`|
|----|----|


|`21`|`    ``public``static``void````main(String[] args) {```|
|----|----|


|`22`|`        ``try``{`|
|----|----|


|`23`|`            ````Properties props =````new``Properties();`|
|----|----|


|`24`|`            ``props.setProperty(``"java.naming.factory.initial"``,`|
|----|----|


|`25`|`                    ``"org.jnp.interfaces.NamingContextFactory"``);`|
|----|----|


|`26`|`            ``props.setProperty(``"java.naming.provider.url"``, ``"localhost:1099"``);`|
|----|----|


|`27`|`            ``props.setProperty(``"java.naming.factory.url.pkgs"``,`|
|----|----|


|`28`|`                    ``"org.jboss.naming"``);`|
|----|----|


|`29`||
|----|----|


|`30`|`            ``System.out.println(````">>>> InitialContext "````);`|
|----|----|


|`31`|`            ````InitialContext ctx =````new``InitialContext(props);`|
|----|----|


|`32`||
|----|----|


|`33`|`            ``System.out.println(````">>>> lookup  "````);`|
|----|----|


|`34`|`            ````String serviceName =````"HelloWorld/remote"``;`|
|----|----|


|`35`|`            ````HelloWorldRemote helloWorld = (HelloWorldRemote) ctx```|
|----|----|


|`36`|`                    ``.lookup(serviceName);`|
|----|----|


|`37`||
|----|----|


|`38`|`            ``System.out.println(````">>>> 调用sayHello(\"micmiu.com\")"````);`|
|----|----|


|`39`|`            ````String result = helloWorld.sayHello(````"micmiu.com"``);`|
|----|----|


|`40`|`            ``System.out.println(````">>>> 结果 = "``````+ result);```|
|----|----|


|`41`|`        ``} ``catch````(NamingException e) {```|
|----|----|


|`42`|`            ``e.printStackTrace();`|
|----|----|


|`43`|`        ``}`|
|----|----|


|`44`|`    ``}`|
|----|----|


|`45`|`}`|
|----|----|




[六]、测试

先运行EJB-HellowWorld 工程，日志如下：

> 

14:46:37,439 INFO  [JBossASKernel] Created KernelDeployment for: EJB-HelloWorld.jar

14:46:37,441 INFO  [JBossASKernel] installing bean: jboss.j2ee:jar=EJB-HelloWorld.jar,name=HelloWorld,service=EJB3

14:46:37,441 INFO  [JBossASKernel]   with dependencies:

14:46:37,441 INFO  [JBossASKernel]   and demands:

14:46:37,442 INFO  [JBossASKernel] jboss.ejb:service=EJBTimerService

14:46:37,442 INFO  [JBossASKernel]   and supplies:

14:46:37,442 INFO  [JBossASKernel] jndi:HelloWorld/remote-com.micmiu.ejb.HelloWorldRemote

14:46:37,442 INFO  [JBossASKernel] jndi:HelloWorld/remote

14:46:37,442 INFO  [JBossASKernel] Class:com.micmiu.ejb.HelloWorldRemote

14:46:37,442 INFO  [JBossASKernel] Added bean(jboss.j2ee:jar=EJB-HelloWorld.jar,name=HelloWorld,service=EJB3) to KernelDeployment of: EJB-HelloWorld.jar

14:46:37,512 INFO  [SessionSpecContainer] Starting jboss.j2ee:jar=EJB-HelloWorld.jar,name=HelloWorld,service=EJB3

14:46:37,521 INFO  [EJBContainer] STARTED EJB: com.micmiu.ejb.HelloWorld ejbName: HelloWorld

14:46:37,550 INFO  [JndiSessionRegistrarBase] Binding the following Entries in Global JNDI:

HelloWorld/remote – EJB3.x Default Remote Business Interface

HelloWorld/remote-com.micmiu.ejb.HelloWorldRemote – EJB3.x Remote Business Interface

14:46:37,602 INFO  [Http11Protocol] Starting Coyote HTTP/1.1 on http-127.0.0.1-8080

14:46:37,631 INFO  [AjpProtocol] Starting Coyote AJP/1.3 on ajp-127.0.0.1-8009

14:46:37,643 INFO  [ServerImpl] JBoss (Microcontainer) [5.0.1.GA (build: SVNTag=JBoss_5_0_1_GA date=200902231221)] Started in 19s:412ms
再运行客户端的测试类，日志如下：

> 

>>>> InitialContext


>>>> lookup


>>>> 调用sayHello(“micmiu.com”)


>>>> 结果 = Hello,micmiu.com welcome to EJB.



本文介绍到此结束@[Michael Sun](http://www.micmiu.com/).






