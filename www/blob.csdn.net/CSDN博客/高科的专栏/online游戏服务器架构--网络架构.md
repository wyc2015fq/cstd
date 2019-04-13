
# online游戏服务器架构--网络架构 - 高科的专栏 - CSDN博客

2012年11月24日 00:14:47[高科](https://me.csdn.net/pbymw8iwm)阅读数：5891个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)[架构管理																](https://blog.csdn.net/pbymw8iwm/article/category/1219853)[游戏																](https://blog.csdn.net/pbymw8iwm/article/category/1252249)[
							](https://blog.csdn.net/pbymw8iwm/article/category/1219853)
[
																								](https://blog.csdn.net/pbymw8iwm/article/category/863753)


启动：父进程启动；子进程启动；网络架构。
每个父进程携带N个子进程，子进程负责处理业务逻辑和其它数据，而父进程只是将客户端的请求路由到各个子进程，路由的策略非常简单，父进程将请求包按照轮流的法则分发到这N个子进程。
子进程接收到请求包的时候，它便开始处理，处理完后再将结果反还给父进程。注意，子进程并不处理网络连接，它并不知道请求包的源的信息，它只处理业务，相反地，父进程并不知道请求包的内容，它的任务就是处理连接。
父子进程之间通过共享内存进行通信，具体来讲就是父进程将请求包放入和对应子进程共享的内存中，然后通过一个管道唤醒子进程，子进程探测到管道消息以后就从共享内存将请求拉出来然后进行处理，处理完毕后再将结果放回到共享内存，然后同样唤醒父进程，父进程被唤醒之后便拉出子进程的回复数据，最后通过它自己保存的连接返回给客户端。
这个服务器解除了接收数据和处理数据之间的耦合，便于进行任何一边的扩展，不像那种消息映射服务器，直接在本进程内部通过分发回调函数来处理业务逻辑，或者用线程的方式进行处理，线程的方式虽然解决了吞吐量的问题，但是无法解决稳定性的问题，必须默认所有的数据都是安全的或者开发出繁复的处理逻辑来处理异常情况，额外增加了服务器的负担。子进程的关于业务逻辑的处理方式非常类似于那种消息映射服务器，不同之处在于，典型的消息映射服务器是从网络上将数据拉回，而该online服务器却是从共享内存中将数据拉回，多了共享内存这么一个中间层。
关于业务逻辑的处理还有一个类似的层次，就是online子进程和数据库之间的关系，它们通过一个数据库代理（DBProxy）来将子进程的处理逻辑和数据库之间的耦合解除，并且这数据库代理还可以隐藏数据库的访问接口，只有代理知道后端连接了什么数据库，而处理逻辑不必知道，它只需要将访库请求作为网络请求发送给数据库代理就好了，然后用消息映射服务器的方式处理数据库代理的回复。数据库只管保存数据，而不管这些数据之间的除了关系模型之外的额外事宜，比如有效性验证之类的，所有的数据验证和处理工作在online子进程那里进行。这样处理的优点就是易于扩展新业务，缺点就是要来回几次的进程访库，因为每次只取当次的数据，在业务处理过程中可能还需要别的数据…不过缺点可以通过高速网络和高性能数据库以及数据库代理服务器来弥补。
for ( ; i != bc->bind_num; ++i ) {
bind_config_elem_t* bc_elem = &(bc->configs[i]);
shmq_create(bc_elem); //通过mmap的方式创建共享内存
… //错误处理
} else if (pid > 0) {
close_shmq_pipe(bc, i, 0);
do_add_conn(bc_elem->sendq.pipe_handles[0], PIPE_TYPE_FD, 0, bc_elem);
} else {
run_worker_process(bc, i, i + 1);
}
}
run_worker_process函数开始了子进程的历程，可以看到最后这个函数调用了一个叫做net_loop的无限循环，这个函数在父进程初始化完毕后也最终调用，原型如下：
int net_loop(int timeout, int max_len, int is_conn);
该函数通过最后一个参数is_conn来区分是子进程还是父进程，函数内部实现也是通过该参数一分为二的，online的父进程负责网络收发，主要是基于epoll的，epoll已经被证明拥有很高的性能，在linux平台上的性能测试已经超越了原来的poll/select模型，甚至比windows的IO完成端口在大负载，高并发环境下表现更加出色。在net_loop中用epoll_wait等待有事件的文件描述符，然后判断文件描述符的类型(套结字在创建之初就将描述符和类型等信息打包成一个数据结构了)，如果是管道文件的事件，那么肯定是不需要处理数据的，仅仅察看事件类型以及判断是否父进程就可以判断发生了什么事了，由于子进程根本就不会将套结字描述符加入到epoll监控向量，因此子进程只能有管道类型的事件发生，注意这里不涉及online子进程和DB的通信。接下来的net_loop中关于epoll的处理流程就是父进程的事了，具体过程就是处理套结字类型的文件描述符了，就是从套结字接收数据，然后放到和一个子进程共享的内存区域中，最后往子进程管道里写一个数据，告诉子进程现在该处理业务逻辑了，子进程在net_loop中监控到管道事件之后，最终调用net_loop最后的handle_recv_queue()函数，该函数开始处理业务逻辑：
if(!is_conn) {
\#ifdef USE_CMD_QUEUE
handle_cmd_busy_sprite(); //handle the busy sprite list first
\#endif
handle_recv_queue();
handle_timer();
}
以上是net_loop的大致流程，对于父进程如何将请求路由给子进程有两种选择，一种是父进程网络服务器按照某种策略比如负载均衡采取轮换路由，另一种就是将选择留给用户，用户登录online之前首先登录一个switch服务器，自行选择online子进程，每个online子进程都有一个ID，用户选择后就用这个ID作为数据打包，另外switch服务器上的online子进程链表中包含了足够的其对应于父进程的IP地址和端口信息，然后向online子进程对应的父进程发送LOGIN包，父进程在net_loop中最终调用net_recv，然后解出LOGIN包，由于该包中包含了其子进程的id，而这个id又和其与子进程的共享内存相关联，一个数据结构最起码关联了父进程接收的套结字描述符，子进程ID，父子进程的共享内存缓冲区这三个元素。
关键数据结构：
typedef struct bind_config_elem {
int online_id;
char online_name[16];
char bind_ip[16]; //邦定的ip地址
in_port_t bind_port; //邦定的端口
char gameserv_ip[16]; //游戏服务器的ip
in_port_t gameserv_port;
char gameserv_test_ip[16];
in_port_t gameserv_test_port;
struct shm_queue sendq; //发送缓冲区，被分割成一个一个的块，因此叫队列
struct shm_queue recvq; //接收缓冲区，被分割成一个一个的块，因此叫队列
} bind_config_elem_t;
该结构描述了每一个传输套结字都应该拥有的一个结构，也就是每一个子进程一个这样的结构
typedef struct bind_config {
int online_start_id;
int bind_num;
bind_config_elem_t configs[MAX_LISTEN_FDS];
} bind_config_t;
这个结构是上面结构的容器，main中的bind_config_elem_t* bc_elem = &(bc->configs[i]);体现了一切，所有的一切都是从配置文件中读取的。
typedef struct shm_head {
volatile int head;
volatile int tail;
atomic_t blk_cnt;
} __attribute__ ((packed)) shm_head_t;
这个结构分割了一个缓冲区，将一个连续的缓冲区分割成了一个队列
struct shm_queue {
shm_head_t* addr;
u_int length;
int pipe_handles[2];
};
这个结构代表了一个缓冲区，分割的过程在shm_head_t中体现。
struct epinfo {
struct fdinfo *fds;
struct epoll_event *evs;
struct list_head close_head;
struct list_head etin_head;
int epfd;
int maxfd;
int fdsize;
int count;
};
这个结构代表了epoll事件。
在LOGIN包被父进程解析到的时候：
if ((ntohl(proto->cmd) == PROTO_LOGIN) && (epi.fds[fd].bc_elem == 0) )为真，接着：
uint16_t online_id = ntohs(*(uint16_t*)(proto->body)); //得到用户选择的online_id
…
epi.fds[fd].bc_elem = &(bc->configs[online_id - bc->online_start_id]); //得到该id对应的config结构体。
得到了bind_config_elem_t结构体之后就可以将请求包转发到从该结构体中取出的共享内存缓冲区了，然后将请求包放到这个内存中。所有的请求包中，LOGIN请求包是父进程直接处理的，后续的游戏逻辑请求包由子进程处理，另外子进程虽然不处理网络连接，但是对于和数据库代理服务器和switch中心跳服务器的连接还是要自己处理的，因此子进程中也有网络相关的内容，在net_rcv中有以下片断：
if (!is_conn) {
handle_process(epi.fds[fd].cb.recvptr, epi.fds[fd].cb.rcvprotlen, fd, 0);
}
这个就是直接处理数据库代理以及心跳的处理过程。另外关于网络架构中还有一点就是链表的使用，在net_rcv中首先调用do_read_conn读取网络数据，但是一旦当前积压的未处理的数据达到了一个最大值的时候，后续的请求就要丢到链表中，然后在下一轮net_loop中接收新的数据前优先处理之；在net_loop中有一句：
if (is_conn) handle_send_queue();
该句的意思就是说，如果是父进程，那么首先处理发送队列，这些发送队列中的数据都是子进程放入的请求包的回复，父进程优先将这些回复返回给各个客户端：
static inline void handle_send_queue()
{
struct shm_block *mb;
struct shm_queue *q;
int i = 0;
for ( ; i != bindconf.bind_num; ++i ) {
q = &(bindconf.configs[i].sendq);
while ( shmq_pop(q, &mb) == 0 ) {
schedule_output(mb);
}
}
}
虽然这个过程比较优先，但是更优先是前面说的过程，就是处理积压链表，下面片断在上面的之前调用：
list_for_each_safe (p, l, &epi.close_head) { //优先便利需要关闭的套结字，第一时间关闭连接
fi = list_entry (p, struct fdinfo, list);
if (fi->cb.sendlen > 0)
do_write_conn (fi->sockfd);
do_del_conn (fi->sockfd, 0);
}
list_for_each_safe (p, l, &epi.etin_head) { //优先处理积压队列，提高响应速度
fi = list_entry (p, struct fdinfo, list);
if (net_recv(fi->sockfd, max_len, is_conn) == -1)
do_del_conn(fi->sockfd, is_conn);
}
该服务器中大量运用了链表，此链表的定义就是list_head，是从linux内核中抽取出来的。
接收新连接的时候，在net_loop中：
if (epi.fds[fd].type == LISTEN_TYPE_FD) {
while (do_open_conn(fd, is_conn) > 0);
接收了新的连接，并且加入了一个列表，将新连接的套结字描述符和一个空的bind_config_elem_t相关联，注意此时并没有初始化这个bind_config_elem_t，因为在LOGIN包到来之前还不知道和哪一个bind_config_elem_t相关联，该函数仅仅初始化了一个epi结构。


