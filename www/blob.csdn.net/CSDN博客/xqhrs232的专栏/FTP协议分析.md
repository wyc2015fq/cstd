# FTP协议分析 - xqhrs232的专栏 - CSDN博客
2017年08月15日 11:24:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：183
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/chongshangyunxiao321/article/details/51095390](http://blog.csdn.net/chongshangyunxiao321/article/details/51095390)
相关文章
1、在Linux下实现一个文件传输协议（FTP）的简单实现----[http://blog.sina.com.cn/s/blog_670d5f330100jyjw.html](http://blog.sina.com.cn/s/blog_670d5f330100jyjw.html)
2、
# [FTP协议的分析和扩展](http://blog.csdn.net/xjbclz/article/details/51901734)----[http://blog.csdn.net/xjbclz/article/details/51901734](http://blog.csdn.net/xjbclz/article/details/51901734)
一、实验名称
FTP协议分析
二、实验目标
1、理解FTP协议的工作原理； 
2、了解FTP协议的常用命令； 
3、了解应用层协议与传输层协议的关系
三、实验原理
FTP（File Transfer Protocal），是文件传输协议的简称。FTP使得主机间可以共享文件，用于控制Internet上文件的双向传输。它是一个客户机/服务器系统。用户通过一个支持FTP协议的客户机程序，连接到在远程主机上的FTP服务器程序。用户通过客户机程序向服务器程序发出命令，服务器程序执行用户所发出的命令，并将执行的结果返回到客户机。 当FTP客户端与服务器建立FTP连接时，将与服务器上的两个端口建立联系：端口20和21。FTP使用不同的端口号传输不同的内容，会建立不同的TCP连接。首先，使用
 TCP 生成一个虚拟连接用于控制信息，然后再生成一个单独的 TCP 连接用于数据传输。
FTP的工作过程: FTP使用2个TCP端口，一个数据端口和一个命令端口（也可叫做控制端口）。通常来说这两个端口是21——命令端口和20——数据端口，但根据FTP工作在主动模式还是被动模式，21和20端口的使用方法略有不同。 主动模式的FTP是这样的：客户机从一个任意的非特权端口N（N≥1024），连接到FTP服务器的命令端口，也就是21端口，建立一个控制连接。这个连接用于传递客户端的命令和服务器端对命令的响应，生存期是整个FTP会话时间。
如果期间需要传输文件和其它数据，例如：目录列表等，客户端就需要建立数据连接了。这种连接在需要数据传输时建立，而一旦数据传输完毕就关闭，整个FTP期间可能会建立多次。在主动模式下，建立数据连接时，客户端会开始监听端口N+1，并发送FTP命令“port N+1”到FTP服务器。接着服务器会从它自己的数据端口（20）连接到客户端指定的数据端口（N+1），开始进行数据传输。
四、实验步骤
1. 打开Wireshark，选择工具栏上的“Capture”->“interfaces选择网关”，如图： 
![这里写图片描述](https://img-blog.csdn.net/20160408121432883)
2、然后在Wireshark，选择工具栏上的“Capture”->“optoins”选择过滤器，并在capture filter中输入 tcp port 20 or tcp port 21(表示要抓tcp的包)，如图： 
![这里写图片描述](https://img-blog.csdn.net/20160408121505883)
3.打开命令提示符，键入CMD后，输入ping 192.168.1.104发现能ping通 
![这里写图片描述](https://img-blog.csdn.net/20160408121555086)
4.ping 通后输入ftp 192.168.1.104 出现如下界面 
![这里写图片描述](https://img-blog.csdn.net/20160408121629807)
此时抓到5个包 
前面三帧是客户端与主机的三次握手 
![这里写图片描述](https://img-blog.csdn.net/20160408121701993)
第四帧 
![这里写图片描述](https://img-blog.csdn.net/20160408121729933)
第四帧是服务器端的响应报文，并以ascii码方式明文传输数据，这一帧表明控制连接完成。 
第五帧是客户端向服务器端发的确认 
第六、七、八、九、十，十一帧是分别是：客户端输入用户名；服务器端确认并提示输密码；客户端确认；客户端发送密码；服务器端确认密码正确；客户端确认收到。 
其中，密码和用户名都是明文的，如下图 
![这里写图片描述](https://img-blog.csdn.net/20160408121809634)
第十二帧客户端向服务器端请求数据连接，并打开端口号5001 
![这里写图片描述](https://img-blog.csdn.net/20160408121854341)
第十三帧服务器端确认，并告诉客户端打开200端口 
第十四帧客户端确认并请求能访问的文件目录 
![这里写图片描述](https://img-blog.csdn.net/20160408121927045)
第十五、十六、十七帧是数据连接的创建（数据连接创建） 
![这里写图片描述](https://img-blog.csdn.net/20160408122004588)
第十八帧服务器端回到控制连接向第14帧确认，并表示请求的文件目录将以ASCII码的方式发送。 
第十九帧服务器端向客户端发送传输了195个比特。 
第二十帧服务器端向客户端发送完成信息并要求断开数据连接。 
![这里写图片描述](https://img-blog.csdn.net/20160408122036546)
第二十一帧客户端向服务器端确认 
第二十二帧返回控制连接,服务器端断开数据连接； 
第二十三帧分别是客户端向服务器端确认收到； 
第二十四帧是客户端断开数据连接。 
第二十五帧是服务器端对客户端断开连接的确认。（至此，数据连接断开） 
![这里写图片描述](https://img-blog.csdn.net/20160408122109563)
第二十六帧客户端向服务器端请求数据连接，并打开端口号5002（数据连接再次开始建立） 
![这里写图片描述](https://img-blog.csdn.net/20160408122135470)
第二十七帧服务器端确认，并告诉客户端打开200端口 
第二十八帧客户端请求从服务器端下载2.txt.txt 
![这里写图片描述](https://img-blog.csdn.net/20160408122201401)
第三十二帧回到数据连接，向第28帧确认，并告诉客户端文件将以ASCII码的方式发送。 
![这里写图片描述](https://img-blog.csdn.net/20160408122239737)
第三十三帧是服务器端将2.txt.txt以ASCII码的方式传送到客户端 
![这里写图片描述](https://img-blog.csdn.net/20160408122311629)
第三十四帧表示服务器端发完了，要断开数据连接
![这里写图片描述](https://img-blog.csdn.net/20160408122342636)
第三十五帧客户端向服务器端确认 
第三十六帧返回控制连接,服务器端断开数据连接； 
第三十七帧分别是客户端向服务器端确认收到； 
第三十八帧是客户端断开数据连接。 
第三十九帧是服务器端对客户端断开连接的确认。（至此，数据连接再次断开） 
![这里写图片描述](https://img-blog.csdn.net/20160408122411652)
由于忘记退出（EXIT）所以保存下来的包没有断开控制连接的包，但后面应该有4帧断开。 
FTP协议分析结束。
[](http://blog.csdn.net/chongshangyunxiao321/article/details/51095390#)[](http://blog.csdn.net/chongshangyunxiao321/article/details/51095390#)[](http://blog.csdn.net/chongshangyunxiao321/article/details/51095390#)[](http://blog.csdn.net/chongshangyunxiao321/article/details/51095390#)[](http://blog.csdn.net/chongshangyunxiao321/article/details/51095390#)[](http://blog.csdn.net/chongshangyunxiao321/article/details/51095390#)

