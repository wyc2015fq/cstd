# JAVA实现长连接(含心跳检测)Demo - 零度的博客专栏 - CSDN博客
2017年01月12日 15:25:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：35565标签：[java																[websocket																[心跳检测](https://so.csdn.net/so/search/s.do?q=心跳检测&t=blog)](https://so.csdn.net/so/search/s.do?q=websocket&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java																[WebSocket](https://blog.csdn.net/zmx729618/article/category/6357082)](https://blog.csdn.net/zmx729618/article/category/3108443)
实现原理：
       长连接的维持，是要客户端程序，定时向服务端程序，发送一个维持连接包的。
       如果，长时间未发送维持连接包，服务端程序将断开连接。
客户端：
       Client通过持有Socket的对象，可以随时（使用sendObject方法）发送Massage Object(消息)给服务端。
       如果keepAliveDelay毫秒（程序中是2秒）内未发送任何数据，则自动发送一个KeepAlive Object(心跳)给服务端，用于维持连接。
       由于，我们向服务端，可以发送很多不同的消息对象，服务端也可以返回不同的对象。所以，对于返回对象的处理，要编写具体的ObjectAction实现类进行处理。通过Client.addActionMap方法进行添加。这样，程序会回调处理。
服务端：
        由于客户端会定时（keepAliveDelay毫秒）发送维持连接的信息过来，所以，服务端要有一个检测机制。
        即当服务端receiveTimeDelay毫秒（程序中是3秒）内未接收任何数据，则自动断开与客户端的连接。
         ActionMapping的原理与客户端相似（相同）。
         通过添加相应的ObjectAction实现类，可以实现不同对象的响应、应答过程。
Demo:
```
package socket.keepalive.test;
import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.Date;
/**
 * 
 * 维持连接的消息对象（心跳对象）
 */
public class KeepAlive implements Serializable{
	private static final long serialVersionUID = -2813120366138988480L;
	/* 覆盖该方法，仅用于测试使用。
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())+"\t维持连接包";
	}
}
```
```
package socket.keepalive.test;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Collections;
import java.util.concurrent.ConcurrentHashMap;
/**
 *	C/S架构的客户端对象，持有该对象，可以随时向服务端发送消息。
 * <p>
 * 创建时间：2010-7-18 上午12:17:25
 * @author HouLei
 * @since 1.0
 */
public class Client {
	/**
	 * 处理服务端发回的对象，可实现该接口。
	 */
	public static interface ObjectAction{
		void doAction(Object obj,Client client);
	}
	
	public static final class DefaultObjectAction implements ObjectAction{
		public void doAction(Object obj,Client client) {
			System.out.println("处理：\t"+obj.toString());
		}
	}
	
	
	public static void main(String[] args) throws UnknownHostException, IOException {
		String serverIp = "127.0.0.1";
		int port = 65432;
		Client client = new Client(serverIp,port);
		client.start();
	}
	
	private String serverIp;
	private int port;
	private Socket socket;
	private boolean running=false; //连接状态
	
	private long lastSendTime; //最后一次发送数据的时间
	
	//用于保存接收消息对象类型及该类型消息处理的对象
	private ConcurrentHashMap<Class, ObjectAction> actionMapping = new ConcurrentHashMap<Class,ObjectAction>();
	
	public Client(String serverIp, int port) {
		this.serverIp=serverIp;
		this.port=port;
	}
	
	public void start() throws UnknownHostException, IOException {
		if(running)return;
		socket = new Socket(serverIp,port);
		System.out.println("本地端口："+socket.getLocalPort());
		lastSendTime=System.currentTimeMillis();
		running=true;
		new Thread(new KeepAliveWatchDog()).start();  //保持长连接的线程，每隔2秒项服务器发一个一个保持连接的心跳消息
		new Thread(new ReceiveWatchDog()).start();    //接受消息的线程，处理消息
	}
	
	public void stop(){
		if(running)running=false;
	}
	
	/**
	 * 添加接收对象的处理对象。
	 * @param cls 待处理的对象，其所属的类。
	 * @param action 处理过程对象。
	 */
	public void addActionMap(Class<Object> cls,ObjectAction action){
		actionMapping.put(cls, action);
	}
	public void sendObject(Object obj) throws IOException {
		ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
		oos.writeObject(obj);
		System.out.println("发送：\t"+obj);
		oos.flush();
	}
	
	class KeepAliveWatchDog implements Runnable{
		long checkDelay = 10;
		long keepAliveDelay = 2000;
		public void run() {
			while(running){
				if(System.currentTimeMillis()-lastSendTime>keepAliveDelay){
					try {
						Client.this.sendObject(new KeepAlive());
					} catch (IOException e) {
						e.printStackTrace();
						Client.this.stop();
					}
					lastSendTime = System.currentTimeMillis();
				}else{
					try {
						Thread.sleep(checkDelay);
					} catch (InterruptedException e) {
						e.printStackTrace();
						Client.this.stop();
					}
				}
			}
		}
	}
	
	class ReceiveWatchDog implements Runnable{
		public void run() {
			while(running){
				try {
					InputStream in = socket.getInputStream();
					if(in.available()>0){
						ObjectInputStream ois = new ObjectInputStream(in);
						Object obj = ois.readObject();
						System.out.println("接收：\t"+obj);
						ObjectAction oa = actionMapping.get(obj.getClass());
						oa = oa==null?new DefaultObjectAction():oa;
						oa.doAction(obj, Client.this);
					}else{
						Thread.sleep(10);
					}
				} catch (Exception e) {
					e.printStackTrace();
					Client.this.stop();
				} 
			}
		}
	}
	
}
```
```
package socket.keepalive.test;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ConcurrentHashMap;
/**
 * C/S架构的服务端对象。
 * <p>
 * 创建时间：2010-7-18 上午12:17:37
 * @author HouLei
 * @since 1.0
 */
public class Server {
	/**
	 * 要处理客户端发来的对象，并返回一个对象，可实现该接口。
	 */
	public interface ObjectAction{
		Object doAction(Object rev, Server server);
	}
	
	public static final class DefaultObjectAction implements ObjectAction{
		public Object doAction(Object rev,Server server) {
			System.out.println("处理并返回："+rev);
			return rev;
		}
	}
	
	public static void main(String[] args) {
		int port = 65432;
		Server server = new Server(port);
		server.start();
	}
	
	private int port;
	private volatile boolean running=false;
	private long receiveTimeDelay=3000;
	private ConcurrentHashMap<Class, ObjectAction> actionMapping = new ConcurrentHashMap<Class,ObjectAction>();
	private Thread connWatchDog;
	
	public Server(int port) {
		this.port = port;
	}
	public void start(){
		if(running)return;
		running=true;
		connWatchDog = new Thread(new ConnWatchDog());
		connWatchDog.start();
	}
	
	@SuppressWarnings("deprecation")
	public void stop(){
		if(running)running=false;
		if(connWatchDog!=null)connWatchDog.stop();
	}
	
	public void addActionMap(Class<Object> cls,ObjectAction action){
		actionMapping.put(cls, action);
	}
	
	class ConnWatchDog implements Runnable{
		public void run(){
			try {
				ServerSocket ss = new ServerSocket(port,5);
				while(running){
					Socket s = ss.accept();
					new Thread(new SocketAction(s)).start();
				}
			} catch (IOException e) {
				e.printStackTrace();
				Server.this.stop();
			}
			
		}
	}
	
	class SocketAction implements Runnable{
		Socket s;
		boolean run=true;
		long lastReceiveTime = System.currentTimeMillis();
		public SocketAction(Socket s) {
			this.s = s;
		}
		public void run() {
			while(running && run){
				if(System.currentTimeMillis()-lastReceiveTime>receiveTimeDelay){
					overThis();
				}else{
					try {
						InputStream in = s.getInputStream();
						if(in.available()>0){
							ObjectInputStream ois = new ObjectInputStream(in);
							Object obj = ois.readObject();
							lastReceiveTime = System.currentTimeMillis();
							System.out.println("接收：\t"+obj);
							ObjectAction oa = actionMapping.get(obj.getClass());
							oa = oa==null?new DefaultObjectAction():oa;
							Object out = oa.doAction(obj,Server.this);
							if(out!=null){
								ObjectOutputStream oos = new ObjectOutputStream(s.getOutputStream());
								oos.writeObject(out);
								oos.flush();
							}
						}else{
							Thread.sleep(10);
						}
					} catch (Exception e) {
						e.printStackTrace();
						overThis();
					} 
				}
			}
		}
		
		private void overThis() {
			if(run)run=false;
			if(s!=null){
				try {
					s.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			System.out.println("关闭："+s.getRemoteSocketAddress());
		}
		
	}
	
}
```

