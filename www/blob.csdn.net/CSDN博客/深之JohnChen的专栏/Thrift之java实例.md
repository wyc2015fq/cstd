# Thrift之java实例 - 深之JohnChen的专栏 - CSDN博客

2017年07月04日 19:20:23[byxdaz](https://me.csdn.net/byxdaz)阅读数：515


一、java实例

1.下载与安装thrift工具

[http://thrift.apache.org/download/](http://thrift.apache.org/download/)

[http://archive.apache.org/dist/thrift/](http://archive.apache.org/dist/thrift/)

2.编写Thrift文件（定义接口，结构，异常等）,保存为test.thrift

namespace java com.zychen.thrift

struct User{

1:i64 id,

2:string name,

3:i32 age,

4:bool vip

}

service Test{

i32add(1:i32 a,2:i32 b)

User getById(1:i64 id)

}

3.生成接口代码

把thrift-0.9.3.exe和test.thrift文件放在同一个目录。

进入DOS命令执行：thrift-0.9.3.exe --gen java test.thrift

生成文件gen-java/ com/zychen/thrift/Test.java

4、服务器代码

服务Test实现类

```java
package com.zychen.thrift;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.thrift.TException;

public class TestImpl implements Test.Iface {

	public void Init(){
		int n = 0;
		for(n=0; n<10; n++){
			User user = new User();
			user.setId(n+1);
			user.setName(String.format("name_%d", n+1));
			user.setAge(18+n+1);
			user.setVip(true);
			listUser.add(user);
		}
	}
	
	@Override
	public int add(int a, int b) throws TException {
		return a+b;
	}

	@Override
	public User getById(long id) throws TException {
		User userObj = null;
		Iterator<User> iter = listUser.iterator();
		while(iter.hasNext()){
			userObj = iter.next();
			if(userObj.getId() == id){
				return userObj;
			}
		}
		
		return null;
	}

	//插入用户信息
	void InsertUser(User user){
		listUser.add(user);
	}
	
	protected List<User> listUser = new ArrayList<User>();
}
```

//服务端代码

```java
package com.zychen.test;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TBinaryProtocol.Factory;
import org.apache.thrift.protocol.TCompactProtocol;
import org.apache.thrift.server.TNonblockingServer;
import org.apache.thrift.server.TServer;
import org.apache.thrift.server.TThreadPoolServer;
import org.apache.thrift.server.TThreadPoolServer.Args;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TNonblockingServerSocket;
import org.apache.thrift.transport.TServerSocket;
import org.apache.thrift.transport.TTransportException;

import com.zychen.thrift.Test;
import com.zychen.thrift.Test.Processor;
import com.zychen.thrift.TestImpl;

public class Server {
	//阻塞式IO
	public void startOIOServer() {
        try {
            TServerSocket serverTransport = new TServerSocket(1234);
            TestImpl testImpl = new TestImpl();
            testImpl.Init();
            Test.Processor process = new Processor(testImpl);
            Factory portFactory = new TBinaryProtocol.Factory(true, true);
            Args args = new Args(serverTransport);
            args.processor(process);
            args.protocolFactory(portFactory);
            TServer server = new TThreadPoolServer(args);
            System.out.println("init...");
            server.serve();
        } catch (TTransportException e) {
            e.printStackTrace();
        }
    }
	
	//使用非阻塞式IO,服务端和客户端需要指定TFramedTransport数据传输的方式。
	public void startNIOServer() {
        try {
            TestImpl testImpl = new TestImpl();
            testImpl.Init();
            Test.Processor tprocessor = new Processor(testImpl);
            TNonblockingServerSocket tnbServerTransport = new TNonblockingServerSocket(1234);
            TNonblockingServer.Args tArgs = new TNonblockingServer.Args(tnbServerTransport);
            tArgs.processor(tprocessor);
            tArgs.transportFactory(new TFramedTransport.Factory());
            tArgs.protocolFactory(new TCompactProtocol.Factory());
            TServer server = new TNonblockingServer(tArgs);
            System.out.println("init...");
            server.serve();
        }catch(TTransportException e) {
        	e.printStackTrace();
        }
    }
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
	     Server server = new Server();
	     //server.startOIOServer();
	     server.startNIOServer();
	}

}
```

依赖jar包

![](https://img-blog.csdn.net/20170707093302363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

5、客户端代码

```java
package com.zychen.test;

import org.apache.thrift.TApplicationException;
import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TCompactProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import com.zychen.thrift.Test;
import com.zychen.thrift.User;

public class Client {

	 //阻塞io
	 public void startOIOClient() {
	        TTransport transport;
	        try {
	            transport = new TSocket("localhost", 1234);
	            TProtocol protocol = new TBinaryProtocol(transport);
	            Test.Client client = new Test.Client(protocol);
	            transport.open();
	            int nResult = client.add(100, 200);
	            String strTemp = String.format("client.add(100, 200) = %d", nResult);
	            System.out.println(strTemp);
	            User u1 = client.getById(1);
	            if(u1 != null){
	            	System.out.println(u1.getName());
	            }
	            User u2 = client.getById(11);
	            if(u2 == null){
	            	System.out.println("null");
	            }
	            u2 = client.getById(9);
	            transport.close();
	        } catch (TTransportException e) {
	            e.printStackTrace();
	        } catch (TException e) {
	        	if(e instanceof TApplicationException && ((TApplicationException) e).getType() == TApplicationException.MISSING_RESULT){
	        		System.out.println("Theresult of Method function is NULL");
	        	}else{
		            e.printStackTrace();	
	        	}
	        }
	    }

	 //非阻塞io
	 public void startNIOClient() {
	        TTransport transport;
	        try {
	            transport = new TFramedTransport(new TSocket("localhost", 1234,30000));
	            TProtocol protocol = new TCompactProtocol(transport);
	            Test.Client client = new Test.Client(protocol);
	            transport.open();
	            int nResult = client.add(100, 200);
	            String strTemp = String.format("client.add(100, 200) = %d", nResult);
	            System.out.println(strTemp);
	            User u1 = client.getById(1);
	            if(u1 != null){
	            	System.out.println(u1.getName());
	            }
	            User u2 = client.getById(11);
	            if(u2 == null){
	            	System.out.println("null");
	            }
	            u2 = client.getById(9);
	            transport.close();
	        } catch (TTransportException e) {
	            e.printStackTrace();
	        } catch (TException e) {
	        	if(e instanceof TApplicationException && ((TApplicationException) e).getType() == TApplicationException.MISSING_RESULT){
	        		System.out.println("Theresult of Method function is NULL");
	        	}else{
		            e.printStackTrace();	
	        	}
	        }
	    }

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
        Client client = new Client();
        //client.startOIOClient();
        client.startNIOClient();
	}

}
```

依赖jar包

![](https://img-blog.csdn.net/20170707093315108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

二、Thrift-0.9.3 jar包编译生成。

1、myeclipse新建一个java工程。

2、打开thrift目录，查找到thrift-0.9.3\thrift-0.9.3\lib\java\src，将这个src目录下的代码拷贝到工程目录下。

3、引入httpClient、httpcore、javax.servlet、slf4j.api jar包。如图：

![](https://img-blog.csdn.net/20170704191855433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4、编译，导出thrift-0.9.3jar包，导出时选择Exportall output folders for checked projects，否则scheme类无法导出到jar包中。
![](https://img-blog.csdn.net/20170704191953047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

