# 远程方法调用（RMI）原理与示例 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [咸菜](http://www.jobbole.com/members/Pickle) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# RMI 介绍
远程方法调用（RMI）顾名思义是一台机器上的程序调用另一台机器上的方法。这样可以大致知道RMI是用来干什么的，但是这种理解还不太确切。RMI是Java支撑分布式系统的基石，例如著名的EJB组件。RMI是远程过程调用（RPC）的一种面向对象实现，RMI底层是通过socket通信和对象序列化技术来实现的。这里引用[Wikipedia](https://en.wikipedia.org/wiki/Java_remote_method_invocation)对RMI的介绍：
The **Java Remote Method Invocation** (**Java RMI**) is a Java API that performs remote method invocation, the object-oriented equivalent of remote procedure calls (RPC), with support for direct transfer of serialized Java classes and distributed garbage collection.
- The original implementation depends on Java Virtual Machine(JVM) class representation mechanisms and it thus only supports making calls from one JVM to another. The protocol underlying this Java-only implementation is known as Java Remote Method Protocol (JRMP).
- In order to support code running in a non-JVM context, a CORBA version was later developed.
Usage of the term RMI may denote solely the programming interface or may signify both the API and JRMP, IIOP, or another implementation, whereas the term RMI-IIOP (read: RMI over IIOP) specifically denotes the RMI interface delegating most of the functionality to the supporting CORBA implementation.
The basic idea of Java RMI, the distributed garbage-collection (DGC) protocol, and much of the architecture underlying the original Sun implementation, come from the ‘network objects’ feature of Modula-3.
# RMI 基本原理
RMI的目的就是要使运行在不同的计算机中的对象之间的调用表现得像本地调用一样。RMI 应用程序通常包括两个独立的程序:服务器程序和客户机程序。RMI 需要将行为的定义与行为的实现分别定义, 并允许将行为定义代码与行为实现代码存放并运行在不同的 JVM 上。在 RMI 中, 远程服务的定义是存放在继承了 Remote 的接口中。远程服务的实现代码存放在实现该定义接口的类中。
RMI 支持两个类实现一个相同的远程服务接口: 一个类实现行为并运行在服务器上, 而另一个类作为一个远程服务的代理运行在客户机上。客户程序发出关于代理对象的调用方法, RMI 将该调用请求发送到远程 JVM 上, 并且进一步发送到实现的方法中。实现方法将结果发送给代理, 再通过代理将结果返回给调用者。
RMI 构建三个抽象层, 高层覆盖低层, 分别负责Socket通信, 参数和结果的序列化和反序列化等工作。存根( Stub) 和骨架( Skeleton) 合在一起形成了 RMI 构架协议。下面的引用层被用来寻找各自的通信伙伴, 在这一层还有一个提供名字服务的部分, 称为注册表( registry) 。最下一层是传输层, 是依赖于 TCP/IP 协议实现客户机与服务器的互联。
![](http://jbcdn2.b0.upaiyun.com/2016/08/238310f75b550014ddfdb1692b65c33a.png)
当客户端调用远程对象方法时, 存根负责把要调用的远程对象方法的方法名及其参数编组打包,并将该包向下经远程引用层、传输层转发给远程对象所在的服务器。通过 RMI 系统的 RMI 注册表实现的简单服务器名字服务, 可定位远程对象所在的服务器。
该包到达服务器后, 向上经远程引用层, 被远程对象的 Skeleton 接收, 此 Skeleton 解析客户包中的方法名及编组的参数后, 在服务器端执行客户要调用的远程对象方法, 然后将该方法的返回值( 或产生的异常) 打包后通过相反路线返回给客户端, 客户端的 Stub 将返回结果解析后传递给客户程序。事实上, 不仅客户端程序可以通过存根调用服务器端的远程对象的方法, 而服务器端的程序亦可通过由客户端传递的远程接口回调客户端的远程对象方法。在分布式系统中, 所有的计算机可以是服务器, 同时又可以是客户机。
![](http://jbcdn2.b0.upaiyun.com/2016/08/3d7877359da9ce21cb5f57306d02f207.jpg)
# RMI 应用示例
Remote 接口用于标识其方法可以从非本地虚拟机上调用的接口。任何远程对象都必须直接或间接实现此接口。只有在“远程接口”（扩展 java.rmi.Remote 的接口）中指定的这些方法才可远程使用。 也就是说需要远程调用的方法必须在扩展Remote接口的接口中声名并且要抛出RemoteException异常才能被远程调用。
远程对象必须实现java.rmi.server.UniCastRemoteObject类，这样才能保证客户端访问获得远程对象时，该远程对象将会把自身的一个拷贝序列化后以Socket的形式传输给客户端，此时客户端所获得的这个拷贝称为“存根”，而服务器端本身已存在的远程对象则称之为“骨架”。其实此时的存根是客户端的一个代理，用于与服务器端的通信，而骨架也可认为是服务器端的一个代理，用于接收客户端的请求之后调用远程方法来响应客户端的请求。 远程对象的接口和实现必须在客户端和服务器端同时存在并且保持一致才行。
![](http://jbcdn2.b0.upaiyun.com/2016/08/82a80da9557f31a0286a1dc6eac13504.png)
实现代码：
远程接口定义

```
package com.wxisme.rmi;
import java.rmi.Remote;
import java.rmi.RemoteException;
/**
 *@Description:<p>远程接口定义</p>
 *@author 王旭
 *<a href="http://www.jobbole.com/members/q677911625">@time</a> 2016年3月14日 下午4:53:48
 */
public interface HelloDefine extends Remote {
    
    public String helloWorld() throws RemoteException;
    
    public String sayHello(String name) throws RemoteException;
    
}
```
远程接口实现


```
package com.wxisme.rmi;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
/**
 *@Description:<p>远程接口实现</p>
 *@author 王旭
 *<a href="http://www.jobbole.com/members/q677911625">@time</a> 2016年3月14日 下午4:57:50
 */
public class HelloDefineImp extends UnicastRemoteObject implements HelloDefine {
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    public HelloDefineImp() throws RemoteException {
        super();
    }
    public String helloWorld() throws RemoteException {
        return "Hello AlphaGo!";
    }
    public String sayHello(String name) throws RemoteException {
        return "Hello" + name +"!";
    }
}
```
服务端绑定


```
package com.wxisme.rmi;
import java.net.MalformedURLException;
import java.rmi.AlreadyBoundException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
/**
 *@Description:<p>服务端绑定</p>
 *@author 王旭
 *<a href="http://www.jobbole.com/members/q677911625">@time</a> 2016年3月14日 下午4:59:33
 */
public class HelloServer {
    
    HelloDefine hello;
    
    public void server() throws RemoteException, MalformedURLException, AlreadyBoundException {
        hello = new HelloDefineImp();
        
        //远程对象注册表实例
        LocateRegistry.createRegistry(8888);
        //把远程对象注册到RMI注册服务器上
        Naming.bind("rmi://localhost:8888/Hello", hello);
        System.out.println("server:对象绑定成功！");
    }
}
```
客户端调用


```
package com.wxisme.rmi;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
/**
 *@Description:<p>客户端调用</p>
 *@author 王旭
 *<a href="http://www.jobbole.com/members/q677911625">@time</a> 2016年3月14日 下午5:08:51
 */public class HelloClient {
    
    public HelloDefine hello;
    
    public void client() throws MalformedURLException, RemoteException, NotBoundException {
        //在RMI注册表中查找指定对象
        hello = (HelloDefine) Naming.lookup("rmi://localhost:8888/Hello");
        //调用远程对象方法
        System.out.println("client:");
        System.out.println(hello.helloWorld());
        System.out.println(hello.sayHello("神之一手"));
    }
}
```
测试


```
package com.wxisme.rmi;
import java.net.MalformedURLException;
import java.rmi.AlreadyBoundException;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import org.junit.Test;
/**
 *@Description:<p>测试</p>
 *@author 王旭
 *<a href="http://www.jobbole.com/members/q677911625">@time</a> 2016年3月14日 下午5:14:36
 */
public class RMITest {
    <a href="http://www.jobbole.com/members/madao">@Test</a>
    public void testServer() throws RemoteException, MalformedURLException, AlreadyBoundException {
        HelloServer server = new HelloServer();
        server.server();
        while(true);
    }
    
    <a href="http://www.jobbole.com/members/madao">@Test</a>
    public void testClient() throws MalformedURLException, RemoteException, NotBoundException {
        HelloClient client = new HelloClient();
        client.client();
    }
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/08/7fbaf9e0876f4add0b0d1b474694b25d.png)
![](http://jbcdn2.b0.upaiyun.com/2016/08/1b98b6e1b615d89967f19e81f8209748.png)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/09/c67bd37238d8422176f282e5b98a80a9.png)![](http://jbcdn2.b0.upaiyun.com/2016/09/40382ba66fc52885f43198654438713e.jpg)
