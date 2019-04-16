# JAVA编写的使用Socket模拟Http的GET操作 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月24日 15:53:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35
个人分类：[网络																[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)](https://blog.csdn.net/leixiaohua1020/article/category/8433201)









JAVA编写的使用Socket模拟Http的GET操作，看后就很容易理解Web浏览器的原理了。



```java
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;

public class TestSocket {
  String host = "www.javathinker.org";
  int port = 80;
  Socket socket;

  public void createSocket() throws Exception {
    socket = new Socket("www.163.com", 80);
  }

  public void communicate() throws Exception {
    StringBuffer sb = new StringBuffer("GET / HTTP/1.1/r/n");
    sb.append("Host: <A href="www.163.com" mce_href="www.163.com" target=_blank>www.163.com</A>/r/n");
    sb.append("Connection: Keep-Alive/r/n");
    sb.append("Accept: */*/r/n/r/n");

    // 发出HTTP请求
    OutputStream socketOut = socket.getOutputStream();
    socketOut.write(sb.toString().getBytes());
    socket.shutdownOutput(); // 关闭输出流

    // 接收响应结果
    System.out.println(socket);

    InputStream socketIn = socket.getInputStream();
    BufferedReader br = new BufferedReader(new InputStreamReader(socketIn));
    String data;
    while ((data = br.readLine()) != null) {
      System.out.println(data);
    }
    socket.close();
  }

  public static void main(String args[]) throws Exception {
    TestSocket client = new TestSocket();
    client.createSocket();
    client.communicate();
  }

}
```











