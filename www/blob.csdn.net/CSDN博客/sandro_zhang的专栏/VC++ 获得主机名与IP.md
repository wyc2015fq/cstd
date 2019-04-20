# VC++ 获得主机名与IP - sandro_zhang的专栏 - CSDN博客
2011年09月29日 16:03:49[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：993标签：[vc++																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)
个人分类：[vc++](https://blog.csdn.net/sandro_zhang/article/category/884062)
```cpp
char hostName[256];
gethostname(hostName,256);
char hostAddress[256];
HOSTENT *hostEntry;
hostEntry=gethostbyname(hostName);
wsprintf(hostAddress , "%d.%d.%d.%d" ,
                (hostEntry>h_addr_list[0][0]&0x00ff) ,
                (hostEntry->h_addr_list[0][1]&0x00ff) ,
                (hostEntry->h_addr_list[0][2]&0x00ff) ,
                (hostEntry->h_addr_list[0][3]&0x00ff)
            );
```
感谢：[http://viking.blog.51cto.com/272298/57234](http://viking.blog.51cto.com/272298/57234)
