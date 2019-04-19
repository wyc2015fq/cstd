# linux下C语言获取网卡MAC地址 - xqhrs232的专栏 - CSDN博客
2016年05月10日 14:26:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3532
原文地址::[http://www.cnblogs.com/hnrainll/archive/2012/01/17/2324967.html](http://www.cnblogs.com/hnrainll/archive/2012/01/17/2324967.html)
相关文章
1、[Linux下获取eth网卡MAC地址的代码](http://www.oschina.net/code/snippet_54100_2151)----[http://www.oschina.net/code/snippet_54100_2151](http://www.oschina.net/code/snippet_54100_2151)
2、linux下有关网卡mac地址的查看方法 
 ----[http://gongqinggang.blog.163.com/blog/static/175548152012108103621602/](http://gongqinggang.blog.163.com/blog/static/175548152012108103621602/)
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
int main(int argc, char *argv[])
{
struct ifreq ifreq;
int sock = 0;
char mac[32] = "";
if(argc < 2){
        printf("Usage: ./main eth0");
return 1;
    }
    sock = socket(AF_INET,SOCK_STREAM,0);
if(sock < 0)
    {
        perror("error sock");
return 2;
    }
    strcpy(ifreq.ifr_name,argv[1]);
if(ioctl(sock,SIOCGIFHWADDR,&ifreq) < 0)
    {
        perror("error ioctl");
return 3;
    }
int i = 0;
for(i = 0; i < 6; i++){
        sprintf(mac+3*i, "%02X:", (unsigned char)ifreq.ifr_hwaddr.sa_data[i]);
    }
    mac[strlen(mac) - 1] = 0;
    printf("MAC: %s\n", mac);
return 0;
}
```
输入:./mac eth0
显示eth0的MAC地址.
