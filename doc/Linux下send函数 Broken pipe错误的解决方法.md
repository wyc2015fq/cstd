# Linux下send函数 Broken pipe错误的解决方法

2013年11月21日 16:48:28 [feisy](https://me.csdn.net/feisy) 阅读数：7015









在linux下写socket的程序的时候，如果尝试send到一个disconnected socket上，就会让底层抛出一个SIGPIPE信号。

client端通过 pipe 发送信息到server端后，就关闭client端, 这时server端,返回信息给 client 端时就产生Broken pipe 信号了。

对于产生信号，我们可以在产生信号前利用方法 signal(int signum, sighandler_t handler) 设置信号的处理。如果没有调用此方法，系统就会调用默认处理方法：中止程序，显示提示信息(就是我们经常遇到的问题)。我们可以调用系统的处理方法，也可以自定义处理方法。

对一个已经收到FIN包的socket调用read方法, 如果接收缓冲已空, 则返回0, 
这就是常说的表示连接关闭. 但第一次对其调用write方法时, 如果发送缓冲没问题, 
会返回正确写入(发送). 但发送的报文会导致对端发送RST报文, 
因为对端的socket已经调用了close, 完全关闭, 既不发送, 也不接收数据. 所以, 
第二次调用write方法(假设在收到RST之后), 会生成SIGPIPE信号, 导致进程退出.

为了避免进程退出, 可以捕获SIGPIPE信号, 或者忽略它, 
给它设置SIG_IGN信号处理函数:

signal(SIGPIPE, SIG_IGN);

这样, 第二次调用write方法时, 会返回-1, 同时errno置为SIGPIPE. 
程序便能知道对端已经关闭.

PS: Linux下的SIGALRM似乎会每1秒钟往后偏移1毫秒, 
但Windows下经过测试完全准时, 不差1毫秒.

头文件 #include <signal.h>

struct sigaction sa;
sa.sa_handler = SIG_IGN;//设定接受到指定信号后的动作为忽略
sa.sa_flags = 0;
if (sigemptyset(&sa.sa_mask) == -1 || //初始化信号集为空
sigaction(SIGPIPE, &sa, 0) == -1) { //屏蔽SIGPIPE信号
perror("failed to ignore SIGPIPE; sigaction");
exit(EXIT_FAILURE);
}

pthread线程里如何屏蔽SIGPIPE异常

在pthread中，可能会遇到Program received signal SIGPIPE, Broken 
pipe的问题，解决方法是每一个线程启动之前时，先执行下面代码：

\#ifndef WIN32
sigset_t signal_mask;
sigemptyset (&signal_mask);
sigaddset (&signal_mask, SIGPIPE);
int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
if (rc != 0) {
printf("block sigpipe error\n");
}







linux下当连接断开，还发数据的时候，不仅send()的返回值会有反映，而且还会向系统发送一个异常消息，如果不作处理，系统会出BrokePipe，程序会退出，这对于服务器提供稳定的服务将造成巨大的灾难。为此，send()函数的最后一个参数可以设MSG_NOSIGNAL，禁止send()函数向系统发送异常消息



