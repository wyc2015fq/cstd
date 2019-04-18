# Thrift之双向通讯 - 深之JohnChen的专栏 - CSDN博客

2017年08月06日 18:47:22[byxdaz](https://me.csdn.net/byxdaz)阅读数：1814


在实际应用中，却经常会有客户端建立连接后，等待服务端数据的长连接模式，也可以称为双向连接。
一、双连接，服务端与客户端都开ThriftServer
如果网络环境可控，可以让服务端与客户端互相访问，你可以给服务端与客户端，两者都开一个ThriftServer，也就是两者互为服务端与客户端。这样就可以简单实现互相访问，比如：
客户端： <------------------->  服务端：
ThriftClient --------------> ThriftService
ThriftService <------------- ThriftClient

二、单连接，利用ProcessorFactory中TConnectionInfo的transport定时向客户端发送消息，让thrift保持长连接不立即关闭。
thrift是rpc结构的通信框架，rpc结构默认是 【客户端请求 -> 服务端回应 -> 连接断开】 的这种短连接形式，因此rpc默认是没有服务端回调功能，自然也没有长连接。
如果要保持连接不关闭且被动接收到对方的数据，需要指定双方连接的service必须为oneway，服务端定时向客户端发送信息（利用客户端发送数据到服务端时连接成功时产生的transport，需客户端也创建服务Processor），同时客户端实时检测transport的状态，以便出现与服务端连接断开的情况出现。具体流程：
1、双向连接的service必须为oneway，否则会因为recv函数抛出remote close异常。
2、客户端重用建立client的protocol，开线程使用processor.Process(protocol,protocol)监听服务端回调发送过来的消息。
3、服务端Processor的创建，使用ProcessorFactory创建Processor，通过getProcessor函数中transport作为向客户端发送消息的client的transport而创建一个Processor。

java实例

定义test.thrift

```java
namespace java com.zychen.thrift
service ClientHandshakeService{
    oneway void HandShake();
}

service ServerCallbackService{
    oneway void Push(1: string msg); 
}
```

生成接口代码

把thrift-0.9.3.exe和test.thrift文件放在同一个目录。

进入DOS命令执行：thrift-0.9.3.exe --gen java test.thrift

生成文件gen-java/ com/zychen/thrift/Test.java

服务端代码
ClientHandshakeServiceHandler.java

```java
package com.zychen.thrift;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.transport.TTransport;

public class ClientHandshakeServiceHandler implements ClientHandshakeService.Iface {
	public ClientHandshakeServiceHandler(TTransport trans){
		client = new ServerCallbackService.Client(new TBinaryProtocol(trans));
	}
	
	@Override
	public void HandShake() throws TException {
		 System.out.println("HandShake\n");
		 StartThread();
	}

	//开始线程
	public void StartThread(){
		if(threadCallback == null){
			stopThread = false;
			threadCallback = new Thread(new CallbackThread());
			threadCallback.start();
		}
	}
	
	//停止线程
	public void StopThread(){
		stopThread = true;
		if(threadCallback != null){
			try {
				threadCallback.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			threadCallback = null;
		}
	}
	
	@Override
	protected void finalize() throws Throwable {
		// TODO Auto-generated method stub
		StopThread();
		super.finalize();
	}

	protected ServerCallbackService.Client client;
	protected boolean stopThread = false;
	protected Thread  threadCallback = null;
	
	class CallbackThread implements Runnable {
		public void run() {
			while(true){
				if(stopThread){
					break;
				}
				try {
					client.Push("aaaaaaa");
					Thread.sleep(50);
				} catch (TException | InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					return;
				}	
			}
		}
	};
}
```
ProcessorFactoryImpl.java

```java
package com.zychen.thrift;

import org.apache.thrift.TProcessor;
import org.apache.thrift.TProcessorFactory;
import org.apache.thrift.transport.TTransport;

import com.zychen.thrift.ClientHandshakeService.Processor;

public class ProcessorFactoryImpl extends TProcessorFactory {

	public ProcessorFactoryImpl(TProcessor processor) {
		super(processor);
		// TODO Auto-generated constructor stub
	}

	@Override
	public TProcessor getProcessor(TTransport trans) {
		// TODO Auto-generated method stub
		//return super.getProcessor(trans);
        return new ClientHandshakeService.Processor(new ClientHandshakeServiceHandler(trans));
	}
}
```
ServerTest.java

```java
package com.zychen.thrift;

import org.apache.thrift.TProcessorFactory;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TBinaryProtocol.Factory;
import org.apache.thrift.server.TServer;
import org.apache.thrift.server.TThreadPoolServer;
import org.apache.thrift.server.TThreadPoolServer.Args;
import org.apache.thrift.transport.TServerSocket;
import org.apache.thrift.transport.TServerTransport;
import org.apache.thrift.transport.TTransportException;

import com.zychen.thrift.ClientHandshakeService.Processor;

public class ServerTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		TServerSocket tServerSocket;
		try {
			tServerSocket = new TServerSocket(9999);
	        TThreadPoolServer.Args targs = new TThreadPoolServer.Args(tServerSocket);
	        TBinaryProtocol.Factory factory = new TBinaryProtocol.Factory();
	        //获取processFactory
	        TProcessorFactory tProcessorFactory = new ProcessorFactoryImpl(null);
	        targs.protocolFactory(factory);
	        targs.processorFactory(tProcessorFactory);
	        TThreadPoolServer tThreadPoolServer = new TThreadPoolServer(targs); 
	        System.out.println("start server...");
	        tThreadPoolServer.serve();
	        
		} catch (TTransportException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
```

客户端代码
ServerCallbackServiceImpl.java

```java
package com.zychen.thrift;

import java.io.IOException;

import org.apache.thrift.TException;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.transport.TSocket;

public class ServerCallbackServiceImpl implements ServerCallbackService.Iface{
	public ServerCallbackServiceImpl(TSocket socket){
		this.socket = socket;
	}
	
	@Override
	public void Push(String msg) throws TException {
		// TODO Auto-generated method stub
		String str = String.format("receive msg %d: %s", nMsgCount++, msg);
		System.out.println(str);
	}
	
	public void process(){
		processor = new ServerCallbackService.Processor<ServerCallbackService.Iface>(this);
		 TBinaryProtocol protocol = new TBinaryProtocol(socket);
		 while (true)
         {
             try
             {
            	 //TProcessor，负责调用用户定义的服务接口，从一个接口读入数据，写入一个输出接口
                 while (processor.process(protocol, protocol)){
                	//阻塞式方法,不需要内容
                     System.out.println("走阻塞式方法");
                     //关闭socket
                     //socket.close();
                 }
                 //connection lost, return
                 return;
             }catch (TException e){
            	 System.out.println("连接已断开...");
                 e.printStackTrace();
                 return;
             }
         }
	}
	
	protected int nMsgCount = 0;
	protected TSocket socket;
	protected TProcessor processor;
}
```
ClientTest.java

```java
package com.zychen.thrift;

import java.io.IOException;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransportException;

import com.zychen.thrift.ServerCallbackService.Iface;

public class ClientTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		TSocket tSocket = new TSocket("localhost",9999);
		ClientHandshakeService.Client client = new ClientHandshakeService.Client(new TBinaryProtocol(tSocket));
		try {
			tSocket.open();
			runMethod(tSocket);
			//向服务端发送消息
			for (int i = 0; i < 100; ++i){
	            client.HandShake();
	            Thread.sleep(50);
	        }
			System.in.read();
			tSocket.close();
		} catch (TTransportException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (TException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public static void runMethod(final TSocket tSocket){
		Thread thread = new Thread(new Runnable(){
			ServerCallbackServiceImpl serverCallbackServiceImpl = new ServerCallbackServiceImpl(tSocket);
			@Override
			public void run() {
				// TODO Auto-generated method stub
				serverCallbackServiceImpl.process();
			}
			
		});
		thread.start();
	};
}
```

完整代码：[下载](http://download.csdn.net/detail/byxdaz/9922861)

参考资料：
http://www.cnblogs.com/xiaosuiba/p/4122459.html
http://blog.csdn.net/qq_27989757/article/details/50761051

