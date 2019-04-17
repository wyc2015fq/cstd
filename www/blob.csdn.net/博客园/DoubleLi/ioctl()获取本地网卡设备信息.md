# ioctl()获取本地网卡设备信息 - DoubleLi - 博客园






获得eth0接口所有信息:

- #include <stdio.h>
- #include <stdlib.h>
- #include <sys/types.h>
- #include <sys/stat.h>
- #include <unistd.h>
- #include <sys/ioctl.h>
- #include <sys/socket.h>
- #include <netinet/in.h>
- #include <arpa/inet.h>
- #include <net/if.h>
- #include <string.h>
- 
- unsigned char g_eth_name[16];
- unsigned char g_macaddr[16];
- unsigned int g_subnetmask;
- unsigned int g_ipaddr;
- unsigned int g_broadcast_ipaddr;
- 
- //初始化网络，获取当前网络设备信息
- void init_net(void)
- {
-     int i ;
-     int sock;
-     struct sockaddr_in sin;
-     struct ifreq ifr;
- 
-     sock=socket(AF_INET, SOCK_DGRAM, 0);
-     if(sock==-1)
-         perror("socket");
-     strcpy(g_eth_name, "eth0");
-     strcpy(ifr.ifr_name, g_eth_name);
-     printf("eth name:\t%s\n", g_eth_name);
- 
-     //获取并打印网卡地址
-     if(ioctl(sock, SIOCGIFHWADDR, &ifr) <0 )
-         perror("ioctl error\n");
-     memcpy(g_macaddr, ifr.ifr_hwaddr.sa_data, 6);
-     printf("local mac:\t");
-     for(i=0;i<5;i++)
-         printf("%.2x:", g_macaddr[i]);
-     printf("%.2x\n", g_macaddr[i]);
- 
-     //获取并打印IP地址
-     if(ioctl(sock, SIOCGIFADDR, &ifr)<0)
-         perror("ioctl error\n");
-     memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
-     g_ipaddr = sin.sin_addr.s_addr;
-     printf("local eth0:\t%s\n", inet_ntoa(sin.sin_addr));
- 
-     //获取并打印广播地址
-     if(ioctl(sock, SIOCGIFBRDADDR, &ifr)<0)
-         perror("ioctl error\n");
-     memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
-     g_broadcast_ipaddr = sin.sin_addr.s_addr;
-     printf("broadcast:\t%s\n", inet_ntoa(sin.sin_addr));
- 
-     //获取并打印子网掩码
-     if(ioctl(sock,SIOCGIFNETMASK,&ifr)<0)
-         perror("ioctl error\n");
-     memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
-     g_subnetmask = sin.sin_addr.s_addr;
-     printf("subnetmask:\t%s\n", inet_ntoa(sin.sin_addr));
- 
-     close(sock); 
- }
- 
- int main()
- {
-     //initialize
-     init_net();
-     //do something
-     //....
-     return 0;
- }



获取所有IP:

- #include <string.h>
- #include <sys/ioctl.h>
- #include <sys/socket.h>
- #include <stdlib.h>
- #include <stdio.h>
- #include <linux/if.h>
- #include <arpa/inet.h>
- 
- int main()
- {
-     int i=0;
-     int sockfd;
-     struct ifconf ifconf;
-     unsigned char buf[512];
-     struct ifreq *ifreq;
-     //初始化ifconf
-     ifconf.ifc_len = 512;
-     ifconf.ifc_buf = buf;
- 
-     if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
-     {
-         perror("socket open failure!\n" );
-         exit(1);
-     }
- 
-     ioctl(sockfd, SIOCGIFCONF, &ifconf); //获取所有接口信息
-     //接下来一个一个的获取IP地址
-     ifreq = (struct ifreq*)buf;
-     for (i=(ifconf.ifc_len/sizeof (struct ifreq)); i>0; i--)
-     {
-         if(ifreq->ifr_flags == AF_INET)
-         {
-             printf("name = [%s] " , ifreq->ifr_name);
-             printf("local addr = [%s]",
-                 inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
-             printf();
-         ifreq++;
-         }
-     }
-     return 0;
- }




获取本机的IP和MAC地址:


- /* 得到本机的mac地址和ip地址 */
- int GetLocalMac ( const char *device,char *mac,char *ip )
- {
-     int sockfd;
-     struct ifreq req;
-     struct sockaddr_in * sin;
- 
-     if ( ( sockfd = socket ( PF_INET,SOCK_DGRAM,0 ) ) ==-1 )
-     {
-         fprintf ( stderr,"Sock Error:%s\n\a",strerror ( errno ) );
-         return ( -1 );
-     }
- 
-     memset ( &req,0,sizeof ( req ) );
-     strcpy ( req.ifr_name,device );
-     if ( ioctl ( sockfd,SIOCGIFHWADDR, ( char * ) &req ) ==-1 )
-     {
-         fprintf ( stderr,"ioctl SIOCGIFHWADDR:%s\n\a",strerror ( errno ) );
-         close ( sockfd );
-         return ( -1 );
-     }
-     memcpy ( mac,req.ifr_hwaddr.sa_data,6 );
- 
-     req.ifr_addr.sa_family = PF_INET;
-     if ( ioctl ( sockfd,SIOCGIFADDR, ( char * ) &req ) ==-1 )
-     {
-         fprintf ( stderr,"ioctl SIOCGIFADDR:%s\n\a",strerror ( errno ) );
-         close ( sockfd );
-         return ( -1 );
-     }
-     sin = ( struct sockaddr_in * ) &req.ifr_addr;
-     memcpy ( ip, ( char * ) &sin->sin_addr,4 );
- 
-     return ( 0 );
- }











