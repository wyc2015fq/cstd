# 通过gethostbyname和inet_ntop获取域名ip - walkabc - 博客园
# [通过gethostbyname和inet_ntop获取域名ip](https://www.cnblogs.com/hitfire/articles/4595642.html)
```
1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <netdb.h>
 4 
 5 /*
 6 *用法:main www.baidu.com
 7 */
 8 
 9 int main(int argc, char* argv[])
10 {
11     char* name;
12     if(argc > 1) {
13         name = argv[1];
14     } else {
15         printf("get name from cmd error !\n");
16         return -1;
17     }
18     struct hostent* host = gethostbyname(name);
19     printf("official name is %s\n", host->h_name);
20     printf("h_length is %d\n", host->h_length);
21     char** addrListPtr = host->h_addr_list;
22     char* dst = malloc(256);
23     for(; *addrListPtr != NULL; addrListPtr++) {
24         if(host->h_addrtype == AF_INET || host->h_addrtype == AF_INET6) {
25             inet_ntop(host->h_addrtype, *addrListPtr, dst, 256);
26             if(dst != NULL) {
27                 printf("ip is %s\n", dst);
28             }
29         }
30     }
31     return 0;
32 }
```

