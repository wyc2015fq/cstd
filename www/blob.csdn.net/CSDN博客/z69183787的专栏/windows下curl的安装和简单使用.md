# windows下curl的安装和简单使用 - z69183787的专栏 - CSDN博客
2015年06月27日 13:05:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1629
curl是利用URL语法在命令行方式下工作的开源文件传输工具。它支持很多协议：FTP, FTPS, HTTP, HTTPS, GOPHER, TELNET, DICT, FILE 以及 LDAP。
一.windows下curl的安装
1.      到这里 [http://curl.haxx.se/download/](http://curl.haxx.se/download/) 下载文件curl-7.17.1-win32-ssl.zip，如果不需要使用ssl的话，可以直接下载curl-7.17.1-win32-nossl.zip，并且直接就可以使用。为了便于以后请求https格式的网址，我们最好使用带ssl功能的。
2.      在Windows下为了使用带ssl的curl命令，我们得安装OpenSSL，到[http://www.slproweb.com/products/Win32OpenSSL.html](http://www.slproweb.com/products/Win32OpenSSL.html) 这里下载Win32OpenSSL_Light-1_0_1h.exe,安装即可。
3.      接下来打开cmd，切换到curl目录下，就可以直接使用curl命令了。
二.curl基本命令行
1.查看网页源码，以www.sina.com为例
![](http://img0.tuicool.com/Ffqmey.png)
要想保存这个文件就使用
curl -o [文件名] www.sina.com
2.只显示头信息
![](http://img0.tuicool.com/6FRb2ye.png)
想同时显示头信息和源码就使用
curl -i www.sina.com 结果如图
![](http://img0.tuicool.com/iyyYzi2.png)
3.显示通信过程（ip地址，端口号）
![](http://img2.tuicool.com/6jEBR3.png)
想看更详细的就使用
curl --trace output.txt www.sina.com
打开output.txt文件就有相关信息
4.发送表单信息
发送表单信息有GET和POST两种方法。GET方法相对简单，只要把数据附在网址后面就行。
curl example.com/form.cgi?data=xxx
POST方法必须把数据和网址分开，curl就要用到--data参数。
curl --data "data=xxx" example.com/form.cgi
5.文件上传
假如我们下了个这样的表单
<form method="POST" enctype='multipart/form-data' action="upload.cgi">
<input type=file name=upload>
<input type=submit name=press value="OK">
</form>
就可以使用
curl --form upload=@localfilename --form press=OK [URL]
6.文件下载
假如下载一张图片
![](http://img0.tuicool.com/Evuyui.png)
想要按照服务器上的文件名自动存在本地只要把小写的o改成大写的O，去掉自己加的文件名就行了
如果下载中途掉线了，还可以使用命令续传
curl -c -O http://cgi2.tky.3wb.ne.jp/~zzh/screen1.JPG
7.User Agent字段
这个字段是用来表示客户端的设备信息。服务器有时会根据这个字段，针对不同设备，返回不同格式的网页，比如手机版和桌面版。
curl --user-agent "[User Agent]" [URL]
8.cookie
使用--cookie参数，可以让curl发送cookie。
curl --cookie "name=xxx" www.example.com
9.增加头信息
有时需要在http request之中，自行增加一个头信息。--header参数就可以起到这个作用。
curl --header "Content-Type:application/json" http://example.com
