# epoll - xcw0754 - 博客园
# [epoll](https://www.cnblogs.com/xcw0754/p/10073552.html)
epoll为什么这么快？当数据包到达时，socket是怎么通知epoll的？
(PS：既然要看内核，那就只关心想知道的内容，否则可能会把自己绕晕了！)
先看怎么注册监听句柄的：
```
long sys_epoll_ctl(int epfd, int op, int fd, struct epoll_event __user *event)
{
    struct file *file, *tfile;
    struct eventpoll *ep;
            // 从user space拷到kernel space
        if (ep_op_has_event(op) && copy_from_user(&epds, event, sizeof(struct epoll_event)))
            goto error_return;
    file = fget(epfd);
    tfile = fget(fd);
    ep = file->private_data;    // 这个file关联epoll实例 
    switch (op) {
    case EPOLL_CTL_ADD:
        epds.events |= POLLERR | POLLHUP;
        error = ep_insert(ep, &epds, tfile, fd);    // 关键看插入操作
        break;
    case EPOLL_CTL_DEL: ...
    case EPOLL_CTL_MOD: ...
    }
    ...
}
static int ep_insert(struct eventpoll *ep, struct epoll_event *event, struct file *tfile, int fd)
{
    ...
    init_poll_funcptr(&epq.pt, ep_ptable_queue_proc);   // 等同于赋值:epq.pt.qproc=ep_ptable_queue_proc
    int revents = tfile->f_op->poll(tfile, &epq.pt);    // 扫一遍就绪事件，再回调ep_ptable_queue_proc挂监听钩子
    ep_rbtree_insert(ep, epi);      // 插入到rbtree里
      ...
      // 检查事件
      if ((revents & event->events) && !ep_is_linked(&epi->rdllink)) {
          /* 怎么可能会走这里？才刚insert就想dispatch到哪去？真正挂监听的是在epoll_wait里边呀 */
          list_add_tail(&epi->rdllink, &ep->rdllist);
    
          if (waitqueue_active(&ep->wq))    // wq的初始化见ep_alloc(),插入见ep_poll()
              __wake_up_locked(&ep->wq, TASK_UNINTERRUPTIBLE | TASK_INTERRUPTIBLE);
          if (waitqueue_active(&ep->poll_wait))
              pwake++;
      }
    ...
}
```
那个`tfile->f_op->poll`很关键，它是哪来的？这得看sys_socket了，创建待监听的socket的时候初始化的(这里我们只关注socket，不关注普通的文件)。这里略过socket创建时的来龙去脉，它其实指向sock_poll函数。
```
unsigned int sock_poll(struct file *file, poll_table *wait)
{
    struct socket *sock = file->private_data;
    return sock->ops->poll(file, sock, wait);   // 关键是ops
}
// 传输层的socket结构
struct socket {
    const struct proto_ops  *ops;   // 就是这个
    struct file     *file;
    struct sock     *sk;
    ...
}
```
现在来关注ops是啥，下面是常见的两种socket的ops：
```
struct inet_protosw inetsw_array[] =
{
    {
        .type =       SOCK_STREAM,
        .protocol =   IPPROTO_TCP,
        .prot =       &tcp_prot,
        .ops =        &inet_stream_ops, // TCP关注这里
        .capability = -1,
        .no_check =   0,
        .flags =      INET_PROTOSW_PERMANENT | INET_PROTOSW_ICSK,
    },
    {
        .type =       SOCK_DGRAM,
        .protocol =   IPPROTO_UDP,
        .prot =       &udp_prot,
        .ops =        &inet_dgram_ops,  // UDP关注这里
        .capability = -1,
        .no_check =   UDP_CSUM_DEFAULT,
        .flags =      INET_PROTOSW_PERMANENT,
    },
    ...
};
```
拿udp举例：
```
struct proto_ops inet_dgram_ops = {
    .family        = PF_INET,
    .owner         = THIS_MODULE,
    .release       = inet_release,
    .bind          = inet_bind,
    .connect       = inet_dgram_connect,
    .socketpair    = sock_no_socketpair,
    .accept        = sock_no_accept,
    .getname       = inet_getname,
    .poll          = udp_poll,      // 关键
    .ioctl         = inet_ioctl,
    .listen        = sock_no_listen,
    .shutdown      = inet_shutdown,
    .setsockopt    = sock_common_setsockopt,
    .getsockopt    = sock_common_getsockopt,
    .sendmsg       = inet_sendmsg,
    .recvmsg       = sock_common_recvmsg,
    .mmap          = sock_no_mmap,
    .sendpage      = inet_sendpage,
    .compat_setsockopt = compat_sock_common_setsockopt,
    .compat_getsockopt = compat_sock_common_getsockopt,
};
unsigned int udp_poll(struct file *file, struct socket *sock, poll_table *wait)
{
    unsigned int mask = datagram_poll(file, sock, wait);    // 关键
    ...
}
// 上面提到的sock->ops->poll就是这个函数
// file是关联待监听fd的,sock是待监听socket的,wait是新建的关联ep_ptable_queue_proc
unsigned int datagram_poll(struct file *file, struct socket *sock, poll_table *wait)
{
    struct sock *sk = sock->sk;
    unsigned int mask;
    poll_wait(file, sk->sk_sleep, wait);    // 关键
    mask = 0;
    /* exceptional events? */
    if (sk->sk_err || !skb_queue_empty(&sk->sk_error_queue))
        mask |= POLLERR;
    if (sk->sk_shutdown & RCV_SHUTDOWN)
        mask |= POLLRDHUP;
    if (sk->sk_shutdown == SHUTDOWN_MASK)
        mask |= POLLHUP;
    /* readable? */
    if (!skb_queue_empty(&sk->sk_receive_queue) ||
        (sk->sk_shutdown & RCV_SHUTDOWN))
        mask |= POLLIN | POLLRDNORM;
    /* Connection-based need to check for termination and startup */
    if (connection_based(sk)) {
        if (sk->sk_state == TCP_CLOSE)
            mask |= POLLHUP;
        /* connection hasn't started yet? */
        if (sk->sk_state == TCP_SYN_SENT)
            return mask;
    }
    /* writable? */
    if (sock_writeable(sk))
        mask |= POLLOUT | POLLWRNORM | POLLWRBAND;
    else
        set_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags);
    return mask;
}
static inline void poll_wait(struct file * filp, wait_queue_head_t * wait_address, poll_table *p)
{
    if (p && wait_address)
        p->qproc(filp, wait_address, p);    // qproc其实就是 ep_ptable_queue_proc
}
// file是关联待监听fd
// whead是待监听网络层sk->sk_sleep
// pt关联ep_ptable_queue_proc的结构体
void ep_ptable_queue_proc(struct file *file, wait_queue_head_t *whead, poll_table *pt)
{
    struct epitem *epi = ep_item_from_epqueue(pt);  // epi在ep_insert出现过，表示一个待监听的fd
    struct eppoll_entry *pwq;   // 这个玩意准备挂在epi上面
    if (epi->nwait >= 0 && (pwq = kmem_cache_alloc(pwq_cache, GFP_KERNEL))) {
        init_waitqueue_func_entry(&pwq->wait, ep_poll_callback);    // ep_poll_callback是回调
        pwq->whead = whead;
        pwq->base = epi;
        add_wait_queue(whead, &pwq->wait);  // whead插到pwq->wait链表中
        list_add_tail(&pwq->llink, &epi->pwqlist);  // pwq->llink插到epi->pwqlist链表中
        epi->nwait++;
    } else {
        /* We have to signal that an error occurred */
        epi->nwait = -1;
    }
}
```
### 等待事件发生
```
long sys_epoll_wait(int epfd, struct epoll_event __user *events,
                   int maxevents, int timeout)
{
    ...
    error = ep_poll(ep, events, maxevents, timeout);
    return error;
}
int ep_poll(struct eventpoll *ep, struct epoll_event __user *events, int maxevents, long timeout)
{
    int res, eavail;
    unsigned long flags;
    long jtimeout;
    wait_queue_t wait;
    jtimeout = (timeout < 0 || timeout >= EP_MAX_MSTIMEO) ?
        MAX_SCHEDULE_TIMEOUT : (timeout * HZ + 999) / 1000;
retry:
    spin_lock_irqsave(&ep->lock, flags);
    res = 0;
    if (list_empty(&ep->rdllist)) {
        init_waitqueue_entry(&wait, current);   // 当前进程关联wait
        wait.flags |= WQ_FLAG_EXCLUSIVE;
        __add_wait_queue(&ep->wq, &wait);   // 加入ep的等待队列,有事件发生就会notify本进程
        for (;;) {  // 运行期间无限循环
            set_current_state(TASK_INTERRUPTIBLE);  // 让当前进程随时可被打断
            if (!list_empty(&ep->rdllist) || !jtimeout)
                break;
            if (signal_pending(current)) {
                res = -EINTR;
                break;
            }
            spin_unlock_irqrestore(&ep->lock, flags);
            jtimeout = schedule_timeout(jtimeout); // 调度,睡眠
            spin_lock_irqsave(&ep->lock, flags);
        }
        __remove_wait_queue(&ep->wq, &wait);  // 移除等待队列
        set_current_state(TASK_RUNNING);    // 置本进程状态为running
    }
    eavail = !list_empty(&ep->rdllist);
    spin_unlock_irqrestore(&ep->lock, flags);
    if (!res && eavail && !(res = ep_send_events(ep, events, maxevents)) && jtimeout)
        goto retry;
    return res;
}
```
由于设置了TASK_INTERRUPTIBLE状态，schedule_timeout(jtimeout)可能还没睡够jtimeout就返回，比如接收到信号。等到返回时，状态自动被切换到TASK_RUNNING。
### 创建socket流程
socket是怎么创建的？
```
// 系统调用入口
long sys_socket(int family, int type, int protocol)
{
    ...
    retval = sock_create(family, type, protocol, &sock);
    return sock_map_fd(sock);   // 将socket映射成fd
}
int sock_create(int family, int type, int protocol, struct socket **res)
{
    return __sock_create(current->nsproxy->net_ns, family, type, protocol, res, 0);
}
// 系统调用sys_socket是这样创建
int __sock_create(struct net *net, int family, int type, int protocol, struct socket **res, int kern)
{
    ...
    struct socket *sock = sock_alloc(); // 这是传输层的!
    pf = rcu_dereference(net_families[family]); // net_families是全局变量，供其他模块注册
    err = pf->create(net, sock, protocol);  // 创建对应IP层的sock，见下面分析
    ...
}
```
对于AF_INET，pf就是下面这样来的。
```
// families注册接口
int sock_register(const struct net_proto_family *ops)
{
    if (net_families[ops->family])
        err = -EEXIST;
    else {
        net_families[ops->family] = ops;
        err = 0;
    }
}
int inet_init(void)
{
    ...
    sock_register(&inet_family_ops);    // 注册
    ...
}
struct net_proto_family inet_family_ops = {
    .family = PF_INET,
    .create = inet_create,      // 就是那个pf->create
    .owner  = THIS_MODULE,
};
```
IP层的socket才是关键，底层数据包到达终端时是先到达IP层的：
```
// Create an inet socket.(这是IP层的socket)
int inet_create(struct net *net, struct socket *sock, int protocol)
{
    ...
    struct sock *sk = sk_alloc(net, PF_INET, GFP_KERNEL, answer_prot);  // 申请sk
    sock_init_data(sock, sk);   // 初始化sk和sock之间关联部分
    ...
}
void sock_init_data(struct socket *sock, struct sock *sk)
{
    ...
    if (sock) {
        sk->sk_type = sock->type;
        sk->sk_sleep = &sock->wait;
        sock->sk = sk;  // sk挂在sock上
    } else
        sk->sk_sleep = NULL;
    ...
}
```
将socket映射成文件：有些重要的东西是放在文件里的，须要关注下。
```
int sock_map_fd(struct socket *sock)
{
    struct file *newfile;
    int fd = sock_alloc_fd(&newfile);
    int err = sock_attach_fd(sock, newfile);    // 关键
    fd_install(fd, newfile);
    ...
}
static int sock_attach_fd(struct socket *sock, struct file *file)
{
    ...
    sock->file = file;
    init_file(file, sock_mnt, dentry, FMODE_READ | FMODE_WRITE, &socket_file_ops);  // 关键
    file->private_data = sock;
    ...
}
int init_file(struct file *file, struct vfsmount *mnt, struct dentry *dentry,
   mode_t mode, const struct file_operations *fop)
{
    int error = 0;
    file->f_path.dentry = dentry;
    file->f_path.mnt = mntget(mnt);
    file->f_mapping = dentry->d_inode->i_mapping;
    file->f_mode = mode;
    file->f_op = fop;       // 关键
    return error;
}
const struct file_operations socket_file_ops = {
    .owner =    THIS_MODULE,
    .llseek =   no_llseek,
    .aio_read = sock_aio_read,
    .aio_write =    sock_aio_write,
    .poll =     sock_poll,          // 关键
    .unlocked_ioctl = sock_ioctl,
    .compat_ioctl = compat_sock_ioctl,
    .mmap =     sock_mmap,
    .open =     sock_no_open,
    .release =  sock_close,
    .fasync =   sock_fasync,
    .sendpage = sock_sendpage,
    .splice_write = generic_splice_sendpage,
};
```
### 怎么通知阻塞的进程的？

