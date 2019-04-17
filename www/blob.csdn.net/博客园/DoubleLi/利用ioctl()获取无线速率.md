# 利用ioctl()获取无线速率 - DoubleLi - 博客园






其实对于自己装了网卡驱动的来说，应该从最根本的驱动中获取速率。

但是用ioctl()也可以，其实实现和iwconfig命令相同。



仅仅获取速率这部分：







**[cpp]**[view plain](http://blog.csdn.net/forsiny/article/details/4818081)[copy](http://blog.csdn.net/forsiny/article/details/4818081)



- #include <stdio.h>  
- #include <stdlib.h>  
- #include <sys/socket.h>  
- #include <string.h>  
- #include "wireless_copy.h"  
- #define dvname "ath0"  
- int get_rate(int sock, struct iwreq* wrq,__s32 rate);  
- int main()  
- {  
- struct iwreq wrq;  
- int sock;  
- char gInterfaceName[16];  
-     __s32 rate;  
-     memset(gInterfaceName, 0, sizeof(gInterfaceName));  
-     strcat(gInterfaceName,dvname);  
-     sock = socket(AF_INET, SOCK_DGRAM, 0);  
- if (sock < 0)  
-         {  
-             printf("Error Creating Socket for ioctl/n");  
- return 0;  
-         }  
-     memset(&wrq, 0, sizeof(wrq));  
-     strncpy(wrq.ifr_name, gInterfaceName, IFNAMSIZ);  
-     get_rate(sock, &wrq,rate);  
-     printf("/nrate:%dM/n/n",wrq.u.bitrate.value/1000000);  
- return 0;  
- }  
- int get_rate(int sock, struct iwreq* wrq,__s32 rate)  
- {  
- if(ioctl(sock, SIOCGIWRATE, wrq) < 0)  
-     {  
-         perror("Ioctl error");  
- return(0);  
-     }  
- return 1;  
- }  






其中wireless_copy.h可以从madwifi /tools 文件夹中找到。









