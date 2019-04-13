
# java入门教程-12.2Java网络编程之统一资源定位符URL -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:46:27[seven-soft](https://me.csdn.net/softn)阅读数：268


统一资源定位符URL(Uniform Resource Locator)是www客户机访问Internet时用来标识资源的名字和地址。超文本链路由统一资源定位符URL维持。URL的格式是：
<METHOD>://<HOSTNAME:PORT>/<PATH>/<FILE>
其中：Method是传输协议：HOSTNAME是文档和服务器所在的Internet主机名（域名系统中DNS中的点地址）;PORT是服务端口号（可省略）；PATH是路径名，FILE是文件名。例如：
http://www.weixueyuan.net/(http是协议名，www.weixueyuan.net是主机名)
http://www.weixueyuan.net/view/6079.html (www.weixueyuan.net是主机名，view/6079.html是文件路径和文件名)
## URL类
Java.net包有URL类，一个URL对象可以表示一个网络资源。程序利用URL对象能实现Internet寻址、网络资源的定位连接、在客户机与服务器之间直接访问等。URL类的构造方法是
URL(String s)
其中，s指出网络中的一个资源。
利用URL对象访问网上资源的方法是：先创建URL对象，如以下代码所示：
URL myURL;
try {
myURL = new URL(“http://www.weixueyuan.net:80/”);
}catch(MalformedURLException e){
System.out.println(“有错的URL:”+url+e);
}
因创建URL对象可能会产生MalformedURLException异常。所以，创建URL对象的代码应出现在try…catch语句块中，以便能捕捉网址错误异常。
## URLConnection类
要接收和发关信息还要用URLConnection类，程序获得一个URLConnection对象，相当于完成对指定URL的一个HTTP连接。以下是示意获得URLConnection对象的代码。
URL mu = new URL(“http://www.sun.com/”);//先要创建一个URL对象
URLConnection muC = mu.openConnection();//获得URLConnection对象
上述代码说明，先要创建一个URL对象，然后利用URL对象的openConnection()方法，从系统获得一个URLConnection对象。程序有了URLConnection对象后，就可使用URLConnection类提供的以下方法获得流对象和实现网络连接：getOutputStream()：获得向远程主机发送信息的OutputStream流对象；
getInputStream()：获得从远程主机获取信息的InputStream流对象。有了网络连接的输入和输出流，程序就可实现远程通信；
connect()：设置网络连接。
## 信息的发送和接收
发送和接收信息要获得流对象，并由流对象创建输入或输出数据流对象。然后，就可以用流的方法访问网上资源。
参见【例 13-2】程序中的方法readByURL()，该方法说明已知网址读取网页内容的过程。方法利用网址参数创建一个URL对象url，接着利用对象url的openConnect()方法，获得URLConnection对象tc，用对象tc的connect()方法建立网络连接，接着获得网络连接的InputStreamReader类对象in，将对象in，转化成为BufferedRead对象dis，改为缓冲式输入。最后，用对象dis的readLine()方法完成读取网络文本数据。
如同本地数据流一样，网上资源使用结束后，数据流也应及时关闭。例如，代码
dis.close();
关闭先前代码建立的流dis。
【例 13-2】以数据流方法读取网页内容的应用程序。程序运行时，网址从文本框中读取（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/13-2.txt)）。

