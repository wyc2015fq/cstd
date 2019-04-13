
# java入门教程-12.3Java 套接字(Socket) -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:47:25[seven-soft](https://me.csdn.net/softn)阅读数：564个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



网络应用模式主要有：主机/终端模式：集中计算，集中管理；
客户机/服务器（Client/Server,简称C/S）模式：分布计算，分布管理；
浏览器/服务器模式：利用Internet跨平台。
www（万维网）就是建立在客户机/服务器模式上，以HTML语言和HTTP协议为基础，能够提供各种Internet服务的信息浏览系统。网络信息放在主机的不同位置，www服务器利用超文本链路链接各项信息。www客户机（浏览器Brower）负责与服务器建立联系，向服务器发送请求，处理HTML超媒体，提供图形用户界面（GUI），显示信息等。
在客户机/服务器工作模式中，在Server端，要准备接受多个Client端计算机的通信。为此，除用IP地址标识Internet上的计算机之外，另还引入端口号，用端口号标识正在Server端后台服务的线程。端口号与IP地址的组合称为网络套接字（socket）。
Java语言在实现C/S模式中，套接字分为两类：在Server端，ServerSocket类支持底层的网络通信；
在Client端，Socket类支持网络的底层通信。
Server机通过端口（总线I/O地址）提供面向Client机的服务；Server机在它的几个不同端口分别同时提供几种不同的服务。Client接入Server的某一端口，通过这个端口提请Server机为其服务。规定：端口号0~1023供系统专用。例如，HTTP协议在端口80，telnet协议在端口23。端口1024~65535供应用程序使用。
当Client程序和Server程序需要通信时，可以用Socket类建立套接字连接。套接字连接可想象为一个电话呼叫：最初是Client程序建立呼叫，Server程序监听；呼叫完成后，任何一方都可以随时讲话。
双方实现通信有流式socket和数据报式socket两种可选方式：流式socket是有连接的通信，即TCP(Transmission Control Protocol)：每次通信前建立连接，通信结束后断开连接。特点是可以保证传输的正确性、可靠性。
数据报式socket是无连接的通信，即UDP(User Datagram Protocol)：将欲传输的数据分成 小包，直接上网发送。无需建立连接和拆除连接，速度快，但无可靠保证。
流式socket在Client程序和Server程序间建立通信的通道。每个socket可以进行读和写两种操作。对于任一端，与对方的通信会话过程是：
建立socket连接，获得输入/输出流，读数据/写数据，通信完成后关闭socket（拆除连接）。流式Socket的通信过程见【例 13-1】。
利用socket的构造方法，可以在客户端建立到服务器的套接字对象：
Socket(String host,int port)：host是服务器的IP地址，port是端口号，这些是预先约定的。
例如，代码：
try{
Socket mySocket = new Socket(“http://www.weixueyuan.net” ,1860);
}catch(IOException e){}
然后，用getInputStream()方法获得输入流，用这个输入流读取服务器放入“线路”的信息；用getOutputStream()方法获得输出流，用这个输出流将信息写入“线路”。
利用ServerSocket的构造方法可以在服务器建立接受客户套接字的服务器套接字对象：
ServerSocket(int port)：指定端口号，创建一个ServerSocket对象。端口号port要与客户呼叫的端口号相同。为此，用以下形式代码：
try{
ServerSocket serverSocket = new ServerSocket(1860);
}catch(IOException e){}
服务器端程序在指定的端口监听，当收到Client程序发出的服务请求时，创建一个套接字对象与该端口对应的Client程序通信。例如，执行上述建立服务器套接字对象的代码，确立了对象serverSocket后，就可能它使用accept()方法，得到Socket对象，接收Client程序来自套接字mySocket的信息。如以下代码所示：
try{
Socket sc = serverSocket.accept();//ac是一个Socket对象
}catch(IOException e){}
要撤销服务，可以关闭Socket对象sc：
sc.close();
【例 13-3】C/S模式中的Client端应用程序。这是一个Client端的流式Socket通信的简单实例，代码说明Client端程序的编写方法。例中，Client程序向服务器主机的端口4441提出请求，连接建立后完成对服务器的读写（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/13-3.txt)）。
【例 13-4】与例10.3 Client端应用程序对应的Server端应用程序。程序在4441端口监听，当检测到有客户机请求时，产生一个内为“客户，你好，我是服务器”的字符串输出到客户端（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/13-4.txt)）。
为了充分发挥计算机的平行工作能力，可以把套接字连接工作让一个线程完成。当客户端要请求服务器给予服务，或当服务器端接收到一个客户的服务请求，就启动一个专门完成信息通信的线程，在该线程中创建输入输出流，并完成客户端与服务器端的信息交流。
【例 13-5】 将套接字连接工作置于线程的客户端小应用程序。界面在有一个发送信息按纽、一个文本框和一个文本区。客户端应用程序首先与服务器建立套接字连接。使用数据输入流in反复读取服务器放入线路里的信息，将收到的信息在文本区中显示。婐志取的信息是“结束”，则关闭套接字连接，并结束程序。用户也可在文本框输入信息，并按发送信息按钮，则客户端程序利用数据输出流out,将文本框中的内容发送给服务器（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/13-5.txt)）。
【例 13-6】对应例10.5客户端小应用程序的服务器端小应用程序。程序以端4441建立与客户端的套接字连接，服务器端收到客户端的申请后，以客户的套接字建立一个线程，并启动。如果没有客户申请，则继续监听客户的申请。线程按客户的套接字建立输入数据流in和输数据流out。线程利用in读取客户放入线路里的信息。如果接受的信息是“结束”，则服务器回复“结束”后关闭套接字连接；否则回复：“我是服务器你对我说“，以及服务器接收到的信息（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/13-6.txt)）。

