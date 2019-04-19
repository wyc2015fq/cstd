# INET Sockets Tour: ioctl() - Koma Hub - CSDN博客
2019年01月26日 10:06:19[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：21
个人分类：[Linux kernel																[Socket](https://blog.csdn.net/Rong_Toa/article/category/7792858)](https://blog.csdn.net/Rong_Toa/article/category/7348890)
**目录**
[arch/i386/kernel/entry.S](#arch%2Fi386%2Fkernel%2Fentry.S)
[fs/ioctl.c/sys_ioctl()](#fs%2Fioctl.c%2Fsys_ioctl())
[net/socket.c/sock_ioctl()](#net%2Fsocket.c%2Fsock_ioctl())
[net/ipv4/af_inet.c](#net%2Fipv4%2Faf_inet.c)
[net/ipv4/af_inet.c/inet_ioctl()](#net%2Fipv4%2Faf_inet.c%2Finet_ioctl())
[net/ipv4/devinet.c/devinet_ioctl()](#net%2Fipv4%2Fdevinet.c%2Fdevinet_ioctl())
[net/core/dev.c/dev_ioctl()](#net%2Fcore%2Fdev.c%2Fdev_ioctl())
[net/core/dev.c/dev_ifsioc()](#net%2Fcore%2Fdev.c%2Fdev_ifsioc())
[net/ipv4/fib_frontend.c/ip_rt_ioctl()](#net%2Fipv4%2Ffib_frontend.c%2Fip_rt_ioctl())
**![](http://www.skyfree.org/linux/icons/RedDiamond.gif) ioctl() tour**
**![](http://www.skyfree.org/linux/icons/GreenTriangle.gif) Anatomy of ioctl() thread**
PLEASE WAIT FOR THE EXPLANATION.
## arch/i386/kernel/entry.S
```cpp
ENTRY(sys_call_table)
.....
        .long SYMBOL_NAME(sys_ioctl)         /* 54 */
```
## fs/ioctl.c/sys_ioctl()
```cpp
asmlinkage int sys_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg)
{
        struct file * filp;
        unsigned int flag;
        int on, error = -EBADF;
        lock_kernel();
        filp = fget(fd);
        if (!filp)
                goto out;
        error = 0;
        switch (cmd) {
.....
                default:
.....
                        else if (filp->f_op && filp->f_op->ioctl)
                                error = filp->f_op->ioctl(filp->f_dentry->d_inode, filp, cmd, arg);
        }
        fput(filp);
out:
        unlock_kernel();
        return error;
}
```
## net/socket.c/sock_ioctl()
```cpp
/*
 *      With an ioctl arg may well be a user mode pointer, but we don't know what to do
 *      with it - that's up to the protocol still.
 */
int sock_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
           unsigned long arg)
{
        struct socket *sock = socki_lookup(inode);
        return sock->ops->ioctl(sock, cmd, arg);
}
```
## net/ipv4/af_inet.c
```cpp
struct proto_ops inet_stream_ops = {
        PF_INET,
        sock_no_dup,
        inet_release,
        inet_bind,
        inet_stream_connect,
        sock_no_socketpair,
        inet_accept,
        inet_getname,
        inet_poll,
        inet_ioctl,
        inet_listen,
        inet_shutdown,
        inet_setsockopt,
        inet_getsockopt,
        sock_no_fcntl,
        inet_sendmsg,
        inet_recvmsg
};
struct proto_ops inet_dgram_ops = {
        PF_INET,
        sock_no_dup,
        inet_release,
        inet_bind,
        inet_dgram_connect,
        sock_no_socketpair,
        sock_no_accept,
        inet_getname,
        datagram_poll,
        inet_ioctl,
        sock_no_listen,
        inet_shutdown,
        inet_setsockopt,
        inet_getsockopt,
        sock_no_fcntl,
        inet_sendmsg,
        inet_recvmsg
};
```
## net/ipv4/af_inet.c/inet_ioctl()
```cpp
static int inet_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
        struct sock *sk = sock->sk;
        int err;
        int pid;
        switch(cmd)
        {
.....
                case SIOCADDRT:
                case SIOCDELRT:
                case SIOCRTMSG:
                        return(ip_rt_ioctl(cmd,(void *) arg));
                case SIOCDARP:
                case SIOCGARP:
                case SIOCSARP:
                        return(arp_ioctl(cmd,(void *) arg));
.....
                case SIOCGIFADDR:
                case SIOCSIFADDR:
                case SIOCGIFBRDADDR:
                case SIOCSIFBRDADDR:
                case SIOCGIFNETMASK:
                case SIOCSIFNETMASK:
                case SIOCGIFDSTADDR:
                case SIOCSIFDSTADDR:
                case SIOCSIFPFLAGS:
                case SIOCGIFPFLAGS:
                case SIOCSIFFLAGS:
                        return(devinet_ioctl(cmd,(void *) arg));
.....
                default:
//              case SIOCGIFFLAGS:
//              case SIOCSIFHWADDR:
//              case SIOCGIFHWADDR:
// etc.
.....
                        if (sk->prot->ioctl==NULL || (err=sk->prot->ioctl(sk, cmd, arg))==-ENOIOCTLC
MD)
                                return(dev_ioctl(cmd,(void *) arg));
                        return err;
        }
        /*NOTREACHED*/
        return(0);
}
```
## net/ipv4/devinet.c/devinet_ioctl()
```cpp
int devinet_ioctl(unsigned int cmd, void *arg)
{
        struct ifreq ifr;
        struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
        struct in_device *in_dev;
        struct in_ifaddr **ifap = NULL;
        struct in_ifaddr *ifa = NULL;
        struct device *dev;
.....
        switch(cmd) {
        case SIOCGIFADDR:       /* Get interface address */
        case SIOCGIFBRDADDR:    /* Get the broadcast address */
        case SIOCGIFDSTADDR:    /* Get the destination address */
        case SIOCGIFNETMASK:    /* Get the netmask for the interface */
                /* Note that this ioctls will not sleep,
                   so that we do not impose a lock.
                   One day we will be forced to put shlock here (I mean SMP)
                 */
                memset(sin, 0, sizeof(*sin));
                sin->sin_family = AF_INET;
                break;
        case SIOCSIFFLAGS:
                if (!capable(CAP_NET_ADMIN))
                        return -EACCES;
                rtnl_lock();
                exclusive = 1;
                break;
        case SIOCSIFADDR:       /* Set interface address (and family) */
        case SIOCSIFBRDADDR:    /* Set the broadcast address */
        case SIOCSIFDSTADDR:    /* Set the destination address */
        case SIOCSIFNETMASK:    /* Set the netmask for the interface */
                if (!capable(CAP_NET_ADMIN))
                        return -EACCES;
                if (sin->sin_family != AF_INET)
                        return -EINVAL;
                rtnl_lock();
                exclusive = 1;
                break;
        default:
                return -EINVAL;
        }
.....
        switch(cmd) {
                case SIOCGIFADDR:       /* Get interface address */
                        sin->sin_addr.s_addr = ifa->ifa_local;
                        goto rarok;
                case SIOCGIFBRDADDR:    /* Get the broadcast address */
                        sin->sin_addr.s_addr = ifa->ifa_broadcast;
                        goto rarok;
                case SIOCGIFDSTADDR:    /* Get the destination address */
                        sin->sin_addr.s_addr = ifa->ifa_address;
                        goto rarok;
                case SIOCGIFNETMASK:    /* Get the netmask for the interface */
                        sin->sin_addr.s_addr = ifa->ifa_mask;
                        goto rarok;
                case SIOCSIFFLAGS:
.....
                        ret = dev_change_flags(dev, ifr.ifr_flags);
                        break;
                case SIOCSIFADDR:       /* Set interface address (and family) */
.....
                        ifa->ifa_address =
                        ifa->ifa_local = sin->sin_addr.s_addr;
.....
                        ret = inet_set_ifa(dev, ifa);
                        break;
                case SIOCSIFBRDADDR:    /* Set the broadcast address */
.....
                                ifa->ifa_broadcast = sin->sin_addr.s_addr;
                                inet_insert_ifa(in_dev, ifa);
                        }
                        break;
                case SIOCSIFDSTADDR:    /* Set the destination address */
.....
                                ifa->ifa_address = sin->sin_addr.s_addr;
                                inet_insert_ifa(in_dev, ifa);
                        }
                        break;
                case SIOCSIFNETMASK:    /* Set the netmask for the interface */
.....
                                ifa->ifa_mask = sin->sin_addr.s_addr;
                                ifa->ifa_prefixlen = inet_mask_len(ifa->ifa_mask);
                                inet_set_ifa(dev, ifa);
                        }
                        break;
        }
done:
        if (exclusive)
                rtnl_unlock();
        return ret;
rarok:
        if (copy_to_user(arg, &ifr, sizeof(struct ifreq)))
                return -EFAULT;
        return 0;
}
```
## net/core/dev.c/dev_ioctl()
```cpp
/*
 *      This function handles all "interface"-type I/O control requests. The actual
 *      'doing' part of this is dev_ifsioc above.
 */
int dev_ioctl(unsigned int cmd, void *arg)
{
        struct ifreq ifr;
        int ret;
        char *colon;
.....
        if (copy_from_user(&ifr, arg, sizeof(struct ifreq)))
                return -EFAULT;
.....
        /*
         *      See which interface the caller is talking about.
         */
        switch(cmd)
        {
                /*
                 *      These ioctl calls:
                 *      - can be done by all.
                 *      - atomic and do not require locking.
                 *      - return a value
                 */
                case SIOCGIFFLAGS:
.....
                case SIOCGIFHWADDR:
.....
                        dev_load(ifr.ifr_name);
                        ret = dev_ifsioc(&ifr, cmd);
                        if (!ret) {
                                if (colon)
                                        *colon = ':';
                                if (copy_to_user(arg, &ifr, sizeof(struct ifreq)))
                                        return -EFAULT;
                        }
                        return ret;
                /*
                 *      These ioctl calls:
                 *      - require superuser power.
                 *      - require strict serialization.
                 *      - do not return a value
                 */
.....
                case SIOCSIFHWADDR:
.....
                        if (!capable(CAP_NET_ADMIN))
                                return -EPERM;
                        dev_load(ifr.ifr_name);
                        rtnl_lock();
                        ret = dev_ifsioc(&ifr, cmd);
                        rtnl_unlock();
                        return ret;
.....
        }
}
```
## net/core/dev.c/dev_ifsioc()
```cpp
/*
 *      Perform the SIOCxIFxxx calls.
 */
static int dev_ifsioc(struct ifreq *ifr, unsigned int cmd)
{
        struct device *dev;
        int err;
        if ((dev = dev_get(ifr->ifr_name)) == NULL)
                return -ENODEV;
        switch(cmd)
        {
                case SIOCGIFFLAGS:      /* Get interface flags */
                        ifr->ifr_flags = (dev->flags&~(IFF_PROMISC|IFF_ALLMULTI))
                                |(dev->gflags&(IFF_PROMISC|IFF_ALLMULTI));
                        return 0;
.....
                case SIOCGIFHWADDR:
                        memcpy(ifr->ifr_hwaddr.sa_data,dev->dev_addr, MAX_ADDR_LEN);
                        ifr->ifr_hwaddr.sa_family=dev->type;
                        return 0;
                case SIOCSIFHWADDR:
                        if(dev->set_mac_address==NULL)
                                return -EOPNOTSUPP;
                        if(ifr->ifr_hwaddr.sa_family!=dev->type)
                                return -EINVAL;
                        err=dev->set_mac_address(dev,&ifr->ifr_hwaddr);
                        if (!err)
                                notifier_call_chain(&netdev_chain, NETDEV_CHANGEADDR, dev);
                        return err;
.....
        }
}
```
## net/ipv4/fib_frontend.c/ip_rt_ioctl()
```cpp
/*
 *      Handle IP routing ioctl calls. These are used to manipulate the routing tables
 */
int ip_rt_ioctl(unsigned int cmd, void *arg)
{
        int err;
        struct kern_rta rta;
        struct rtentry  r;
        struct {
                struct nlmsghdr nlh;
                struct rtmsg    rtm;
        } req;
        switch (cmd) {
        case SIOCADDRT:         /* Add a route */
        case SIOCDELRT:         /* Delete a route */
                if (!capable(CAP_NET_ADMIN))
                        return -EPERM;
                if (copy_from_user(&r, arg, sizeof(struct rtentry)))
                        return -EFAULT;
                rtnl_lock();
                err = fib_convert_rtentry(cmd, &req.nlh, &req.rtm, &rta, &r);
                if (err == 0) {
                        if (cmd == SIOCDELRT) {
                                struct fib_table *tb = fib_get_table(req.rtm.rtm_table);
                                err = -ESRCH;
                                if (tb)
                                        err = tb->tb_delete(tb, &req.rtm, &rta, &req.nlh, NULL);
                        } else {
                                struct fib_table *tb = fib_new_table(req.rtm.rtm_table);
                                err = -ENOBUFS;
                                if (tb)
                                        err = tb->tb_insert(tb, &req.rtm, &rta, &req.nlh, NULL);
                        }
                        if (rta.rta_mx)
                                kfree(rta.rta_mx);
                }
                rtnl_unlock();
                return err;
        }
        return -EINVAL;
}
```
To be continued.
[http://www.skyfree.org/linux/kernel_network/ioctl.html](http://www.skyfree.org/linux/kernel_network/ioctl.html)
