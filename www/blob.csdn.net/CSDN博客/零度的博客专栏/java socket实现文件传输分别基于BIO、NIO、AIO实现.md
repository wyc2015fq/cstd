# java socket实现文件传输分别基于BIO、NIO、AIO实现 - 零度的博客专栏 - CSDN博客
2017年04月14日 15:03:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：536
       为了加强对socket及java IO流编程的回顾，本文作了一个文件传输的实例。针对java不同对IO流的不同实现，本文也分别基于BIO、NIO、AIO实现三种传输，同时本文采用了缓冲输入/输出流来包装输出流，再采用数据输入/输出输出流包装缓冲流，加快传输的速度。最后本文基于大文件对三种方式基本传输效率对比。对于每一种传输方式分别实现服务端和客户端，并通过服务端发送文件到客户端，（同理可以实现客户端到服务器端的文件发送）
### 1、基于BIO的文件发送实现
服务端：BIOServer.java
```
package zmx.socket.filethransfer.bio;
import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
/**
 * 本例实现基于BIO的Socket文件传输，客户端连接服务器后，服务器发送一个文件到客户端，
 * 客户端接受文件，保存到本地。(同理可实现客户端向服务器的文件发送)
 * 
 * 本类是服务端：实现接受连接和发送文件
 * @author zhangwenchao
 */
public class BIOServer {
	
	public static final int port=8821;
	
	public static final int bufferSize = 8192;
	
	public void serverStart(){
		
		Socket s =null;  //连接
		
		try {
			ServerSocket ss = new ServerSocket(port);//初始化ServerSocket
			File f = new File("E:\\1.jpg"); 
			System.out.println("要发送的文件为："+f.getName()+" 文件大小："+f.length());
			while(true){//不断循环接受客户端请求
				System.out.println("服务器正在监听端口："+port+"...");
	            // public Socket accept() throws IOException
				// 侦听并接受到此套接字的连接。此方法在进行连接之前一直阻塞。
				s = ss.accept();  //建立连接
				System.out.println("Socket连接已经建立成功！");
                				
                    //Socket输入流： 读取客户端发送过来的数据
	            DataInputStream dis = new DataInputStream(new BufferedInputStream(s.getInputStream()));
	            byte message = dis.readByte();
	            System.out.println(message);
				
				//文件输入流：读取文件
				DataInputStream fis = new DataInputStream(new BufferedInputStream(new FileInputStream(f)));
				
				//Socket输出流： 发送文件
				DataOutputStream  dos = new DataOutputStream(s.getOutputStream()); 
				
	            //将文件名及长度传给客户端。这里要真正适用所有平台，例如中文名的处理，还需要加工，具体可以参见Think In Java 4th里有现成的代码。
				dos.writeUTF(f.getName());
				dos.flush();
				
				dos.writeLong((long) f.length());
				dos.flush();
	              
	            byte[] buf = new byte[bufferSize];
				int len = 0;
				while((len=fis.read(buf))!=-1){
					dos.write(buf, 0, len);
				}
	            dos.flush();
	            // 注意关闭socket链接哦，不然客户端会等待server的数据过来，
	            // 直到socket超时，导致数据不完整。                
	            fis.close();
	            s.close();                
	            System.out.println("文件传输完成");
				
			}
			
		} catch (IOException e) {
			
			e.printStackTrace();
			System.out.println("网络连接出现异常！");
		} 				
		
	}
	
	
    public static void main(String arg[]) {
        new BIOServer().serverStart();
    }
	
	
}
```
客户端：BIOSocketUti.java+BIOClient.java
BIOSocketUtil.java
```
package zmx.socket.filethransfer.bio;
import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;
/**
 * Socket连接工具
 * @author zhangwenchao
 *
 */
public class BIOSocketUtil{
	
    /**
     * 创建socket连接 
     * @throws Exception
     *  exception
     */
    public static Socket CreateConnection(String ip, int port) throws Exception {
        try {
            Socket socket = new Socket(ip, port);
            return socket;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
        }
    }
    /**
     * 发送一条信息，通知服务器本机的操作系统类型
     * @param sendMessage 0x1:windows  0x2:unix 0x3:Linux
     * @throws Exception
     */
    public static void sendMessage(Socket socket,String sendMessage) throws Exception {
    	DataOutputStream outputStream = null;
        try {
            outputStream = new DataOutputStream(socket.getOutputStream());
            if (sendMessage.equals("Windows")) {
                outputStream.writeByte(0x1);
                outputStream.flush();
                return;
            }
            if (sendMessage.equals("Unix")) {
                outputStream.writeByte(0x2);
                outputStream.flush();
                return;
            }
            if (sendMessage.equals("Linux")) {
                outputStream.writeByte(0x3);
                outputStream.flush();
            } else {
                outputStream.writeUTF(sendMessage);
                outputStream.flush();
            }
        } catch (Exception e) {
        	System.out.println("发送消息错误" + "\n");
            e.printStackTrace();
            if (outputStream != null)
                outputStream.close();
            throw e;
        } finally {
        }
    }
    /**
     * 获取输入流
     * @return
     * @throws Exception
     */
    public static DataInputStream getMessageStream(Socket socket) throws Exception {
        try {
        	DataInputStream  inputStream = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
            return inputStream;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
        }
    }
    /**
     * 关闭连接
     */
    public static void shutDownConnection(Socket socket) {
        try {
            if (socket != null){
                socket.close();
            }
        } catch (Exception e) {
        }
    }
}
```
BIOClient.java
```
package zmx.socket.filethransfer.bio;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.net.Socket;
/**
 * 客户端：发送一条消息 通知服务器本地操作系统类型，然后接受服务器发送的文件
 * @author zhangwenchao
 *
 */
public class BIOClient {
	
	public static final int bufferSize = 8192;
    private static void getFileMessage(Socket socket) {
        if (socket == null)
            return;
        DataInputStream inputStream = null;
        try {
            inputStream = BIOSocketUtil.getMessageStream(socket); //根据socket获取输入流
        } catch (Exception e) {
            System.out.print("接收消息缓存错误\n");
            return;
        }
        try {
            //本地保存路径，文件名会自动从服务器端继承而来。
            String savePath = "E:\\local\\";
            byte[] buf = new byte[bufferSize];
            int passedlen = 0; //统计收到的数据
            //1、读取文件名
            savePath += inputStream.readUTF(); 
            //根据文件名新建一个文件输出流
            DataOutputStream fileOut = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(savePath)));
            
            //2、读取文件长度
            long fileLength = inputStream.readLong();
            System.out.println("文件的长度为:" + fileLength + "\n");
            System.out.println("开始接收文件..." + "\n");
            
            //3、读取文件内容
            int len = 0;
			while((len=inputStream.read(buf))!=-1){
                passedlen += len;
                //下面进度条本为图形界面的prograssBar做的，这里如果是打文件，可能会重复打印出一些相同的百分比
                System.out.println("文件接收了" +  (passedlen * 100/ fileLength) + "%\n");
                fileOut.write(buf, 0, len);
                
			}
            System.out.println("接收完成，文件存为" + savePath + "\n");
            fileOut.close();
        } catch (Exception e) {
            System.out.println("接收消息错误" + "\n");
            return;
        }
    }
    public static void main(String arg[]) {
        String ip = "localhost";// 设置成服务器IP
        int port = 8821;
        String sendMessage = "Windwos";
        try {
        	Socket socket = BIOSocketUtil.CreateConnection(ip, port);
        	System.out.print("连接服务器成功!" + "\n");
        	BIOSocketUtil.sendMessage(socket, sendMessage); //发送数据
        	getFileMessage(socket); //获取数据
           
       
        } catch (Exception e) {
            System.out.print("连接服务器失败!" + "\n");
            
        }
    }
}
```
运行测试结果：
要发送的文件为：1.jpg 文件大小：917070
服务器正在监听端口：8821...
Socket连接已经建立成功！
0
文件传输完成！
### 2、基于NIO的文件传输
1、NIOServer.java
```
package zmx.socket.filethransfer.nio;
import java.io.FileOutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Set;
 
public class NIOServer{
    private int port = 8000;
    
    
    /* 发送数据缓冲区 */
    private static ByteBuffer sendBuffer = ByteBuffer.allocate(1024);
    
    /* 接受数据缓冲区 */
    private static ByteBuffer revBuffer = ByteBuffer.allocate(1024);
    
    private static Selector selector;
    private static FileOutputStream fout;
    private static FileChannel ch;
    public NIOServer(){
        try{
            init();
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }
    private void init() throws Exception{
        ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.configureBlocking(false);
        ServerSocket serverSocket = serverSocketChannel.socket();
        serverSocket.bind(new InetSocketAddress(port));
        selector = Selector.open();
        serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
        System.out.println("server start on port:" + port);
        while (true) {
            try {
                selector.select();// 返回值为本次触发的事件数
                Set<SelectionKey> selectionKeys = selector.selectedKeys();                
                for (SelectionKey key : selectionKeys) {
                    ServerSocketChannel server = null;
                    SocketChannel client = null;
                    int count = 0;
                    if (key.isAcceptable()) {
                        server = (ServerSocketChannel) key.channel();
                        System.out.println("有客户端连接进入=============)");
                        client = server.accept();
                        client.configureBlocking(false);
                        client.register(selector, SelectionKey.OP_READ|SelectionKey.OP_WRITE);
                        fout = new FileOutputStream("E:\\local\\" + client.hashCode() + ".jpg");
                        ch = fout.getChannel();
                    } else if (key.isReadable()) {
                        client = (SocketChannel) key.channel();
                        revBuffer.clear();
                        count = client.read(revBuffer);
                        int k = 0;
                        // 循环读取缓存区的数据，
                        while(count > 0){
                            System.out.println("k=" + (k++) + " 读取到数据量:" + count);
                            revBuffer.flip();
                            ch.write(revBuffer);
                            fout.flush();
                            revBuffer.clear();
                            count = client.read(revBuffer);
                        }
                        if(count == -1){
                            client.close();
                            ch.close();
                            fout.close();
                        }                                            
                    }else if (key.isWritable()) {
                    	System.out.println("selectionKey.isWritable()");            
                    }
                }
                System.out.println("=======selectionKeys.clear()");
                selectionKeys.clear();
            } catch (Exception e) {
                e.printStackTrace();
                break;
            }
 
        }
    }
    public static void main(String[] args){
        new NIOServer();
    }
}
```
2、NIOClient.java
```
package zmx.socket.filethransfer.nio;
import java.io.FileInputStream;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Set;
 
public class NIOClient {
    private int port = 8000;
    
    /* 发送数据缓冲区 */
    private static ByteBuffer sendBuffer = ByteBuffer.allocate(1024);
    
    /* 接受数据缓冲区 */
    private static ByteBuffer revBuffer = ByteBuffer.allocate(1024);
    
    private InetSocketAddress SERVER;
    private static Selector selector;
    private static SocketChannel client;
     
    public NIOClient(){
        try{
            SERVER = new InetSocketAddress("localhost", port);
            init();
        }
        catch(Exception e){
            e.printStackTrace();
        }
         
    }
    private void init(){
        try {
            SocketChannel socketChannel = SocketChannel.open();
            socketChannel.configureBlocking(false);
            selector = Selector.open();
            socketChannel.register(selector, SelectionKey.OP_CONNECT);
            socketChannel.connect(SERVER);
            while (true) {
                selector.select();
                Set<SelectionKey> keySet = selector.selectedKeys();
                for(SelectionKey key : keySet) {
                    if(key.isConnectable()){
                        client = (SocketChannel)key.channel();
                        client.finishConnect();
                        client.register(selector, SelectionKey.OP_WRITE|SelectionKey.OP_READ);
 
                    }else if(key.isWritable()){                  	
                        sendFile(client);
                    }else if(key.isReadable()){
                    	System.out.println("selectionKey.isReadable()");
                    }
                }
                keySet.clear();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
     
    private void sendFile(SocketChannel client) {
        FileInputStream fis = null;
        FileChannel channel = null;
        try {
        	System.out.println("文件发送开始....");
            fis = new FileInputStream("E:\\1.jpg");
            channel = fis.getChannel();
            int i = 1;
            int count = 0;
            while((count = channel.read(sendBuffer)) != -1) {
                sendBuffer.flip(); 
                int send = client.write(sendBuffer);
                System.out.println("i===========" + (i++) + "   count:" + count + " send:" + send);
                // 服务器端可能因为缓存区满，而导致数据传输失败，需要重新发送
                while(send == 0){
                    Thread.sleep(10);
                    send = client.write(sendBuffer);
                    System.out.println("i重新传输====" + i + "   count:" + count + " send:" + send);
                }
                sendBuffer.clear(); 
             }
             System.out.println("文件发送完成！");
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                channel.close();
                fis.close();
                client.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
 
        }
    }
     
     
    public static void main(String[] args){
        new NIOClient();
    }
}
```
运行测试结果：
文件发送开始....
i===========1   count:1024 send:1024
i===========2   count:1024 send:1024
i===========3   count:1024 send:1024
i===========4   count:1024 send:1024
i===========5   count:1024 send:1024
...
文件发送完成！
