# 这辈子真的找不到SIOCIFCREATE了吗？ - Koma Hub - CSDN博客
2019年01月27日 12:55:24[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：139
个人分类：[计算机网络																[Socket																[Linux																[CentOS](https://blog.csdn.net/Rong_Toa/article/category/8122413)](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/7792858)](https://blog.csdn.net/Rong_Toa/article/category/7569489)
```cpp
#include <sys/types.h>                                                          
#include <sys/ioctl.h>                                                          
#include <sys/socket.h>                                                        
#include <net/if.h>                                                            
#include <sys/stat.h>                                                          
                                                                               
#include <err.h>                                                                
#include <stdio.h>                                                              
#include <stdlib.h>                                                            
#include <string.h>                                                            
#include <unistd.h>                                                            
                              
#include <sys/socket.h> 
#include <net/route.h> 
#include <net/if.h>
                               
int main(int argc, char **argv)                                                
{                                                                              
        int s, rc;                                                              
        const char *name = "tap0";                                              
        struct ifreq ifr;                                                      
                                                                               
        s = socket(AF_INET, SOCK_DGRAM, 0);                                    
        if (s == -1) {                                                          
                perror("socket");                                              
                exit(1);                                                        
        }                                                                      
                                                                               
        memset(&ifr, 0, sizeof(ifr));                                          
        (void) strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name));              
        rc = ioctl(s, SIOCIFCREATE, &ifr);                                      
        if (rc < 0) {                                                          
                perror("SIOCIFCREATE");                                        
                exit(1);                                                        
        }                                                                      
        close(s);                                                              
                                                                               
        return 0;                                                              
}
```
```
$ gcc ifcreate.c 
ifcreate.c: 在函数‘main’中:
ifcreate.c:31:23: 错误：‘SIOCIFCREATE’未声明(在此函数内第一次使用)
         rc = ioctl(s, SIOCIFCREATE, &ifr);                                      
                       ^
ifcreate.c:31:23: 附注：每个未声明的标识符在其出现的函数内只报告一次
```
