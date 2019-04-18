# Java RMI技术以及Spring封装了的RMI技术 - z69183787的专栏 - CSDN博客
2014年09月12日 11:58:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：779
       最近应用到了Java RMI技术，所以总结一下RMI的使用，另外EJB的实现也是以Java RMI为基础核心的。RMI远程方法调用，可以跨机器通过网络调用，不过Java RMI只支持两边都是Java程序，如果要实现完全两边异构，那么就要用到传说中的Web Service了。为了看好效果，都要建立两个或两个以上工程，当然，如果你有两个机器并通过局域网相连更好，如果能同有互联网的条件就更好了，以下是同一机器不同工程的实验。
**Java RMI**
首先新建一个工程，随便什么工程，为了方便，就Java Project吧。
1、创建一个接口，继承Remote
**[java]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- package leon.rmi.iface;  
- 
- import java.rmi.Remote;  
- import java.rmi.RemoteException;  
- 
- /**
- * 定义远程接口，必须继承Remote接口，
- * 其中所有需要远程调用的方法都必须抛出RemoteException异常 
- */
- publicinterface IHello extends Remote {   
- public String sayHello(String name) throws RemoteException;   
- publicint sum(int a, int b)throws RemoteException;  
- }  
2、创建接口的实现类
**[java]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- package leon.rmi.impl;  
- 
- import java.rmi.RemoteException;  
- import java.rmi.server.UnicastRemoteObject;  
- 
- import leon.rmi.iface.IHello;  
- 
- publicclass HelloImpl extends UnicastRemoteObject implements IHello {  
- 
- privatestaticfinallong serialVersionUID = 1L;  
- 
- public HelloImpl() throws RemoteException {  
- super();  
-     }  
- 
- @Override
- public String sayHello(String name) throws RemoteException {  
- return"Welcome, " + name;  
-     }  
- 
- @Override
- publicint sum(int a, int b) throws RemoteException{  
- return a + b;  
-     }  
- }  
**说明：**接口的实现类同时要实现Serializable接口，这里继承UnicastRemoteObject也是间接实现Serializable接口，同时，因为构造方法需要抛出RemoteException，所以不能缺省使用隐含的无参构造方法，而应该自己显式定义构造方法。
3、创建应用类，注册和启动服务端RMI，以被客户端调用
**[java]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- package leon.rmi.impl;  
- 
- import java.net.MalformedURLException;  
- import java.rmi.AlreadyBoundException;  
- import java.rmi.Naming;  
- import java.rmi.RemoteException;  
- import java.rmi.registry.LocateRegistry;  
- 
- import leon.rmi.iface.IHello;  
- 
- publicclass HelloServer {  
- publicstaticvoid main(String args[]) {  
- try {  
- //创建一个远程对象
-             IHello rhello = new HelloImpl();      
- //生成远程对象注册表Registry的实例，并指定端口为8888（默认端口是1099）
-             LocateRegistry.createRegistry(8888);  
- 
- //把远程对象注册到RMI注册服务器上，并命名为RHello
- //绑定的URL标准格式为：rmi://host:port/name(协议名可以省略，下面两种写法都可以）
-             Naming.bind("rmi://10.225.112.86:8888/RHello", rhello);  
- //Naming.bind("//10.225.112.86:8888/RHello",rhello);
- 
-             System.out.println(">>INFO:远程IHello对象绑定成功！");  
-         } catch (RemoteException e) {  
-             System.out.println("创建远程对象发生异常！");  
-             e.printStackTrace();  
-         } catch (AlreadyBoundException e) {  
-             System.out.println("发生重复绑定对象异常！");  
-             e.printStackTrace();  
-         } catch (MalformedURLException e) {  
-             System.out.println("发生URL畸形异常！");  
-             e.printStackTrace();  
-         }  
-     }  
- }  
**说明：**绑定的地址10.225.112.86是我的局域网地址可以在DOS命令行用ipconfig查看，如果你的机器没有任何联网，可以使用127.0.0.1或localhost。
运行HelloServer.java看到，红色方块显示正在运行：
        >>INFO:远程IHello对象绑定成功！
好了，现在远程服务提供端建立完成，下面建立客户端。
新建一个新的工程，为了方便，也是Java Project吧，
1、 因为客户端需要有服务端那边提供的接口，才可以访问，所以要将服务端的**IHello**接口完全拷贝（连同包）到客户端，当然为了方便，你在客户端工程中新建一个完全一样的接口也可以。实际运用中通常是要服务端接口打成jar包来提供的。
2、 创建客户端调用类
**[java]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- package testrmi;  
- 
- import java.rmi.Naming;  
- 
- import leon.rmi.iface.IHello;  
- 
- publicclass HelloClient {  
- publicstaticvoid main(String args[]) {  
- try {  
- // 在RMI服务注册表中查找名称为RHello的对象，并调用其上的方法
-             IHello rhello = (IHello) Naming.lookup("rmi://10.225.112.86:8888/RHello");  
-             System.out.println(rhello.sayHello("水哥"));  
-             System.out.println(rhello.sum(454, 5457));  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
运行，显示，成功。
呵呵，是不是很简单？对吧。
下面我们要使用Spring封装的Java RMI技术，也是很多项目都会用到的。后面我有个Spring RMI的例子。要看懂下面Spring的例子，你需要已经会用Spring，会配置Spring，否则怕你看不懂，所以如果还不懂Spring的，先学学Spring，入下门吧。
**Spring RMI**
       Spring RMI中，主要有两个类：*org.springframework.remoting.rmi.RmiServiceExporter*和*org.springframework.remoting.rmi.RmiProxyFactoryBean*
服务端使用*RmiServiceExporter*暴露RMI远程方法，客户端用*RmiProxyFactoryBean*间接调用远程方法。
maven依赖：
<dependency>
<groupId>org.springframework</groupId>
<artifactId>spring-remoting</artifactId>
<version>2.0.8</version>
</dependency>
   首先，也是两个工程，服务端用Web工程，因为使用Spring，我们依托Web容器来完成。
1、在该服务端Web工程中添加接口，普通接口，无需继承其他
**[java]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- package leon.rmi.iface;  
- 
- publicinterface IUserDao {  
- public String getUserList();  
- publicint sum(int a, int b);  
- }  
  2、接口的实现类
**[java]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- package leon.rmi.impl;  
- 
- import leon.rmi.iface.IUserDao;  
- 
- publicclass UserDaoImpl implements IUserDao {  
- 
- @Override
- public String getUserList() {  
- return"Hello,Get the user list from database!";  
-     }  
- 
- @Override
- publicint sum(int a, int b) {  
- return a+b;  
-     }  
- }  
3、在该服务端Web工程中添加Spring的bean配置文件，比如命名为rmi.xml，内容如下：
**[html]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- 
- <beans:beansxmlns="http://www.springframework.org/schema/security"
- xmlns:beans="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-     http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-     http://www.springframework.org/schema/security  
-     http://www.springframework.org/schema/security/spring-security-3.0.xsd">
- 
- <beans:beanid="userDaoRmi"class="leon.rmi.impl.UserDaoImpl">
- </beans:bean>
- 
- <beans:beanid="userSvcExporter"class="org.springframework.remoting.rmi.RmiServiceExporter">
- <beans:propertyname="service"ref="userDaoRmi"/>
- <beans:propertyname="serviceName"value="userDaoService"/>
- <beans:propertyname="serviceInterface"value="leon.rmi.iface.IUserDao"/>
- <beans:propertyname="registryPort"value="9111"/>
- <beans:propertyname="servicePort"value="10023"/>
- </beans:bean>
- </beans:beans>
**   说明：**这里不详细的说明了，主要配置了真实实现类，用RmiServiceExporter暴露时，配置property要注意的有service,serviceName,serviceInterface,端口registryPort。
   启动Web工程的服务器，该配置文件应该被Spring的监听器监听，并加载，启动成功后，服务端就算建好了。如果服务器是在localhost启动的，那么暴露的RMI的IP也是localhost，如果需要使用其他IP，需要让服务器在其他的IP启动。
好了，吃完午饭继续写
**客户端调用**
为了方便也只新建一个简单的Java Project，使用静态的java代码来调用了。
1、 在源文件src下建立一个springbeans.xml
**[html]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- 
- <beans:beansxmlns="http://www.springframework.org/schema/security"
- xmlns:beans="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-     http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-     http://www.springframework.org/schema/security  
-     http://www.springframework.org/schema/security/spring-security-3.0.xsd">
- 
- <beans:beanid="userDaoProxy"class="org.springframework.remoting.rmi.RmiProxyFactoryBean">
- <beans:propertyname="serviceUrl"value="rmi://localhost:9111/userDaoService"/>
- <beans:propertyname="serviceInterface"value="leon.rmi.iface.IUserDao"/>
- </beans:bean>
- 
- </beans:beans>
这里注意到RmiProxyFactoryBean的两个重要的property：**serviceUrl**和**serviceInterface**，IUserDao接口可以从服务端的接口打成jar包来提供。
2、 新建java类
**[java]**[view plain](http://blog.csdn.net/leon709/article/details/7090436#)[copy](http://blog.csdn.net/leon709/article/details/7090436#)
- package testrmi;  
- 
- import leon.rmi.iface.IUserDao;  
- 
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- 
- publicclass TestRMI2 {  
- publicstaticvoid main(String[] args) {  
-          ApplicationContext ctx = new ClassPathXmlApplicationContext("springbeans.xml");  
-          IUserDao userDao = (IUserDao) ctx.getBean("userDaoProxy");  
-          System.out.println(userDao.getUserList());  
-          System.out.println(userDao.sum(145, 487));  
-     }  
- }  
运行，成功。
好了，这就是一个采用Spring封装的RMI的例子，项目工作中应该经常使用的。
有什么问题，欢迎讨论交流。
