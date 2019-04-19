# 关于Epoll，你应该知道的那些细节 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [卢亮](http://www.jobbole.com/members/lulyon) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
Epoll，位于头文件sys/epoll.h，是Linux系统上的I/O事件通知基础设施。epoll API为Linux系统专有，于内核2.5.44中首次引入，glibc于2.3.2版本加入支持。其它提供类似的功能的系统，包括FreeBSD kqueue，Solaris /dev/poll等。
**Epoll API**
Epoll API实现了与poll类似的功能：监测多个文件描述符上是否可以执行I/O操作。支持边缘触发ET和水平触发LT，相比poll支持监测数量更多的文件描述符。
以下API用于创建和管理epoll实例：
**epoll_create**：创建Epoll实例，并返回Epoll实例关联的文件描述符。（最新的epoll_create1扩展了epoll_create的功能）
**create_ctl**：注册关注的文件描述符。注册于同一epoll实例的一组文件描述符被称为epoll set，可以通过进程对应的/proc/[pid]/fdinfo目录查看。
**epoll_wait**：等待I/O事件，如果当前没有任何注册事件处于可用状态，调用线程会被阻塞。
**水平触发LT与边缘触发ET**
Epoll事件分发接口可以使用ET和LT两种模式。两种模式的差别描述如下。
典型场景：
1 管道(pipe)读端的文件描述符(rfd)注册于Epoll实例。
2 写者(Writer)向管道(pipe)写端写2KB的数据。
3 epoll_wait调用结束，返回rfd作为就绪的文件描述符。
4 管道读者(pipe reader) 从rfd读1KB的数据。
5 下一次epoll_wait调用。
如果rfd文件描述符使用EPOLLET(边缘触发)标记加入Epoll接口，第5步对epoll_wait的调用可能会挂住，尽管文件输入缓冲区中仍然有可用数据；与此同时，远端实体由于已经发送数据，可能正在等待回应。其原因是边缘触发模式仅在所监控的文件描述符状态发生变化时才投递事件。所以，第5步的调用方可能最终一直在等待数据到来，但数据其实已经在输入缓存区。经过第2步的写操作和第3步的事件处理，rfd上只会产生一次事件。由于第4步的读操作没有读完全部的缓冲区数据，第5步对epoll_wait的调用可能会永远阻塞。
使用EPOLLET标记时，应该设置文件描述符为非阻塞，以避免阻塞读写，使处理多个文件描述符的任务饿死。因此，使用Epoll 边缘触发(EPOLLET)模式的接口，以下有两点建议：
1 使用非阻塞的文件描述符
2 只有在read或write返回EAGAIN之后，才继续等待事件（调用epoll_wait）
相比之下，当Epoll作为水平触发接口（LT，默认模式）使用时，epoll相当于一个更快的poll，可以用于poll适用的任何场景，因为二者语义相同。
在边缘触发模式下，当收到多个数据块时也可能会产生多个事件，调用方可以通过设置EPOLLONESHOT标记，告诉epoll当通过epoll_wait收到事件时，取消关联的文件描述符。当给epoll设置EPOLLONESHOT标记时，调用方需要通过epoll_ctl对文件描述符设置EPOLL_CTL_MOD标记。
**使用范例**
当Epoll作为水平触发接口使用时与poll语义相同，而作为边缘触发接口使用时需要注意应用层事件循环的细节，以避免错误。以下举例。设想一个非阻塞的socket为监听者，可以在该socket上调用listen。函数do_use_fd()处理新就绪的文件描述符，直到遇到读(read)或写(write)返回EAGAIN。事件驱动的状态机应用，应该在收到EAGAIN之后记录当前状态，以便在下次调用do_use_fd时，能够继续从之前停止读写数据的地方继续读写（read / write）。


```
#define MAX_EVENTS 10
structepoll_event ev, events[MAX_EVENTS];
intlisten_sock, conn_sock, nfds, epollfd;
 
/*Code to set up listening socket, 'listen_sock',
 (socket(), bind(), listen()) omitted */
 
epollfd= epoll_create1(0);
if(epollfd == -1) {
  perror("epoll_create1");
  exit(EXIT_FAILURE);
}
 
ev.events= EPOLLIN;
ev.data.fd= listen_sock;
if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
  perror("epoll_ctl: listen_sock");
  exit(EXIT_FAILURE);
}
 
for(;;) {
  nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
  if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
  }
 
  for (n = 0; n < nfds; ++n) {
      if (events[n].data.fd == listen_sock) {
          conn_sock = accept(listen_sock,
                            (struct sockaddr *) &addr, &addrlen);
          if (conn_sock == -1) {
              perror("accept");
              exit(EXIT_FAILURE);
          }
          setnonblocking(conn_sock);
          ev.events = EPOLLIN | EPOLLET;
          ev.data.fd = conn_sock;
          if (epoll_ctl(epollfd, EPOLL_CTL_ADD,conn_sock,
                     &ev) == -1) {
              perror("epoll_ctl:conn_sock");
              exit(EXIT_FAILURE);
          }
      } else {
          do_use_fd(events[n].data.fd);
      }
  }
}
```
当作为边缘触发接口使用时，为性能考虑，可以通过设置EPOLLIN|EPOLLOUT，一次加入文件描述符到epoll接口(EPOLL_CTL_ADD)。这样可以避免在EPOLLIN和EPOLLOUT之间通过调用epoll_ctl(EPOLL_CTL_MOD)切换。
**自动休眠问题**
如果系统设置了自动休眠模式（通过/sys/power/autosleep），当唤醒设备的事件发生时，设备驱动会保持唤醒状态，直到事件进入排队状态。为了保持设备唤醒直到事件处理完成，必须使用epoll EPOLLWAKEUP 标记。
一旦给structe poll_event中的events字段设置了EPOLLWAKEUP标记，系统会在事件排队时就保持唤醒，从epoll_wait调用开始，持续要下一次epoll_wait调用。
**监测数量限制**
以下文件可以用来限制epoll使用的内核态内存空间大小(Linux 2.6.28 开始)：
/proc/sys/fs/epoll/max_user_watches
max_user_watches文件用来设置用户在所有epoll实例中注册的文件描述符数量上限，作用于每个用户ID。单个注册文件描述符在32位内核上消耗90字节，在64位内核上消耗160字节。max_user_watches的默认值是可用内核内存空间的1/25（4%）除以单个注册文件描述符消耗的字节数。
**避免饥饿（边缘触发）**
如果I/O数据量很大，可能在读取数据的过程中其他文件得不到处理，造成饥饿。解决方法是维护一个就绪列表，在关联数据结构中标记文件描述符为就绪状态，由此可以记住哪些文件在等待，并对所有就绪文件作轮转处理。
**事件缓存陷阱**
如果使用事件缓存，或者存储epoll_wait返回的所有文件描述符，就需要提供方法动态标记关闭状态（比如，由于其他事件处理造成文件描述符关闭），假设从epoll_wait收到100个事件，A事件造成B事件关闭，如果移除B事件结构并关闭文件描述符，事件缓存仍然认为有事件在等待文件描述符，从而造成混乱。
解决方法是，在A事件处理过程中，调用epoll_ctl(EPOLL_CTL_DEL)来移除B文件描述符并关闭，然后标记关联的数据结构为已移除，并关联到移除列表。在后续事件处理过程中，当发现B文件描述符的新事件时，可以通过检查标记发现文件描述符已移除，避免产生混乱。
