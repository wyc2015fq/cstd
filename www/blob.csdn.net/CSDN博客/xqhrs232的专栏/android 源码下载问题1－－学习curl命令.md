# android 源码下载问题1－－学习curl命令 - xqhrs232的专栏 - CSDN博客
2011年03月22日 17:35:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1668标签：[android																[下载工具																[服务器																[多线程																[user																[gtk](https://so.csdn.net/so/search/s.do?q=gtk&t=blog)](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=下载工具&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.163.com/zmhot88@126/blog/static/1698466472010830538479/](http://blog.163.com/zmhot88@126/blog/static/1698466472010830538479/)
[cURL - 命令行下的下载工具](http://www.cnblogs.com/Tealcwu/archive/2010/04/23/1718309.html) ::[http://www.cnblogs.com/Tealcwu/archive/2010/04/23/1718309.html](http://www.cnblogs.com/Tealcwu/archive/2010/04/23/1718309.html)
之前android repo是这样的：
./repo: line 1: !DOCTYPE: No such file or directory
./repo: line 2: syntax error near unexpected token `<'
'/repo: line 2: `<HTML><HEAD><TITLE>Error Message</TITLE>
总是这样，不知道理由，网上也找不到，难道全中国就我一个人出这种错误
总算知道原因了，repo文件错误，因为我在公司上网用的是代理，所以curl下载的命令是不对的，应该这样用：
curl -U user:passwd -x 10.1.27.10:1022 http://android.git.kernel.org/repo >~/bin/repo
转来的文章：
**学习curl命令**
Curl也是Linux下不错的命令行下载工具，小巧、高速，唯一的缺点是不支持多线程下载。在http://curl.haxx.se /download/curl-7.14.0.tar.gz下载最新版本。下载后便可使用如下命令编译安装：
    #tar zxvf curl-7.14.0.tar.gz
    #cd curl-7.14.0/
    #./configure
    #make
    #make test
    #make install
Curl使用格式如下：
   #curl [选项][下载地址] Curl典型下载如下：
   #curl -O http://10.1.27.10/~kennycx/tools/lumaqq_2004-linux_gtk2_x86_with_jre.tar.gz
    使用Curl下载一个文件并保存到当前目录。此外，Curl虽然不支持多线程下载，但它可同时下载多个文件或下载文件的某一部分，可使用如下命令实现：
   #curl -r 0-199 http://www.netscape.com/ 获得文件的前200 bytes。 
    对于常用的代理下载Curl也可轻松实现，具体操作如下：
   #curl -x 10.1.27.10:1022 ftp://ftp.funet.fi/README 
    使用代理地址为10.1.27.10端口为1022的代理服务器下载一个文件。
   #curl -U user:passwd -x 10.1.27.10:1022 ftp://ftp.funet.fi/README 
    如果代理服务器需要特别的验证，则需要在user:passwd处输入合法的帐号和密码。 
------------
使用 cURL 度量 Web 站点的响应时间
$curl -o /dev/null -s -w %{time_connect}:%{time_starttransfer}:%{time_total} http://www.chinaunix.net
0.081:0.272:0.779
通过 -o 参数发送到 /dev/null。 -s 参数去掉所有状态信息。-w参数让 curl 写出列出的计时器的状态信息：
time_connect     建立到服务器的 TCP 连接所用的时间
time_starttransfer     在发出请求之后，Web 服务器返回数据的第一个字节所用的时间
time_total         完成请求所用的时间
这些计时器都相对于事务的起始时间，甚至要先于 Domain Name Service（DNS）查询。
因此，在发出请求之后，Web 服务器处理请求并开始发回数据所用的时间是 0.272 - 0.081 = 0.191 秒。
客户机从服务器下载数据所用的时间是 0.779 - 0.272 = 0.507 秒。

