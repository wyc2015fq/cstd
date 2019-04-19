# Linux的TUN/TAP编程 - Koma Hub - CSDN博客
2019年01月27日 12:17:34[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：70
个人分类：[Linux																[计算机网络																[TUN/TAP](https://blog.csdn.net/Rong_Toa/article/category/8644578)](https://blog.csdn.net/Rong_Toa/article/category/7569489)](https://blog.csdn.net/Rong_Toa/article/category/7156254)
TUN/TAP虚拟网络设备为用户空间程序提供了网络数据包的发送和接收能力。他既可以当做点对点设备（TUN），也可以当做以太网设备（TAP）。实际上，不仅Linux支持TUN/TAP虚拟网络设备，其他UNIX也是支持的，他们之间只有少许差别。
原理简介
TUN/TAP虚拟网络设备的原理比较简单，他在Linux内核中添加了一个TUN/TAP虚拟网络设备的驱动程序和一个与之相关连的字符设备/dev/net/tun，字符设备tun作为用户空间和内核空间交换数据的接口。当内核将数据包发送到虚拟网络设备时，数据包被保存在设备相关的一个队列中，直到用户空间程序通过打开的字符设备tun的描述符读取时，它才会被拷贝到用户空间的缓冲区中，其效果就相当于，数据包直接发送到了用户空间。通过系统调用write发送数据包时其原理与此类似。
值得注意的是：一次read系统调用，有且只有一个数据包被传送到用户空间，并且当用户空间的缓冲区比较小时，数据包将被截断，剩余部分将永久地消失，write系统调用与read类似，每次只发送一个数据包。所以在编写此类程序的时候，请用**足够大的缓冲区**，**直接调用系统调用read/write**，避免采用C语言的带缓存的IO函数。
准备工作
首先你需要一个能工作的Linux操作系统，并且内核支持TUN/TAP虚拟网络设备，如果没有，请在内核中选中：
Device Drivers => Network device support => Universal TUN/TAP device driver support
你可以选择编译进内核或者是编译成模块，然后重新编译内核并用新内核启动。如果你编译的是模块，那么在下步开始之前，你需要手工加载它。
root@gentux ~ # modprobe tun
开始编程
从代码开始，
```
#include <linux/if_tun.h>
int tun_create(char *dev, int flags)
{
			    struct ifreq ifr;
    int fd, err;
			    assert(dev != NULL);
			    if ((fd = open("/dev/net/tun", O_RDWR)) < 0)
        return fd;
    memset(&ifr, 0, sizeof(ifr));
			    ifr.ifr_flags |= flags;
    if (*dev != '\0')
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
			    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        close(fd);
        return err;
    }
			    strcpy(dev, ifr.ifr_name);
    return fd;
}
```
为了使用TUN/TAP设备，我们必须包含特定的头文件linux/if_tun.h，如12行所示。在21行，我们打开了字符设备/dev/net/tun。接下来我们需要为ioctl的TUNSETIFF命令初始化一个结构体ifr，一般的时候我们只需要关心其中的两个成员ifr_name, ifr_flags。ifr_name定义了要创建或者是打开的虚拟网络设备的名字，如果它为空或者是此网络设备不存在，内核将新建一个虚拟网络设备，并返回新建的虚拟网络设备的名字，同时文件描述符fd也将和此网络设备建立起关联。如果并没有指定网络设备的名字，内核将根据其类型自动选择tunXX和tapXX作为其名字。ifr_flags用来描述网络设备的一些属性，比如说是点对点设备还是以太网设备。详细的选项解释如下:
- IFF_TUN: 创建一个点对点设备
- IFF_TAP: 创建一个以太网设备
- IFF_NO_PI: 不包含包信息，默认的每个数据包当传到用户空间时，都将包含一个附加的包头来保存包信息
- IFF_ONE_QUEUE: 采用单一队列模式，即当数据包队列满的时候，由虚拟网络设备自已丢弃以后的数据包直到数据包队列再有空闲。
配置的时候，IFF_TUN和IFF_TAP必须择一，其他选项则可任意组合。其中IFF_NO_PI没有开启时所附加的包信息头如下:
```
struct tun_pi {
			    unsigned short flags;
			    unsigned short proto;
};
```
目前，flags只在收取数据包的时候有效，当它的TUN_PKT_STRIP标志被置时，表示当前的用户空间缓冲区太小，以致数据包被截断。proto成员表示发送/接收的数据包的协议。
上面代码中的文件描述符fd除了支持TUN_SETIFF和其他的常规ioctl命令外，还支持以下命令:
- TUNSETNOCSUM: 不做校验和校验。参数为int型的bool值。
- TUNSETPERSIST: 把对应网络设备设置成持续模式，默认的虚拟网络设备，当其相关的文件符被关闭时，也将会伴随着与之相关的路由等信息同时消失。如果设置成持续模式，那么它将会被保留供以后使用。参数为int型的bool值。
- TUNSETOWNER: 设置网络设备的属主。参数类型为uid_t。
- TUNSETLINK: 设置网络设备的链路类型，此命令只有在虚拟网络设备关闭的情况下有效。参数为int型。
一个小实例
```
int main(int argc, char *argv[])
{
			        int tun, ret;
			        char tun_name[IFNAMSIZ];
			        unsigned char buf[4096];
			        tun_name[0] = '\0';
			        tun = tun_create(tun_name, IFF_TUN | IFF_NO_PI);
			        if (tun < 0) {
			                perror("tun_create");
			                return 1;
			        }
			        printf("TUN name is %s\n", tun_name);
			        while (1) {
			                unsigned char ip[4];
			                ret = read(tun, buf, sizeof(buf));
			                if (ret < 0)
			                        break;
			                memcpy(ip, &buf[12], 4);
			                memcpy(&buf[12], &buf[16], 4);
			                memcpy(&buf[16], ip, 4);
			                buf[20] = 0;
			                *((unsigned short*)&buf[22]) += 8;
			                printf("read %d bytes\n", ret);
			                ret = write(tun, buf, ret);
			                printf("write %d bytes\n", ret);
			        }
			        return 0;
}
```
以上代码简答地处理了ICMP的ECHO包，并回应以ECHO REPLY。
首先运行这个程序:
root@gentux test # ./a.out
			TUN name is tun0
接着在另外一个终端运行如下命令:
root@gentux linux-2.6.15-gentoo # ifconfig tun0 0.0.0.0 up
			root@gentux linux-2.6.15-gentoo # route add 10.10.10.1 dev tun0
			root@gentux linux-2.6.15-gentoo # ping 10.10.10.1
			PING 10.10.10.1 (10.10.10.1) 56(84) bytes of data.
			64 bytes from 10.10.10.1: icmp_seq=1 ttl=64 time=1.09 ms
			64 bytes from 10.10.10.1: icmp_seq=2 ttl=64 time=5.18 ms
			64 bytes from 10.10.10.1: icmp_seq=3 ttl=64 time=3.37 ms
			--- 10.10.10.1 ping statistics ---
			3 packets transmitted, 3 received, 0% packet loss, time 2011ms
			rtt min/avg/max/mdev = 1.097/3.218/5.181/1.671 ms
可见，我们顺利地接受到了回应包，这时，切回到前一个终端下:
read 84 bytes
			write 84 bytes
			read 84 bytes
			write 84 bytes
			read 84 bytes
			write 84 bytes
一切正如我们所预想的那样。
TUN/TAP能做什么？
hoho，问这个问题似乎有些傻，你说一个网卡能做什么？我可以告诉你两个基于此的开源项目：[vtun](http://vtun.sourceforge.net/tun/)和[openvpn](http://openvpn.net/)，至于其他的应用，请自由发挥你的想像力吧！
**参考资料**:
[2.6.15内核源码](http://www.kernel.org/)
原文地址：[http://blog.chinaunix.net/uid-317451-id-92474.html](http://blog.chinaunix.net/uid-317451-id-92474.html)
附上一个GitHub上的项目代码：
```cpp
/* Copyright 2002 Jeff Dike
 * Licensed under the GPL
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_tun.h>
/* TUNSETGROUP appeared in 2.6.23 */
#ifndef TUNSETGROUP
#define TUNSETGROUP   _IOW('T', 206, int)
#endif
static void Usage(char *name)
{
  fprintf(stderr, "Create: %s [-b] [-u owner] [-g group] [-t device-name] "
          "[-f tun-clone-device]\n", name);
  fprintf(stderr, "Delete: %s -d device-name [-f tun-clone-device]\n\n",
          name);
  fprintf(stderr, "The default tun clone device is /dev/net/tun - some systems"                                                                                         
          " use\n/dev/misc/net/tun instead\n\n");
  fprintf(stderr, "-b will result in brief output (just the device name)\n");
  exit(1);
}
int main(int argc, char **argv)
{
  struct ifreq ifr;
  struct passwd *pw;
  struct group *gr;
  uid_t owner = -1;
  gid_t group = -1;
  int tap_fd, opt, delete = 0, brief = 0;
  char *tun = "", *file = "/dev/net/tun", *name = argv[0], *end;
  while((opt = getopt(argc, argv, "bd:f:t:u:g:")) > 0){
    switch(opt) {
      case 'b':
        brief = 1;
        break;
      case 'd':
        delete = 1;
        tun = optarg;
        break;
      case 'f':
        file = optarg;
        break;
      case 'u':
        pw = getpwnam(optarg);
        if(pw != NULL){
          owner = pw->pw_uid;
          break;
        }
        owner = strtol(optarg, &end, 0);
        if(*end != '\0'){
          fprintf(stderr, "'%s' is neither a username nor a numeric uid.\n",
                  optarg);
          Usage(name);
        }
        break;
      case 'g':
        gr = getgrnam(optarg);
        if(gr != NULL){
          group = gr->gr_gid;
          break;
        }
        group = strtol(optarg, &end, 0);
        if(*end != '\0'){
          fprintf(stderr, "'%s' is neither a groupname nor a numeric group.\n",
                  optarg);
          Usage(name);
        }
        break;
      case 't':
        tun = optarg;
        break;
      case 'h':
      default:
        Usage(name);
    }
  }
  argv += optind;
  argc -= optind;
  if(argc == 1)
    Usage(name);
  if((tap_fd = open(file, O_RDWR)) < 0){
    fprintf(stderr, "Failed to open '%s' : ", file);
    perror("");
    exit(1);
  }
  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  strncpy(ifr.ifr_name, tun, sizeof(ifr.ifr_name) - 1);
    if(0 != inet_aton("10.21.3.242", &((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr))
    {
        perror("inet_aton");
    }
  if(ioctl(tap_fd, TUNSETIFF, (void *) &ifr) < 0){
    perror("TUNSETIFF");
    exit(1);
  }
  if(delete){
    if(ioctl(tap_fd, TUNSETPERSIST, 0) < 0){
      perror("disabling TUNSETPERSIST");
      exit(1);
    }
    printf("Set '%s' nonpersistent\n", ifr.ifr_name);
  }
  else {
    /* emulate behaviour prior to TUNSETGROUP */
    if(owner == -1 && group == -1) {
      owner = geteuid();
    }
    if(owner != -1) {
      if(ioctl(tap_fd, TUNSETOWNER, owner) < 0){
        perror("TUNSETOWNER");
        exit(1);
      }
    }
    if(group != -1) {
      if(ioctl(tap_fd, TUNSETGROUP, group) < 0){
        perror("TUNSETGROUP");
        exit(1);
      }
    }
    if(ioctl(tap_fd, TUNSETPERSIST, 1) < 0){
      perror("enabling TUNSETPERSIST");
      exit(1);
    }
    if(brief)
      printf("%s\n", ifr.ifr_name);
    else {
      printf("Set '%s' persistent and owned by", ifr.ifr_name);
      if(owner != -1)
          printf(" uid %d", owner);
      if(group != -1)
          printf(" gid %d", group);
      printf("\n");
    }
  }
  return(0);
}
```
地址：[https://github.com/Prajithp/tapctl/blob/master/tapctl.c](https://github.com/Prajithp/tapctl/blob/master/tapctl.c)
