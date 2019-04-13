
# java中UDP协议的开发步骤 - 李昆鹏的博客 - CSDN博客


2018年04月21日 11:06:17[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：93


--------------------------------------------------java中UDP协议的开发步骤------------------------------------------------------

Socket是网络编程提供的一种机制，通信的两端都要有socket，
网络通信其实就是socket间的通信，数据在两个socket间的io流上通信。
![](https://img-blog.csdn.net/20180421110401555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**UDP通信**
**UDP服务端的步骤**
1.    创建socket服务对象
UDP的服务端的构造器

|DatagramSocket|()
|构造数据报套接字并将其绑定到本地主机上任何可用的端口。
|
|protected
|DatagramSocket|(|DatagramSocketImpl|impl)
|创建带有指定 DatagramSocketImpl 的未绑定数据报套接字。
|
|DatagramSocket|(int port)
|创建数据报套接字并将其绑定到本地主机上的指定端口。
|
|DatagramSocket|(int port,|InetAddress|laddr)
|创建数据报套接字，将其绑定到指定的本地地址。
|
|DatagramSocket|(|SocketAddress|bindaddr)
|创建数据报套接字，将其绑定到指定的本地套接字地址。
|

2.接收数据：**使用数据包的方式来接收，用到了Datagrampacket此类表示数据包。**
|DatagramPacket|(byte[] buf, int length)
|构造 DatagramPacket，用来接收长度为 length  的数据包。
|
|DatagramPacket|(byte[] buf, int length,|InetAddress|address,  int port)
|构造数据报包，用来将长度为 length 的包发送到指定主机上的指定端口号。
|
|DatagramPacket|(byte[] buf, int offset, int length)
|构造 DatagramPacket，用来接收长度为 length  的包，在缓冲区中指定了偏移量。
|
|DatagramPacket|(byte[] buf, int offset, int length,|InetAddress|address,  int port)
|构造数据报包，用来将长度为 length 偏移量为 offset  的包发送到指定主机上的指定端口号。
|
|DatagramPacket|(byte[] buf, int offset, int length,|SocketAddress|address)
|构造数据报包，用来将长度为 length 偏移量为 offset  的包发送到指定主机上的指定端口号。
|
|DatagramPacket|(byte[] buf, int length,|SocketAddress|address)
|构造数据报包，用来将长度为 length 的包发送到指定主机上的指定端口号。
|
**代码示例：**
/**
*服务端
***@author**李昆鹏
*
*/
**public****class**UDPServer {
**public****static****void**main(String[]args) {
DatagramSocketds=**null**;
**try**{
//创建UDP服务端的对象，必须指定端口
//端口最好指定在一万以上，因为八千之前的端口很多都被占用了
ds=**new**DatagramSocket(10000);
//定义接收的字节数组
**byte**[]bs=**new****byte**[1024];
System.**out**.println("服务器已经启动");
**while**(**true**) {
//定义接收数据包
DatagramPacketdp=**new**DatagramPacket(bs,bs.length);
//数据包的接收
ds.receive(dp);
//获得发生端的IP
InetAddressia=dp.getAddress();
//获得数据包中的数据,这个数组的长度是我们自己定义的长度（1024）
**byte**[]bs1=dp.getData();
//或得接收数据的长度(实际接收到数据的长度)
**int**len=dp.getLength();
//组装接收到的数据
Stringdata=**new**String(bs1,0,len);
//退出程序
**if**("exit".equals(data)) {
System.**out**.println("服务端已退出");
**break**;
}
System.**out**.println(ia.getHostAddress()+"发送了"+data);
}
}**catch**(Exceptione) {
e.printStackTrace();
}**finally**{
//关闭服务端
**if**(ds!=**null**)
ds.close();
}
}
}
---------------------------------------------------------------------------------

/**
*客服端
***@author**李昆鹏
*
*/
**public****class**UDPClient {
**public****static****void**main(String[]args) {
DatagramSocketds=**null**;
BufferedReaderbr=**null**;
**try**{
//创建控制台的输入流对象
br=**new**BufferedReader(**new**InputStreamReader(System.**in**));
//创建客服端端接收对象
ds=**new**DatagramSocket();
System.**out**.println("客服端已启动");
**while**(**true**) {
System.**out**.println("请输入你要发送的内容：");
//读取控制台输入的数据并且转换成字节数组
**byte**[]bs=br.readLine().getBytes();
//创建要发送的目的地的IP对象
InetAddressia= InetAddress.*getByName*("192.168.1.104");
//指定数据包
//第一个参数是打包的字节数组，第二个参数是要打包的字节长度
//第三个参数是要发送的IP对象，第四个参数是要发送的服务端
DatagramPacketdp=**new**DatagramPacket(bs,bs.length,ia,10000);
//发送
ds.send(dp);
//退出程序
**if**("exit".equals(**new**String(bs,0,bs.length)))
System.**out**.println("客服端已退出");
**break**;
}
}**catch**(Exceptione) {
e.printStackTrace();
}**finally**{
**if**(ds!=**null**)
ds.close();
**try**{
**if**(br!=**null**)
br.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}


