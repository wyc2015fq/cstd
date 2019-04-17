# 基于UDP高性能传输协议UDT doc翻译(一) - DoubleLi - 博客园






原文转自：[http://hi.baidu.com/doodlezone/item/74a203155efe26dbbf9042dd](http://hi.baidu.com/doodlezone/item/74a203155efe26dbbf9042dd)

**                 UDT文档阅读理解**

**一、  概述**

UDT是一个高性能的基于UDP的数据传输协议，它是为支持高速广域网上海量数据传输而设计，为解决TCP的效率和公平问题，同时提供可靠的数据流和报文传输。

UDT是C++库，几乎类同于BSD socket APIs。

UDT是多线程安全的，但并不是多进程共享。

**二、  原理**

UDT有两种传输模式：数据流模式（SOCK_STREAM）和数据报模式(SOCK_DGRAM)

数据流模式类似于传统的BSD套接字，这种模式下不能保证任何一端一个调用就把所有的数据发送了，因为在数据流中没有边界信息，进程需用loop来发送和接收。

数据报模式会将数据作为整个单元来传送，不需要循环来接收和发送数据，要么全部发送，要么一点也不发送。在接收端如果缓冲区不够大，则只会接收到部分数据，其他的将被丢弃。



UDT发送数据有阻塞与非阻塞方式，在阻塞方式下，会直到把需要发送的数据发送完再返回，而非阻塞方式下，会根据socket底层的可用缓冲的大小，将缓冲区中的数据拷贝过去，有多大缓冲就拷贝多少，缓冲区满了就立即返回，这个时候的返回值只是拷贝了多少，不代表发送了多少，同时剩下的部分需要再次调用send



UDT增加了rendezvous模式，这是一种连接模式，用来穿透防火墙。这种模式下，UDT不能调用listen和accept，而是两端bind后同时建立连接。



UDT允许用户自己定义拥塞控制。可以继承DUT/CCC下的CCC类来改变一些变量，如拥塞窗口，./app/cc.h下的实例是学习的快速途径。



**三、  安装及平台**

UDT是基于源码的库，所以没有安装文件工具，我们只需要根据不同的系统和CPU[架构](http://lib.csdn.net/base/architecture)使用命令来make相应的库即可。

UDT支持的系统：[Linux](http://lib.csdn.net/base/linux)，BSD，OSX

UDT支持的架构：IA32，IA64，POWERPC,AMD64

命令：  make –e os=XXX arch=XXX



UDT来源于BSD socket API只有一个头文件<udt.h>，一些继续使用BSDAPI 另一些需要加标示符UDT::

库：libudt.h       udt.dll       udt.dylib     libudt.a   udt.lib 

**四、  配置设置**

读取和设置选项通过getsockopt和setsockopt方法，一般不要修改默认选项除非应用不能正常运行。



UDT_MSS用来设置包的大小，一般情况下最佳的UDT包的大小是网络MTU（默认1500字节）的大小，连接的两端都要设置这个值，传输时取两端的较小者。

UDT用不同的同步方式语义UDT_SNDSYN和UDT_RCVSYN，它可以独立的设置发送和接收同步，具有更多的灵活性。它不允许在连接建立和关闭的时候进行非阻塞操作。



UDT缓冲区的大小理论上越大越好，要运行的好两端buffer至少为【带宽*RTT】

UDT使用UDP数据通道，所以UDP缓冲大小影响程序运行，但随着buffer变大效果也会越来越不明显。一般来说发送端的buffer小一点，因为包的发送没有限制太多，但太大会增加端到端的延时。



UDT_LINGER是设置socket关闭时是否立即停止发送缓冲区的数据。

UDT_RENDEZVOUS设置集合点模式，在穿越防火墙时很有用。

UDT_SNDTIMEO和UDT_RCVTIMEO是timeout值

UDT_REUSEADDR设置UDP端口是否可以给其他UDT使用，默认值是true。

以下情况需设置false

1，两个UDT socket不能在同一端口监听。

2，两个UDT socket绑定在同一IP同一端口而不能建立连接。

发送发有两种选择：

1，TTL（默认无限）为timeout时间。

2，消息有序到达，直到上一个消息到达或被丢弃才发下一个。



UDT提供文件传输，UDT::sendfile和UDT::recvfile这种发送接收方式跟

UDT::send和UDT::recv是正交的。也就是说用sendfile发送不一定要用recvfile接收。另外，sendfile和recvfile不受SNDSYN,RCVSYN,SNDTIMEO,RCVTIMEO影响。它使用C++ fstream进行文件IO。



UDT打洞，在传统方式下，穿越防火墙时是用SO_REUSEADDR选项去打开两个socket绑定同一个端口，一个监听一个建立连接。而UDT提供直接相连的方式。



UDT允许一个进程中的所有socket绑定到同一端口但只允许一个监听。

UDT允许绑定已经存在的UDP端口有两个好处：

1，当应用程序向服务器发送一个空包去获得它的地址（尤其是在NAT防火墙下）时，用户会创建一个UDP包发送个server确定绑定的端口，然后UDP端口可以顺便给UDT使用。

2，一些本地防火墙在关闭“打洞”时会改变映射端口,新的UDT绑定的端口将失效，此时用UDP的是必须的。



错误处理：所有的UDT API在遇到错误时都会返回error UDT定义两种错误，

UDT::INVALID_SOCK和UDT::ERROR。可以用getErrorCode和getErrorMessage方法查看存放在ERRORINFO[数据结构](http://lib.csdn.net/base/datastructure)中的错误代码及信息。

成功的调用不会清楚错误，所以应用程序应该利用返回值检查调用结果，可以调用个体lasterror().clean()来清除错误日志。 









