# 自己实现一个简易web服务器 - 毛小亮 - 博客园
# [自己实现一个简易web服务器](https://www.cnblogs.com/xianerwonder/p/5710351.html)
**一个web服务器是网络应用中最基础的环节。**
构建需要理解三个内容：
1.http协议
2.socket类
3.服务端实现原理
**1.1 HTTP http请求** 一般一个http请求包括以下三个部分：
 1 请求方法，如get,post
 2 请求头3 实体
**1.2http响应**与http请求类似，http响应也包括三个部分
1 协议-状态码-描述
2 响应头
3 响应实体段
**2.Socket**
**2.1**不同的应用程序可以通过套接字发送或接受字节流。java中提供了Socket类来实现这个功能。
**2.2**光靠Socket类还是不能够实现我们构建一个服务器应用程序的功能的，因为服务器必须时刻待命，因此java里面提供了ServerSocket类来处理等待来自客户端的请求，当ServerSocket接受到了来自客户端的请求之后，它就会创建一个实例来处理与客户端的通信。
**3.实现原理**
**3.1**构建一个封装请求信息的类requst,一个响应请求的类response,还要有一个主程序httpServer来处理客户端来的请求
**下面是一个requst类，主要处理uri**
```
1 package com.lwq;
 2 
 3 import java.io.IOException;
 4 import java.io.InputStream;
 5 
 6 /**10  * 将浏览器发来的请求信息转化成字符和截取url
11  */
12 public class Request {
14     //输入流
15     private InputStream input;
16     //截取url,如http://localhost:8080/index.html ，截取部分为 /index.html
17     private String uri;
18     public Request(InputStream inputStream){
19         this.input = inputStream;
20     }
22     public InputStream getInput() {
23         return input;
24     }
25     public void setInput(InputStream input) {
26         this.input = input;
27     }
28     public String getUri() {
29         return uri;
30     }
31     public void setUri(String uri) {
32         this.uri = uri;
33     }
35     //从套接字中读取字符信息
36     public void parse(){
38             StringBuffer request = new StringBuffer(2048);
39             int i = 0;
40             byte[] buffer = new byte[2048];
41             
42             try {
43                 i = input.read(buffer);
44             } catch (IOException e) {
45                 // TODO Auto-generated catch block
46                 e.printStackTrace();
47                 i = -1;
48             }
49             for(int j = 0;j<i;j++){
50                     request.append((char)(buffer[j]));//拼接请求字符串
51             }
52             System.out.println(request.toString());
53             uri = parseUri(request.toString());//截取url，存入uri字段
54             }
55     //截取请求的url
56     private String parseUri(String requestString){
58         int index1 = 0;
59         int index2 = 0;
60         index1 = requestString.indexOf(' ');
61         if(index1!=-1){
62             index2 = requestString.indexOf(' ',index1+1);
63             if(index2>index1){
64                 return requestString.substring(index1+1,index2);
65             }
66         }
68         return null;
69     }
74     }
```
**下面是封装了响应请求的类response，用于返回文件或数据**
```
1 package com.lwq;
 2 
 3 import java.io.File;
 4 import java.io.FileInputStream;
 5 import java.io.FileNotFoundException;
 6 import java.io.IOException;
 7 import java.io.OutputStream;
 8 import java.io.PrintWriter;
 9 
10 /**13  * 类说明 根据相应信息返回结果
14  */
15 public class Response {
16     
17     private static final int BUFFER_SIZE = 1024;
18     Request request;
19     OutputStream output;
20     public Response(OutputStream output){
21         this.output = output;
22     }
23     
24     public void sendStaticResource() throws IOException{
26         byte[] bytes = new byte[BUFFER_SIZE];
27         FileInputStream fis = null;
29         File file = new File(HttpServer.WEB_ROOT,request.getUri());
30         if(file.exists()){
31             try {
32                 fis = new FileInputStream(file);
33                 int ch = fis.read(bytes,0,BUFFER_SIZE);
34                 while(ch != -1){
35                     output.write(bytes,0,ch);//将文件的一部分写入流
36                     ch = fis.read(bytes,0,BUFFER_SIZE);
37                 }
38                 
39             } catch (FileNotFoundException e) {
40                 // TODO Auto-generated catch block
41                 e.printStackTrace();
42             }catch(IOException e){
43                 e.printStackTrace();
44             }finally{
45                 if(fis !=null){
46                     fis.close();
47                 }
48             }
50         }else{
51             //找不到文件
52              String errorMessage = "HTTP/1.1 404 File Not Found\r\n" +
53      "Content-Type: text/html\r\n" +
54      "Content-Length: 23\r\n" +
55      "\r\n" +
56      "
57 File Not Found
58 ";
59              try {
60                 output.write(errorMessage.getBytes());
61                 output.flush();
62             } catch (IOException e) {
63                 // TODO Auto-generated catch block
64                 e.printStackTrace();
65             }
66         }
67     }
68     public Request getRequest() {
69         return request;
70     }
71     public void setRequest(Request request) {
72         this.request = request;
73     }
74     public OutputStream getOutput() {
75         return output;
76     }
77     public void setOutput(OutputStream output) {
78         this.output = output;
79     }
80     public static int getBUFFER_SIZE() {
81         return BUFFER_SIZE;
82     }
86 }
```
**主程序：**
```
1 package com.lwq;
 2 
 3 import java.io.File;
 4 import java.io.InputStream;
 5 import java.io.OutputStream;
 6 import java.net.InetAddress;
 7 import java.net.ServerSocket;
 8 import java.net.Socket;
 9 
10 /**13  * 类说明
14  */
15 public class HttpServer {
17     /**
18      * @param args
19      */
20     
21     //WEB_ROOT是服务器的根目录
22     public static final String WEB_ROOT = System.getProperty("user.dir")+File.separator+"webroot";
23     
24     //关闭的命令
25     private static final String SHUTDOWN_COMMAND= "/SHUTDOWN";
26     
27     public static void main(String[] args) {
28         // TODO Auto-generated method stub
29         HttpServer server = new HttpServer();
30         server.await();
31 
32     }
33     public void await(){
34         ServerSocket serverSocket = null;
35         int port = 8080;
36         try {
37             serverSocket = new ServerSocket(port,1,InetAddress.getByName("127.0.0.1"));
38             while(true)
39             {
40                 try {
41             Socket socket = null;
42             InputStream input = null;
43             OutputStream output = null;
44             socket = serverSocket.accept();
45             input = socket.getInputStream();
46             output = socket.getOutputStream();
47             //封装request请求
48             Request request = new Request(input);
49             request.parse();
50             //封装response对象
51             Response response = new Response(output);
52             response.setRequest(request);
53             response.sendStaticResource();
54             socket.close();
55                 } catch (Exception e) {
56                     // TODO Auto-generated catch block
57                     e.printStackTrace();
58                     continue;
59                 }
61             }
62         } catch (Exception e) {
63             // TODO Auto-generated catch block
64             e.printStackTrace();
65         }
69     }
72 }
```
**运行httpServer,在浏览器中打下http://localhost:8080/index.jsp,就能看到服务器响应的结果了。**
**侵删。**

