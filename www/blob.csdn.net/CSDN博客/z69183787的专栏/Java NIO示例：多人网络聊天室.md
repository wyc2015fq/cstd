# Java NIO示例：多人网络聊天室 - z69183787的专栏 - CSDN博客
2016年10月27日 14:52:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：800
个人分类：[IO-NIO](https://blog.csdn.net/z69183787/article/category/2191483)
一个多客户端聊天室，支持多客户端聊天，有如下功能：
- 功能1： 客户端通过Java NIO连接到服务端，支持多客户端的连接
- 功能2：客户端初次连接时，服务端提示输入昵称，如果昵称已经有人使用，提示重新输入，如果昵称唯一，则登录成功，之后发送消息都需要按照规定格式带着昵称发送消息
- 功能3：客户端登录后，发送已经设置好的欢迎信息和在线人数给客户端，并且通知其他客户端该客户端上线
- 功能4：服务器收到已登录客户端输入内容，转发至其他登录客户端。
- 功能5 TODO：客户端下线检测  方案是：客户端在线的时候发送心跳，服务端用TimeCacheMap自动删除过期对象，同时通知线上用户删掉的用户下线。
下面演示下效果，代码见附录！
## 启动服务器，监听某个端口
服务器console
```
Server is listening now...
```
## 启动一个客户端，连接服务器
服务器console
```
Server is listening now...
Server is listening from client :/127.0.0.1:50206
```
客户端console
```
Please input your name.
```
## 客户端输入一个昵称
服务端console
```
Server is listening now...
Server is listening from client :/127.0.0.1:50206
Server is listening from client /127.0.0.1:50206 data rev is: byr1#@#
```
客户端console
```
Please input your name.
byr1
welcome byr1 to chat room! Online numbers:1
```
## 再启动另外一个客户端，并输入昵称
服务端console
```
Server is listening now...
Server is listening from client :/127.0.0.1:50206
Server is listening from client /127.0.0.1:50206 data rev is: byr1#@#
Server is listening from client :/127.0.0.1:50261
Server is listening from client /127.0.0.1:50261 data rev is: byr2#@#
```
客户端byr1 console
```
Please input your name.
byr1
welcome byr1 to chat room! Online numbers:1
welcome byr2 to chat room! Online numbers:2
```
客户端byr2 console
```
Please input your name.
byr2
welcome byr2 to chat room! Online numbers:2
```
## 客户端byr1发送一个消息，客户byr2回一条消息
服务端console
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
Server is listening now...
Server is listening from client :/127.0.0.1:50206
Server is listening from client /127.0.0.1:50206 data rev is: byr1#@#
Server is listening from client :/127.0.0.1:50261
Server is listening from client /127.0.0.1:50261 data rev is: byr2#@#
Server is listening from client /127.0.0.1:50206 data rev is: byr1#@#hello byr2, a nice day, isn't it?
Server is listening from client /127.0.0.1:50261 data rev is: byr2#@#fine
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
客户端byr1 console
```
Please input your name.
byr1
welcome byr1 to chat room! Online numbers:1
welcome byr2 to chat room! Online numbers:2
hello byr2, a nice day, isn't it?
byr2 say fine
```
客户端byr2 console
```
Please input your name.
byr2
welcome byr2 to chat room! Online numbers:2
byr1 say hello byr2, a nice day, isn't it?
fine
```
## 附录：server和client代码
server代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.huahuiyang.channel;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.Channel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
/**
 * 网络多客户端聊天室
 * 功能1： 客户端通过Java NIO连接到服务端，支持多客户端的连接
 * 功能2：客户端初次连接时，服务端提示输入昵称，如果昵称已经有人使用，提示重新输入，如果昵称唯一，则登录成功，之后发送消息都需要按照规定格式带着昵称发送消息
 * 功能3：客户端登录后，发送已经设置好的欢迎信息和在线人数给客户端，并且通知其他客户端该客户端上线
 * 功能4：服务器收到已登录客户端输入内容，转发至其他登录客户端。
 * 
 * TODO 客户端下线检测
 */
public class ChatRoomServer {
    private Selector selector = null;
    static final int port = 9999;
    private Charset charset = Charset.forName("UTF-8");
    //用来记录在线人数，以及昵称
    private static HashSet<String> users = new HashSet<String>();
    
    private static String USER_EXIST = "system message: user exist, please change a name";
    //相当于自定义协议格式，与客户端协商好
    private static String USER_CONTENT_SPILIT = "#@#";
    
    private static boolean flag = false;
    
    public void init() throws IOException
    {
        selector = Selector.open();
        ServerSocketChannel server = ServerSocketChannel.open();
        server.bind(new InetSocketAddress(port));
        //非阻塞的方式
        server.configureBlocking(false);
        //注册到选择器上，设置为监听状态
        server.register(selector, SelectionKey.OP_ACCEPT);
        
        System.out.println("Server is listening now...");
        
        while(true) {
            int readyChannels = selector.select();
            if(readyChannels == 0) continue; 
            Set selectedKeys = selector.selectedKeys();  //可以通过这个方法，知道可用通道的集合
            Iterator keyIterator = selectedKeys.iterator();
            while(keyIterator.hasNext()) {
                 SelectionKey sk = (SelectionKey) keyIterator.next();
                 keyIterator.remove();
                 dealWithSelectionKey(server,sk);
            }
        }
    }
    
    public void dealWithSelectionKey(ServerSocketChannel server,SelectionKey sk) throws IOException {
        if(sk.isAcceptable())
        {
            SocketChannel sc = server.accept();
            //非阻塞模式
            sc.configureBlocking(false);
            //注册选择器，并设置为读取模式，收到一个连接请求，然后起一个SocketChannel，并注册到selector上，之后这个连接的数据，就由这个SocketChannel处理
            sc.register(selector, SelectionKey.OP_READ);
            
            //将此对应的channel设置为准备接受其他客户端请求
            sk.interestOps(SelectionKey.OP_ACCEPT);
            System.out.println("Server is listening from client :" + sc.getRemoteAddress());
            sc.write(charset.encode("Please input your name."));
        }
        //处理来自客户端的数据读取请求
        if(sk.isReadable())
        {
            //返回该SelectionKey对应的 Channel，其中有数据需要读取
            SocketChannel sc = (SocketChannel)sk.channel(); 
            ByteBuffer buff = ByteBuffer.allocate(1024);
            StringBuilder content = new StringBuilder();
            try
            {
                while(sc.read(buff) > 0)
                {
                    buff.flip();
                    content.append(charset.decode(buff));
                    
                }
                System.out.println("Server is listening from client " + sc.getRemoteAddress() + " data rev is: " + content);
                //将此对应的channel设置为准备下一次接受数据
                sk.interestOps(SelectionKey.OP_READ);
            }
            catch (IOException io)
            {
                sk.cancel();
                if(sk.channel() != null)
                {
                    sk.channel().close();
                }
            }
            if(content.length() > 0)
            {
                String[] arrayContent = content.toString().split(USER_CONTENT_SPILIT);
                //注册用户
                if(arrayContent != null && arrayContent.length ==1) {
                    String name = arrayContent[0];
                    if(users.contains(name)) {
                        sc.write(charset.encode(USER_EXIST));
                        
                    } else {
                        users.add(name);
                        int num = OnlineNum(selector);
                        String message = "welcome "+name+" to chat room! Online numbers:"+num;
                        BroadCast(selector, null, message);
                    }
                } 
                //注册完了，发送消息
                else if(arrayContent != null && arrayContent.length >1){
                    String name = arrayContent[0];
                    String message = content.substring(name.length()+USER_CONTENT_SPILIT.length());
                    message = name + " say " + message;
                    if(users.contains(name)) {
                        //不回发给发送此内容的客户端
                        BroadCast(selector, sc, message);
                    }
                }
            }
            
        }
    }
    
    //TODO 要是能检测下线，就不用这么统计了
    public static int OnlineNum(Selector selector) {
        int res = 0;
        for(SelectionKey key : selector.keys())
        {
            Channel targetchannel = key.channel();
            
            if(targetchannel instanceof SocketChannel)
            {
                res++;
            }
        }
        return res;
    }
    
    public void BroadCast(Selector selector, SocketChannel except, String content) throws IOException {
        //广播数据到所有的SocketChannel中
        for(SelectionKey key : selector.keys())
        {
            Channel targetchannel = key.channel();
            //如果except不为空，不回发给发送此内容的客户端
            if(targetchannel instanceof SocketChannel && targetchannel!=except)
            {
                SocketChannel dest = (SocketChannel)targetchannel;
                dest.write(charset.encode(content));
            }
        }
    }
    
    public static void main(String[] args) throws IOException 
    {
        new ChatRoomServer().init();
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
client代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.huahuiyang.channel;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.util.Iterator;
import java.util.Scanner;
import java.util.Set;
public class ChatRoomClient {
    private Selector selector = null;
    static final int port = 9999;
    private Charset charset = Charset.forName("UTF-8");
    private SocketChannel sc = null;
    private String name = "";
    private static String USER_EXIST = "system message: user exist, please change a name";
    private static String USER_CONTENT_SPILIT = "#@#";
    
    public void init() throws IOException
    {
        selector = Selector.open();
        //连接远程主机的IP和端口
        sc = SocketChannel.open(new InetSocketAddress("127.0.0.1",port));
        sc.configureBlocking(false);
        sc.register(selector, SelectionKey.OP_READ);
        //开辟一个新线程来读取从服务器端的数据
        new Thread(new ClientThread()).start();
        //在主线程中 从键盘读取数据输入到服务器端
        Scanner scan = new Scanner(System.in);
        while(scan.hasNextLine())
        {
            String line = scan.nextLine();
            if("".equals(line)) continue; //不允许发空消息
            if("".equals(name)) {
                name = line;
                line = name+USER_CONTENT_SPILIT;
            } else {
                line = name+USER_CONTENT_SPILIT+line;
            }
            sc.write(charset.encode(line));//sc既能写也能读，这边是写
        }
        
    }
    private class ClientThread implements Runnable
    {
        public void run()
        {
            try
            {
                while(true) {
                    int readyChannels = selector.select();
                    if(readyChannels == 0) continue; 
                    Set selectedKeys = selector.selectedKeys();  //可以通过这个方法，知道可用通道的集合
                    Iterator keyIterator = selectedKeys.iterator();
                    while(keyIterator.hasNext()) {
                         SelectionKey sk = (SelectionKey) keyIterator.next();
                         keyIterator.remove();
                         dealWithSelectionKey(sk);
                    }
                }
            }
            catch (IOException io)
            {}
        }
        private void dealWithSelectionKey(SelectionKey sk) throws IOException {
            if(sk.isReadable())
            {
                //使用 NIO 读取 Channel中的数据，这个和全局变量sc是一样的，因为只注册了一个SocketChannel
                //sc既能写也能读，这边是读
                SocketChannel sc = (SocketChannel)sk.channel();
                
                ByteBuffer buff = ByteBuffer.allocate(1024);
                String content = "";
                while(sc.read(buff) > 0)
                {
                    buff.flip();
                    content += charset.decode(buff);
                }
                //若系统发送通知名字已经存在，则需要换个昵称
                if(USER_EXIST.equals(content)) {
                    name = "";
                }
                System.out.println(content);
                sk.interestOps(SelectionKey.OP_READ);
            }
        }
    }
    
    
    
    public static void main(String[] args) throws IOException
    {
        new ChatRoomClient().init();
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
E-mail: huahuiyang@gmail.com https://cn.linkedin.com/pub/huahui-yang/91/13a/105
