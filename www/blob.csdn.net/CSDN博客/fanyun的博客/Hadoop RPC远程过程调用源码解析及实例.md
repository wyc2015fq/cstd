# Hadoop RPC远程过程调用源码解析及实例 - fanyun的博客 - CSDN博客
2016年03月24日 10:31:10[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：947
所属专栏：[大数据](https://blog.csdn.net/column/details/bigdata01.html)
**RPC简介：**
        RPC(Remote Procedure Call）远程过程调用，它允许一台计算机程序远程调用另外一台计算机的子程序，而不用去关心底层的网络通信细节，对我们来说是透明的。经常用于分布式网络通信中。
       Hadoop的进程间交互都是通过RPC来进行的，比如Namenode与Datanode之间，Jobtracker与Tasktracker之间等。
       RPC协议假定某些传输协议的存在，如TCP或UDP，为通信程序之间携带信息数据。在OSI网络通信模型中， RPC跨越了传输层和应用层。 RPC使得开发包括网络分布式多程序在内的应用程序更加容易。  
       RPC采用客户机/服务器模式。请求程序就是一个客户机，而服务提供程序就是一个服务器。
       首先，客户机调用进程发送一个有进程参数的调用信息到服务进程，然后等待应答信息，在服务器端，进程保持睡眠状态直到调用信息的到达为止。当一个调用信息到达，服务器获得进程参数，计算结果，发送答复信息给client，然后等待下一个调用信息，最后，客户端调用进程接收答复信息，获得进程结果，然后调用执行继续进行。
RPC特点:
（1）透明性：远程调用其他机器上的程序，对用户来说就像是调用本地方法一样。
（2）高性能：RPC server能够并发处理多个来自Client的请求（请求队列）。3、可控性：jdk中已经提供了一个RPC框架–RMI，但是该RPC框架过于重量级并且可控之处比较少，所以               HadoopRPC实现了自定义的RPC框架。
Hadoop RPC通信:
（1）序列化层：Client与Server端通信传递的信息采用了Hadoop里提供的序列化类或自定义Writable类型。
（2）函数调用层：Hadoop RPC通过动态代理以及Java反射机制实现函数调用。
（3）网络传输层：Hadoop RPC采用了基于TCP/IP的socket机制。
（4）服务器端框架层：RPC Server利用Java NIO以及采用了事件驱动的I/O模型，提高RPC Server的并发处理能力。
Hadoop的整个体系结构就是构建在RPC之上(org.apache.hadoop.ipc)。
Hadoop RPC设计技术
（1）动态代理
（2）反射3、序列化4、非阻塞的异步IO（NIO）
动态代理:
（1）动态代理可以提供对另一个对象的访问，同时隐藏实际对象的具体事实，代理对象对客户隐藏了实际对象。
（2）动态代理可以对请求进行其他的一些处理，在不允许直接访问某些类，或需要对访问做一些特殊处理等，这时候可以考虑使用代理。3）目前Java开发包中提供了对动态代理的支持，但现在只支持对接口的实现。相关的类与接口：java.lang.reflect.Proxy--类java.lang.reflect.InvocationHandler--接口
动态代理创建对象过程：
InvocationHandler handler= new InvocationHandlerImpl(...) Proxy.newInstance(...)
具体实现可参考如下：
![](https://img-blog.csdn.net/20160324103233604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)[](http://s6.51cto.com/wyfs02/M02/6E/37/wKiom1V2gKeQAoehAADPabK1ogA247.jpg)
根据上图查看hadoop2.6.0源码
**Client:**
![](https://img-blog.csdn.net/20160324103442450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Server**:
![](https://img-blog.csdn.net/20160324103453951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**RPC:**
![](https://img-blog.csdn.net/20160324103507247?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
几个重要的协议:
（1）ClientProtocol是客户端(FileSystem)与NameNode通信的接口。
（2）DatanodeProtocol是DataNode与NameNode通信的接口NamenodeProtocol是SecondaryNameNode与NameNode通信的接口。
（3）DFSClient是直接调用NameNode接口的对象。用户代码是通过DistributedFileSystem调用DFSClient对象，才能与NameNode打交道。
![](https://img-blog.csdn.net/20160324103658984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160324103729626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
模拟Hadoop RPC通信
package MyRPC; 
import org.apache.hadoop.io.Text; 
import org.apache.hadoop.ipc.VersionedProtocol; 
public interface MyRPCProtocal extends VersionedProtocol{ 
    public static long versionID = 23234l;//很重要很重要，搞了一下午才解决掉。 
    public Text test(Text t); 
} 
package MyRPC; 
import java.io.IOException; 
import org.apache.hadoop.conf.Configuration; 
import org.apache.hadoop.io.Text; 
import org.apache.hadoop.ipc.ProtocolSignature; 
import org.apache.hadoop.ipc.RPC; 
import org.apache.hadoop.ipc.RPC.Server; 
public class RPCServer implements MyRPCProtocal{     
    Server server = null; 
    public RPCServer() throws IOException, InterruptedException{ 
        //server = RPC.getServer(this,"localhost",8888,new Configuration()); 
        //相对于以前的版本有略微的改动 
        RPC.Builder ins = new RPC.Builder(new Configuration()); 
        ins.setInstance(this); 
        ins.setBindAddress("localhost"); 
        ins.setPort(9999); 
        ins.setProtocol(MyRPCProtocal.class); 
        //RPC.setProtocolEngine(new Configuration(), MyRPCProtocal.class, RpcEngine.class); 
        server = ins.build();//获得一个server实例 
        server.start(); 
        server.join();   
    } 
    public static void main(String[] args) throws IOException, InterruptedException { 
        new RPCServer(); 
    } 
    @Override 
    public long getProtocolVersion(String protocol, long clientVersion) 
            throws IOException { 
        return MyRPCProtocal.versionID; 
    } 
    @Override 
    public ProtocolSignature getProtocolSignature(String protocol, 
            long clientVersion, int clientMethodsHash) throws IOException {      
        return new ProtocolSignature(); 
    } 
    @Override 
    public Text test(Text t) { 
        if(t.toString().equals("RPC")){ 
            return new Text("ok"); 
        } 
        return new Text("false"); 
    } 
} 
package MyRPC; 
import java.net.InetSocketAddress; 
import org.apache.hadoop.conf.Configuration; 
import org.apache.hadoop.io.Text; 
import org.apache.hadoop.ipc.RPC; 
public class RPCClient { 
    private MyRPCProtocal protocal; 
    public RPCClient() throws Exception{ 
        InetSocketAddress address = new InetSocketAddress("localhost",9999); 
        protocal = (MyRPCProtocal)RPC.waitForProxy 
                (MyRPCProtocal.class,MyRPCProtocal.versionID, address, new Configuration()); 
        //RPC.setProtocolEngine(new Configuration(), MyRPCProtocal.class, RpcEngine.class); 
    } 
    public void call(String s){ 
        final Text string = protocal.test(new Text(s)); 
        System.out.println(string.toString()); 
    } 
    public static void main(String[] args) throws Exception { 
        RPCClient client = new RPCClient(); 
        client.call("RPC"); 
    } 
} 
