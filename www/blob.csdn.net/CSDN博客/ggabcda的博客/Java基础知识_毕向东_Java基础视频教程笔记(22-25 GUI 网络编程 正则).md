# Java基础知识_毕向东_Java基础视频教程笔记(22-25 GUI 网络编程 正则) - ggabcda的博客 - CSDN博客





2017年09月11日 15:30:05[ggabcda](https://me.csdn.net/ggabcda)阅读数：1012








22天-01-GUI

GUI：Graphical User Interface 图形用户接口

Java为GUI提供的对象都存在java.Awt和javax.Swing两个包中

CLI：Common line User Interface 命令行用户接口

Awt：Abstract Window ToolKit(抽象工具包)，需要调用本地系统方法实现功能，属于重量级控件。

Swing：在Awt的基础上，建立的一套图形界面系统，其中提供了更多的组件，而且完全由Java实现，增强了移植性，属于轻量级控件。

继承关系：

Component 　　　　　　Container Window、Panel Frame、　　　　　　　　Dialog FileDialog

　　　　　　　　　　　  Button、Label、Checkbox、TextComponent 　　　  TextArea、TextField

Container：为容器，是一个特殊的组件，该组件可以通过add方法添加其他组件进来。

Swing继承关系图：

![](http://images2017.cnblogs.com/blog/1035949/201709/1035949-20170910160207819-1265809886.png)

布局：容器中的组件排放方式

常见的布局管理器：

FlowLayout:　　　　流式布局管理器，从左到右的顺序排列，Panel默认的布局管理器

BorderLayout：　　 边界布局管理器，中东南西北，Frame默认的布局管理器

GridLayout：　　　  网格布局管理器，规则的矩阵

CardLayout：　　　 卡片布局管理器，选项卡

GridBagLayout：　   网格包布局管理器，非规则的矩阵

事件监听机制的特点：

1.事件源：就是awt包或者swing包中的那些图形界面组件。

2.事件：每一个事件源都有自己特有的对应事件和共性事件。

3.监听器：将可以触发某个事件的动作(不止一个动作)都已经封装到了监听器中。

4.事件处理

制作可执行文件：

1.制作配置文件config.txt，冒号后面必须有一个空格，最后一行必须换行

Main-Class: myPackage.ExecuteFile

2.先编译　　 javac -d packagePath File.java 

javac -d . ExecuteFile.java

3.打包　　　 jar -cvfm package.jar config.txt PackageDirectory

jar -cvfm ExecuteFile.jar config.txt myPackage

ExecuteFile示例： 

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

 1 package myPackage;
 2 import java.util.*;
 3 import java.io.*;
 4 import java.awt.*;
 5 import java.awt.event.*;
 6 
 7 public class ExecuteFile
 8 {
 9     private Frame fr;
10     public static void main(String[] args)throws IOException
11     {
12         new ExecuteFile();
13     }    
14     public ExecuteFile()
15     {
16         init();
17     }    
18     private void init()
19     {
20         fr=new Frame("Demo");
21         fr.setSize(400,500);
22         fr.setLocation(200,100);
23         fr.setVisible(true);
24         fr.addWindowListener(new WindowAdapter(){
25             public void windowClosing(WindowEvent e) {
26                 System.exit(0);
27             }        
28         });
29     }
30 }


ExecuteFile
```

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

1 Main-Class: myPackage.ExecuteFile


config.txt
```

23天-01-网络编程

网络基础知识：

IP地址：网络中设备的标识
本地回环地址：　　127.0.0.1 　　 主机名：localhost

端口号：用于标识进程的逻辑地址，不同进程的标识。有效端口号：0~65535，其中0~1024为系统使用或者保留端口。
Internet默认端口：　80 　　　　 tomcat默认端口：8080 　　　　数据库默认端口：3306

传输协议：计算机之间通信的规则。常见协议：TCP，UDP

OSI模型：　　应用层，表示层，会话层，传输层，网络层，数据链路层，物理层

TCP/IP模型： 　　 应用层， 　　　　　  传输层，网络层，　　主机至网络层

UDP：将数据及源和目的封装成数据包中，不需要建立连接，每个数据报的大小限制64k内，因无连接，是不可靠协议，不需要建立连接，速度快。
TCP：建立连接，形成传输数据的通道，在连接中进行大量数据传输，通过三次握手完成连接，是可靠协议，必须建立连接，效率稍低。

TCP与UDP区别：

1、TCP面向连接（如打电话要先拨号建立连接）; 　　 　　　　　　UDP是无连接的，即发送数据之前不需要建立连接

2、TCP提供可靠的服务。也就是说，通过TCP连接传送的数据，无差错，不丢失，不重复，且按序到达; 　　　　 UDP尽最大努力交付，即不保证可靠交付

3、TCP面向字节流，实际上是TCP把数据看成一连串无结构的字节流;UDP是面向报文的，UDP没有拥塞控制，因此网络出现拥塞不会使源主机的发送速率降低（对实时应用很有用，如IP电话，实时视频会议等）

4、每一条TCP连接只能是点到点的; 　　　　　　　　　　　　　　 
UDP支持一对一，一对多，多对一和多对多的交互通信

5、TCP首部开销20字节; 　　　　　　　　　　　　　　　　　　　 UDP的首部开销小，只有8个字节

6、TCP的逻辑通信信道是全双工的可靠信道；　　　　　　　　　　UDP则是不可靠信道

Socket：是为网络服务提供的一种机制，通信的两端都有Socket，数据在两个Socket间通过IO传输。

注：IP地址段中，一个网段的最后两段是1.0和1.255时比较特殊，1.0 表示一个区域的网络地址-网络段，1.255代表此网段的广播地址

UDP传输：

UDP发送端：

1.建立UDPSocket服务，通过DatagramSocket对象建立；

2.获取数据，并将数据封装到数据包中，数据包对象DatagramPacket；

3.通过Socket服务的发送功能send();方法，将数据包发送出去；

4.关闭资源。

UDP接收端：

1.定义UDPSocket服务，建立DatagramSocket对象时通常会监听一个端口，其实就是给这个接收网络应用程序定义数字标识。方便于明确哪些数据过来该应用程序可以处理；

2.定义一个数据包用来存储接收到的字节数据。数据包对象中有很多功能可以提取字节数据中的不同数据信息，数据包DatagramPacket对象；

3.通过服务DatagramSocket的receive();方法将接收到的数据存入数据包对象中；

4.在数据包DatagramPacket对象中将这些不同的数据取出来。

5.关闭资源。



Socket UDP聊天软件聊天示例：

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

 1 /**
 2  * 模拟聊天软件，有收数据的部分和发送数据的部分。这两部分需要同时执行，
 3  * 使用多线程技术一个线程控制收，一个线程控制发。
 4  * 因为收和发动作不是一致的，所以要定义两个run方法，而这两个方法要封装到不同类中
 5  */
 6 class Demo
 7 {
 8     public static void main(String[] args) throws Exception
 9     {
10         DatagramSocket send = new DatagramSocket();
11         DatagramSocket receive = new DatagramSocket(10001);
12         new Thread(new UDPSend(send)).start();
13         new Thread(new UDPReceive(receive)).start();
14     }
15 }
16 
17 class UDPSend implements Runnable
18 {    //发送端
19     private DatagramSocket ds;
20     UDPSend(DatagramSocket ds)
21     {
22         this.ds = ds;
23     }
24     public void run()
25     {
26         try
27         {
28             BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
29             String temp;
30             while ((temp = br.readLine()) != null)
31             {
32                 if ("exit".equals(temp))
33                 { break;}
34                 byte[] line = temp.getBytes();
35                 DatagramPacket dp = new DatagramPacket(line, line.length, InetAddress.getByName("192.168.0.255"), 10001);
36                 ds.send(dp);
37             }
38             ds.close();
39         }
40         catch (Exception e)
41         {
42             e.getStackTrace();
43         }
44     }
45 }
46 
47 class UDPReceive implements Runnable
48 {    //接收端
49     private DatagramSocket ds;
50     UDPReceive(DatagramSocket ds)
51     {
52         this.ds = ds;
53     }
54     public void run()
55     {
56         try
57         {
58             while (true)
59             {
60                 byte[] line = new byte[1024 * 64];
61                 DatagramPacket dp = new DatagramPacket(line, line.length);
62                 ds.receive(dp);
63                 String ip = dp.getAddress().getHostAddress();
64                 String data = new String(dp.getData(), 0, dp.getLength());
65                 System.out.println(ip + ": " + data);
66             }
67         }
68         catch (Exception e)
69         {
70             e.getStackTrace();
71         }
72     }
73 }


Socket UDP
```

TCP传输：

1.TCP分客户端和服务端,分别是独立的两个应用程序。

2.客户端对应的对象是Socket，服务端对应的是ServerSocket，建立连接后，通过Socket中的IO流进行数据的传输

客户端：通过Socket对象，建立连接指定主机的连接。因为TCP是面向连接的，所以在建立Socket连接时，就要有服务端存在，并确保连接成功形成通路后，在该通道进行数据的传输。

客户端建立步骤：

1.创建客户端Socket服务，并指定要连接的主机和端口；

2.准备发送数据，获取Socket流中的输出流；

3.关闭资源。

服务端：

服务端建立步骤：

1.建立服务端的Socket服务，ServerSocket(port);并监听一个端口；

2.获取连接过来的客户端对象。通过ServerSocket的accept();方法，在没有连接时就会等待，所以这个方法是阻塞式的；

3.客户端如果发过来数据，那么服务端要使用对应的客服端对象，并获取到该客户端对象的读取流来读取发过来的数据；

4.关闭客户端，服务端一般不关闭(可选)。

TCP文件上传示例：

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

 1 /**
 2  * 实现并发上传数据
 3  * 服务端采用多线程，给每一个客户端访问连接分配一个线程，实现并发多客户端访问服务端
 4  */
 5 class FileClient
 6 {
 7     public static void main(String[] agrs) throws Exception
 8     {
 9         Socket s = new Socket("192.168.0.102", 10000);
10         OutputStream out = s.getOutputStream();
11         BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
12         System.out.println("upload file name or full path:");
13         //客户端准备上传的文件名
14         String fileName = br.readLine();
15         System.out.println("server file name:");
16         //上传文件在服务端名称
17         String modifyName = br.readLine();
18         out.write(modifyName.getBytes());
19 
20         FileInputStream fis = new FileInputStream(fileName);
21         byte[] b = new byte[1024];
22         int len;
23         while ((len = fis.read(b)) != -1)
24         {
25             out.write(b, 0, len);
26         }
27         s.shutdownInput();
28         s.close();
29     }
30 }
31 class ServerThread implements Runnable
32 {
33     private Socket s;
34     ServerThread(Socket s)
35     {
36         this.s = s;
37     }
38     @Override
39     public void run()
40     {
41         String ip = s.getInetAddress().getHostAddress();
42         try
43         {
44             System.out.println(ip + ".....connected success");
45             byte[] b = new byte[1024];
46             int len;
47             InputStream in = s.getInputStream();
48             len = in.read(b);
49             String fileName = new String(b, 0, len);
50             FileOutputStream fos = new FileOutputStream(fileName);
51             byte[] bt=new byte[1024];
52             while ((len = in.read(bt)) != -1)
53             {
54                 fos.write(bt, 0, len);
55             }
56             OutputStream out = s.getOutputStream();
57             out.write(("上传成功：" + fileName).getBytes());
58             fos.close();
59             s.close();
60         }
61         catch (Exception e)
62         {
63             System.out.println(ip + "......upload fail");
64             System.out.println(e.getStackTrace());
65         }
66     }
67 }
68 class FileServer
69 {
70     public static void main(String[] agrs) throws Exception
71     {
72         ServerSocket ss = new ServerSocket(10000);
73         while (true)
74         {
75             Socket s = ss.accept();
76             new Thread(new ServerThread(s)).start();
77         }
78     }
79 }


Upload File
```

TCP登录示例：

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

  1 /**
  2  * 客户端通过键盘录入用户名和密码，服务端对这个用户名和密码进行校验
  3  * 如果该用户名存在，并且密码正确，在服务端显示XX已登录，并在客户端显示XXX，欢迎光临
  4  * 如果该用户尝试三次登录，用户名和密码没有同时校验通过，则关闭客户端
  5  */
  6 class LoginClient
  7 {
  8     public static void main(String[] args) throws Exception
  9     {
 10         Socket s = new Socket("192.168.0.102", 10000);
 11         BufferedReader bfr = new BufferedReader(new InputStreamReader(System.in));
 12         BufferedReader bfw = new BufferedReader(new InputStreamReader(s.getInputStream()));
 13         PrintWriter pw = new PrintWriter(s.getOutputStream(), true);
 14 
 15         for (int i = 0; i < 3; i++)
 16         {
 17             System.out.println("user name:");
 18             String userName = bfr.readLine();
 19             System.out.println("user password:");
 20             String userPwd = bfr.readLine();
 21             pw.println(userName);
 22             pw.println(userPwd);
 23             String data = bfw.readLine();
 24             String[] result = data.split(":");
 25             if ("true".equals(result[0]))
 26             {
 27                 System.out.println(userName + "，欢迎光临！");
 28                 break;
 29             }
 30             else
 31             {
 32                 System.out.println(userName + "，登录失败! " + result[1]);
 33             }
 34         }
 35         bfr.close();
 36         pw.close();
 37         s.close();
 38     }
 39 }
 40 class UserThread implements Runnable
 41 {
 42     private Socket s;
 43     UserThread(Socket s)
 44     {
 45         this.s = s;
 46     }
 47     @Override
 48     public void run()
 49     {
 50         String ip = s.getInetAddress().getHostAddress();
 51         try
 52         {
 53             System.out.println(ip + "......connected success");
 54             BufferedReader br = new BufferedReader(new InputStreamReader(s.getInputStream()));
 55             BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
 56 
 57             for (int i = 0; i < 3; i++)
 58             {
 59                 String userName = br.readLine();
 60                 String userPwd = br.readLine();
 61                 if (userName.isEmpty() || userPwd.isEmpty())
 62                 {
 63                     bw.write("false:The user name or password can not be empty");
 64                     bw.newLine();
 65                     bw.flush();
 66                     System.out.println("user name or password is empty");
 67                     continue;
 68                 }
 69                 String pwd = getConfigInfo("config.properties", userName);
 70                 if (!userPwd.equals(pwd))
 71                 {
 72                     bw.write("false:The username or password is wrong");
 73                     bw.newLine();
 74                     bw.flush();
 75                     System.out.println("user password is wrong");
 76                 }
 77                 else
 78                 {
 79                     System.out.println("user is Login");
 80                     bw.write("true:Login success");
 81                     bw.newLine();
 82                     bw.flush();
 83                     break;
 84                 }
 85             }
 86             br.close();
 87             bw.close();
 88             s.close();
 89             System.out.println(ip + "......disconnected");
 90         }
 91         catch (Exception e)
 92         {
 93             System.out.println(ip + "......connected fail");
 94             System.out.println(e.getMessage());
 95             StackTraceElement[] stack = e.getStackTrace();
 96             if (stack != null)
 97             {
 98                 for (int i = 0; i < stack.length; i++)
 99                 {
100                     System.out.println(stack[i].toString());
101                 }
102             }
103         }
104     }
105 
106     /**
107      * 获取配置文件信息
108      *
109      * @param filePath 文件路径
110      * @param key      键值
111      * @return 值
112      */
113     private String getConfigInfo(String filePath, String key)
114     {
115         Properties properties = new Properties();
116         try
117         {
118             properties.load(new BufferedReader(new FileReader(filePath)));
119             return properties.getProperty(key);
120         }
121         catch (FileNotFoundException e)
122         {
123             e.printStackTrace();
124         }
125         catch (IOException e)
126         {
127             e.printStackTrace();
128         }
129         return null;
130     }
131 }
132 class LoginServer
133 {
134     public static void main(String[] args) throws Exception
135     {
136         ServerSocket ss = new ServerSocket(10000);
137         while (true)
138         {
139             Socket s = ss.accept();
140             new Thread(new UserThread(s)).start();
141         }
142     }
143 }


Login
```

URL类：统一资源定位符

URLConnection openConnection();　　//获取URL 所引用的远程对象的连接

URLConnection conn=url.openConnection();

abstract void connect();　　//子类实现方法建立到远程对象的实际连接

OutputStream getOutputStream();　　//获取连接读取的输入流

InputStream getInputStream();　　　 //获取连接的输出流

void setConnectTimeout(int timeout);  //设置一个指定的超时值（以毫秒为单位）

void setReadTimeout(int timeout);//将读超时设置为指定的超时值，以毫秒为单位

模拟浏览器客户端：

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

 1 class Demo
 2 {
 3     public static void main(String[] args) throws Exception
 4     {
 5         URL url = new URL("http://www.google.cn/");
 6         URLConnection conn = url.openConnection();
 7         conn.connect();
 8         InputStream in = conn.getInputStream();
 9         int len;
10         byte[] bt = new byte[1024*1024];
11         while ((len = in.read(bt)) != -1)
12         {
13             //注意编码格式，GBK，UTF-8,Unicode
14             System.out.println(new String(bt, 0, len, "UTF-8"));
15         }
16     }
17 }


Browser
```

25天-01-正则表达式

正则表达式：符合一定规则的表达式，用于专门操作字符串。

特点：用一些特定的符号来表示一些代码操作，简化对特殊字符串的校验

使用正则表达式思路：

1.如果只想知道该字符串是对是错，使用匹配；

2.如果想要将已有的字符串变成另一个字符串，替换；

3.如果想要按照自定的方式将字符串变成多个字符串，切割，获取规则以外的子串；

4.如果想要拿到符合需求的字符串子串，获取，获取符合规则的子串。

正则表达式校验邮箱地址：

//较为精确匹配
String reg="[a-zA-Z0-9]+@[a-zA-Z0-9]+(\\.[a-zA-Z]+)+";

//相对上面,@后面采用模糊匹配，1@1.1会通过校验
String reg="\\w+@\\w+(\\.\\w+)+";

具体操作功能：用规则匹配整个字符串，只要有一处不符合规则，匹配就结束，返回false
1.匹配方法：boolean matches(String regex); 　　 Pattern.matches(regex, str);

Regex示例：

//QQ号匹配：全部为数字，第一位不为0，总共有5到15位

String reg="[1-9]\\d{4,14}";

//手机号匹配：13xxx、15xxx、18xxx三个段位，总共11位数字

String reg="1[358]\\d{9}";

2.切割方法：String[] split(String regex); String[] split(String regex, int limit);

Regex示例：

//按照多个空格进行切割

String reg=" +";

//按照.号进行切割

String reg="\\.";

//按照\\进行切割，用于文件路径操作

String reg="\\\\";

//按照叠词完成切割。为了可以让规则的结果被重用，可以将规则封装成一个组，用()表示。组的出现都有编号，从1开始，定义编号\\n，想要使用已有的组可以通过$n(n就是组的编号)的形式来获取
String reg="(.)\\1+";

3.替换方法：String replace(char oldChar, char newChar);　　String replace(CharSequence target, CharSequence replacement);

String replaceAll(String regex, String replacement);　　　　　String replaceFirst(String regex, String replacement);

Regex示例：

//将字符串中的数字替换成*，防止联系方式泄露

replaceAll("\\d{5,}","*");

//将重叠的字符替换成单个字母,$1 是获取前面组的编号.例如：dafaaaffffc ->dafafc
replaceAll("(.)\\1+","$1");

4.获取方法：将字符串中符合规则的子集取出

步骤：

1.将正则表达式封装成对象；

2.让正则对象和要操作的字符串相关联；

3.关联后，获取正则匹配引擎；

4.通过引擎对符合规则的子串进行操作。

获取单词示例：

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

 1 import java.util.regex.Matcher;
 2 import java.util.regex.Pattern;
 3 /**
 4  * 其实String类中的matches方法用的就是Pattern和Matcher对象完成的。只不过被String的方法封装后用起来较为简单，但是功能却单一。
 5  */
 6 class Demo
 7 {
 8     public static void main(String[] args)
 9         throws Exception
10     {
11         String str = "ming tian ni shi fou yao jia gei wo!";
12         String reg = "\\b[a-zA-Z]{2,}\\b";
13         //将规则封装成对象
14         Pattern p = Pattern.compile(reg);
15         //让正则对象和要作用的字符串相关联，获取匹配器对象
16         Matcher m = p.matcher(str);
17         //将规则作用到字符串上，并进行符合规则的子集查找，group前必须find
18         while (m.find())
19         {
20             //用于获取匹配后的结果
21             System.out.println(m.group());
22             System.out.println(m.start() + "..." + m.end());
23         }
24     }
25 }


Obtain word
```

获取网页邮箱示例：

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

 1 import java.io.BufferedReader;
 2 import java.io.InputStream;
 3 import java.io.InputStreamReader;
 4 import java.net.URL;
 5 import java.net.URLConnection;
 6 import java.util.regex.Matcher;
 7 import java.util.regex.Pattern;
 8 class Demo
 9 {
10     private static final String REGEX_MAIL = "\\w+@\\w+(\\.\\w+)+";
11     public static void main(String[] args)throws Exception
12     {
13         String dad = "dadaffa@dada.com";
14         System.out.println(dad.matches(REGEX_MAIL));
15 
16         BufferedReader bfr = new BufferedReader(new InputStreamReader(System.in));
17         boolean flag = false;
18         while (true)
19         {
20             if (flag)
21             {
22                 break;
23             }
24             String str = null;
25             while ((str = bfr.readLine()) != null)
26             {
27                 if ("exit".equals(str))
28                 {
29                     flag = true;
30                     break;
31                 }
32                 else
33                 {
34                     try
35                     {
36                         regexMail(str);
37                     }
38                     catch (Exception e)
39                     {
40                         System.out.println("input url is wrong");
41                         System.out.println(e.getMessage());
42                     }
43                 }
44                 System.out.println("**********************");
45                 System.out.println("please input next url:");
46             }
47         }
48         System.out.println("program is over!");
49     }
50     private static void regexMail(String param)throws Exception
51     {
52         Pattern pattern = Pattern.compile(REGEX_MAIL);
53         URL url = new URL(param);
54         URLConnection conn = url.openConnection();
55         InputStream in = conn.getInputStream();
56         BufferedReader bfr = new BufferedReader(new InputStreamReader(in, "utf-8"));
57 
58         String len = null;
59         while ((len = bfr.readLine()) != null)
60         {
61             Matcher m = pattern.matcher(len);
62             while (m.find())
63             {
64                 System.out.println(m.group());
65             }
66         }
67     }
68 }


Web Mail
```

Regex正则表达式规则：

字符

x 　　　　 　　 字符 x 

\\ 　　　　 　　反斜线字符 

\0n 　　　　　  带有八进制值 0 的字符 n (0 <= n <= 7) 

\0nn 　　　　　带有八进制值 0 的字符 nn (0 <= n <= 7) 

\0mnn 　　　　带有八进制值 0 的字符 mnn（0 <= m <= 3、0 <= n <= 7） 

\xhh 　　　　　带有十六进制值 0x 的字符 hh 

\uhhhh 　　　   带有十六进制值 0x 的字符 hhhh 

\t 　　　　　　  制表符 ('\u0009') 

\n 　　　　　　 新行（换行）符 ('\u000A') 

\r 　　　　　　 回车符 ('\u000D') 

\f 　　　　　　 换页符 ('\u000C') 

\a 　　　　　　 报警 (bell) 符 ('\u0007') 

\e 　　　　　　 转义符 ('\u001B') 

\cx 　　　　　　对应于 x 的控制符 

字符类

[abc]　　　　　　　a、b 或 c（简单类） 

[^abc] 　　　　　　 任何字符，除了 a、b 或 c（否定） 

[a-zA-Z] 　　　 　　a 到 z 或 A 到 Z，两头的字母包括在内（范围） 

[a-d[m-p]] 　　  　　a 到 d 或 m 到 p：[a-dm-p]（并集） 

[a-z&&[def]] 　　　  d、e 或 f（交集） 

[a-z&&[^bc]] 　　　  a 到 z，除了 b 和 c：[ad-z]（减去） 

[a-z&&[^m-p]] 　　　a 到 z，而非 m 到 p：[a-lq-z]（减去） 

预定义字符类

. 　　　　　　　　  任何字符（与行结束符可能匹配也可能不匹配） 

\d 　　　　　　　    数字：[0-9] 

\D 　　　　　　　   非数字： [^0-9] 

\s 　　　　　　　　空白字符：[ \t\n\x0B\f\r] 

\S 　　　　　　　　非空白字符：[^\s] 

\w 　　　　　　　　单词字符：[a-zA-Z_0-9] 

\W 　　　　　　　   非单词字符：[^\w] 

边界匹配器

^ 　　　　　　　　 行的开头 

$ 　　　　　　　　 行的结尾 

\b 　　　　　　　　单词边界 

\B 　　　　　　　　非单词边界 

\A　　　　　　　　 输入的开头 

\G 　　　　　　　　上一个匹配的结尾 

\Z 　　　　　　　　输入的结尾，仅用于最后的结束符（如果有的话） 

\z 　　　　　　　　输入的结尾 

Greedy 数量词

X? 　　　　　　　　 X，一次或一次也没有 

X* 　　　　　　　　  X，零次或多次 

X+ 　　　　　　　　 X，一次或多次 

X{n} 　　　　　　　  X，恰好 n 次 

X{n,} 　　　　　　　 X，至少 n 次 

X{n,m} 　　　　　　 X，至少 n 次，但是不超过 m 次 





