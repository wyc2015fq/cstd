
# java用UDP作简易聊天室 - 李昆鹏的博客 - CSDN博客


2018年04月21日 13:50:36[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：620


-------------------------------------------------java用UDP作简易聊天室----------------------------------------------------
/**
*服务端
***@author**李昆鹏
*
*/
**public****class**UDPServer**implements**Runnable{
//指定端口
**private****int**port;
**public****int**getPort() {
**return**port;
}
**public****void**setPort(**int**port) {
**this**.port=port;
}
**public**UDPServer(**int**port) {
**super**();
**this**.port=port;
}
@Override
**public****void**run() {
DatagramSocketds=**null**;
**try**{
//创建UDP服务端的对象，必须指定端口
//端口最好指定在一万以上，因为八千之前的端口很多都被占用了
ds=**new**DatagramSocket(port);
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
System.**out**.println(ia.getHostAddress()+"说:\r\n"+data);
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

-----------------------------------------------
/**
*客服端
***@author**李昆鹏
*
*/
**public****class**UDPClient**implements**Runnable {
//发送目标的IP
**private**Stringip;
**public**UDPClient(Stringip,**int**port) {
**super**();
**this**.ip=ip;
**this**.port=port;
}

//发送端口
**private****int**port;

@Override
**public****void**run() {
DatagramSocketds=**null**;
BufferedReaderbr=**null**;
**try**{
//创建控制台的输入流对象
br=**new**BufferedReader(**new**InputStreamReader(System.**in**));
//创建客服端端接收对象
ds=**new**DatagramSocket();
System.**out**.println("已经接入"+ip);
**while**(**true**) {
System.**out**.println("请输入你要发送的内容：");
//读取控制台输入的数据并且转换成字节数组
**byte**[]bs=br.readLine().getBytes();
//创建要发送的目的地的IP对象
InetAddressia= InetAddress.*getByName*(ip);
//指定数据包
//第一个参数是打包的字节数组，第二个参数是要打包的字节长度
//第三个参数是要发送的IP对象，第四个参数是要发送的服务端
DatagramPacketdp=**new**DatagramPacket(bs,bs.length,ia,port);
//发送
ds.send(dp);
System.**out**.println("我说:\r\n"+**new**String(bs,0,bs.length));
//退出程序
**if**("exit".equals(**new**String(bs,0,bs.length))) {
System.**out**.println("客服端已退出");
**break**;
}
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

-----------------------------------------------------------
/**
*聊天室
***@author**李昆鹏
*
*/
**public****class**ChatRoom {
**public****static****void**main(String[]args) {
//创建接收端对象的线程的实现
UDPClientuc=**new**UDPClient("192.168.1.104", 10000);
//创建服务端
UDPServerus=**new**UDPServer(10001);
//发送端的线程
Threadt=**new**Thread(uc);
//接收端的线程
Threadt1=**new**Thread(us);
//启动线程
t.start();
t1.start();
}
}


