# Java--网络编程（一） - weixin_33985507的博客 - CSDN博客
2017年06月12日 17:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
#### 一、网络基础
![3200716-e889e19da73dfb10.png](https://upload-images.jianshu.io/upload_images/3200716-e889e19da73dfb10.png)
从上图可以看出，两台主机通信必备的三个条件：
- IP地址
- 协议
- 端口号
![3200716-d7ede0e3fb66d821.png](https://upload-images.jianshu.io/upload_images/3200716-d7ede0e3fb66d821.png)
![3200716-a55a82e45fffa746.png](https://upload-images.jianshu.io/upload_images/3200716-a55a82e45fffa746.png)
![3200716-07816322428a9b55.png](https://upload-images.jianshu.io/upload_images/3200716-07816322428a9b55.png)
网络参考模型
![3200716-a951b5d3b53d6c3c.png](https://upload-images.jianshu.io/upload_images/3200716-a951b5d3b53d6c3c.png)
###### Java中的网络支持
- **InetAddress**：用于标识网络上的硬件资源。
- **URL**：统一资源定位符，通过URL可以直接读取或写入网络上的数据。
- **Sockets:**：使用TCP协议实现网络通信的Socket相关的类。
- **Datagram**：使用UDP协议，将数据保存在数据报中，通过网路进行通信。
#### 二、Java 中网络相关 API
###### 1、InetAddress类
InetAddress类用于标识网络上的硬件资源。表示互联网协议（IP）地址。
```
// 获取netAddress实例
InetAddress.getLocalHost();
InetAddress.getByName(String hoet);
InetAddress.getByAddress(byte[]addr);
InetAddress.getByAddress(String host,byte[]addr);
// 通过实例获取主机名
getHostName();
// 主机IP
getHostAddress();
// 获取字节数组形式的IP地址
getAddress();
```
###### 2、URL类
![3200716-4136109fa3116f7c.png](https://upload-images.jianshu.io/upload_images/3200716-4136109fa3116f7c.png)
```
// 创建URL实例
URL url = new URL("http://www.baidu.com");
// 获取协议
url.getProtocol();
// 获取主机
url.getHost();
// 获取端口
url.getPort();
// 获取文件路劲
url.getPath();
// 获取文件名
url.getFile();
// 获取相对路径（锚点，#号后面的内容）
url.getRef();
// 查询字符串（参数）
url.getQuery();
```
![3200716-baf80a579228f070.png](https://upload-images.jianshu.io/upload_images/3200716-baf80a579228f070.png)
读取百度首页：
```
public class SocketTest {
    public static void main(String[] args)  {
        // 创建URL实例
        URL url = null;
        try {
            url = new URL("http://www.baidu.com");
            InputStream is = url.openStream();
            InputStreamReader isr = new InputStreamReader(is, "utf-8");
            BufferedReader br = new BufferedReader(isr);
            String data = br.readLine();
            while (data != null) {
                System.out.println(data);
                data = br.readLine();
            }
            br.close();
            isr.close();
            is.close();
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
#### 三、TCP编程
![3200716-48b7f3bcf1f2d0f8.png](https://upload-images.jianshu.io/upload_images/3200716-48b7f3bcf1f2d0f8.png)
![3200716-58a5f0b4c963515b.png](https://upload-images.jianshu.io/upload_images/3200716-58a5f0b4c963515b.png)
![3200716-9e32b9fa01ce4795.png](https://upload-images.jianshu.io/upload_images/3200716-9e32b9fa01ce4795.png)
###### 1、开始实现
![3200716-e689a2e4f76f23bc.png](https://upload-images.jianshu.io/upload_images/3200716-e689a2e4f76f23bc.png)
![3200716-27b9f581b3b7593a.png](https://upload-images.jianshu.io/upload_images/3200716-27b9f581b3b7593a.png)
服务器端：
```
package tcp;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
public class Server {
    public static void main(String[] args) {
        try {
            //1.创建一个服务器端Socket，即ServerSocket，指定绑定的端口，并监听此端口
            ServerSocket serverSocket = new ServerSocket(8888);
            //2.调用accep()方法开始监听，等待客户端连接
            System.out.println("***服务器即将启动，等待客户端连接***");
            Socket socket = serverSocket.accept();
            //3.（服务器与客户端交互）获取输入流，并读取客户端信息
            InputStream is = socket.getInputStream();//字节输入流
            InputStreamReader isr = new InputStreamReader(is);//将字节流转换为字符流
            BufferedReader br = new BufferedReader(isr);//为输入流添加缓冲
            String info = null;
            while((info = br.readLine()) != null) {//循环读取客户端信息
                System.out.println("我是服务器，客户端说："+info);
            }
            socket.shutdownInput();//关闭输入流
            //4.获取输出流，响应客户端的请求
            OutputStream os = socket.getOutputStream();
            PrintWriter pw = new PrintWriter(os);//包装为打印流
            pw.write("欢迎您");
            pw.flush();//调用flush()方法将缓冲输出
            //5.关闭资源
            pw.close();
            os.close();
            br.close();
            isr.close();
            is.close();
            socket.close();
            serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
客户端：
```
package tcp;
import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;
public class Client {
    public static void main(String[] args) {
        try {
            //1.创建客户端Socket，指定服务器的端口
            Socket socket = new Socket("localhost", 8888);
            //2.获取输出流，向服务器端发送信息
            OutputStream os = socket.getOutputStream();//字节输出流
            PrintWriter pw = new PrintWriter(os);//将输出流包装为打印流
            pw.write("用户名：admin；密码：123");
            pw.flush();
            socket.shutdownOutput(); // 关闭输出流
            //3.获取输入流，并读取服务器端的响应信息
            InputStream is = socket.getInputStream();
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            String info = null;
            while ((info = br.readLine()) != null) {
                System.out.println("我是客户端，服务器说：" + info);
            }
            //4.关闭资源
            br.close();
            is.close();
            pw.close();
            socket.close();
            os.close();
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
在运行时，要先启动Server，后启动Client。
###### 2、多线程服务器
![3200716-941f8b9f012f97fa.png](https://upload-images.jianshu.io/upload_images/3200716-941f8b9f012f97fa.png)
修改服务器端：
```
package tcp;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
public class Server {
    public static void main(String[] args) {
        try {
            //1.创建一个服务器端Socket，即ServerSocket，指定绑定的端口，并监听此端口
            ServerSocket serverSocket = new ServerSocket(8888);
            Socket socket = null;
            //记录客户端的数量
            int count = 0;
            System.out.println("***服务器即将启动，等待客户端连接***");
            // 循环监听等待客户端的连接
            while (true) {
                //2.调用accep()方法开始监听，等待客户端连接
                socket = serverSocket.accept();
                //创建一个新的线程
                ServerThread serverThread = new ServerThread(socket);
                //启动线程
                serverThread.start();
                count ++;
                System.out.println("客户端的数量：" + count);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
```
package tcp;
import java.io.*;
import java.net.Socket;
/**
 * 服务器线程处理类
 */
public class ServerThread extends Thread {
    // 和本线程相关的Socket
    Socket socket = null;
    public ServerThread(Socket socket) {
        this.socket = socket;
    }
    /**
     * 线程执行操作，响应客户端请求
     */
    @Override
    public void run() {
        InputStream is = null;
        InputStreamReader isr = null;
        BufferedReader br = null;
        OutputStream os = null;
        PrintWriter pw = null;
        try {
            // 获取输入流，并读取客户端信息
            is = socket.getInputStream();
            isr = new InputStreamReader(is);//将字节流转换为字符流
            br = new BufferedReader(isr);//为输入流添加缓冲
            String info = null;
            while((info = br.readLine()) != null) {//循环读取客户端信息
                System.out.println("我是服务器，客户端说："+info);
            }
            socket.shutdownInput();//关闭输入流
            //4.获取输出流，响应客户端的请求
            os = socket.getOutputStream();
            pw = new PrintWriter(os);//包装为打印流
            pw.write("欢迎您");
            pw.flush();//调用flush()方法将缓冲输出
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                //5.关闭资源
                pw.close();
                os.close();
                br.close();
                isr.close();
                is.close();
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```
