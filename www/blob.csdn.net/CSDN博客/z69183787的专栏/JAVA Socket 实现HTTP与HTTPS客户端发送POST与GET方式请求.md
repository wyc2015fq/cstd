# JAVA Socket 实现HTTP与HTTPS客户端发送POST与GET方式请求 - z69183787的专栏 - CSDN博客
2013年12月26日 12:26:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4523
个人分类：[Http请求-HttpURLConnection																[Http请求-原理](https://blog.csdn.net/z69183787/article/category/2175793)](https://blog.csdn.net/z69183787/article/category/2175775)
**JAVA Socket 实现HTTP与HTTPS客户端发送POST与GET方式请求**
哇,一看标题怎么这么长啊,其实意思很简单,哥讨厌用HTTP Client做POST与GET提交
觉得那个毕竟是别人写得API库,所以我就自己实现了一个简单的HTTP客户端,支持POST
方式提交数据,GET方式查询数据,是测试Restful API比较方便点,然后支持form与JSON两
种方式提交数据,当然也支持返回数据为JSON格式.当然这些东西都是基于JAVA Socket直
接完成的,不借助任何第三方的库,主要是JDK的API其实已经够用啦. 当然我也没有用
URLConnect这个东西,毕竟它在Socket基础上又包装了一下,有违我写这篇文章的目的.
好啦,讲完理由,下面就说说要怎么样才能实现啊,光说不练假把式啊!大致分了几个步骤
一:当然是要知道HTTP协议,知道常用的HTTP请求头,比如Host, Accept, Content-Type
    知道HTTP协议支持的方法,常用有GET/POST/PUT/DELETE等
    如果不知道,也不用担心,我保证你读完这篇文章,你就知道一些啦,当然有个最好的参考
    文档就是HTTP相关的RFC文档,认真读一下肯定解决你自己心中的HTTP那些疑惑
二: 知道发送HTTP GET与POST格式很重要, 固定的格式如下:
    [REQUEST]<SP><URL><SP>[HTTP VERSION]<CLRF>
    [REQUEST HEADER: ]<SP>[VALUE]<CLRF>
    可以有多个请求头
    最后<CLRF>
    发送完HTTP请求头部以后, 针对不同请求如POST要发送内容部分,发送完成以后同样
    以<CLRF>结尾.
    解释: <SP>表示空格, <CLRF>表示回车换行JAVA中表示为”\r\n”
    REQUEST表示HTTP请求命令,可以为POST, GET, PUT, DELETE等之一
    HTTP VERSION的常见可能值为HTTP/1.1或者HTTP/1.0
三: 如果1与2的知识你都具备了,下面就来介绍一下JAVA Socket的相关知识
    如何创建一个JAVA客户端套接字Socket s = new Socket()如此即可,简单吧!
    如何连接到远程的主机与端口, 当提供URL字符串时候,可以这么做
   URL url = new URL(“http://blog.csdn.net/jia20003”);
   String host = url.getHost;
   int port = url.getDefaultPort();
SocketAddress dest = new InetSocketAddress(this.host, this.port);
   s.connect(dest);
   即可连接到远程主机,下面就可以请求文章内容了, 也很容易的
**[java]**[view
 plain](http://blog.csdn.net/jia20003/article/details/17104791#)[copy](http://blog.csdn.net/jia20003/article/details/17104791#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/95455/fork)
- String path = "/jia20003/article/details/16908661";  
- SocketAddress dest = new InetSocketAddress(this.host, this.port);  
- socket.connect(dest);  
- OutputStreamWriter streamWriter = new OutputStreamWriter(socket.getOutputStream());  
- bufferedWriter = new BufferedWriter(streamWriter);  
- bufferedWriter.write("GET " + path + " HTTP/1.1\r\n");  
- bufferedWriter.write("Host: " + this.host + "\r\n");  
- bufferedWriter.write("\r\n");  
- bufferedWriter.flush();  
最后也是最重要的一点,字符编码,尽量都用同一种字符编码来发
送请求数据,推荐使用utf-8测试程序, 我写两个简单的PHP文件放
在wamp server上,完成对全部代码的测试
**四:关于HTTP与HTTPS**
HTTP协议是位于第四层协议TCP之上完成的应用层协议, 端到端都是明文传送,别人一
旦网络抓包以后都可以看到你的提交与请求数据,这个好像不太安全. HTTP协议的默认
端口是80这个是RFC文档声明的,属于官方标准,没什么道理可以讲.
HTTPS是基于SSL加密传输的,这样别人截获你的数据包破解的概率要小一点,比HTTP安
全一点,其默认端口是443, 好像QQ邮箱与谷歌的WEB Mail邮箱都是基于HTTPS. 但是
HTTPS通信方式只是传输数据加密,都客户端来说是透明的,它还是一样要遵守HTTP协
议规范来发送POST与GET请求等.
完整的测试程序如下:
**[java]**[view
 plain](http://blog.csdn.net/jia20003/article/details/17104791#)[copy](http://blog.csdn.net/jia20003/article/details/17104791#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/95455/fork)
- package com.gloomyfish.http.client;  
- 
- import java.io.BufferedInputStream;  
- import java.io.BufferedReader;  
- import java.io.BufferedWriter;  
- import java.io.IOException;  
- import java.io.InputStreamReader;  
- import java.io.OutputStreamWriter;  
- import java.net.InetSocketAddress;  
- import java.net.Socket;  
- import java.net.SocketAddress;  
- import java.net.URLEncoder;  
- 
- publicclass TestDemo {  
- privateint port;  
- private String host;  
- private Socket socket;  
- private BufferedReader bufferedReader;  
- private BufferedWriter bufferedWriter;  
- public TestDemo(String host, int port) {  
-         socket = new Socket();  
- this.host = host;  
- this.port = port;  
-     }  
- 
- publicvoid sendGet() throws IOException  
-     {  
-         String path = "/zhigang/getDemo.php";  
-         SocketAddress dest = new InetSocketAddress(this.host, this.port);  
-         socket.connect(dest);  
-         OutputStreamWriter streamWriter = new OutputStreamWriter(socket.getOutputStream());  
-         bufferedWriter = new BufferedWriter(streamWriter);  
- 
-         bufferedWriter.write("GET " + path + " HTTP/1.1\r\n");  
-         bufferedWriter.write("Host: " + this.host + "\r\n");  
-         bufferedWriter.write("\r\n");  
-         bufferedWriter.flush();  
- 
-         BufferedInputStream streamReader = new BufferedInputStream(socket.getInputStream());  
-         bufferedReader = new BufferedReader(new InputStreamReader(streamReader, "utf-8"));  
-         String line = null;  
- while((line = bufferedReader.readLine())!= null)  
-         {  
-             System.out.println(line);  
-         }  
-         bufferedReader.close();  
-         bufferedWriter.close();  
-         socket.close();  
-     }  
- 
- publicvoid sendPost() throws IOException  
-     {  
-         String path = "/zhigang/postDemo.php";  
-         String data = URLEncoder.encode("name", "utf-8") + "=" + URLEncoder.encode("gloomyfish", "utf-8") + "&" +  
-                         URLEncoder.encode("age", "utf-8") + "=" + URLEncoder.encode("32", "utf-8");  
- // String data = "name=zhigang_jia";
-         SocketAddress dest = new InetSocketAddress(this.host, this.port);  
-         socket.connect(dest);  
-         OutputStreamWriter streamWriter = new OutputStreamWriter(socket.getOutputStream(), "utf-8");  
-         bufferedWriter = new BufferedWriter(streamWriter);  
- 
-         bufferedWriter.write("POST " + path + " HTTP/1.1\r\n");  
-         bufferedWriter.write("Host: " + this.host + "\r\n");  
-         bufferedWriter.write("Content-Length: " + data.length() + "\r\n");  
-         bufferedWriter.write("Content-Type: application/x-www-form-urlencoded\r\n");  
-         bufferedWriter.write("\r\n");  
-         bufferedWriter.write(data);  
-         bufferedWriter.flush();  
-         bufferedWriter.write("\r\n");  
-         bufferedWriter.flush();  
- 
-         BufferedInputStream streamReader = new BufferedInputStream(socket.getInputStream());  
-         bufferedReader = new BufferedReader(new InputStreamReader(streamReader, "utf-8"));  
-         String line = null;  
- while((line = bufferedReader.readLine())!= null)  
-         {  
-             System.out.println(line);  
-         }  
-         bufferedReader.close();  
-         bufferedWriter.close();  
-         socket.close();  
-     }  
- 
- publicstaticvoid main(String[] args)  
-     {  
-         TestDemo td = new TestDemo("127.0.0.1",8099);  
- try {  
- // td.sendGet(); //send HTTP GET Request
- 
-             td.sendPost(); // send HTTP POST Request
-         } catch (IOException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-     }  
- 
- }
想要成为支持HTTPS客户端, 只要在创建Socket的时候如下:
**[java]**[view
 plain](http://blog.csdn.net/jia20003/article/details/17104791#)[copy](http://blog.csdn.net/jia20003/article/details/17104791#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/95455/fork)
- socket = (SSLSocket)((SSLSocketFactory)SSLSocketFactory.getDefault()).createSocket(this.host, port);
  
所以有时候离开apache的HTTP Client等第三方Jar,程序员也是一样活!
