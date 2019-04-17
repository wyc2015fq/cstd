# 怎样通过MSG_WAITALL设置阻塞时间，IO模式精细讲解： MSG_DONTWAIT 、 MSG_WAITALL - DoubleLi - 博客园






首先给出MSDN上一段设置阻塞超时的代码：（网址为[http://social.msdn.microsoft.com/Forums/zh-SG/visualcpluszhchs/thread/3d9daec0-f000-4bf1-add1-3beab8e398eb](http://social.msdn.microsoft.com/Forums/zh-SG/visualcpluszhchs/thread/3d9daec0-f000-4bf1-add1-3beab8e398eb)）




```
`您可以利用``select``模型来设置connect连接超时,当前之前需要调用`

`unsigned ``long``ul = 1;`

`ioctlsocket(SOCKET, FIOBIO, (unsigned ``long``*)&ul);`

`。。。`

`connect(...);`

`之后的就使用``select``()`

`下面有段网上的例子代码,您不妨参考一下：`



`WSADATA wsd;`

`SOCKET cClient;`

`int``ret;`

`struct``sockaddr_in server;`

`hostent *host=NULL;`



`if``(WSAStartup(MAKEWORD(2,0),&wsd)){``return``0;}`

`cClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);`

`if``(cClient==INVALID_SOCKET){``return``0;}`

`//set Recv and Send time out`

`int``TimeOut=6000; ``//设置发送超时6秒`

`if``(::setsockopt(cClient,SOL_SOCKET,SO_SNDTIMEO,(``char``*)&TimeOut,``sizeof``(TimeOut))==SOCKET_ERROR){`

`return``0;`

`}`

`TimeOut=6000;``//设置接收超时6秒`

`if``(::setsockopt(cClient,SOL_SOCKET,SO_RCVTIMEO,(``char``*)&TimeOut,``sizeof``(TimeOut))==SOCKET_ERROR){`

`return``0;`

`}`

`//设置非阻塞方式连接`

`unsigned ``long``ul = 1;`

`ret = ioctlsocket(cClient, FIONBIO, (unsigned ``long``*)&ul);`

`if``(ret==SOCKET_ERROR)``return``0;`



`//连接`

`server.sin_family = AF_INET;`

`server.sin_port = htons(25);`

`server.sin_addr .s_addr = inet_addr((LPCSTR)pSmtp);`

`if``(server.sin_addr.s_addr == INADDR_NONE){``return``0;}`



`connect(cClient,(``const``struct``sockaddr *)&server,``sizeof``(server));`



`//select 模型，即设置超时`

`struct``timeval timeout ;`

`fd_set r;`



`FD_ZERO(&r);`

`FD_SET(cClient, &r);`

`timeout.tv_sec = 15; ``//连接超时15秒`

`timeout.tv_usec =0;`

`ret = ``select``(0, 0, &r, 0, &timeout);`

`if``( ret <= 0 )`

`{`

`::closesocket(cClient);`

`return``0;`

`}`

`//一般非锁定模式套接比较难控制，可以根据实际情况考虑 再设回阻塞模式`

`unsigned ``long``ul1= 0 ;`

`ret = ioctlsocket(cClient, FIONBIO, (unsigned ``long``*)&ul1);`

`if``(ret==SOCKET_ERROR){`

`::closesocket (cClient);`

`return``0;`

`}`
```






基本概念：

阻塞IO::

socket 的阻塞模式意味着必须要做完IO 操作（包括错误）才会返回。

非阻塞IO::

非阻塞模式下无论操作是否完成都会立刻返回，需要通过其他方式来判断具体操作是否成功。



IO模式设置：
一般对于一个socket 是阻塞模式还是非阻塞模式有两种方式::

 方法1、fcntl 设置；

方法2、recv,send 系列的参数。(读取，发送时，临时将sockfd或filefd设置为非阻塞)



方法一、fcntl 函数可以将一个socket 句柄设置成非阻塞模式:
flags = fcntl(sockfd, F_GETFL, 0); //获取文件的flags值。

 fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); //设置成非阻塞模式；

[](http://www.hotdomains.net/browse/api/robot.api.php?__get=M2RjMjA5YTZhOjg6e3M6NzoiaXRlbV9pZCI7czo0OiIxNTA5IjtzOjg6InJlc291cmNlIjtzOjU6ImltYWdlIjtzOjM6ImFjdCI7czo3OiJkaXNwbGF5IjtzOjQ6InRpbWUiO2k6MTMwMDY3MTc1NjtzOjM6ImRpciI7aTo5NDI3ODg2NjE7czo4OiJmaWxlbmFtZSI7czoxNDoiMTEyNzI1MzI0Ny5wbmciO3M6NToid2lkdGgiO2k6MTU3O3M6NjoiaGVpZ2h0IjtpOjE5Nzt9)

flags = fcntl(sockfd,F_GETFL,0);

fcntl(sockfd,F_SETFL,flags&~O_NUNBLOCK); //设置成阻塞模式；

设置之后每次的对于sockfd 的操作都是非阻塞的。



方法二、recv, send 函数的最后有一个flag 参数可以设置成MSG_DONTWAIT

临时将sockfd 设置为非阻塞模式,而无论原有是阻塞还是非阻塞。

recv(sockfd, buff, buff_size,MSG_DONTWAIT); //非阻塞模式的消息发送

send(scokfd, buff, buff_size, MSG_DONTWAIT); //非阻塞模式的消息接受





阻塞与非阻塞读的区别: //阻塞和非阻塞的区别在于没有数据到达的时候是否立刻返回．

读(read/recv/msgrcv):

读的本质来说其实不能是读,在实际中, 具体的接管数据不是由这些调用来进行,是由于系统底层自动完成的。read 也好,recv 也好只负责把数据从底层缓冲copy 到我们指定的位置.

对于读来说(read, 或者recv) ::

阻塞情况下::

在阻塞条件下，read/recv/msgrcv的行为::

1、如果没有发现数据在网络缓冲中会一直等待，

2、当发现有数据的时候会把数据读到用户指定的缓冲区，但是如果这个时候读到的数据量比较少，比参数中指定的长度要小，read 并不会一直等待下去，而是立刻返回。

read 的原则::是数据在不超过指定的长度的时候有多少读多少，没有数据就会一直等待。

所以一般情况下::我们读取数据都需要采用循环读的方式读取数据，因为一次read 完毕不能保证读到我们需要长度的数据，

read 完一次需要判断读到的数据长度再决定是否还需要再次读取。

非阻塞情况下::

在非阻塞的情况下，read 的行为::

1、如果发现没有数据就直接返回，

2、如果发现有数据那么也是采用有多少读多少的进行处理．

所以::read 完一次需要判断读到的数据长度再决定是否还需要再次读取。



对于读而言:: 阻塞和非阻塞的区别在于没有数据到达的时候是否立刻返回．
recv 中有一个MSG_WAITALL 的参数::

recv(sockfd, buff, buff_size, MSG_WAITALL),
在正常情况下recv 是会等待直到读取到buff_size 长度的数据，但是这里的WAITALL 也只是尽量读全，在有中断的情况下recv 还是可能会被打断，造成没有读完指定的buff_size的长度。

所以即使是采用recv + WAITALL 参数还是要考虑是否需要循环读取的问题，在实验中对于多数情况下recv (使用了MSG_WAITALL)还是可以读完buff_size，

所以相应的性能会比直接read 进行循环读要好一些。



注意:: //使用MSG_WAITALL时，sockfd必须处于阻塞模式下，否则不起作用。

//所以MSG_WAITALL不能和MSG_NONBLOCK同时使用。

要注意的是使用MSG_WAITALL的时候，sockfd 必须是处于阻塞模式下，否则WAITALL不能起作用。





阻塞与非阻塞写的区别: //
写(send/write/msgsnd)::

写的本质也不是进行发送操作,而是把用户态的数据copy 到系统底层去,然后再由系统进行发送操作,send，write返回成功，只表示数据已经copy 到底层缓冲,而不表示数据已经发出,更不能表示对方端口已经接管到数据.
对于write(或者send)而言，

阻塞情况下:: //阻塞情况下，write会将数据发送完。(不过可能被中断)

在阻塞的情况下，是会一直等待，直到write 完，全部的数据再返回．这点行为上与读操作有所不同。

原因::

读，究其原因主要是读数据的时候我们并不知道对端到底有没有数据，数据是在什么时候结束发送的，如果一直等待就可能会造成死循环，所以并没有去进行这方面的处理；

写，而对于write, 由于需要写的长度是已知的，所以可以一直再写，直到写完．不过问题是write 是可能被打断吗，造成write 一次只write 一部分数据, 所以write 的过程还是需要考虑循环write, 只不过多数情况下一次write 调用就可能成功.



非阻塞写的情况下:: //

非阻塞写的情况下，是采用可以写多少就写多少的策略．与读不一样的地方在于，有多少读多少是由网络发送的那一端是否有数据传输到为标准，但是对于可以写多少是由本地的网络堵塞情况为标准的，在网络阻塞严重的时候，网络层没有足够的内存来进行写操作，这时候就会出现写不成功的情况，阻塞情况下会尽可能(有可能被中断)等待到数据全部发送完毕， 对于非阻塞的情况就是一次写多少算多少,没有中断的情况下也还是会出现write 到一部分的情况.









