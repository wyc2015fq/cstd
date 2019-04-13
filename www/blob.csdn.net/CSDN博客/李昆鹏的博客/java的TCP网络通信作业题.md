
# java的TCP网络通信作业题 - 李昆鹏的博客 - CSDN博客


2018年04月21日 20:26:43[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：58标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


------------------------------------java的TCP网络通信作业题------------------------------------------
**本机IP127.0.01，只能进行本地访问。**
**先有服务端，再有客服端**

**题目：用TCP做相互聊天的程序**
**聊天必须要有服务端，和客服端**
**首先创建服务端**
**public****class**TCPServer {
/**
*服务端的端口
*/
**private****int**port;

**public**TCPServer(**int**port) {
**super**();
**this**.port=port;
Receiver=**new**Receive();
Threadt=**new**Thread(r);
t.start();
}

**class**Receive**implements**Runnable{
@Override
**public****void**run() {
//创建服务端的Socket
ServerSocketss=**null**;
Sockets=**null**;
BufferedReaderbr=**null**;
**try**{
ss=**new**ServerSocket(port);
System.**out**.println(port+"服务器已经启动");
//接收Socket
s=ss.accept();
//获得客服端的IP
InetAddressia=s.getInetAddress();
//获得输入流
InputStreamin=s.getInputStream();
br=**new**BufferedReader(**new**InputStreamReader(in));
Stringline=**null**;
**while**((line=br.readLine()) !=**null**) {
System.**out**.println(ia.getHostAddress() +"说："+line);
**if**("exit".equals(line)) {
System.**out**.println("服务端关闭");
**break**;
}
}
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(br!=**null**)
br.close();
**if**(s!=**null**)
s.close();
**if**(ss!=**null**)
ss.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
**public****static****void**main(String[]args) {
**new**TCPServer(10003);
}
}

**-------------------------------------------------------**
**创建客服端**
**public****class**TCPClient {
**private**Stringip;
**private****int**port;
**public**TCPClient(Stringip,**int**port) {
**super**();
**this**.ip=ip;
**this**.port=port;
Senders=**new**Sender();
Threadt=**new**Thread(s);
t.start();
}
**class**Sender**implements**Runnable{
@Override
**public****void**run() {
Sockets=**null**;
BufferedReaderbr=**null**;
BufferedWriterbw=**null**;
**try**{
s=**new**Socket(ip,port);
br=**new**BufferedReader(**new**InputStreamReader(System.**in**));
//获得输出通道
OutputStreamos=s.getOutputStream();
bw=**new**BufferedWriter(**new**OutputStreamWriter(os));
Stringline=**null**;
**while**((line=br.readLine())!=**null**) {
System.**out**.println("请说话：");
**if**("exit".equals(line)) {
System.**out**.println("客服端关闭");
**break**;
}
System.**out**.println("我说：/r/n"+line);
bw.write(line);
bw.newLine();
bw.flush();
}
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(bw!=**null**)
bw.close();
**if**(br!=**null**)
br.close();
**if**(s!=**null**)
s.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}
**public****static****void**main(String[]args) {
**new**TCPClient("192.168.1.104", 10003);
}
}


