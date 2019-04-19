# RMI(远程接口调用) - 毛小亮 - 博客园
# [RMI(远程接口调用)](https://www.cnblogs.com/xianerwonder/p/5710439.html)
**1. RMI的原理: **
RMI系统结构，在客户端和服务器端都有几层结构。 
方法调用从客户对象经占位程序（Stub)、远程引用层(Remote Reference Layer)和传输层（Transport Layer）向下，传递给主机，然后再次经传 输层，向上穿过远程调用层和骨干网（Skeleton），到达服务器对象。 占位程序扮演着远程服务器对象的代理的角色，使该对象可被客户激活。 远程引用层处理语义、管理单一或多重对象的通信，决定调用是应发往一个服务器还是多个。传输层管理实际的连接，并且追追踪可以接受方法调用的远程对象。服务器端的骨干网完成对服务器对象实际的方法调用，并获取返回值。返回值向下经远程引用层、服务器端的传输层传递回客户端，再向上经传输层和远程调用层返回。最后，占位程序获得返回值。 
**2.	RMI（远程方法调用）的组成 **
一个正常工作的RMI系统由下面几个部分组成： 
  •远程服务的接口定义 
  •远程服务接口的具体实现 
  •桩（Stub）和框架（Skeleton）文件 
  •一个运行远程服务的服务器 
  •一个RMI命名服务，它允许客户端去发现这个远程服务 
  •类文件的提供者（一个HTTP或者FTP服务器） 
      •一个需要这个远程服务的客户端程序 
**3.	RMI的实现 **
(1)	服务器端的实现 
编写一个远程接口 
```
1 package com.gjy.rmi.service;
2 
3 import java.rmi.Remote;
4 import java.rmi.RemoteException;
5 
6 public interface AddServer extends Remote { 
7     public int AddNumbers(int firstnumber,int secondnumber) throws RemoteException; 
8 }
```
(2) 编写远程接口的实现 
```
1 package com.gjy.rmi.service;  
 2   
 3 import java.rmi.RemoteException;  
 4 import java.rmi.server.UnicastRemoteObject;  
 5   
 6 public class AddServerImpl extends UnicastRemoteObject implements AddServer {   
 7     public AddServerImpl() throws RemoteException {   
 8         super();   
 9     }   
10     public int AddNumbers(int firstnumber,int secondnumber) throws RemoteException {   
11         return firstnumber + secondnumber;   
12     }   
13 }
```
(3) 编写服务器端程序 
```
1 package com.gjy.rmi.service;  
 2   
 3 import java.rmi.Naming;  
 4 import java.rmi.registry.LocateRegistry;  
 5   
 6 public class RmiServer {   
 7      /**    
 8         * 启动 RMI 注册服务并进行对象注册    
 9         */     
10        public static void main(String[] argv)      
11        {      
12           try     
13           {      
14              //启动RMI注册服务，指定端口为1099　（1099为默认端口）      
15              //也可以通过命令 ＄java_home/bin/rmiregistry 1099启动      
16              //这里用这种方式避免了再打开一个DOS窗口      
17              //而且用命令rmiregistry启动注册服务还必须事先用RMIC生成一个占位程序(stub类)为它所用      
18              LocateRegistry.createRegistry(1099);      
19                   
20              //创建远程对象的一个或多个实例，下面是hello对象      
21              //可以用不同名字注册不同的实例      
22              AddServer add = new AddServerImpl();      
23                   
24              //把hello注册到RMI注册服务器上，命名为Hello      
25              Naming.rebind("Hello", add);      
26              //如果要把hello实例注册到另一台启动了RMI注册服务的机器上      
27              //Naming.rebind("//192.168.1.105:1099/Hello",hello);      
28                   
29              System.out.println("Hello Server is ready.");      
30           }      
31           catch (Exception e)      
32           {      
33              System.out.println("Hello Server failed: " + e);      
34           }      
35        }     
36 }  
View Code
```
(4) 编写客户端程序 
```
1 package com.gjy.rmi.client;  
 2   
 3 import java.net.MalformedURLException;  
 4 import java.rmi.Naming;  
 5 import java.rmi.NotBoundException;  
 6 import java.rmi.RemoteException;  
 7   
 8 import com.gjy.rmi.service.AddServer;  
 9   
10 public class RmiClient {   
11 public static void main(String args[]) throws RemoteException, MalformedURLException, NotBoundException {   
12         String url="rmi://127.0.0.1/Hello";   
13         AddServer add;   
14         add = (AddServer)Naming.lookup(url);   
15         int result=0;  
16         for (int i =0;i<10;i++){  
17         result = add.AddNumbers(10,i);  
18         System.out.println(result);   
19         }  
20     }   
21 }  
客户端
```

