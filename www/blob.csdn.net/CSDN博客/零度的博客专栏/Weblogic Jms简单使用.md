# Weblogic Jms简单使用 - 零度的博客专栏 - CSDN博客
2016年05月25日 15:56:30[零度anngle](https://me.csdn.net/zmx729618)阅读数：3447
1       基本用法
       对于Weblogic Jms的使用可以分为两部分，一部分是Weblogic作为服务端发布对应的Jms服务为客户端程序提供服务，一部分是我们的应用程序作为客户端发布消息到Jms服务端或从Jms服务端获取消息进行消费。
## 1.1     Jms服务端 
       Weblogic作为Jms服务端提供Jms服务需要在Weblogic的控制台进行一些配置，主要是Jms服务器和Jms模块。Jms服务器作为Jms服务的核心提供Jms服务，也可以说是作为Jms模块的一个管理容器；Jms模块下可以进行ConnectionFactory、Topic、Queue等资源的定义，Weblogic允许我们通过JNDI对它们进行访问。
### 1.1.1配置Jms服务器
       首先我们需要配置一个Jms服务器。在控制台左边的域结构下面找到对应的域，进入到服务->消息传送->Jms服务器，如下图所示。
![](http://dl2.iteye.com/upload/attachment/0112/5696/9361469a-df56-3410-9211-998bf6aa0490.png)
       然后在右边出现的列表点击“新建”按钮进入新建Jms服务器的导航页面，之后根据提示一步一步操作即可。
### 1.1.2配置Jms模块
       配置好Jms服务器后我们需要配置对应的Jms模块，Jms模块用来分模块管理一组资源，一个Jms模块下可包括ConnectionFactory、Topic、Queue等资源。在控制台左边的域结构下面，进入到服务->消息传送->Jms模块，如下图所示。
![](http://dl2.iteye.com/upload/attachment/0112/5698/663163b7-6892-34bf-ae0c-f5c922b3b166.png)
       然后在右边出现的列表中点击“新建”按钮进入新建Jms模块的导航界面，之后就根据提示进行操作即可。新建好Jms模块后，我们可以在Jms模块列表点击对应Jms模块的链接到对应Jms模块下的资源列表页面，在对应的资源列表页面，我们点击“新建”按钮新建对应的资源。
## 1.2     Jms客户端 
### 1.2.1Jar包导入
       在进行客户端开发的时候我们首先需要导入对应的jar包。按照Weblogic官网文档的说明，我们在开发Weblogic Jms客户端的时候只需要把wlclient.jar和wljmsclient.jar加入classpath即可。这两个jar包在Weblogic安装目录（WL_HOME）下的server/lib目录下，如：/home/weblogic/Oracle/Middleware/Oracle_Home/wlserver/server/lib。但是我在开发的时候只导入这两个jar包，客户端程序跑不起来，错误信息是“Root
 exception is org.omg.CORBA.COMM_FAILURE:   vmcid: SUN  minor code: 203  completed: No”。而加上wlfullclient.jar则访问正常。wlfullclient.jar默认不在WL_HOME/server/lib目录下，需要我们在WL_HOME/server/lib目录下通过运行“[Java](http://lib.csdn.net/base/17)
 –jar wljarbuilder.jar”来生成，生成后的wljarbuilder.jar非常大，有60多M，可以将里面一些与工程现有jar包有冲突的类删除。
### 1.2.2程序开发
       客户端程序的开发的时候获取如ConnectionFactory、Topic、Queue等这些在服务端配置的资源时都需要通过JNDI从服务端获取。所以我们首先得构造一个获取JNDI对象的Context。在构造Context时我们需要提供两个参数，Context.PROVIDER_URL和Context.INITIAL_CONTEXT_FACTORY，这两个参数都由Weblogic提供，第一个参数表示提供JNDI对象的服务地址，这里对应的值是“t3://host:port”，host是Weblogic服务主机的IP地址，port是对应的端口；第二个参数是用于获取InitialContext的InitialContextFactory接口的实现类名称，对于Weblogic而言，它应当是“weblogic.jndi.WLInitialContextFactory”。这里我针对它提供了一个工具类，这个工具类的主要作用是把Context封装起来了，然后提供了对应的根据JNDI名称获取对应对象的方法，还封装了一个ConnectionFactory，提供了获取Connection的方法。
**import** java.io.IOException;
**import** java.util.Hashtable;
**import** java.util.Properties;
**import** javax.jms.Connection;
**import** javax.jms.ConnectionFactory;
**import** javax.jms.JMSException;
**import** javax.naming.Context;
**import** javax.naming.InitialContext;
**import** javax.naming.NamingException;
**import** org.apache.log4j.Logger;
**public class** JmsUtil {
**private static** Context
*context*;
**private static final** Properties
*props* = **new** Properties();
**private static final** ConnectionFactory
*connectionFactory*;
**private static final** String
*CONNECTION_FACTORY_JNDI* = 
"weblogic.jms.XAConnectionFactory";
**private static final** Logger
*logger* = Logger.*getLogger*(JmsUtil.**class**);
**static** {
**try** {
*props*.load(JmsUtil.**class**.getResourceAsStream("jms.properties"));
      } **catch** (IOException e1) {
*logger*.error("读取jms.properties配置文件出错",
 e1);
      }
      Hashtable<String, String> environment = **new** Hashtable<>();
//指定对应JNDI提供者的地址
      environment.put(Context.*PROVIDER_URL*,
*props*.getProperty(Context.*PROVIDER_URL*));
//指定获取InitialContext的InitialContextFactory类
      environment.put(Context.*INITIAL_CONTEXT_FACTORY*,
*props*.getProperty(Context.*INITIAL_CONTEXT_FACTORY*));
**try** {
//构造一个获取JNDI对象的Context
*context* = **new** InitialContext(environment);
      } **catch** (NamingException e) {
*logger*.error("初始化InitialContext失败",
 e);
      }
*connectionFactory* = JmsUtil.*lookup*(*CONNECTION_FACTORY_JNDI*);
   }
/**
    * 获取Jms连接
    * **@return**
    */
**public static** Connection getConnection() {
**try** {
**return***connectionFactory*.createConnection();
      } **catch** (JMSException e) {
*logger*.error("创建Connection失败", e);
**returnnull**;
      }
   }
/**
    * 获取指定名称的JNDI对象
    * **@param** name
    * **@return**
    */
**public static** <T> T lookup(String name) {
**try** {
**return** (T) *context*.lookup(name);
      } **catch** (NamingException e) {
*logger*.error("对应的资源不存在", e);
**returnnull**;
      }
   }
}
       上述所使用到的jms.properties文件的内容如下。
java.naming.provider.url=t3://10.10.10.3:7001
java.naming.factory.initial=weblogic.jndi.WLInitialContextFactory
具体程序开发的步骤如下。
**1、获取ConnectionFactory**
       获取ConnectionFactory的时候可以获取我们自己定义好的ConnectionFactory，也可以直接获取Weblogic预先为我们定义好的两个ConnectionFactory，它们对应的JNDI名称如下。
·         `**weblogic.jms.ConnectionFactory**`
·         `**weblogic.jms.XAConnectionFactory**`
       这两个内置的ConnectionFactory的区别是第二个支持JTA事务，而第一个不支持，只支持本地事务。
//根据JNDI名称获取对应的ConnectionFactory
      ConnectionFactory connectionFactory = JmsUtil.*lookup*("weblogic.jms.XAConnectionFactory");
       这里获取的ConnectionFactory是一个Weblogic提供的JMSConnectionFactory对象或JMSXAConnectionFactory对象，你可以根据需要将它们强转为一个QueueConnectionFactory对象或TopicConnectionFactory对象，QueueConnectionFactory和TopicConnectionFactory是Weblogic提供的专门针对为Queue服务和为Topic服务的两个接口。在下文的步骤中都不会将它们进行类型强转，而是基于标准的Jms接口进行开发。有兴趣的朋友可以查看Weblogic的Jms文档或对应的API文档了解更多内容。
**2、获取Connection**
       Connection是通过ConnectionFactory进行获取的。
//通过ConnectionFactory获取一个Connection
      Connection conn = connectionFactory.createConnection();
**3、创建一个Session**
//第一个参数表示是否需要事务支持，第二个参数表示消息的应答方式
      Session session = conn.createSession(**false**, Session.*AUTO_ACKNOWLEDGE*);
**4、获取目的地**
       无论我们的程序是作为生产者还是作为消费者，它都需要一个目的地，这个目的地可以是一个Topic，也可以是一个Queue。
//获取一个目的地，这里暂时不区分是Topic还是Queue
      Destination dest = JmsUtil.*lookup*("dest_queue");
**5、生成者模式**
       （1）如果我们的客户端是作为生产者进行消息的生产，那么我们就需要创建一个对应的生产者。生产者是通过Session进行创建的。
//创建指定目的地的生产者
      MessageProducer producer = session.createProducer(dest);
       （2）有了生产者之后我们还需要创建对应的需要进行发送的Message，Message是通过Session进行发送的，Message有很多种类型，以下只是简单的文本类型的Message。
//创建一个消息
      Message message = session.createTextMessage("Hello");
       （3）有了Message后我们就可以把它交给生产者进行发送了。
//发送消息
      producer.send(message);
**6、消费者模式**
       （1）如果我们的客户端程序是作为消费者进行消费的，那么我们首先应该创建一个消费指定目的地Message的消费者。
//创建消费者
      MessageConsumer consumer = session.createConsumer(dest);
       （2）创建了消费者后就是对消息的消费了，MessageConsumer中定义了好几个接收消息的方法，不过它们都是一次消费，也就是说调用一次最多只会消费一条Message，也有不消费的，方法名是以receive打头的，这里就不说了有兴趣的朋友可以去看一下API文档。这里准备介绍的是通过MessageListener进行消费的情形，即给MessageConsumer指定一个MessageListener，这样一旦有对应的可以进行消费的消息过来后就会自动的回调MessageListener的onMessage方法。
//指定消息监听器
      consumer.setMessageListener(**new** MessageListener() {
@Override
**publicvoid** onMessage(Message msg) {
*logger*.info("收到一条消息： " + msg);
         }
      });
**7、启动Connection**
       当我们的客户端程序是作为消费者时最后需要调用Connection的start方法，这样我们的Consumer才能接收到对应的信息，如果什么时候希望暂停接收信息，则可以调用对应Connection的stop方法，等到想再次接收Message的时候可以再次调用Connection的start方法。当我们的程序是作为生产者时可以不用调用Connection的start方法。
       到此我们的Weblogic Jms客户端程序开发已经基本完成了，Jms开发的大体思路也都是这样的。上述内容只是比较简单的描述了下服务端的配置和客户端的开发过程。如果希望进行更深入的了解可以参考Weblogic的官方文档和Jms相关的API文档。
# 2       整合Spring 
       Spring整合Jms的文章之前写过，是基于ActiceMQ写的，这里关于Spring整合Jms的内容就不详细说了，只是简单的说一下基于Weblogic的Jms整合Spring时一些不一样的地方。Weblogic的Jms资源都是通过JNDI获取的，所以在整合Spring时这些资源也还是需要通过JNDI进行获取。Spring为我们提供了一个JndiTemplate类，其中封装了获取InitialContext的方法，通过它我们可以获取到指定InitialContext下指定JNDI名称的对象。Spring还为我们提供了一个JndiObjectFactoryBean类，它实现了FactoryBean接口，通过给它指定一个jndiTemplate和一个jndiName可以让它返回指定jndiName在JndiTemplate中对应的对象。所以在整合Spring的时候我们可以通过它们来获取Weblogic提供的那些Jms对象，其它的就喝Spring整合标准的Jms是一样的用法了，具体可以参考我之前写的关于Spring整合JMS的文章。以下是Weblogic
 Jms整合Spring的一段示例配置。
<beanid=*"jndiTemplate"*class=*"org.springframework.jndi.JndiTemplate"*>
<propertyname=*"environment"*>
<props>
<propkey=*"java.naming.factory.initial"*>${java.naming.factory.initial}</prop>
<propkey=*"java.naming.provider.url"*>${java.naming.provider.url}</prop>
</props>
</property>
</bean>
<!-- 提供Jms服务的ConnectionFactory -->
<beanid=*"jmsConnectionFactory"*class=*"org.springframework.jndi.JndiObjectFactoryBean"*
p:jndiTemplate-ref=*"jndiTemplate"*p:jndiName=*"${jms.jndi.example.connection.factory}"*/>
<!-- Spring封装的Jms ConnectionFactory，需要注入一个真正提供Jms服务的ConnectionFactory
 -->
<beanid=*"springJmsConnectionFactory"*
class=*"org.springframework.jms.connection.SingleConnectionFactory"*
p:targetConnectionFactory-ref=*"jmsConnectionFactory"*/>
<beanid=*"statusQueue"*class=*"org.springframework.jndi.JndiObjectFactoryBean"*
p:jndiTemplate-ref=*"jndiTemplate"*p:jndiName=*"${jms.jndi.example.status.queue}"*/>
<beanid=*"jmsTemplate"*class=*"org.springframework.jms.core.JmsTemplate"*
p:connectionFactory-ref=*"springJmsConnectionFactory"*/>
<!-- Spring提供的消息监听容器，用以绑定ConnectionFactory、Destination和MessageListener，为指定ConnectionFactory的Destination指定MessageListener
 -->
<beanid=*"statusQueueListenerContainer"*
class=*"org.springframework.jms.listener.DefaultMessageListenerContainer"*
p:connectionFactory-ref=*"springJmsConnectionFactory"*
p:destination-ref=*"statusQueue"*p:messageListener-ref=*"statusMessageListener"*/>
<!-- 消息监听器 -->
<beanid=*"statusMessageListener"*class=*"com.xxx.StatusMessageListener"*/>
