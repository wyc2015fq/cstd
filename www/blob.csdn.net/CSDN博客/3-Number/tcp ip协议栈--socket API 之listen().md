
# tcp-ip协议栈--socket API 之listen() - 3-Number - CSDN博客


置顶2017年09月28日 12:07:36[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：185标签：[inet_csk_listen_star																](https://so.csdn.net/so/search/s.do?q=inet_csk_listen_star&t=blog)[listen																](https://so.csdn.net/so/search/s.do?q=listen&t=blog)[inet_listen																](https://so.csdn.net/so/search/s.do?q=inet_listen&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=listen&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=inet_csk_listen_star&t=blog)个人分类：[Linux 网络协议栈																](https://blog.csdn.net/pangyemeng/article/category/7130138)
[
																								](https://so.csdn.net/so/search/s.do?q=inet_csk_listen_star&t=blog)


# 0x01 缘由
上篇博文介绍了bind的连接，了解了相关细节，这章继续学习socket API ，这篇关注listen。listen几个关键参数和队列是经常被面试官提出的问题。
# 0x02 API介绍
![](https://img-blog.csdn.net/20170928120923044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**全连接队列的最大长度：**
backlog保存的是完成三次握手、等待accept的全连接，而不是半连接。
负载不高时，backlog不用太大。(For complete connections)
系统最大的、未处理的全连接数量为：min(backlog, somaxconn)，net.core.somaxconn默认为128。
这个值最终存储于sk->sk_max_ack_backlog。
**半连接队列的最大长度：**
tcp_max_syn_backlog默认值为256。(For incomplete connections)
当使用SYN Cookie时，这个参数变为无效。
半连接队列的最大长度为backlog、somaxconn、tcp_max_syn_backlog的最小值。
# 0x03 单步跟踪分析
![](https://img-blog.csdn.net/20170928120941768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

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
    case SYS_LISTEN:
        /* a0 = 3,a1 = 20 是在server.c代码中设设置队列一样*/
        err = sys_listen(a0, a1);
        break;
    .......
    default:
        err = -EINVAL;
        break;
    }
    return err;
}
```
![](https://img-blog.csdn.net/20170928121002056?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 3.2 SYSCALL_DEFINE2  listen
```python
SYSCALL_DEFINE2(listen, int, fd, int, backlog)
{
    struct socket *sock;
    int err, fput_needed;
    int somaxconn;
    sock = sockfd_lookup_light(fd, &err, &fput_needed);
    if (sock) {
        /*
        * [root@B200-45 test]# sysctl -a | grep somaxconn
        *    net.core.somaxconn = 128
        */
        somaxconn = sock_net(sock->sk)->core.sysctl_somaxconn;
        /*如果backlog大于somaxconn则默认为somaxconn。k可以通过调整相关参数来提高相关连接数*/
        if ((unsigned)backlog > somaxconn)
            backlog = somaxconn;
        /*SELInux相关 */
        err = security_socket_listen(sock, backlog);
        if (!err)
            /*如果tcp,调用inet_listen*/
            err = sock->ops->listen(sock, backlog);
        /* 将相关参数放入*/
        fput_light(sock->file, fput_needed);
    }
    return err;
}
```

## 3.3 inet_listen
```python
/*
    启动监听时，做的工作主要包括：
    1.创建半连接队列的实例，初始化全连接队列。
    2.初始化sock的一些变量，把它的状态设为TCP_LISTEN。
    3.检查端口是否可用，防止bind()后其它进程修改了端口信息。
    4.把sock链接进入监听哈希表listening_hash中。
*/
int inet_csk_listen_start(struct sock *sk, const int nr_table_entries)
{
    struct inet_sock *inet = inet_sk(sk);
    struct inet_connection_sock *icsk = inet_csk(sk);
    /* 初始化全连接队列，创建半连接队列的实例 */
    int rc = reqsk_queue_alloc(&icsk->icsk_accept_queue, nr_table_entries);
    if (rc != 0)
        return rc;
    /* 在返回inet_listen()时赋值 *
    sk->sk_max_ack_backlog = 0;
    sk->sk_ack_backlog = 0;
    /* icsk->icsk_ack c初始化清零 */
    inet_csk_delack_init(sk);
    /* There is race window here: we announce ourselves listening,
        此处有一个竞争窗口：我们宣告我们正在监听，但是这个事务仍然没有被get_port校验。
        这是可以的，因为这个套接字只有在验证完成后才进入哈希表。
     */
    sk->sk_state = TCP_LISTEN; /* 把sock的状态置为LISTEN */ 
    if (!sk->sk_prot->get_port(sk, inet->num)) {
        inet->sport = htons(inet->num); //源端口
        sk_dst_reset(sk);
        /*要么把自己加入到 tcp_hashinfo 中的 ehash 中，要么加入到 listening_hash 中，这要根据
            sk_state 的值来操作，如果是 LISTEN，就加入后者，如果是除 LISTEN 之外的值，那么就加入
            ehash 表，我们会在研究 connect 的代码中看到。*/
        sk->sk_prot->hash(sk);/* 把sock链接入监听哈希表中 */
        return 0;
    }
    sk->sk_state = TCP_CLOSE;
    /* 如果端口不可用，则释放半连接队列 */ 
    __reqsk_queue_destroy(&icsk->icsk_accept_queue);
    return -EADDRINUSE;
}
```
相关可参考：[http://blog.csdn.net/zhangskd/article/details/14446581](http://blog.csdn.net/zhangskd/article/details/14446581)
# 0x04 总结
关键参数backlog再次强调配张图：
![](https://img-blog.csdn.net/20170928121046008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



