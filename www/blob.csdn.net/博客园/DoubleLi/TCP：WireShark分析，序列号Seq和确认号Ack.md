# TCP：WireShark分析，序列号Seq和确认号Ack - DoubleLi - 博客园






转载自 [http://blog.csdn.net/a19881029/article/details/38091243](http://blog.csdn.net/a19881029/article/details/38091243)



序列号为当前端成功发送的数据位数，确认号为当前端成功接收的数据位数，SYN标志位和FIN标志位也要占1位

如果你正在读这篇文章，很可能你对TCP“非著名”的“三次握手”或者说“SYN,SYN/ACK,ACK”已经很熟悉了。不幸的是，对很多人来说，对TCP的学习就仅限于此了。尽管年代久远，TCP仍是一个相当复杂并且值得研究的协议。这篇文章的目的是让你能够更加熟练的检查Wireshark中的TCP序列号和确认号



# TCP包的具体内容



 从下图可以看到wireshark捕获到的TCP包中的每个字段。

![](https://img-blog.csdn.net/20151120171935259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在我们开始之前，确保在Wireshark中打开示例（请到作者原文中下载）并亲自实践一下

示例中仅包含一个单独的HTTP请求，请求的流程是：web浏览器向web服务器请求一个单独的图片文件，服务器返回一个成功的响应（HTTP/1.1200 OK），响应中包含请求的文件。右键示例文件中任意一个TCP包并且选择Follow TCP Stream就可在单独的窗口查看原始的TCP流

![](https://img-blog.csdn.net/20140725100914671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

客户端请求使用红色显示，服务端响应使用蓝色显示

![](https://img-blog.csdn.net/20140725101301370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

TCP三次握手（参见：[http://blog.csdn.net/a19881029/article/details/30241561](http://blog.csdn.net/a19881029/article/details/30241561)）

TCP在其协议头中使用大量的标志位或者说1位（bit）布尔域来控制连接状态，我们最感兴趣的3个标志位如下：

SYN - 创建一个连接

FIN -  终结一个连接

ACK - 确认接收到的数据

就像我们看见的那样，一个包中有可以设置多个标志位

选择Wireshark中的“包”1并且展开中间面板的TCP层解析，然后展开TCP头中的标志位域，这里我们可以看见所有解析出来的TCP标志位，需要注意的是，“包1”设置了SYN标志位

![](https://img-blog.csdn.net/20140724211336296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

使用同样的方式操作“包2”。可以看到"包2"设置了2个标志位：ACK - 用来确认收到客户端的SYN包，SYN - 用来表明服务端也希望建立TCP连接

![](https://img-blog.csdn.net/20140724212023046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

从客户端发来的“包3”只设置了ACK标志位。这3个包完成了最初的TCP3次握手

![](https://img-blog.csdn.net/20140724213129997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

序列号和确认号：

TCP会话的每一端都包含一个32位（bit）的序列号，该序列号被用来跟踪该端发送的数据量。每一个包中都包含序列号，在接收端则通过确认号用来通知发送端数据成功接收

当某个主机开启一个TCP会话时，他的初始序列号是随机的，可能是0和4,294,967,295之间的任意值，然而，像Wireshark这种工具，通常显示的都是相对序列号/确认号，而不是实际序列号/确认号，相对序列号/确认号是和TCP会话的初始序列号相关联的。这是很方便的，因为比起真实序列号/确认号，跟踪更小的相对序列号/确认号会相对容易一些

比如，在“包1”中，最初的相对序列号的值是0，但是最下方面板中的ASCII码显示真实序列号的值是0xf61c6cbe，转化为10进制为4129057982

如果想要关闭相对序列号/确认号，可以选择Wireshark菜单栏中的 Edit -> Preferences ->protocols ->TCP，去掉Relative sequence number后面勾选框中的√即可

![](https://img-blog.csdn.net/20140725092301017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

需要注意的是，文章接下来的部分依然使用相对序列号/确认号

为了更好的理解在整个TCP会话期间，TCP序列号和确认号是如何工作的，我们可以使用Wireshark内置的绘制流功能，选择菜单栏中的 Statistics ->Flow Graph...->TCP flow ->OK

![](https://img-blog.csdn.net/20140725093504031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Wireshark会自动创建一个TCP流的图形摘要

![](https://img-blog.csdn.net/20140725104320005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

每行代表一个单独的TCP包，左边列显示时间，中间列显示包的方向、TCP端口、段长度和设置的标志位，右边列以10进制的方式显示相关序列号/确认号，在这里选中任意行会高亮主窗口中该行所关联的包

我们可以利用这个流图更好的理解序列号和确认号是如何工作的

包1：

TCP会话的每一端的序列号都从0开始，同样的，确认号也从0开始，因为此时通话还未开始，没有通话的另一端需要确认（我使用的Wireshark版本和原作者不同，Wireshark1.10.2中，包1不显示确认号）

包2：

服务端响应客户端的请求，响应中附带序列号0（由于这是服务端在该次TCP会话中发送的第一个包，所以序列号为0）和相对确认号1（表明服务端收到了客户端发送的包1中的SYN）

需要注意的是，尽管客户端没有发送任何有效数据，确认号还是被加1，这是因为接收的包中包含SYN或FIN标志位（并不会对有效数据的计数产生影响，因为含有SYN或FIN标志位的包并不携带有效数据）

包3：

和包2中一样，客户端使用确认号1响应服务端的序列号0，同时响应中也包含了客户端自己的序列号（由于服务端发送的包中确认收到了客户端发送的SYN，故客户端的序列号由0变为1）

此时，通信的两端的序列号都为1，通信两端的序列号增1发生在所有TCP会话的建立过程中

包4：

这是流中第一个携带有效数据的包（确切的说，是客户端发送的HTTP请求），序列号依然为1，因为到上个包为止，还没有发送任何数据，确认号也保持1不变，因为客户端没有从服务端接收到任何数据

需要注意的是，包中有效数据的长度为725字节

包5：

当上层处理HTTP请求时，服务端发送该包来确认客户端在包4中发来的数据，需要注意的是，确认号的值增加了725（725是包4中有效数据长度），变为726，简单来说，服务端以此来告知客户端端，目前为止，我总共收到了726字节的数据，服务端的序列号保持为1不变

包6：

这个包标志着服务端返回HTTP响应的开始，序列号依然为1，因为服务端在该包之前返回的包中都不带有有效数据，该包带有1448字节的有效数据

包7：

由于上个数据包的发送，TCP客户端的序列号增长至726，从服务端接收了1448字节的数据，客户端的确认号由1增长至1449

在抓包文件的主体部分，我们可以看到上述过程的不断的重复，客户端的序列号一直是726，因为客户端除了最初的725字节数据没有再向服务端发送数据，服务端的序列号则与此相反，由于服务端不断的发送HTTP响应，故其序列号一直在增长

序列号为当前端成功发送的数据位数，确认号为当前端成功接收的数据位数，SYN标志位和FIN标志位也要占1位

关闭连接

![](https://img-blog.csdn.net/20140725125056842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTE5ODgxMDI5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

包38：

在确认了服务端发送过来的最后一个数据段之后，客户端将处理整个HTTP响应并决定不需要进一步通信了。此时客户端发送设置了FIN标志位的包38，其确认号和之前的包37一样

包39：

服务端通过将确认号加1的方式回应客户端期望关闭连接的请求（这里和包2中确认SYN标志位时所作的操作是一样的），同时设置当前包的FIN标志位

包40：

客户端发送最终序列号727，通过将确认号加1的方式确认服务端的FIN包

此时，通信双方都终结了会话并且可以释放用于维持会话所占用的资源









