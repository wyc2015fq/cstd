# Zero Copy - z69183787的专栏 - CSDN博客
2018年05月15日 16:12:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：113
个人分类：[Mq-Kafka](https://blog.csdn.net/z69183787/article/category/7665372)
原文出处: http://www.ibm.com/developerworks/library/j-zerocopy/
### 传统的I/O
使用传统的I/O程序读取文件内容, 并写入到另一个文件(或Socket), 如下程序:
```
File.read(fileDesc, buf, len);
Socket.send(socket, buf, len);
```
会有较大的性能开销, 主要表现在一下两方面:
1. 上下文切换(context switch), 此处有4次用户态和内核态的切换
2. Buffer内存开销, 一个是应用程序buffer, 另一个是系统读取buffer以及socket buffer
其运行示意图如下
![](https://images0.cnblogs.com/i/399666/201404/280055549984508.gif)
1) 先将文件内容从磁盘中拷贝到操作系统buffer
2) 再从操作系统buffer拷贝到程序应用buffer
3) 从程序buffer拷贝到socket buffer
4) 从socket buffer拷贝到协议引擎.
![](https://images0.cnblogs.com/i/399666/201404/280058111076424.gif)
这是上下文切换图
1) 调用read(), 程序切换到内核态
2) read()调用完毕, 返回数据, 程序切换回用户态
3) 调用send(), 程序切换到内核态
4) send()完毕, 程序切换回用户态
操作系统使用 read buffer 的好处是"预读", 当你的程序需要对文件数据做处理, 并且每次读取的数据小于read buffer 的时候, 可以先将多数数据预读到 read buffer, 这样程序在读取的时候效率会更高. 但是当你需要读取的数据大于操作系统的read buffer的时候, read buffer则会成为累赘.
另外, 在你的程序不需要处理数据, 而仅仅只是做数据转移的时候, 程序buffer则会成为不必要的开销.
上面会涉及到多次上下文切换以及多次数据拷贝, 很大一部分cpu及内存开销是可以避免的, 于是有了zerocopy技术. 
### ZeroCopy
zerocopy技术省去了将操作系统的read buffer拷贝到程序的buffer, 以及从程序buffer拷贝到socket buffer的步骤, 直接将 read buffer 拷贝到 socket buffer. java 的 FileChannel.transferTo() 方法就是这样的实现, 这个实现是依赖于操作系统底层的sendFile()实现的.
`public void transferTo(long position, long count, WritableByteChannel target);`
他的底层调用的是系统调用sendFile()方法
```
#include <sys/socket.h>
ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```
如下图
![](https://images0.cnblogs.com/i/399666/201404/280111422834712.gif)
![](https://images0.cnblogs.com/i/399666/201404/280111503305104.gif)
这样, 省去了两次buffer的copy, 并且上下文切换降到了2次(调用transferTo()进入内核态, 调用完毕返回用户态)
Linux 2.4 及以后的内核, 又做了改进, 不再使用socket buffer, 而是直接将read buffer数据拷贝到协议引擎, 而socket buffer只会记录数据位置的描述符和数据长度,如下
![](https://images0.cnblogs.com/i/399666/201404/280117369558033.gif)
