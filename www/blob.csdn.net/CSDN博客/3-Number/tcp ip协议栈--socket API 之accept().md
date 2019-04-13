
# tcp-ip协议栈--socket API 之accept() - 3-Number - CSDN博客


置顶2017年09月28日 15:43:01[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：383


# 0x01 缘由
前面博文已经学习了socket()\bind()，一切准备好后，开始可以接受客户端的连接请求，建立连接了。下面学习下accept API相关细节。
# 0x02 API介绍
![](https://img-blog.csdn.net/20170928154305564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在建立好接收队列以后，服务器就调用accept()，然后睡眠直到有客户端的连接请求到达。默认为阻塞API。
# 0x03 源码单步跟踪
环境：linux kernel 2.6.32
guest： 运行server host：设置断点
![](https://img-blog.csdn.net/20170928154335040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170928154349523?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 3.1 SYSCALL_DEFINE2
```python
/*
 *    系统调用向量
 */
SYSCALL_DEFINE2(socketcall, int, call, unsigned long __user *, args)
{
    unsigned long a[6];
    unsigned long a0, a1;
    int err;
    unsigned int len;
    if (call < 1 || call > SYS_ACCEPT4)
        return -EINVAL;
    len = nargs[call];
    if (len > sizeof(a))
        return -EINVAL;
    /* 用户空间复制相关参数 */
    if (copy_from_user(a, args, len))
        return -EFAULT;
    audit_socketcall(nargs[call] / sizeof(unsigned long), a);
    a0 = a[0];
    a1 = a[1];
    /* 根据call子调用号，来处理socket相关调用状态。*/
    switch (call) {
     ......
        case SYS_ACCEPT:
            /* a0 文件描述符  后面两个参数分别对应api描述中的地址和地址长度 */
            err = sys_accept4(a0, (struct sockaddr __user *)a1,
                      (int __user *)a[2], 0);
            break;
    .......
    default:
        err = -EINVAL;
        break;
    }
    return err;
}
```

## 3.2 SYSCALL_DEFINE4
```python
/*  尝试创建一个socket结构，与客户端请求连接，唤醒客户端，然后返回一个新的fd。我们在linux内核空间手机链接的地址，复制到用户空间。*/
SYSCALL_DEFINE4(accept4, int, fd, struct sockaddr __user *, upeer_sockaddr,
        int __user *, upeer_addrlen, int, flags)
{
    struct socket *sock, *newsock;  //新socket结构指针
    struct file *newfile;    //新文件描述符
    int err, len, newfd, fput_needed;
    struct sockaddr_storage address;
    /* 只允许使用这两个标志 */ 
    if (flags & ~(SOCK_CLOEXEC | SOCK_NONBLOCK))
        return -EINVAL;
    if (SOCK_NONBLOCK != O_NONBLOCK && (flags & SOCK_NONBLOCK))
        flags = (flags & ~SOCK_NONBLOCK) | O_NONBLOCK;
    /* 通过文件描述符查找对应的结构 */
    sock = sockfd_lookup_light(fd, &err, &fput_needed);
    if (!sock)
        goto out;
    err = -ENFILE;
    /*申请newsock结构指向newsock */
    if (!(newsock = sock_alloc()))
        goto out_put;
    /* 拷贝相关信息 */
    newsock->type = sock->type;
    newsock->ops = sock->ops;
    /* 这里我们不需要try_module_get，应为在listen环节已处理。
     */
    __module_get(newsock->ops->owner);
    /* 申请新的文件描述符 */
    newfd = sock_alloc_fd(&newfile, flags & O_CLOEXEC);
    if (unlikely(newfd < 0)) {
        err = newfd;
        sock_release(newsock);
        goto out_put;
    }
    /* 将新的文件描述符和socke进行关联。*/
    err = sock_attach_fd(newsock, newfile, flags & O_NONBLOCK);
    if (err < 0)
        goto out_fd_simple;
    /* SELinux相关 */ 
    err = security_socket_accept(sock, newsock);
    if (err)
        goto out_fd;
    /* SOCKET层的操作函数，如果是SOCK_STREAM，proto_ops为inet_stream_ops，
   * 接下来调用inet_accept()。
   */ 
    err = sock->ops->accept(sock, newsock, sock->file->f_flags);
    if (err < 0)
        goto out_fd;
    /* 如果要保存对端地址 */
    if (upeer_sockaddr) {
        /* 获取对端的地址，以及地址的长度 */
        if (newsock->ops->getname(newsock, (struct sockaddr *)&address,
                      &len, 2) < 0) {
            err = -ECONNABORTED;
            goto out_fd;
        }
        /* 把内核空间的socket地址复制到用户空间 */
        err = move_addr_to_user((struct sockaddr *)&address,
                    len, upeer_sockaddr, upeer_addrlen);
        if (err < 0)
            goto out_fd;
    }
    /*以newfd为索引，把newfile加入当前进程的文件描述符表files_struct中。*/
    fd_install(newfd, newfile);
    err = newfd;
out_put:
    fput_light(sock->file, fput_needed);
out:
    return err;
out_fd_simple:
    sock_release(newsock);
    put_filp(newfile);
    put_unused_fd(newfd);
    goto out_put;
out_fd:
    fput(newfile);
    put_unused_fd(newfd);
    goto out_put;
}
sys_accept4()主要做了：
1创建了一个新的socket和inode，以及它所对应的fd、file。
2调用Socket层操作函数inet_accept()。
3保存对端地址到指定的用户空间地址。
```

## 3.3 inet_accept
```python
/*inet_accept()主要做了：
1.调用TCP层的操作函数，获取已建立的连接sock。
2.把新socket和sock关联起来。
3.把新socket的状态设为SS_CONNECTED。*/
int inet_accept(struct socket *sock, struct socket *newsock, int flags)
{
    struct sock *sk1 = sock->sk;
    int err = -EINVAL;
    /* 如果使用的是TCP，则sk_prot为tcp_prot，accept为inet_csk_accept()
   * 获取新连接的sock。
   */ 
    struct sock *sk2 = sk1->sk_prot->accept(sk1, flags, &err);
    if (!sk2)
        goto do_err;
    lock_sock(sk2);
    WARN_ON(!((1 << sk2->sk_state) &
          (TCPF_ESTABLISHED | TCPF_CLOSE_WAIT | TCPF_CLOSE)));
 /* 把sock和socket嫁接起来，让它们能相互索引 */ 
    sock_graft(sk2, newsock);
    /* 把新socket的状态设为已连接 */ 
    newsock->state = SS_CONNECTED;
    err = 0;
    release_sock(sk2);
do_err:
    return err;
}
```

## 3.4 inet_csk_accept
```python
* 将接受下一个未完成的连接。
 * inet_csk_accept()用于从backlog队列（全连接队列）中取出一个ESTABLISHED状态的连接请求块，返回它所对应的连接sock。
 *    1.非阻塞的，且当前没有已建立的连接，则直接退出，返回-EAGAIN。
 *  2.阻塞的，且当前没有已建立的连接：
 *   2.1 用户没有设置超时时间，则无限期阻塞。
 *   2.2 用户设置了超时时间，超时后会退出。
 */
struct sock *inet_csk_accept(struct sock *sk, int flags, int *err)
{
    struct inet_connection_sock *icsk = inet_csk(sk); //连接
    struct sock *newsk;
    int error;
    lock_sock(sk);
    /* 必须保证socket处于监听状态。
     */
    error = -EINVAL;
    if (sk->sk_state != TCP_LISTEN)
        goto out_err;
    /*找已经建立的连接，也就是客户端和服务端完成了三次握手后，将连接放入队列。*/
    if (reqsk_queue_empty(&icsk->icsk_accept_queue)) {
        /* 没有新的连接 ，获取超时时间*/
        long timeo = sock_rcvtimeo(sk, flags & O_NONBLOCK);
        /* 如果是非阻塞socket则直接返回，不用等待 */
        error = -EAGAIN;
        if (!timeo)
            goto out_err;
        /* 阻塞等待，直到有全连接。如果用户有设置等待超时时间，超时后会退出 */ 
        error = inet_csk_wait_for_connect(sk, timeo);
        if (error)
            goto out_err;
    }
/* 获取新连接的sock，释放连接控制块 */ 
    newsk = reqsk_queue_get_child(&icsk->icsk_accept_queue, sk);
    WARN_ON(newsk->sk_state == TCP_SYN_RECV);
out:
    release_sock(sk);
    return newsk;
out_err:
    newsk = NULL;
    *err = error;
    goto out;
}
```

## 3.5 inet_csk_wait_for_connect
```python
/*等待一个进来的连接，为了避免竞争产生，此处必须对socket加锁。*/
static int inet_csk_wait_for_connect(struct sock *sk, long timeo)
{
    struct inet_connection_sock *icsk = inet_csk(sk);
    DEFINE_WAIT(wait);  /* 初始化等待任务 */ 
    int err;
    /*
    真正的唤醒-传入连接的机制：只有一个进程被唤醒，而不是“所有进程”。由于我们不再
    对已建立的套接字进行“竞争和轮询”，所以通常情况下只执行一次循环。 微妙问题：在
    任何当前的非排他性服务器之后，将添加“add_wait_queue_exclusive（）”，并且我们知
    道在任何新的非排他性服务器之后它总是_stay_，因为在等待队列的开头添加了所有非独占
    服务器。 因此，当我们被唤醒而不必删除并重新插入到等待队列中时，可以暂时“放弃”我
    们的排他性。
     */
    for (;;) {
        /* 把等待任务加入到socket的等待队列中，把进程状态设置为TASK_INTERRUPTIBLE */
        prepare_to_wait_exclusive(sk->sk_sleep, &wait,
                      TASK_INTERRUPTIBLE);
        /* 等下可能要阻塞休眠，先释放 */ 
        release_sock(sk);
        /* 如果全连接队列为空 */
        if (reqsk_queue_empty(&icsk->icsk_accept_queue))
            /* 进入睡眠，直到超时或收到信号 */
            timeo = schedule_timeout(timeo);
        /* 超时唤醒后重新上锁 */ 
        lock_sock(sk);
        err = 0;
        /* 全连接队列不为空时，退出 */
        if (!reqsk_queue_empty(&icsk->icsk_accept_queue))
            break;
        err = -EINVAL;
        /* 如果sock不处于监听状态了，退出 */
        if (sk->sk_state != TCP_LISTEN)
            break;
        /* 如果进程有待处理的信号，退出。
     * 因为timeo默认为MAX_SCHEDULE_TIMEOUT，所以err默认为-ERESTARTSYS。
     * 接下来会重新调用此函数，所以accept()依然阻塞。
     */ 
        err = sock_intr_errno(timeo);
        if (signal_pending(current))
            break;
        err = -EAGAIN;
        if (!timeo)
            break;
    }
    /*完成相关等待操作*/
    finish_wait(sk->sk_sleep, &wait);
    return err;
}
```

## 3.6 accept()的唤醒
当收到客户端的ACK后，经过如下调用：
```python
--tcp_v4_rcv
     --tcp_v4_do_rcv
          --tcp_child_process
               --sock_def_readable
                    --wake_up_interruptible_sync_poll
                         --__wake_up_sync_key
                              --__wake_up_common
```
最终调用我们给等待任务注册的唤醒函数。
## 3.7 唤醒后处理建立连接了
```python
void finish_wait(wait_queue_head_t *q, wait_queue_t *wait)
{
    unsigned long flags;
    __set_current_state(TASK_RUNNING);
    if (!list_empty_careful(&wait->task_list)) {
        spin_lock_irqsave(&q->lock, flags);
        list_del_init(&wait->task_list);  /* 从等待队列中删除，初始化此等待任务 */
        spin_unlock_irqrestore(&q->lock, flags);
    }
}
```

# 0x04 总结
经过以上服务端接收一个连接的操作过程已经完成了，没有直接参数tcp层相关操作，通过队列来做为消息来判断。下面分析客户端connect过程。

