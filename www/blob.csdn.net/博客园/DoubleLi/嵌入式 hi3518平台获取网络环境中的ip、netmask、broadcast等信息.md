# 嵌入式 hi3518平台获取网络环境中的ip、netmask、broadcast等信息 - DoubleLi - 博客园







**[html]**[view plain](http://blog.csdn.net/skdkjzz/article/details/40427153)[copy](http://blog.csdn.net/skdkjzz/article/details/40427153)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <span style="font-family:Courier New;">  
- /********************************** (C) COPYRIGHT *******************************  
-  * File Name          : get_netinfo.c  
-  * Author              : skdkjzz  
-  * Date                 : 2014/08/07  
-  * Description        : linux下获取网卡信息  
-  *********************************************************************************/  
- 
- #include <stdio.h>  
- #include <stdlib.h>  
- #include <string.h>  
- #include <sys/types.h>  
- #include <sys/socket.h>  
- #include <netinet/in.h>  
- #include <linux/if.h>  
- #include <arpa/inet.h>  
- #include <linux/sockios.h>  
- 
- int main(int argc,char*argv[]){  
-     int s;  
-     int err;  
- s=socket(AF_INET,SOCK_DGRAM,0);  
-     if(s<0){  
-         perror("socket error");  
-         return;  
-     }  
- 
-     struct ifreq ifr;  
- #if 1  
-     memset(&ifr,0,sizeof(ifr));  
- ifr.ifr_ifindex=1;    /* 获得第1个网络接口的名称 */  
- 
- err=ioctl(s,SIOCGIFNAME,&ifr);  
-     if(err)  
-     {  
-         perror("index error");  
-     }else  
-     {  
-         printf("----------------------------------\n");  
-         printf("The %dst net device is : %s\n",ifr.ifr_ifindex,ifr.ifr_name);  
-     }  
- 
- err=ioctl(s,SIOCGIFHWADDR,&ifr);   /* 获得MAC地址 */  
-     printf("%s:[%d] err is %d\n",__FUNCTION__,__LINE__,err);  
-     if(!err)  
-     {  
-         unsigned char* hw=ifr.ifr_hwaddr.sa_data;  
-         printf("Mac address is : %02x:%02x:%02x:%02x:%02x:%02x\n",hw[0],hw[1],hw[2],hw[3],hw[4],hw[5]);  
-     }  
-     else  
-     {  
-         printf("Mac address is : 00:00:00:00:00:00\n");  
-     }  
- 
-     struct sockaddr_in *sin=(struct sockaddr_in*)&ifr.ifr_addr; /* 获取网络接口IP保存的是二进制IP */  
-     char ip[16];   /* 字符数组，存放字符串 */  
-     memset(ip,0,16);  
- err=ioctl(s,SIOCGIFADDR,&ifr);  
-     if(!err)  
-     {  
-         inet_ntop(AF_INET,&sin->sin_addr.s_addr,ip,16);  /* 转换的字符串保存到ip数组中，第二个参数是要转换的二进制IP指针，第三个参数是转换完成存放IP的缓冲区，最后一个参数是缓冲区的长度 */  
-         printf("IP address is : %s\n",ip);  
-     }else  
-     {  
-         printf("IP address is : 0.0.0.0\n");  
-     }  
- 
- err=ioctl(s,SIOCGIFBRDADDR,&ifr);   /* 得到接口的广播地址 */  
-     if(!err)  
-     {  
-         struct sockaddr_in *broadcast=(struct sockaddr_in*)&ifr.ifr_broadaddr;  
- 
-         inet_ntop(AF_INET,&broadcast->sin_addr.s_addr,ip,16); /* 转换成字符串,inet_ntop将二进制IP转换成点分十进制的字符串 */  
-         printf("BROADCAST IP is : %s\n",ip);  
-     }else  
-     {  
-         printf("BROADCAST IP is : 0.0.0.0\n",ip);  
-     }  
-     printf("----------------------------------\n");  
- 
- #else  
- 
-     memset(&ifr,0,sizeof(ifr));  
- ifr.ifr_ifindex=2;  /* 获得第2个网络接口的名称 */  
- 
- err=ioctl(s,SIOCGIFNAME,&ifr);  
-     memcpy(ifr.ifr_name,argv[1],5);  
-     if(err){  
-         perror("index error");  
-     }else{  
-         printf("The %dst net device is : %s\n",ifr.ifr_ifindex,ifr.ifr_name);  
-     }  
- 
- 
- err=ioctl(s,SIOCGIFHWADDR,&ifr);  /* 获得MAC地址 */  
-     if(!err){  
-         unsigned char* hw=ifr.ifr_hwaddr.sa_data;  
-         printf("Mac address is : %02x:%02x:%02x:%02x:%02x:%02x\n",hw[0],hw[1],hw[2],hw[3],hw[4],hw[5]);  
-     }else{  
-         printf("Mac address is : 00:00:00:00:00:00\n");  
-     }  
- 
- sin=(struct sockaddr_in*)&ifr.ifr_addr;  
-     memset(ip,0,16);  
- err=ioctl(s,SIOCGIFADDR,&ifr);  
-     if(!err){  
-         inet_ntop(AF_INET,&sin->sin_addr.s_addr,ip,16);  
-         printf("IP address is : %s\n",ip);  
-     }else{  
-         printf("IP address is : 0.0.0.0\n");  
-     }  
- 
- err=ioctl(s,SIOCGIFBRDADDR,&ifr);  /* 得到接口的广播地址 */  
-     if(!err){  
-         struct sockaddr_in *broadcast=(struct sockaddr_in*)&ifr.ifr_broadaddr;  
-         inet_ntop(AF_INET,&broadcast->sin_addr.s_addr,ip,16);/* 转换成字符串,inet_ntop将二进制IP转换成点分十进制的字符串 */  
-         printf("BROADCAST IP is : %s\n",ip);  
-     }else{  
-         printf("BROADCAST IP is : 0.0.0.0\n",ip);  
-     }  
- #endif  
- 
-     printf("----------------------------------\n");  
-     close(s);  
- 
- }  
- </span>  
- 









