# MySQL线程池总结(二) - gauss的专栏 - CSDN博客
2015年09月20日 10:49:16[gauss](https://me.csdn.net/mathlmx)阅读数：363
### **one-connection-per-thread**
根据scheduler_functions的模板，我们也可以列出one-connection-per-thread方式的几个关键函数。
```
`static`
```
scheduler_functions con_per_functions=
```
`{ max_connection+1, // max_threads`
`NULL``,`
`NULL``,`
`NULL``, // init`
`Init_new_connection_handler_thread, // init_new_connection_thread`
`create_thread_to_handle_connection, // add_connection`
`NULL``, // thd_wait_begin`
`NULL``, // thd_wait_end`
`NULL``, // post_kill_notification`
`one_thread_per_connection_end, // end_thread`
`NULL`
```
//
```
`end`
`};`
```
**1.init_new_connection_handler_thread**
这个接口比较简单，主要是调用pthread_detach，将线程设置为detach状态，线程结束后自动释放所有资源。
**2.create_thread_to_handle_connection**
这个接口是处理新连接的接口，对于线程池而言，会从thread_id%group_size对应的group中获取一个线程来处理，而one-connection-per-thread方式则会判断是否有thread_cache可以使用，如果没有则新建线程来处理。具体逻辑如下：
(1).判断缓存的线程数是否使用完(比较blocked_pthread_count 和wake_pthread大小)
(2).若还有缓存线程，将thd加入waiting_thd_list的队列，唤醒一个等待COND_thread_cache的线程
(3).若没有，创建一个新的线程处理,线程的入口函数是do_handle_one_connection
(4).调用add_global_thread加入thd数组。
**3.do_handle_one_connection**
这个接口被create_thread_to_handle_connection调用，处理请求的主要实现接口。
(1).循环调用do_command，从socket中读取网络包，并且解析执行；
(2). 当远程客户端发送关闭连接COMMAND(比如COM_QUIT，COM_SHUTDOWN)时，退出循环
(3).调用close_connection关闭连接(thd->disconnect());
(4).调用one_thread_per_connection_end函数,确认是否可以复用线程
(5).根据返回结果，确定退出工作线程还是继续循环执行命令。
**4.one_thread_per_connection_end**
判断是否可以复用线程(thread_cache)的主要函数,逻辑如下：
(1).调用remove_global_thread，移除线程对应的thd实例
(2).调用block_until_new_connection判断是否可以重用thread
(3).判断缓存的线程是否超过阀值，若没有，则blocked_pthread_count++;
(4).阻塞等待条件变量COND_thread_cache
(5).被唤醒后，表示有新的thd需要重用线程，将thd从waiting_thd_list中移除，使用thd初始化线程的thd->thread_stack
(6).调用add_global_thread加入thd数组。
(7).如果可以重用，返回false，否则返回ture
### **线程池与epoll**
在引入线程池之前，server层只有一个监听线程，负责监听mysql端口和本地unixsocket的请求，对于每个新的连接，都会分配一个独立线程来处理，因此监听线程的任务比较轻松，mysql通过poll或select方式来实现IO的多路复用。引入线程池后，除了server层的监听线程，每个group都有一个监听线程负责监听group内的所有连接socket的连接请求，工作线程不负责监听，只处理请求。对于overscribe为1000的线程池设置，每个监听线程需要监听1000个socket的请求，监听线程采用epoll方式来实现监听。
Select,poll,epoll都是IO多路复用机制，IO多路复用通过一种机制，可以监听多个fd(描述符)，比如socket，一旦某个fd就绪(读就绪或写就绪)，能够通知程序进行相应的读写操作。epoll相对于select和poll有了很大的改进，首先epoll通过epoll_ctl函数注册，注册时，将所有fd拷贝进内核，只拷贝一次不需要重复拷贝，而每次调用poll或select时，都需要将fd集合从用户空间拷贝到内核空间(epoll通过epoll_wait进行等待)；其次，epoll为每个描述符指定了一个回调函数，当设备就绪时，唤醒等待者，通过回调函数将描述符加入到就绪链表，无需像select，poll方式采用轮询方式；最后select默认只支持1024个fd，epoll则没有限制，具体数字可以参考cat
 /proc/sys/fs/file-max的设置。epoll贯穿在线程池使用的过程中，下面我就epoll的创建，使用和销毁生命周期来描述epoll在线程中是如何使用的。
- 线程池初始化，epoll通过epoll_create函数创建epoll文件描述符，实现函数是thread_group_init；
- 端口监听线程监听到请求后，创建socket，并创建THD和connection对象，放在对应的group队列中；
- 工作线程获取该connection对象时，若还未登录，则进行登录验证
- 若socket还未注册到epoll，则调用epoll_ctl进行注册,注册方式是EPOLL_CTL_ADD，并将connection对象放入epoll_event结构体中
- 若是老连接的请求，仍然需要调用epoll_ctl注册，注册方式是EPOLL_CTL_MOD
- group内的监听线程调用epoll_wait来监听注册的fd，epoll是一种同步IO方式，所以会进行等待
- 请求到来时，获取epoll_event结构体中的connection，放入到group中的队列
- 线程池销毁时，调用thread_group_close将epoll关闭。
备注：
1.注册在epoll的fd，若请求就绪，则将对应的event放入到events数组，并将该fd的事务类型清空，因此对于老的连接请求，依然需要调用epoll_ctl(pollfd, EPOLL_CTL_MOD, fd, &ev)来注册。
### **线程池函数调用关系**
(1)创建epoll
tp_init->thread_group_init->tp_set_threadpool_size->io_poll_create->epoll_create
(2)关闭epoll
tp_end->thread_group_close->thread_group_destroy->close(pollfd)
(3)关联socket描述符
handle_event->start_io->io_poll_associate_fd->io_poll_start_read->epoll_ctl
(4)处理连接请求
handle_event->threadpool_process_request->do_command->dispatch_command->mysql_parse->mysql_execute_command
(5)工作线程空闲时
worker_main->get_event->pthread_cond_timedwait
等待thread_pool_idle_timeout后，退出。
(6)监听epoll
worker_main->get_event->listener->io_poll_wait->epoll_wait
(7)端口监听线程
main->mysqld_main->handle_connections_sockets->poll
### **one-connection-per-thread函数调用关系**
(1) 工作线程等待请求
handle_one_connection->do_handle_one_connection->do_command->
my_net_read->net_read_packet->net_read_packet_header->net_read_raw_loop->
vio_read->vio_socket_io_wait->vio_io_wait->poll
备注：与线程池的工作线程有监听线程帮助其监听请求不同，one-connection-per-thread方式的工作线程在空闲时，会调用poll阻塞等待网络包过来；
而线程池的工作线程只需要专心处理请求即可，所以使用也更充分。
(2)端口监听线程
与线程池的(7)相同
### **参考文档**
[http://www.cnblogs.com/Anker/p/3265058.html](http://www.cnblogs.com/Anker/p/3265058.html)
[http://blog.csdn.net/zhanglu5227/article/details/7960677](http://blog.csdn.net/zhanglu5227/article/details/7960677)
