# android socket通信 - xqhrs232的专栏 - CSDN博客
2011年03月15日 16:06:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2680标签：[socket																[android																[server																[exception																[通讯																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://android.group.javaeye.com/group/blog/686750](http://android.group.javaeye.com/group/blog/686750)
Android作为完全开源的手机平台，已经被非常多的开发者视作未来最有潜力的智能手机操作系统。 
而且短时间内已经有大量的第三方应用程序出现在Android Market供用户下载和使用， 
其中有一些程序增强了手机的应用功能，而另外一些会充分的发挥手机与其它电子设备的互动。 
今天将主要通过两个例子讲解如何实现Android与PC通讯。 
1. Android与PC通讯的第一个例子: 
PC: Java Server 
Android:Java Client 
Java Server 源代码： 
01.public class TCPDesktopServer implements Runnable{ 
2 
03.    public static final String SERVERIP = "192.168.0.149"; 
04.    public static final int SERVERPORT = 51706; 
5 
06.    public void run() { 
07.         try { 
08.             System.out.println("S: Connecting..."); 
9 
10.             ServerSocket serverSocket = new ServerSocket(SERVERPORT); 
11.             while (true) { 
12 
13.                  Socket client = serverSocket.accept(); 
14.                  System.out.println("S: Receiving..."); 
15 
16.                  try { 
17.                      BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
18.                      String str = in.readLine(); 
19.                      System.out.println("S: Received: '" + str + "'"); 
20.                    } catch(Exception e) { 
21.                        System.out.println("S: Error"); 
22.                        e.printStackTrace(); 
23.                    } finally { 
24.                        client.close(); 
25.                        System.out.println("S: Done."); 
26.                    } 
27 
28.             } 
29 
30.         } catch (Exception e) { 
31.             System.out.println("S: Error"); 
32.             e.printStackTrace(); 
33.         } 
34.    } 
35 
36.    public static void main (String a[]) { 
37.        Thread desktopServerThread = new Thread(new TCPDesktopServer()); 
38.        desktopServerThread.start(); 
39.    } 
40.} 
对上边的源代码做一些介绍： 
指定Server监听的端口和服务器IP地址。 
1.public static final String SERVERIP = "192.168.0.149"; 
2.public static final int SERVERPORT = 51706; 
应用之前所指定的IP和Port创建一个ServerSocket对象。 
1.ServerSocket serverSocket = new ServerSocket(SERVERPORT); 
用于侦听和捕捉通过Socket连接的客户端。 
1.Socket client = serverSocket.accept(); 
应用Socket创建BufferedReader对象,用于接收Socket Stream中的数据。 
1.BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
Android Client 源代码： 
01.InetAddress serverAddr = InetAddress.getByName("192.168.0.149");//TCPServer.SERVERIP
02.Log.d("TCP", "C: Connecting..."); 
03.Socket socket = new Socket(serverAddr, 51706); 
04.String message = "AndroidRes,Where is my Pig (Android)?"; 
05.try { 
06.    Log.d("TCP", "C: Sending: '" + message + "'"); 
07.    PrintWriter out = new PrintWriter( new BufferedWriter( new OutputStreamWriter(socket.getOutputStream())),true);
08.        out.println(message); 
09.} catch(Exception e) { 
10.        Log.e("TCP", "S: Error", e); 
11.} finally { 
12.    socket.close(); 
13.} 
对上边的源代码做一些介绍： 
指定Server的IP地址。 
1.InetAddress serverAddr = InetAddress.getByName("192.168.0.149"); 
应用Server的IP和端口建立Socket对象。 
1.Socket socket = new Socket(serverAddr, 51706); 
根据已经建立的Socket来创建PrintWriter，将信息通过这个对象来发送给Server，其中包含了三个部分： 
OutputStreamWriter 
BufferedWriter 
PrintWriter 
1.PrintWriter out = new PrintWriter( new BufferedWriter( new OutputStreamWriter(socket.getOutputStream())),true);
以上是Android与运行在PC上的Java Server通讯的例子。来源由Anddev.org提供 
2. Android与PC通讯的第二个例子: 
PC: CSharp Server 
Android:Java Client 
通过上边的例子，本人通过一些转换，实现了Android与CSharp Server的通讯方法。原理没有太大差异，仅仅是应用的方法稍有不同。 
CSharp Server 源代码： 
01.TcpClient ConnectedClient = ServerListener.AcceptTcpClient(); 
2 
03.NetworkStream netStream = ConnectedClient.GetStream(); 
4 
05.BinaryReader br = new BinaryReader(netStream); 
6 
07.byte[] tempByte = new byte[8192]; 
8 
09.tempByte = br.ReadBytes(8192); 
10 
11.string RecevieString = System.Text.Encoding.Default.GetString(tempByte); 
对上边的源代码做一些介绍： 
通过TcpListener获得的Client信息创建一个TcpClient对象。 
1.TcpClient ConnectedClient = ServerListener.AcceptTcpClient(); 
从TcpClient对象中提取NetworkStream对象，其目的是建立Socket数据流。 
1.NetworkStream netStream = ConnectedClient.GetStream(); 
建立BinaryReader对象为刚刚创建的NetworkStream提供一个接口，借助于这个接口可以非常方便的读取信息。 
1.BinaryReader br = new BinaryReader(netStream); 
读取Stream中的byte array数据，转化为string字符串。 
1.byte[] tempByte = new byte[8192]; 
2 
3.tempByte = br.ReadBytes(8192); 
4 
5.string RecevieString = System.Text.Encoding.Default.GetString(tempByte); 
Android Client 源代码： 
这个Android Client所应用的方法与第一个例子中有很大区别，请各位特别注意（通过本人的反复验证，这个算是Java 与CSharp间可行的通讯方法）。 
1.String message = "AndroidRes,Where is my Pig (Android)?"; 
2.Socket socket = new Socket("192.168.0.149", 51706); 
3.OutputStream ops=socket.getOutputStream(); 
4.DataOutputStream dos=new DataOutputStream(ops); 
5.dos.write(message.getBytes()); 
6.dos.close(); 
对上边的源代码做一些介绍： 
同样是建立Socket对象。 
1.Socket socket = new Socket("192.168.0.149", 51706); 
根据Socket对象创建OutputStream对象，根据名称可以很容易的理解这个对象的目的，它是建立一个输出的NetworkStream。 
1.OutputStream ops=socket.getOutputStream(); 
DataOutputStream类似于BinaryWriter，是为OutputStream建立一个接口，可以通过这个接口向Socket另外一端写数据。 
1.DataOutputStream dos=new DataOutputStream(ops); 
最后这两行代码是实施写的过程。 
1.dos.write(message.getBytes()); 
2.dos.close(); 
以上就是本站为大家提供的两种Android与PC间通讯的方式（其中第一个方法来自Anddev.org，这里要特别指出)。 
如果你有更巧妙或者更方便的方法，希望能与大家共同分享！ 
注意需要在AndroidManifest.xml中加入（权限开通） 
<uses-permission android:name="android.permission.INTERNET" /> 
