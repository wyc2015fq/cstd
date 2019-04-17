# Wireshark数据抓包分析之FTP协议 - zhusongziye的博客 - CSDN博客





2018年04月17日 20:09:58[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：2471








实验步骤一配置FTP服务器，并在测试者机器上登录FTP服务器

在局域网环境中，我们使用一个小工具来（Quick Easy FTP Server）实现FTP服务器。

配置Quick Easy FTP Server软件

双击桌面的Quick Easy FTP Server，如下图

![](https://img-blog.csdn.net/20180417200002896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





如上图，可以创建匿名的，但是匿名就没有密码，这里我们创建一个，下一步

![](https://img-blog.csdn.net/20180417200023238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





输入密码，这里随意，记住即可，后面客户端登录会用到。下一步

![](https://img-blog.csdn.net/20180417200057832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





选择一个FTP服务器的目录，我们选择C盘下的FTP目录（文件夹下面放入一些数据）。这里目录必须存在。下一步

![](https://img-blog.csdn.net/20180417200118635?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



权限我们选择“允许下载”和“允许上传”即可。这两个是比较常见的，其他的配置默认。点击下一步

![](https://img-blog.csdn.net/20180417200146321?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


这样我们就配置好QuickEasy FTP Server，下面，我们选择点击开启按钮（左上角的绿色按钮，即左上角绿色问号最左边的按钮），开启之后，状态变化，如下；

![](https://img-blog.csdn.net/20180417200201696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





这样我们就配置完Quick Easy FTP Server。

这里介绍一种容易出错的地方，就是开启时提示失败，可能是默认端口21被占用了，这里给出两种办法：

1改变默认端口，21改为数字较大的端口；

2使用netstat –ano| findstr “21”，查看那个进程在占用21端口，然后在任务管理器中关闭占用端口的程序即可。这里推荐方法1。



获取FTP的控制链接数据和数据链接数据

下面我们在测试者机器上，打开Wireshark抓包工具，过滤条件输入ip.addr == 10.1.1.33，这里可以通过cmd的命令行去登录FTP服务器，也可以通过浏览器登录，我们这里，为了熟悉FTP的常用命令，使用cmd的命令行登录，如下

![](https://img-blog.csdn.net/20180417200503428?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180417200454146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




上面的信息就是登录FTP服务器后，进行了一个上传和下载文件的操作，此处我们返回Wireshark界面停止抓包,保存，截图如下:

![](https://img-blog.csdn.net/20180417200515744?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


接下来，我们就详细的对上面的步骤，进行分析。





实验步骤二 分析FTP协议数据包

在FTP工作流程中使用控制链接和数据连接两种方式来实现数据传输，下面我们来分析这两种包的详细信息。



分析控制链接的数据

FTP的控制连接用于传送用户名、密码及设置传输方式等控制信息，下面以保存的ftp.pcapng捕获文件为例，分析FTP协议控制连接数据包。

![](https://img-blog.csdn.net/20180417200537299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


上图中显示了所有数据包的Info列，在这里可以看到ftp传输的所有信息，因为FTP是明文形式传输数据包的，所有我们在InFo列里能看到登录FTP服务器的用户名、密码和传输文件等。在上图中，登录FTP服务器的用户名为Hetian，密码为123456，下载了文件cat.jpg及上传了文件Tulip.jpg。这里显示的都是成功，如果传输过程中出差的话，会返回相应的应答码。

在捕获的FTP数据包中，USER，PASS，CWD，RETR和STOR等都是控制连接使用的控制命令。这些控制命令在包详细信息中，显示的格式都相同。这里以控制用户信息的命令为例，分析包的详细信息。在ftp.pcapng捕获文件中，捕获的用户信息如下：

![](https://img-blog.csdn.net/20180417200704911?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


从该界面可以看到登录FTP服务器时，使用的控制命令是USER和PASS。根据这两个命令，可以看到登录的账号为Hetian，密码为123456.这两个包的详细信息如下

用户名包详细信息

File Transfer Protocol (FTP)

       USERHetian\r\n

              Requestcommand: USER

              Requestarg: Hetian

从上面的信息中，可以看到该包使用了FTP协议，输入的用户名为Hetian，请求的命令是USER，请求参数为Hetian。

密码的详细信息

FileTransfer Protocol (FTP)

       PASS123456\r\n

       Requestcommand: PASS

       Requestarg: 123456

从上面的信息中，可以看到输入的密码为123456，请求的命令是PASS，请求参数为123456。

其余的CMD等命令，学习者自己查看下。



分析数据连接的数据

数据连接用于传送文件数据，也就是通过FTP服务器进行上传和下载文件。下面以捕获的文件为例，分析数据连接的数据。

![](https://img-blog.csdn.net/20180417200719621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


上面的图中，控制命令RETR和PORT的数据分别为上传和下载的数据包。这里我们以下载文件为例（上传相同原理），点击上面的第811帧数据，右键，

![](https://img-blog.csdn.net/20180417200730634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


选择Follow TCPStream选择，

![](https://img-blog.csdn.net/20180417200743881?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


上面显示了所有FTP的传输信息，如果想要查看传输的数据，就要将这些信息去掉。关闭上面的窗口，在Wireshark中，你会发现过滤条件被修改为如下

![](https://img-blog.csdn.net/20180417200806556?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


最简单的方法就是，在前面加一个！，就能达到去掉的效果。如下图：

![](https://img-blog.csdn.net/20180417200815146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


接下来就是要找到我们下载的文件。如何能快速的找到下载的文件呢，这里给出两种方法，大家自行选择：

通过协议的字段去分析：你会发现，去掉了FTP的控制连接数据，显示的是TCP协议的数据，在TCP协议中，PUSH字段表示推送数据，我们就可以在Info中找到PUSH字段，间接找到想要的文件。

我们知道下载的文件是JPG格式，所以也就知道二进制的表示为JFIF（exe格式用二进制分析器打开是MZ的道理是一样的），所以就可以通过Wireshark自带的搜索，快速找到文件所在的帧数。搜索的快捷键是Ctrl+F.如下图

![](https://img-blog.csdn.net/20180417200826490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


这样也能找到相应的帧信息。我们在找到帧信息之后（814帧或者820均可），通过右键，Follow TCPStream，可以看到数据的信息，其中的JFIF表示了文件格式为jpg，如下：

![](https://img-blog.csdn.net/20180417200843497?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


我们点击上图的Save as，输入名字cat.jpg即可。

![](https://img-blog.csdn.net/20180417200854688?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


打开保存的文件，即我们下载的图片

![](https://img-blog.csdn.net/20180417200910275?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


这个时候关闭Follow TCP Stream弹出的窗口，Wireshark显示的信息如下：

![](https://img-blog.csdn.net/20180417200923999?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


上面的数据包显示了传输cat.jpg文件的所有非FTP控制数据包，在该过程中，明细可以看到，经理了TCP的三次握手和四次断开连接。请学习者根据上面的方法，去寻找找到上传的文件信息。






