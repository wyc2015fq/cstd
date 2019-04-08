# Java网络编程相关基础





# 一、理论基础

### 1、网络参考模型

OSI 参考模型：应用层、表示层、会话层、传输层、网络层、数据链路层、物理层
TCP/IP 参考模型：应用层、传输层、网络层、物理层

### 2、网络编程三要素

IP 地址、 端口、协议（规则）

IP地址：网络中、计算机唯一标识；
IP地址组成：网络号段+主机号段 第一部分为网络号段、第二部分为主机号段；
IP地址分类：5 类 ；

IP地址与主机名称相互获取 :

```
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Objects;

//根据计算机名称获取计算机ip地址
//根据计算机ip地址获取计算机名称
public class Main {

    public static void main(String[] args) {
        InetAddress inetAddress = null;
        try {
            //传名称----》ip地址
            //传ip地址----》名称
            inetAddress = InetAddress.getByName("DESKTOP-UQ42018");
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }

        //ip地址
        String ipAddress = Objects.requireNonNull(inetAddress).getHostAddress();
        //主机名称
        String ipName = inetAddress.getHostName();
        System.out.println(ipName + "----------" + ipAddress);

    }
}
```

### 3、协议

UDP 协议：将数据打包、数据有限制为64k、不建立链接、速度快、不可靠

TCP 协议：建立连接、数据无限制、速度慢、可靠

### 4、Scoket 套接字编程 网络编程

1）、原理机制
通信两端都有 Scoket ；
网络通信其实就是 Scoket 间通信；
数据在两个 Scoket 键通过 IO 传输；

##### 简易版 socket udp 通信：

发送端代码：

```
import java.io.IOException;
import java.net.*;
import java.util.Objects;

public class SendMessage {

    public static void main(String[] args) {

        //发送数据
        sendMessage();
    }

    //socket  udp  通信发送端
    private static void sendMessage() {
        //1、创建发送端 Socket 对象
        DatagramSocket ds = null;
        try {
            ds = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }

        //创建数据
        byte[] bytes = "hello udp".getBytes();
        //长度
        int length = bytes.length;
        //ip 地址对象
        InetAddress address = null;
        try {
            address = InetAddress.getByName("DESKTOP-UQ42018");
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        //端口号
        int port = 100;
        //2、创建数据，并且进行打包
        DatagramPacket packet = new DatagramPacket(bytes, length, address, port);
        //3、发送数据
        try {
            Objects.requireNonNull(ds).send(packet);
        } catch (IOException e) {
            e.printStackTrace();
        }
        //4、释放资源
        Objects.requireNonNull(ds).close();
    }
}
```

接收端代码：

```
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.Objects;

public class ReceiveMessage {

    public static void main(String[] args) {
        //接收数据
        receiveMessage();
    }

    //接收 udp 接收端
    private static void receiveMessage() {

        //1、创建接收端 socket 对象
        DatagramSocket ds = null;
        try {
            ds = new DatagramSocket(100);
        } catch (SocketException e) {
            e.printStackTrace();
        }
        //2、创建数据包接收器
        byte[] bytes = new byte[1024];
        int length = bytes.length;

        DatagramPacket packet = new DatagramPacket(bytes, length);

        //3、接收数据包
        try {
            Objects.requireNonNull(ds).receive(packet);// 阻塞方法
        } catch (IOException e) {
            e.printStackTrace();
        }

        //获取发送端的ip
        InetAddress address = packet.getAddress();
        String ip = address.getHostAddress();


        //4、解析数据
        // public byte[] getData();  获取数据缓冲区
        //public int getLength(); 获取数据实际长度
        byte[] bytes2 = packet.getData();
        int len = packet.getLength();
        String str = new String(bytes2, 0, len);

        System.out.println(ip + " : " + str);

        //5、释放资源
        Objects.requireNonNull(ds).close();
    }
}
```

##### 实现循环聊天系统：

发送端：

```
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.*;
import java.util.Objects;

public class SendMessage {

    public static void main(String[] args) {
        try {
            sendMessage();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //循环发送 聊天数据
    private static void sendMessage() throws IOException {
        //1、创建发送端 Socket 对象
        DatagramSocket ds = null;
        try {
            ds = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String line;
        while ((line = br.readLine()) != null) {
            //输入再见，跳出循环
            if ("再见".endsWith(line)) {
                break;
            }
            
            byte[] bytes = line.getBytes();
            int length = bytes.length;
            InetAddress address = InetAddress.getByName("DESKTOP-UQ42018");
            int port = 100;
            //2、创建数据，并且进行打包
            DatagramPacket packet = new DatagramPacket(bytes, length, address, port);
            //3、发送数据
            Objects.requireNonNull(ds).send(packet);
        }
        //4、释放资源
        Objects.requireNonNull(ds).close();
    }

}
```

接收端

```
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Objects;

public class ReceiveMessage {

    public static void main(String[] args) {
        try {
            receiveMessage();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //循环接收  聊天数据
    private static void receiveMessage() throws IOException {
        //1、创建接收端 socket 对象
        DatagramSocket ds = new DatagramSocket(100);
        while (true) {
            //2、创建数据包接收器
            byte[] bytes = new byte[1024];
            DatagramPacket packet = new DatagramPacket(bytes, bytes.length);
            //3、接收数据包
            Objects.requireNonNull(ds).receive(packet);// 阻塞方法
            //4、解析数据
            String str = new String(packet.getData(), 0, packet.getLength());
            System.out.println(packet.getAddress().getHostAddress() + " : " + str);
        }
        //5、释放资源，循环聊天系统，接收方是不能关闭的。
        //        Objects.requireNonNull(ds).close();
    }
}
```

##### 实现单窗口聊天室

启动程序

```
import java.net.DatagramSocket;
import java.net.SocketException;

//聊天室启动程序

public class CharRoom {
    public static void main(String[] args) {
        DatagramSocket socketSend = null;
        DatagramSocket socketReceive = null;
        try {
            socketSend = new DatagramSocket();
            socketReceive = new DatagramSocket(100);
        } catch (SocketException e) {
            e.printStackTrace();
        }
        SendRoom sendRoom = new SendRoom(socketSend);
        ReceiveRoom receiveRoom = new ReceiveRoom(socketReceive);

        Thread send = new Thread(sendRoom);
        Thread receive = new Thread(receiveRoom);

        send.start();
        receive.start();
    }
}
```

信息发送端程序

```
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Objects;

//信息发送端
public class SendRoom implements Runnable {

    private DatagramSocket socket;
    SendRoom(DatagramSocket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            String line;
            while ((line = br.readLine()) != null) {
                //输入再见，跳出循环
                if ("再见".endsWith(line)) {
                    break;
                }
                byte[] bytes = line.getBytes();
                int length = bytes.length;
                InetAddress address = InetAddress.getByName("DESKTOP-UQ42018");
                int port = 100;
                //2、创建数据，并且进行打包
                DatagramPacket packet = new DatagramPacket(bytes, length, address, port);
                //3、发送数据
                Objects.requireNonNull(socket).send(packet);
            }
            //4、释放资源
            Objects.requireNonNull(socket).close();
        } catch (IOException e) {
            System.out.println("发送端出现异常");
        } finally {
            //4、释放资源
            Objects.requireNonNull(socket).close();
        }
    }
}
```

信息接收端程序

```
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Objects;

//信息接收端程序

public class ReceiveRoom implements Runnable {
    
    private DatagramSocket socket;
    ReceiveRoom(DatagramSocket socket) {
        this.socket = socket;
    }
    @Override
    public void run() {
        try {
            while (true) {
                //2、创建数据包接收器
                byte[] bytes = new byte[1024];
                DatagramPacket packet = new DatagramPacket(bytes, bytes.length);
                //3、接收数据包
                Objects.requireNonNull(socket).receive(packet);// 阻塞方法
                //4、解析数据
                String str = new String(packet.getData(), 0, packet.getLength());
                System.out.println(packet.getAddress().getHostAddress() + " : " + str);
            }
        } catch (IOException e) {
            System.out.println("接收端出现异常");
        } finally {
            socket.close();
        }
    }
}
```

### 5、TCP 相关

简单的通信例子,但是必须先运行服务端，再运行客户端，否则会产生错误。

##### 简单版 cs 交互代码

服务端代码

```
import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {

    public static void main(String[] args) throws IOException {

        //1、创建接收端 Socket 对象
        ServerSocket serverSocket = new ServerSocket(1000);

        //2、监听客户端连接，返回 Socket 对象
        Socket socket = serverSocket.accept();//阻塞式

        //3、获取输入流，
        InputStream is = socket.getInputStream();

        byte[] bytes = new byte[1024];
        int len = is.read(bytes);
        String str = new String(bytes, 0, len);
        System.out.println("IP地址：" + socket.getInetAddress().getHostAddress() + " : " + str);

        //4、释放资源
        socket.close();
        //  serverSocket.close();关闭服务器
    }
}
```

客户端代码

```
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

//TCP 的客户端

public class TcpClient {

    public static void main(String[] args) throws IOException {
        //1、创建发送端 Scoket 对象   指定  IP地址和端口号
        Socket socket = new Socket("192.168.1.5", 1000);

        //2、创建输出流对象
        OutputStream os = socket.getOutputStream();
        os.write("你好呀，我是tcp客户端".getBytes());

        //3、关闭资源
        socket.close();
    }

}
```

##### 可录入式 cs 结构

客户端代码：

```
import java.io.*;
import java.net.Socket;

//TCP 的客户端

public class TcpClient {

    public static void main(String[] args) throws IOException {
        //1、创建发送端 Scoket 对象   指定  IP地址和端口号
        Socket socket = new Socket("192.168.0.127", 1000);

        //输入数据
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        //2、创建输出流对象----通道内流------进行封装
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

        String line;
        while ((line = br.readLine()) != null) {

            //聊天终止条件
            if ("再见".equals(line)) {
                break;
            }

            bw.write(line);
            bw.newLine();
            bw.flush();
        }

        //3、关闭资源
        //        bw.close();  由于bw依附于 socket 对象存在，所有socket 关闭，就不需要关闭 bw。
        //        br.close();  由于跳出循环，不在进行键盘录入，所有 不需要进行关闭
        socket.close();
    }
}
```

服务端代码：

```
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {

    public static void main(String[] args) throws IOException {

        //1、创建接收端 Socket 对象
        ServerSocket serverSocket = new ServerSocket(1000);

        //2、监听客户端连接，返回 Socket 对象-----服务器
        Socket socket = serverSocket.accept();//阻塞式
        //3、接收读取数据-----通道内的流-----进行包装
        BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));

        String line;
        while ((line = br.readLine()) != null) {
            System.out.println(line);
        }

        //        br.close();
        //        serverSocket.close();
        socket.close();
    }
}
```

##### 将输入数据写入文件保存

客户端代码：

```
import java.io.*;
import java.net.Socket;

//TCP 的客户端

public class TcpClient {

    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("192.168.0.127", 123);

        //封装录入数据
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        //通道内的数据
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

        String line;

        while ((line = br.readLine()) != null) {
            if ("再见".equals(line)) {
                break;
            }
            bw.write(line);
            bw.newLine();
            bw.flush();
        }
        socket.close();
    }
}
```

服务端代码：

```
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {

    public static void main(String[] args) throws IOException {
        //创建服务器端 socket 对象
        ServerSocket serverSocket = new ServerSocket(123);
        Socket socket = serverSocket.accept();
        //读取通道内数据
        BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        BufferedWriter bw = new BufferedWriter(new FileWriter("test.txt"));
        String line;
        while ((line = br.readLine()) != null) {
            bw.write(line);
            bw.newLine();
            bw.flush();
        }
        br.close();
        socket.close();
    }
}
```

##### 上传文本文件

客户端代码

```
import java.io.*;
import java.net.Socket;

//TCP 的客户端

public class TcpClient {

    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("192.168.0.127", 123);
        //封装录入数据
        BufferedReader br = new BufferedReader(new FileReader("C:\\Users\\Hxd\\Desktop\\AddDeviceActivity.java"));
        //通道内的数据
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        String line;
        while ((line = br.readLine()) != null) {
            bw.write(line);
            bw.newLine();
            bw.flush();
        }
        br.close();
        socket.close();
    }
}
```

服务端代码

```
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {

    public static void main(String[] args) throws IOException {
        //创建服务器端 socket 对象
        ServerSocket serverSocket = new ServerSocket(123);
        Socket socket = serverSocket.accept();
        //读取通道内数据
        BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        BufferedWriter bw = new BufferedWriter(new FileWriter("test.txt"));
        String line;
        while ((line = br.readLine()) != null) {
            bw.write(line);
            bw.newLine();
            bw.flush();
        }
        br.close();
        socket.close();
    }
}
```

##### 多个客户端向服务端上传文件

客户端代码

```
import java.io.*;
import java.net.Socket;

//TCP 的客户端
public class TcpClient {

    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("192.168.43.133", 123);
        //封装录入数据
        BufferedInputStream bis = new BufferedInputStream(new FileInputStream("C:\\Users\\Hxd\\Desktop\\1.jpg"));
        //将图片数据封装到通道
        BufferedOutputStream bos = new BufferedOutputStream(socket.getOutputStream());
        byte[] bytes = new byte[1024];
        int len;
        while ((len = bis.read(bytes)) != -1) {
            bos.write(bytes, 0, len);
            bos.flush();
        }
        //通知服务器，不在上传数据
        socket.shutdownOutput();
        //读取服务器中的数据反馈
        InputStream is = socket.getInputStream();
        byte[] bytes1 = new byte[1024];
        int len1 = is.read(bytes1);
        System.out.println(new String(bytes1, 0, len1));
        bis.close();
        socket.close();
    }
}
```

服务端代码

```
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

//服务器接收端
public class TCPServer {

    public static void main(String[] args) throws IOException {
        //创建服务器端 socket 对象
        ServerSocket serverSocket = new ServerSocket(123);
        while (true) {
            Socket socket = serverSocket.accept();
            new Thread(new CsThread(socket)).start();
        }
    }
}
```

服务器端开启的线程

```
import java.io.*;
import java.net.Socket;

public class CsThread implements Runnable {

    private Socket socket;
    CsThread(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            //读取通道内数据
            BufferedInputStream bis = new BufferedInputStream(socket.getInputStream());
            String name = System.currentTimeMillis() + ".jpg";
            //将读到的数据还原为图片
            BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(name));
            byte[] bytes = new byte[1024];
            int len;
            while ((len = bis.read(bytes)) != -1) {
                bos.write(bytes, 0, len);
                bos.flush();
            }
            //提供一个接收完成的反馈
            OutputStream os = socket.getOutputStream();
            os.write("图片上传成功".getBytes());
            bis.close();
            socket.close();
        } catch (IOException e) {
            System.out.println(e.toString());
        }
    }
}
```



