
# java网络编程三要素 - 李昆鹏的博客 - CSDN博客


2018年04月20日 23:36:23[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：202


-------------------------------------------------java网络编程三要素----------------------------------------------

**计算机网络**
是指将地理位置不同的具有独立功能的多台计算机以及外部设备，通过通信线路连接起来，
在网路操作系统，网络管理软件以及通信的协议的管理下，实现资源共享和信息传递的计算机系统
------------------------------
**网络编程**
就是用来实现网络互通的不同计算机运行程序之间进行数据的交换的编程。
-------------------------------
![](https://img-blog.csdn.net/2018042023325393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180420233318231?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**带dos命令窗口中输入ipconfig查看网络信息**
![](https://img-blog.csdn.net/20180420233328657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**-----------------------------------------------------------------------------------------------**
**网络通信的三大要素**
**IP是第一要素**
![](https://img-blog.csdn.net/20180420233346621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180420233400841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**第二要素是端口号  查看端口netstat -ano**
![](https://img-blog.csdn.net/20180420233414431?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180420233542826?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**第三要素 传输的协议**
![](https://img-blog.csdn.net/20180420233556864?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**常见的有两种协议：**
**TCP和UDP协议**
**UDP****：**
**将数据源和目的地封装到数据包中，不需要建立连接。**
**每个数据包的大小限制在64K****。**
**因为不建立连接，所以安全性差，速度快。**
**TCP****：**
**建立连接形成传输数据的通道，然后再传输数据，通过三次握手。**
**安全性高，效率低。**


