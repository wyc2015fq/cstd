# Socket编程中的强制关闭与优雅关闭及相关socket选项 - DoubleLi - 博客园






以下描述主要是针对windows平台下的TCP socket而言。

首先需要区分一下关闭socket和关闭TCP连接的区别，关闭TCP连接是指TCP协议层的东西，就是两个TCP端之间交换了一些协议包（FIN，RST等），具体的交换过程可以看TCP协议，这里不详细描述了。而关闭socket是指关闭用户应用程序中的socket句柄，释放相关资源。但是当用户关闭socket句柄时会隐含的触发TCP连接的关闭过程。

TCP连接的关闭过程有两种，一种是优雅关闭（graceful close），一种是强制关闭（hard close或abortive close）。所谓优雅关闭是指，如果发送缓存中还有数据未发出则其发出去，并且收到所有数据的ACK之后，发送FIN包，开始关闭过程。而强制关闭是指如果缓存中还有数据，则这些数据都将被丢弃，然后发送RST包，直接重置TCP连接。



下面说一下shutdown及closesocket函数。

shutdown函数的原型是：

int shutdown(

  SOCKET s,

  int how

);

该函数用于关闭TCP连接，单并不关闭socket句柄。其第二个参数可以取三个值：SD_RECEIVE，SD_SEND，SD_BOTH。

SD_RECEIVE表明关闭接收通道，在该socket上不能再接收数据，如果当前接收缓存中仍有未取出数据或者以后再有数据到达，则TCP会向发送端发送RST包，将连接重置。

SD_SEND表明关闭发送通道，TCP会将发送缓存中的数据都发送完毕并收到所有数据的ACK后向对端发送FIN包，表明本端没有更多数据发送。这个是一个优雅关闭过程。

SD_BOTH则表示同时关闭接收通道和发送通道。



closesocket函数的原型是：

int closesocket(

  SOCKET s

);

该函数用于关闭socket句柄，并释放相关资源。前面说过，关闭socket句柄时会隐含触发TCP连接的关闭过程，那么closesocket触发的是一个优雅关闭过程还是强制关闭过程呢？这个与一个socket选项有关：SO_LINGER 选项，该选项的设置值决定了closesocket的行为。该选项的参数值是linger结构，其定义是：

typedef struct linger {

  u_short l_onoff;

  u_short l_linger;

} linger;

当l_onoff值设置为0时，closesocket会立即返回，并关闭用户socket句柄。如果此时缓冲区中有未发送数据，则系统会在后台将这些数据发送完毕后关闭TCP连接，是一个优雅关闭过程，但是这里有一个副作用就是socket的底层资源会被保留直到TCP连接关闭，这个时间用户应用程序是无法控制的。

当l_onoff值设置为非0值，而l_linger也设置为0，那么closesocket也会立即返回并关闭用户socket句柄，但是如果此时缓冲区中有未发送数据，TCP会发送RST包重置连接，所有未发数据都将丢失，这是一个强制关闭过程。

当l_onoff值设置为非0值，而l_linger也设置为非0值时，同时如果socket是阻塞式的，此时如果缓冲区中有未发送数据，如果TCP在l_linger表明的时间内将所有数据发出，则发完后关闭TCP连接，这时是优雅关闭过程；如果如果TCP在l_linger表明的时间内没有将所有数据发出，则会丢弃所有未发数据然后TCP发送RST包重置连接，此时就是一个强制关闭过程了。

另外还有一个socket选项SO_DONTLINGER，它的参数值是一个bool类型的，如果设置为true，则等价于SO_LINGER中将l_onoff设置为0。

注意SO_LINGER和SO_DONTLINGER选项只影响closesocket的行为，而与shutdown函数无关，shutdown总是会立即返回的。



所以为了保证建议的最好的关闭方式是这样的：

发送完了所有数据后：

(1)调用shutdown(s, SD_SEND)，如果本端同时也接收数据时则执行第二步，否则跳到第4步。

(2)继续接收数据，

(3)收到FD_CLOSE事件后，调用recv函数直到recv返回0或-1（保证收到所有数据），

(4)调用closesocket，关闭socket句柄。



在实际编程中，我们经常也不调用shutdown，而是直接调用closesocket，利用closesocket隐含触发TCP连接关闭过程的特性。此时的过程就是：

当发送完所有数据后：

(1)如果本端同时也接受数据则则执行第二步，否则跳到第4步。

(2)继续接收数据，

(3)收到FD_CLOSE事件后，调用recv函数直到recv返回0或-1（保证收到所有数据），

(4)调用closesocket，关闭socket句柄。

但是此时为了保证数据不丢失，则需要设置SO_DONTLINGER选项，不过windows平台下这个也是默认设置。



经过实验发现，发送端应用程序即便是异常退出或被kill掉进程，操作系统也不会丢弃发送缓冲区中的未发送数据，而是会在后台将这些数据发送出去。但是这是在socket的发送缓存不为0的前提下，当socket的发送缓存设置为0（通过SO_SNDBUF选项）时比较特殊，此时不论socket是否是阻塞的，send函数都会被阻塞直到传入的用户缓存中的数据都被发送出去并被确认，因为此时在驱动层没有分配缓存存放用户数据，而是直接使用的应用层的用户缓存，所以必须阻塞直到数据都发出，否则可能会造成系统崩溃。



另外，如果是接收端的应用程序异常退出或被kill掉进程，并且接收缓存中还有数据没有取出的话，那么接收端的TCP会向发送端发送RST包，重置连接，因为后续数据已经无法被提交应用层了。



最后这里说一个感觉是windows的bug，就是做这样的一个测试：

在一端线listen一个socket，然后在另一端connect，connect成功后，listen端会检测到网络事件触发，在listen端accept之前，将connect端kill掉，然后继续运行listen端，listen端任然会accept成功，且在accept出来的socket发送数据也能成功。发送完之后在等网络事件，此时又会等待成功，但是调用WSAEnumNetworkEvents得出的事件标识却是0。之后再也不会等到网络事件。









