# 64位weblogic11g安装 - z69183787的专栏 - CSDN博客
2014年08月06日 14:46:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9310
我们在64位的服务器上为提高性能要安装64位的weblogic。经常在网上看到有人问，weblogic有64位的么？weblogic需要破解么？
weblogic有专门的64位版本，这里安装的是weblogic11g，也就是10.3.6版本，12c的版本安装应该类似。
weblogic从bea被oracle收购后，不需要破解，就只有授权。什么意思呢？就是说从oracle官网上下载的weblogic就是全功能版本，不管是集群还是其他，功能没有任何限制。但是如果要用于商业环境，必须要向oracle买license，当然可以偷偷的用，那就是盗版，侵权，有一天oracle可以告的破产……。
1、下载64位weblogic，打下这个地址：[http://www.oracle.com/technetwork/middleware/ias/downloads/wls-main-097127.html](http://www.oracle.com/technetwork/middleware/ias/downloads/wls-main-097127.html)，在这里可以看到除了mac
 os X操作系统外，其他系统的64位都是同一个版本，wls1036_generic.jar。如下图，weblogic的下载需要注册一个oracle官网的帐号。
![64位weblogic](http://www.huqiwen.com/wp-content/uploads/2012/05/20120522113548.png)
2、下载64位JDK，我们下载的文件wls1036_generic.jar文件里面不包括JDK，如有可能, 请尽量在Windows/Linux平台下使用JRockit虚拟机，下载地址：[http://www.oracle.com/technetwork/middleware/jrockit/downloads/index.html](http://www.oracle.com/technetwork/middleware/jrockit/downloads/index.html)。通用的64位
 Weblogic 安装程序本身并不绑定任何版本的64位 JVM, 因此请安装第二步下载到的Java安装程序.默认的安装路径(C:\Program Files)必须进行修改, 因为Weblogic和很多Java类库都不能**很好**的支持带有空格的目录名.请确保将JDK安装到不带任何空格的目录下(例如 C:\Java);
然后使用命令行安装，如果配置了环境变理直接使在控制台里面输入下面的命令即可。如果没有配置文件变量，在控制台使用cd进入到jdk的bin目录下面执行如下命令。
//界面安装，一般选择这个方法就可以
**java -jar wls1034_generic.jar  **即可启动安装过程, 默认启动的是图形界面的安装向导.
//控制台安装，这个方法一般用在linux等没有安装桌面环境的服务器上，命令行方式的安装命令用如下方式启动:
**java -jar wls1034_generic.jar -mode=console**
//静默安装用如下方式启动:
**java -jar wls1034_generic.jar -mode=console -silent_xml=/path_to_silent.xml**
[](http://www.huqiwen.com/wp-content/uploads/2012/05/20120522142857.png)
3.现在创建域并启动服务器, 或许会发现如下提示的错误信息: 
<Error> <Socket> <BEA-000438> <Unable to load performance pack. Using Java I/O instead. Please ensure that wlntio.dll is in: ‘C:\O
racle\fmwhome\wlserver_10.3\server\native\win\32;C:\Oracle\fmwhome\wlserver_10.3 
\server\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\Oracle\fmwhome\wlserver_10.3\serve 
r\native\win\32\;C:\Oracle\fmwhome\wlserver_10.3\server\bin;C:\Program~\Java\jdk 
1.6.0_24\jre\bin;C:\\Java\jdk1.6.0_24\bin; ‘ 
解决方法
1) 进入如下目录: <<Weblogic_Home_Directory>>\wlserver_10.3\server\native\win\x64, 复制文件wlntio.dll 并将其粘贴到 C:\Java\bin 目录下 (假设JDK事先已安装至 C:\Java);
2)或者修改系统的PATH变量使其包含<<Weblogic_Home_Directory>>\wlserver_10.3\server\native\win\x64, 此为推荐做法;
4. 启动服务器后, 问题应消失并可在Thread Dump中看到Muxer相关线程.
Linux/Unix下的解决方案:
需找到对应目录下的文件libmuxer.so或者libmuxer.sl, 将其加入系统的PATH或者LD_LIBRARY_PATH环境变量, 并确保WebLogic进程的所有者用户对其有读和执行的权限.
