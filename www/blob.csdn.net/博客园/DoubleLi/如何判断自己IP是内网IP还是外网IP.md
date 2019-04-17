# 如何判断自己IP是内网IP还是外网IP - DoubleLi - 博客园






  tcp/ip协议中，专门保留了三个IP地址区域作为私有地址，其地址范围如下：

10.0.0.0/8：10.0.0.0～10.255.255.255 
　　172.16.0.0/12：172.16.0.0～172.31.255.255 
　　192.168.0.0/16：192.168.0.0～192.168.255.255

使用保留地址的网络只能在内部进行通信，而不能与其他网络互连。如果要与外部通信，那么必须通过网关与外部通信，这里使用了NAT, NAPT技术就是用来保证通信的代理机制。



       另外，一些宽带运营商尽管也使用了非私有地址分配给用户使用，但是由于路由设置的原因，Internet上的其他用户并不能访问到这些ip。上面2部分IP都可称为内网IP，下面这部分IP不列入本次讨论范围。



如果自己机器上网络接口的ip地址落在上述保留地址的范围内，则可以肯定自己处于内网模式下。

NAT要求整个服务的连接是从内网向外网主动发起的，而外网的用户无法直接（主动）向内网的服务发起连接请求，除非在NAT的（所有）网关上针对服务的端口作了端口映射。NAT方式要求最外围的网关至少有一个公网的IP，可以访问显IP的外部服务器如：[http://ipid.shat.net/](http://ipid.shat.net/) 获取到外部IP，将这个IP与自己机器上网络接口的ip比较，即可知道自己的ip是不是内网IP。



判断自己IP类型，可使用下面三种任意一种方法：

1)      在windos命令台程序下，用ipconfig。

Eg: 下面内网IP是192.168.0.1，外网IP是125.34.47.25，因此是网关。

C:/Documents and Settings/user>ipconfig



Windows IP Configuration

Ethernet adapter 本地连接:

        Connection-specific DNS Suffix . :

        IP Address. . . . . . . . . . . . : 192.168.0.1

        Subnet Mask . . . . . . . . . . . : 255.255.255.0

        Default Gateway . . . . . . . . . : 192.168.0.1



Ethernet adapter {6C8AEC26-0EC3-40FE-812E-A46778ECA752}:

        Media State . . . . . . . . . . . : Media disconnected



PPP adapter 宽带拨号:

        Connection-specific DNS Suffix . :

        IP Address. . . . . . . . . . . . : 125.34.47.25

        Subnet Mask . . . . . . . . . . . : 255.255.255.255

        Default Gateway . . . . . . . . . : 125.34.47.25



2)      用tracert来判断IP类型

如果第一个hops不是内网IP，那么自己就是外网IP了，反之，如果自己是内网IP，那么第一个hops显示的就是网关的内网IP，下面的例子显然说明是外网IP了。

C:/Documents and Settings/user>tracert www.baidu.com



Tracing route to www.a.shifen.com [202.108.22.5]

over a maximum of 30 hops:

 1    15 ms    16 ms    14 ms 125.34.40.1

 2   14 ms     *        *     61.148.8.9

 3    26 ms    72 ms    40 ms xd-22-5-a8.bta.net.cn [202.108.22.5]



Trace complete.



3）编程实现

获取到本机所有的IP地址列表，对IP列表进行分析：

1） 如果列表中只有局域网IP，那么说明是在内网；

2） 如果列表中有局域网IP，也有公网IP，那么说明是网关；

3） 如果列表中只有公网IP，那么说明是独立IP。

//此处不考虑其它平台，在inet架构下测试, 输入的ip为主机字节顺序

// 0xa -- "10.0.0.0">>24; 0xc0a8--"192.168.0.0.">>16; 0x2b0--"127.17.0.1">>22

int isInnerIP( uint32_t a_ip )

{

       int bValid = -1;

       if( (a_ip>>24 == 0xa) || (a_ip>>16 == 0xc0a8) || (a_ip>>22 == 0x2b0) )

       {

          bValid = 0;

       }

       return bValid;

}

int isInnerIP( char* a_strip )

{

       return 0;

}

**IP相关的应用**

//获取到本机所有的IP地址列表，并分别用字符串与整形形式来显示

int getHostIP()       //return int

{

       struct sockaddr_in localAddr, destAddr;

       struct hostent* h;

       char temp[128];



       int nRect = gethostname(temp, 128);

       printf("ipaddr src3 is: %s/n", temp);

       if(nRect !=0)

       {

              printf("error");

       }

       h = gethostbyname(temp);

       if(h)

       {

              for(int nAdapter=0; h->h_addr_list[nAdapter]; nAdapter++)

              {

                     memcpy(&destAddr.sin_addr.s_addr, h->h_addr_list[nAdapter], h->h_length);

                     // 输出机器的IP地址.

                     printf("Address string: %s/n", inet_ntoa(destAddr.sin_addr)); // 显示地址串

                     printf("Address int: %d/n", destAddr.sin_addr.s_addr); // 转化为整形数字

              }

       }

       return     0;

}



//检查字符串IP是否合法

int isCheckTrue(char* strip)

{

       int value;

       for( int i = 0; i < strlen(strip); i++)

       {

       // let's check if all entered char in entered

       // IP address are digits

              if(strip[i] == '.')

                     continue;



              if(isdigit(strip[i]) == 0)

              {

                     return -1;

              }

       }

       return 0;

}

//将字符串IP转化为整形IP

int str2intIP(char* strip) //return int ip

{

       int intIP;

       if(!(intIP = inet_addr(strip)))

       {

              perror("inet_addr failed./n");

              return -1;

       }

       return intIP;

}









