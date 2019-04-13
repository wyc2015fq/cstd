
# java中TCP通信 - 李昆鹏的博客 - CSDN博客


2018年04月21日 15:24:12[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：298


---------------------------------------java中TCP通信-------------------------------------------
![](https://img-blog.csdn.net/20180421152053272?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**示例代码：**
/**
* TCP服务器
***@author**李昆鹏
*
*/
**public****class**TCPServer {
**public****static****void**main(String[]args) {
//创建服务器的服务套接字
ServerSocketss=**null**;
Sockets=**null**;
BufferedReaderbr=**null**;
**try**{
ss=**new**ServerSocket(10011);
//接收到了一个连接的请求,返回以socket对象
//等待有客服端来建立连接
s=ss.accept();
//获得输入输出流的对象
OutputStreamos=s.getOutputStream();
InputStreamin=s.getInputStream();
br=**new**BufferedReader(**new**InputStreamReader(in));
InetAddressia=s.getInetAddress();
**while**(**true**) {
//读取一行
Stringline=br.readLine();
System.**out**.println(line);
**if**("exit".equals(line)) {
System.**out**.println("程序已经退出");
**break**;
}
//获得客户端的IP地址
System.**out**.println(ia.getHostAddress() +"发送了："+line);
}
}**catch**(IOExceptione) {
e.printStackTrace();
}**finally**{
//先关闭流
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

------------------------------------------------------------
**public****class**TCPClient {
**public****static****void**main(String[]args) {
//创建套接字对象,去请求服务器的ServerSocket
Sockets=**null**;
BufferedWriterwriter=**null**;
BufferedReaderbr=**null**;
**try**{
s=**new**Socket("192.168.1.104",10011);
//获得输出流
OutputStreamout=s.getOutputStream();
//获得从控制台输入的流对象
br=**new**BufferedReader(**new**InputStreamReader(System.**in**));
writer=**new**BufferedWriter(**new**OutputStreamWriter(out));
**while**(**true**) {
System.**out**.println("请输入：");
Stringline=br.readLine();
writer.write(line);
writer.newLine();
writer.flush();
//在退出程序之前一定要先换行再清空缓冲区
**if**("exit".equals(line)) {
System.**out**.println("程序已经退出");
**break**;
}
}
}**catch**(Exceptione) {
e.printStackTrace();
}**finally**{
**try**{
**if**(writer!=**null**)
writer.close();
**if**(s!=**null**)
s.close();
}**catch**(IOExceptione) {
e.printStackTrace();
}
}
}
}


