
# tcp-ip协议栈--socket API 之bind() - 3-Number - CSDN博客


置顶2017年09月28日 11:06:08[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：356


# 0x01 缘由
上篇博文已经学习了socket（） API，但是还不清楚创建的socket等相关结构如何使用。上篇文章仅仅创建了相关存储结构和文件描述符sockfd，并没有表示任何地址和端口。
# 0x02 bind()介绍
![](https://img-blog.csdn.net/20170928110726326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170928110749030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170928110805620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170928110822547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x03 单步跟踪分析
guest主机上运行server.c，此函数是通用的服务端模式，其中调用bind（）函数，同时在内核源码通用处理socket操作的函数中设置断点。
![](https://img-blog.csdn.net/20170928110837374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

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
    case SYS_BIND:
        err = sys_bind(a0, (struct sockaddr __user *)a1, a[2]); 设置断点，传入相关参数sockfd，地址端口等。
        break;
    .......
    default:
        err = -EINVAL;
        break;
    }
    return err;
}
```


## 3.2 SYSCALL_DEFINE3
```python
/* 第一个参数bind */
SYSCALL_DEFINE3(bind, int, fd, struct sockaddr __user *, umyaddr, int, addrlen)
{
    struct socket *sock;
    struct sockaddr_storage address; //内核态存储结构
    int err, fput_needed;
    /* 通过fd查找之前创建的socket,sock等结构，此处就知道与上篇文章如何关联的。*/
    sock = sockfd_lookup_light(fd, &err, &fput_needed);
    if (sock) {
        /* 将地址从用户态拷贝到内核态 */
        err = move_addr_to_kernel(umyaddr, addrlen, (struct sockaddr *)&address);
        if (err >= 0) {
            /* 相关安全机制 SELInux相关 */
            err = security_socket_bind(sock,
                           (struct sockaddr *)&address,
                           addrlen);
            if (!err)
                /* 调用初始化注册的相关回调操作函数。此处为tcp协议，则调用inet_bind*/
                err = sock->ops->bind(sock,
                              (struct sockaddr *)
                              &address, addrlen);
        }
        fput_light(sock->file, fput_needed);
    }
    return err;
}
```

## 3.3 inet_bind
```python
int inet_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
    struct sockaddr_in *addr = (struct sockaddr_in *)uaddr;
    struct sock *sk = sock->sk;
    struct inet_sock *inet = inet_sk(sk);
    unsigned short snum;
    int chk_addr_ret;
    int err;
    /* 调用协议本身的 bind函数(即 struct sock 的成员 sk_prot->bind)，但 UDP 和 TCP 协议本身不提供 bind 函数。 */
    if (sk->sk_prot->bind) {
        err = sk->sk_prot->bind(sk, uaddr, addr_len);
        goto out;
    }
    err = -EINVAL;
    /* 地址长度检查 */
    if (addr_len < sizeof(struct sockaddr_in))
        goto out;
    /* 查找地址类型，就像系统中只存在“dev”一样。如果on_dev为NULL，则会考虑所有接口。在路由中检查IP地址类型，单播、多播还是广播*/
    chk_addr_ret = inet_addr_type(sock_net(sk), addr->sin_addr.s_addr);
    err = -EADDRNOTAVAIL;
    /* sysctl_ip_nonlocal_bind表示是否允许绑定非本地的IP地址
     * inet->freebind表示是否允许绑定非主机地址。
     * 这里需要允许绑定非本地地址，除非是发送给自己、多播或广播。
     */
    if (!sysctl_ip_nonlocal_bind &&
        !(inet->freebind || inet->transparent) &&
        addr->sin_addr.s_addr != htonl(INADDR_ANY) &&
        chk_addr_ret != RTN_LOCAL &&
        chk_addr_ret != RTN_MULTICAST &&
        chk_addr_ret != RTN_BROADCAST)
        goto out;
    /*端口地址，网络序转主机序*/
    snum = ntohs(addr->sin_port);
    err = -EACCES;
    /*端口合理范围检查，0-1023 号端口是只有超级用户才有权限执行绑定的。*/
    if (snum && snum < PROT_SOCK && !capable(CAP_NET_BIND_SERVICE))
        goto out;
    /* 我们保持一个地址对：接收地址和发送地址。接收地址是通过hash查询，源地址是来作为传输。*/
    /* 锁结构*/
    lock_sock(sk);
    /* 如果套接字不在初始状态TCP_CLOSE，或者已经绑定端口了，则出错。一个socket最多可以绑定一个端口，而一个端口则可能被多个socket共用。 */
    err = -EINVAL;
    if (sk->sk_state != TCP_CLOSE || inet->num)
        goto out_release_sock;
    /*地址相关赋值*/
    inet->rcv_saddr = inet->saddr = addr->sin_addr.s_addr;
    if (chk_addr_ret == RTN_MULTICAST || chk_addr_ret == RTN_BROADCAST)
        inet->saddr = 0;  /* Use device */
    /*这儿确定能够绑定，此处为tcp协议，调用int inet_csk_get_port(struct sock *sk, unsigned short snum)端口可用返回0。
 */
    if (sk->sk_prot->get_port(sk, snum)) {
        inet->saddr = inet->rcv_saddr = 0;
        err = -EADDRINUSE;
        goto out_release_sock;
    }
    /* inet_rcv_saddr表示绑定的地址，接收数据时用于查找socket */ 
    if (inet->rcv_saddr)
        sk->sk_userlocks |= SOCK_BINDADDR_LOCK; /* 表示绑定了本地地址 */ 
    if (snum)
        sk->sk_userlocks |= SOCK_BINDPORT_LOCK; /* 表示绑定了本地端口 */ 
    inet->sport = htons(inet->num);/* 绑定端口转网络序 */ 
    inet->daddr = 0;
    inet->dport = 0;
    sk_dst_reset(sk);
    err = 0;
out_release_sock:
    release_sock(sk);
out:
    return err;
}
```

## 3.4 inet_csk_get_port
当指定端口时绑定到指定端口，当参数为0时随机选出一个可用端口。
此函数的介绍传送到前辈：[http://blog.csdn.net/zhangskd/article/details/14170013](http://blog.csdn.net/zhangskd/article/details/14170013)
# 0x04 总结
此处将socket和bind函数串联起来了，相当于相关结构已经建立。下一章讲解listen调用。（学习过程，大神勿喷，多指点）


