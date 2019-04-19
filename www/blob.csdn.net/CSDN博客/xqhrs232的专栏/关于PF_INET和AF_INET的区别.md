# 关于PF_INET和AF_INET的区别 - xqhrs232的专栏 - CSDN博客
2016年05月27日 10:55:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：470
原文地址::[http://blog.csdn.net/xiongmaojiayou/article/details/7584211](http://blog.csdn.net/xiongmaojiayou/article/details/7584211)
在写网络程序的时候，建立TCP socket：
   sock = socket(PF_INET, SOCK_STREAM, 0);
然后在绑定本地地址或连接远程地址时需要初始化sockaddr_in结构，其中指定address family时一般设置为AF_INET，即使用IP。
相关头文件中的定义：AF = Address Family
                    PF = Protocol Family
                    AF_INET = PF_INET
在windows中的Winsock2.h中，
                    #define AF_INET 0
                    #define PF_INET AF_INET
所以在windows中**AF**_INET与**PF**_INET完全一样. 
    而在Unix/Linux系统中，在不同的版本中这两者有微小差别.对于BSD,是**AF**,对于POSIX是**PF.**
    理论上建立socket时是指定协议，应该用PF_xxxx，设置地址时应该用AF_xxxx。当然AF_INET和PF_INET的值是相同的，混用也不会有太大的问题。也就是说你socket时候用PF_xxxx，设置的时候用AF_xxxx也是没关系的，这点随便找个TCPIP例子就可以验证出来了。如下，不论是AF_INET还是PF_INET都是可行的，只不过这样子的话，有点不符合规范。
**[cpp]**[view
 plain](http://blog.csdn.net/xiongmaojiayou/article/details/7584211#)[copy](http://blog.csdn.net/xiongmaojiayou/article/details/7584211#)
- /* 服务器端开始建立socket描述符 */
- / if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)   
- if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1)   
- {  
- fprintf(stderr,"Socket error:%s\n\a",strerror(errno));  
- exit(1);  
- }  
- 
- /* 服务器端填充 sockaddr结构 */
- bzero(&server_addr,sizeof(struct sockaddr_in));  
- server_addr.sin_family=AF_INET;  
- //server_addr.sin_family=PF_INET;
- server_addr.sin_addr.s_addr=htonl(INADDR_ANY);  
- server_addr.sin_port=htons(portnumber);  
 在函数socketpair与socket的domain参数中有AF_UNIX,AF_LOCAL,AF_INET,PF_UNIX,PF_LOCAL,PF_INET.
 这几个参数有AF_UNIX=AF_LOCAL, PF_UNIX=PF_LOCAL, AF_LOCAL=PF_LOCAL**, AF_INET=PF_INET.** 但是对于socketpair与socket的domain参数,使用PF_LOCAL系列,
 而在初始化套接口地址结构时,则使用AF_LOCAL.
 例如:
     z = socket(PF_LOCAL, SOCK_STREAM, 0);
     adr_unix.sin_family = AF_LOCAL;
[](http://blog.csdn.net/xiongmaojiayou/article/details/7584211#)[](http://blog.csdn.net/xiongmaojiayou/article/details/7584211#)[](http://blog.csdn.net/xiongmaojiayou/article/details/7584211#)[](http://blog.csdn.net/xiongmaojiayou/article/details/7584211#)[](http://blog.csdn.net/xiongmaojiayou/article/details/7584211#)[](http://blog.csdn.net/xiongmaojiayou/article/details/7584211#)
- 
顶
1- 
踩

