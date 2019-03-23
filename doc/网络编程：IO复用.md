# 网络编程：I/O复用



I/O多路复用是在多线程或多进程编程中常用技术。主要是通过select/epoll/poll三个函数支持的。在此主要对select和epoll函数详细介绍。

### select函数

- 该函数运行进程指示内核等待多个事件中的任何一个发生，并只有一个或多个事件发生或经历一段指定的时间后才唤醒它。

- 调用select告知内核对哪些描述符(就读、写或异常条件)感兴趣以及等待多长时间。我们感兴趣的描述符不局限于套接字，任何描述符都可以使用select来测试。

- 函数原型：
  ```cpp
  #include<sys/select.h>
  #include<sys/time.h>
  
  int select(int maxfdp1, fd_set *readset, fd_set* writeset, fd_set
   *exceptset, const struct timeval* timeout);
   返回：若有就绪描述符则为其数目，若超时则为0，若出错则为-1
  ```

  - 最后一个参数timeout，它告知内核等待所指定描述符中的任何一个就绪可花多长时间。该参数有三种可能： 

    - 永远等待下去：仅在有一个描述符准备好I/O时才返回，将其设为空指针
    - 等待一段固定时间：在有一个描述符准备好I/O时返回，但是不超过由该参数所指向的timeval结构中指定的秒数和微秒数。
    - 根本不等待：检查描述符后立即返回，这就是轮询。为此，该参数必须指向一个timeval结构，但是其中的值必须设置为0

  - 三个参数readset,writeset,exceptset指定我们要让内核测试读、写和异常条件的描述符。

  - 如何给这三个参数的每一个参数指定一个或多个描述符值是一个设计上的问题。select使用描述符集，通常是一个整数数组，其中每个整数中的每一位对应一个描述符。举例来说，假设使用32位整数，那么该数组的第一个元素对应于描述符0~31，第二个元素对应于描述符32~63，以此类推。所有这些实现细节都与应用程序无关，它们隐藏在名为fd_set的数据类型和以下四个宏中：

    ```cpp
    void FD_ZERO(fd_set *fdset);    //clear all bits in fdset
    void FD_SET(int fd, fd_set *fdset);   //turn on the bit for fd in fdset
    void FD_CLR(int fd, fd_set *fdset);  //turn off the bit for fd in  fdset
    int FD_ISSET(int fd, fd_set *fdset);  //is the bit for fd on in  fdset?
    ```

    我们分配一个fd_set数据类型的描述符集，并用这些宏设置或测试该集合中的每一位，也可以用C语言中的赋值语句把它赋值成另外一个描述符集。
     **注意：前面所讨论的每个描述符占用整数数组中的一位的方法仅仅是select函数的可能实现之一。**

  - maxfdp1参数指定待测试的描述符个数，它的值是待测试的最大描述符加1。描述符0,1,2,...,直到maxfdp1 - 1均被测试。

  - select函数修改由指针readset,writeset和exceptset所指向的描述符集，因而这三个参数都是值-结果参数。该函数返回后，我们使用FD_ISSET宏来测试fd_set数据类型中的描述符。描述符集内任何与未就绪描述符所对应的位返回时均清成0.为此，每次重新调用select函数时，我们都得再次把所有描述符集内所关心的位均置为1

#### select返回套接字的“就绪”条件

- 满足下列四个条件之一的任何一个时，一个套接字准备好读： 
  - 该套接字接收缓冲区中的数据字节数大于等于套接字接收缓冲区低水位标记的当前大小。对于这样的套接字执行读操作不会阻塞并将返回一个大于0的值(也就是返回准备好读入的数据)。我们使用SO_RECVLOWAT套接字选项设置套接字的低水位标记。对于TCP和UDP套接字而言，其默认值为1
  - 该连接的读半部关闭(也就是接收了FIN的TCP连接)。对这样的套接字的读操作将不阻塞并返回0(也就是返回EOF)
  - 该套接字时一个监听套接字且已完成的连接数不为0。
  - 其上有一个套接字错误待处理。对这样的套接字的读操作将不阻塞并返回-1(也就是返回一个错误)，同时把errno设置为确切的错误条件。这些待处理错误也可以通过SO_ERROR套接字选项调用getsockopt获取并清除。
- 下列四个条件的任何一个满足时，一个套接字准备好写： 
  - 该套接字发送缓冲区中的可用字节数大于等于套接字发送缓冲区低水位标记的当前大小，并且或该套接字已连接，或者该套接字不需要连接(如UDP套接字)。这意味着如果我们把这样的套接字设置成非阻塞的，写操作将不阻塞并返回一个正值(如由传输层接收的字节数)。我们使用SO_SNDLOWAT套接字选项来设置该套接字的低水位标记。对于TCP和UDP而言，默认值为2048
  - 该连接的写半部关闭。对这样的套接字的写操作将产生SIGPIPE信号
  - 使用非阻塞式connect套接字已建立连接，或者connect已经已失败告终
  - 其上有一个套接字错误待处理。对这样的套接字的写操作将不阻塞并返回-1(也就是返回一个错误)，同时把errno设置为确切的错误条件。这些待处理错误也可以通过SO_ERROR套接字选项调用getsockopt获取并清除。
- 如果一个套接字存在带外数据或者仍处于带外标记，那么它有异常条件待处理。
- **注意：当某个套接字上发生错误时，它将由select标记为既可读又可写**
- 接收低水位标记和发送低水位标记的目的在于：允许应用进程控制在select可读或可写条件之前有多少数据可读或有多大空间可用于写。
- 任何UDP套接字只要其发送低水位标记小于等于发送缓冲区大小(默认应该总是这种关系)就总是可写的，这是因为UDP套接字不需要连接。

### poll函数

- 函数原型：

  ```cpp
  #include <poll.h>
  int poll(struct pollfd *fdarray, unsigned long nfds, int timeout);
  //返回：若有就绪描述符则为数目，若超时则为0，若出错则为-1
  ```

  

- 第一个参数是指向一个结构数组第一个元素的指针。每个数组元素都是一个pollfd结构，用于指定测试某个给定描述符fd的条件。

   ```cpp
   struct pollfd{
       int fd;    //descriptor to check
       short event;  //events of interest on fd
       short revents;  //events  that occurred on fd
   };
   ```

   要测试的条件由events成员指定，函数在相应的revents成员中返回该描述符的状态。(每个描述符都有两个变量，一个为调用值，另一个为返回结果，从而避免使用值-结果参数。)

- poll事件

![img](https://images2015.cnblogs.com/blog/1122912/201705/1122912-20170510212952160-1059120541.png)

### epoll函数

- epoll是Linux特有的I/O复用函数。它在实现和使用上与select、poll有很大的差异。 

  - 首先，epoll使用一组函数来完成任务，而不是单个函数。
  - 其次，epoll把用户关心的文件描述符上的事件放在内核里的一个事件表中，从而无须像select和poll那样每次调用都要重复传入文件描述符集或事件集。
  - 但epoll需要使用一个额外的文件描述符，来唯一标识内核中的这个事件表

- epoll文件描述符使用如下方式创建：
  ```cpp
  #include <sys/epoll.h>
  int epoll_create(int size);
  ```

   size参数完全不起作用，只是给内核一个提示，告诉它事件表需要多大。该函数返回的文件描述符将用作其他所有epoll系统调用的第一个参数，以指定要访问的内核事件表。

- 下面的函数用来操作epoll的内核事件表：
  ```cpp
  #include <sys/epoll.h>
  int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
   // 返回：若成功返回0，失败返回-1，并置errno
  ```


   fd参数是要操作的文件描述符，op参数则指定操作类型。操作类型有以下三类： 

  - EPOLL_CTL_ADD, 往事件表中注册fd上的事件

  - EPOLL_CTL_MOD, 修改fd上的注册事件

  - EPOLL_CTL_DEL, 删除fd上的注册事件

- event指定事件，它是epoll_event结构指针类型，epoll_event的定义如下：

  ```cpp
  struct epoll_event {
      __uint32_t events;    //epoll事件
      epoll_data_t data;    //用户数据
  };
  ```

  - 其中，events成员描述事件类型。epoll支持的事件类型同poll基本相同。表示epoll事件类型的宏在poll对应的宏前加上"E",比如epoll的数据可读事件是EPOLLIN。

  - epoll有两个额外的事件类型——EPOLLET和EPOLLONESHOT。它们对于epoll的高效运作非常关键。

  - data成员用于存储用户数据，是一个联合体：

     ```cpp
     typedef union epoll_data{
         void *ptr;
         int fd;
         uint32_t u32;
         uint64_t u64;
     }epoll_data_t;
     ```

     其中4个成员用得最多的是fd，它指定事件所从属的目标文件描述符。

     其中4个成员用得最多的是fd，它指定事件所从属的目标文件描述符。

- epoll系列系统调用的主要接口是epoll_wait函数，它在一段超时时间内等待一组文件描述符上的事件，其原型如下：
  ```cpp
  #include <sys/epoll.h>
  int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
  // 返回：若成功返回就绪的文件描述符个数，失败时返回-1，并置errnoo
  ```

- maxevents参数指定最多监听多少个事件，它必须大于0

- event_wait函数如果检测到事件，就将所有就绪事件从内核事件表(由epfd参数指定)中复制到它的第二个参数events指向的数组中。这个数组只用于输出epoll_wait检测到的就绪事件，而不像select和poll的数组参数那样既用于传入用户注册的事件，又用于输出内核检测到的就绪事件。这就极大地提高了应用程序索引就绪文件描述符的效率。

- 下面代码给出 poll和epoll在使用上的差别：

  ```cpp
  //如何索引poll返回的就绪文件描述符
  int ret = poll(fds, MAX_EVENT_NUMBER, -1);
  //必须遍历所有已注册文件描述符并找到其中的就绪者
  for(int i = 0; i < MAX_EVENT_NUMBER; ++i){
      if(fds[i].revents & POLLIN) //判断第 i 个文件描述符是否就绪
      {
          int sockfd = fds[i].fd;
          //处理sockfd
      }
  }
  
  //如何索引epoll返回的文件描述符
   int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
   //仅遍历就绪的ret个文件描述符
   for(int i = 0; i < ret; ++i){
       int sockfd = events[i].data.fd;
       //sockfd肯定就绪，直接处理
   }
  ```

- LT和ET模式

  - LT(Level  Trigger，电平触发)模式：是默认工作模式，在这种模式下的epoll相当于一个效率较高的poll。当epoll_wait检测到其上有事件发生并将此事件通知应用程序后，应用程序可以不立即处理该事件。这样，当应用程序下一次调用epoll_wait时，epoll_wait还会再次向应用程序通告此事件。
  - ET(Edge Trigger，边沿触发)模式。对于ET工作模式下的文件描述符，当epoll_wait检测到其上有事件发生并将此事件通知应用程序后，应用程序必须立即处理该事件，因为后续的epoll_wait调用将不再向应用程序通知这一事件。
  - ET模式在很大程度上降低了同一个epoll事件被重复触发的次数。因此效率要比LT模式高。
  - **每个使用ET模式的文件描述符都应该是非阻塞的。如果文件描述符是阻塞的，那么读或写操作将会因为没有后续的时间而一直处于阻塞状态(饥渴状态)**

- EPOLLONESHOT事件 

  - 即使使用ET模式，一个socket上的某个事件还是可能被触发多次。这在并发程序中引起一个问题。比如一个线程(或进程)在读取完某个socket上的数据后开始处理这些数据，而在数据的处理过程中该socket上又有新数据可读(EPOLLIN再次被触发)，此时另外一个线程被唤醒来读取这些新的数据。于是出现了两个线程同时操作一个socket的场面。这当然不是我们期望的。我们期望的是一个socket连接在任一时刻都只被一个线程处理。
  - 对于注册了EPOLLONESHOT事件的文件描述符，操作系统最多触发其上注册的一个可读、可写或异常事件，且只触发一次，除非我们使用epoll_ctl函数重置该文件描述符上的EPOLLONESHOT事件。这样，当一个线程在处理某个socket时，其他线程时不可能有机会操作该socket的。但反过来思考，注册了EPOLLONESHOT事件的socket一旦被某个线程处理完毕，该线程就应该立即重置这个socket上的EPOLLONESHOT事件，以确保这个socket下一次可读时，其EPOLLIN事件能被触发，进而让其他工作线程有机会继续处理这个socket.