# Mina程序的JMX支持 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月14日 13:14:41[boonya](https://me.csdn.net/boonya)阅读数：1151
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址:[JMX Support](http://mina.apache.org/mina-project/userguide/ch16-jmx-support/ch16-jmx-support.html)


Java管理扩展(JMX)是用于管理和监控Java应用程序。本教程将向您提供一个例子: 怎样建立JMX可用的Mina应用程序。本教程的目的是帮助你让JMX技术集成基于MINA的应用程序。在本教程中,我们将整合MINA-JMX类到虚拟（想象）服务器示例程序。

### **添加JMX支持**

为使JMX支持Mina应用程序我们需要先做好如下操作：
- 创建/获取 MBean服务器
- 实例化所需的mbean(IoAcceptor IoFilter)
- 与MBean服务器注册MBean


剩下来的时间我们会根据\src\main\java\org\apache\mina\example\imagine\step3\server\ImageServer.java来进行讨论。

#### **创建或获取MBean服务端**



```java
// create a JMX MBean Server server instance
MBeanServer mBeanServer = ManagementFactory.getPlatformMBeanServer();
```
这一行代码是获取MBean服务端实例。

**实例化MBean(s)**

我们为IoService创建一个MBean：



```java
// create a JMX-aware bean that wraps a MINA IoService object.  In this
// case, a NioSocketAcceptor. 
IoServiceMBean acceptorMBean = new IoServiceMBean( acceptor );
```
创建一个IoService MBean。它接受通过JMX的接收器实例。相似地,您可以添加IoFilterMBean和其他自定义MBeans。
#### 通过MBean服务器注册MBean


// create a JMX ObjectName.  This has to be in a specific format.  ObjectNameacceptorName=newObjectName(acceptor.getClass().getPackage().getName()+":type=acceptor,name="+acceptor.getClass().getSimpleName());// register the bean on the MBeanServer.  Without this line, no JMX will happen for// this acceptor.mBeanServer.registerMBean(acceptorMBean,acceptorName);我们创建一个对象名,需要用作访问MBean的逻辑名称和注册MBean的MBean服务器。现在我们的应用程序中启用JMX。让我们看它的实际应用。
#### **启动虚拟的MBean服务器端**

如果你使用Java 5 或者更早版本：



```java
java -Dcom.sun.management.jmxremote -classpath <CLASSPATH> org.apache.mina.example.imagine.step3.server.ImageServer
```
如果你使用Java 6：


java-classpath<CLASSPATH>}}{{{}org.apache.mina.example.imagine.step3.server.ImageServer
#### **启动JConsole**

使用如下命令启动Jconsole：



```java
/bin/jconsole
```
我们可以看到不同的属性和操作所暴露的MBeans。





