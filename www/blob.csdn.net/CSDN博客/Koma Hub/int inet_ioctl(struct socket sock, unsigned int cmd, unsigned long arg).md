# int inet_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg) - Koma Hub - CSDN博客
2019年01月24日 22:33:05[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：19
个人分类：[Linux kernel](https://blog.csdn.net/Rong_Toa/article/category/7348890)
这是一个来自linux内核4.20.1的代码段（af_inet.c）：
```cpp
/*
 *	ioctl() calls you can issue on an INET socket. Most of these are
 *	device configuration and stuff and very rarely used. Some ioctls
 *	pass on to the socket itself.
 *
 *	NOTE: I like the idea of a module for the config stuff. ie ifconfig
 *	loads the devconfigure module does its configuring and unloads it.
 *	There's a good 20K of config code hanging around the kernel.
 */
int inet_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	struct sock *sk = sock->sk;
	int err = 0;
	struct net *net = sock_net(sk);
	void __user *p = (void __user *)arg;
	struct ifreq ifr;
	struct rtentry rt;
	switch (cmd) {
	case SIOCGSTAMP:
		err = sock_get_timestamp(sk, (struct timeval __user *)arg);
		break;
	case SIOCGSTAMPNS:
		err = sock_get_timestampns(sk, (struct timespec __user *)arg);
		break;
	case SIOCADDRT:
	case SIOCDELRT:
		if (copy_from_user(&rt, p, sizeof(struct rtentry)))
			return -EFAULT;
		err = ip_rt_ioctl(net, cmd, &rt);
		break;
	case SIOCRTMSG:
		err = -EINVAL;
		break;
	case SIOCDARP:
	case SIOCGARP:
	case SIOCSARP:
		err = arp_ioctl(net, cmd, (void __user *)arg);
		break;
	case SIOCGIFADDR:
	case SIOCGIFBRDADDR:
	case SIOCGIFNETMASK:
	case SIOCGIFDSTADDR:
	case SIOCGIFPFLAGS:
		if (copy_from_user(&ifr, p, sizeof(struct ifreq)))
			return -EFAULT;
		err = devinet_ioctl(net, cmd, &ifr);
		if (!err && copy_to_user(p, &ifr, sizeof(struct ifreq)))
			err = -EFAULT;
		break;
	case SIOCSIFADDR:
	case SIOCSIFBRDADDR:
	case SIOCSIFNETMASK:
	case SIOCSIFDSTADDR:
	case SIOCSIFPFLAGS:
	case SIOCSIFFLAGS:
		if (copy_from_user(&ifr, p, sizeof(struct ifreq)))
			return -EFAULT;
		err = devinet_ioctl(net, cmd, &ifr);
		break;
	default:
		if (sk->sk_prot->ioctl)
			err = sk->sk_prot->ioctl(sk, cmd, arg);
		else
			err = -ENOIOCTLCMD;
		break;
	}
	return err;
}
EXPORT_SYMBOL(inet_ioctl);
```
