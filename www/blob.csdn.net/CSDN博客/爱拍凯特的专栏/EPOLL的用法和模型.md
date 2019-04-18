# EPOLL的用法和模型 - 爱拍凯特的专栏 - CSDN博客
2016年05月28日 16:45:38[爱拍凯特](https://me.csdn.net/apacat)阅读数：415
1、epoll的用法
既然epoll是一种IO多路复用技术，也就是管理大量的描述符的，那么这种机制既可以使用在服务端也可以使用在客户端。而在这两种使用方式下可能会有一些差距，但是差距并不大，毕竟作用是相同的。
首先说一下epoll中常用的几个函数：
int epfd = epoll_create(int size);  这个函数用于创建一个epoll描述符，同时他会创建红黑树和就绪链表，这两个的作用已经说过了（[点这里](http://blog.csdn.net/apacat/article/details/51375950)），就是红黑树存储监控的描述符，就绪链表存储就绪的描述符节点上的数据。
函数中的参数是指定生成描述符的最大范围。在linux-2.4.32内核中根据size大小初始化哈希表的大小，在linux2.6.10内核及以后的内核中该参数无用，使用红黑树管理所有的文件描述符，而不是hash。
int ret = epoll_ctl(int epfd,int option,int fd,struct epoll_event* ev); 这个函数用于向内核注册描述符fd上的回调函数，同时会将这个描述符fd插入到红黑树中，如果这个描述符存在在红黑树中，则立刻返回，如果红黑树中不存在这个描述符的节点，就会插入到红黑树中。
函数中的参数的含义分别是：epfd是epoll_create创建的描述符，option是对这个描述符的操作，包括（ADD,MOD,DEL），fd是要插入的描述符，ev是描述符上的事件结构体，这个结构体里面包含了这个fd上要监控的事件类型和事件中需要的其他信息。
int ret = epoll_wait(int epfd,struct epoll_event* events,int maxsize ,int timeout); 这个函数用来实现等待监控的描述符上的事件发生，这个函数调用会做两件事，分别是1、接受消息，并将对应的数据拷贝到events数组中；2、将就绪链表清空。而在epoll的两种工作模式中，处理方式有所不同，LT模式下，epoll_wait清空了就绪链表之后会检查描述符是什么工作模式，如果是LT工作模式，并且描述符上确实有未处理的事件的时候会将描述符对应的节点再次插入到就绪链表中，然后马上返回，但是ET模式下，是不会做这种检查的，所以才会引发这两个模式工作效率的不同（ET模式比LT模式效率高）和对一些未处理完成的事件的处理方式不同（LT会再次触发响应，而ET模式不会再次触发响应）。
函数中的参数的含义：epfd是epoll_create创建的描述符，这个描述符和一般的文件描述符以及socket描述符是不同的，是一种特殊的描述符。evets是用来存储返回的就绪事件中的数据的，maxsize是告诉第二个参数events的大小，毕竟能存储的个数是不能超过这范围的，否则就溢出了，timeout是epoll_wait的超时时间，这个超时时间的作用就在于当就绪链表上的一直为空的时候，超过一定的时间也要返回，这个计时时间以毫秒计时，如果设为0表示立即返回，如果设为-1表示永久阻塞（也有说未知的）；
2、epoll的模型
下面提供两个常用的epoll模型
**服务器模型**，这种模型也是用的最多的模型：
//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    struct epoll_event ev,events[20];
    epfd=epoll_create(10000); //创建epoll句柄
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //把socket设置为非阻塞方式
    setnonblocking(listenfd);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port=htons(SERV_PORT);
    bind(listenfd,(struct sockaddr )&serveraddr, sizeof(serveraddr));
    listen(listenfd, 255);
    //设置与要处理的事件相关的文件描述符
    ev.data.fd=listenfd;
    //设置要处理的事件类型
    ev.events=EPOLLIN;
    //注册epoll事件
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    for ( ; ; )
    {
      //等待epoll事件的发生
      nfds=epoll_wait(epfd,events,20,1000);
      //处理所发生的所有事件
      for(i=0;i<nfds;++i)
      {
         if(events.data.fd==listenfd)
         {
** connfd = accept(listenfd,(struct sockaddr )&clientaddr, &clilen);**
                if(connfd<0)
                {
                  perror("connfd<0");
                }
                setnonblocking(connfd);
                //设置用于读操作的文件描述符
                ev.data.fd=connfd;
                //设置用于注测的读操作事件
                ev.events=EPOLLIN|EPOLLET;
                //注册event
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
         }
         else if(events.events&EPOLLIN)
         {
                read_socket(events.data.fd);
                ev.data.fd=events.data.fd;
                ev.events=EPOLLIN|EPOLLOUT|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
         }
         else if(events.events&EPOLLOUT)
         {
                write_socket(events.data.fd);
                ev.data.fd=events.data.fd;
                ev.events=EPOLLIN|EPOLLET; //ET模式
            epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
         }
         else
         {
                perror("other event");
         }
      }
    }
很明显，上述过程可以简单的描述为：
1、创建epoll和监听socket;
2、设置socket为非阻塞模式并绑定到地址结构中;
3、bind();
4、listen();
5、将监听socket和监听时间存储到描述符中并加入到epoll中
6、死循环
7、获取epoll_wait的返回，并循环处理每个返回的事件、其中的事件类型包括 accept事件（新建连接）、epollin（可读事件）、epollout(可写事件)
===========================================================================
**客户端模型**
for ( ; ; )
    {
      //等待epoll事件的发生
      nfds=epoll_wait(epfd,events,20,1000);
      //处理所发生的所有事件
      for(i=0;i<nfds;++i)
      {
         if(events[i].events & EPOLLERR || events[i].events& EPOLLHUP)
         {
                 //处理逻辑
                epoll_ctl(epfd,EPOLL_CTL_DEL,connfd,&ev);
         }
         else if(events[i].events&EPOLLIN)
         {
                read_socket(events[i].data.fd);
                ev.data.fd=events[i].data.fd;
                ev.events=EPOLLIN|EPOLLOUT|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
         }
         else if(events[i].events&EPOLLOUT)
         {
                write_socket(events[i].data.fd);
                ev.data.fd=events[i].data.fd;
                ev.events=EPOLLIN|EPOLLET; //ET模式
                  epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
         }
         else
         {
                perror("other event");
         }
      }
从上面看来服务端和客户端的epoll区别很小，客户端socket是没有监听socket一说的，所以就不需要处理accept，当然也没有bind()、listen（）等操作。这两种模式，根本上还是一样的。
