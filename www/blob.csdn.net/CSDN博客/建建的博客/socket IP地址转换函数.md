# socket  IP地址转换函数 - 建建的博客 - CSDN博客
2017年05月07日 19:14:56[纪建](https://me.csdn.net/u013898698)阅读数：1734
# [IP地址转换函数](http://www.cnblogs.com/sycamore/p/4558745.html)
这些天一直在看《UNP》，觉得有必要对一些东西做一下总结（其实只是抄一抄书），加深一下印象。
IP地址转换函数有两组，它们可以在字符串和网络字节序的二进制值之间转换IP地址
适用于IPv4地址的函数
```
1 #include <arpa/inet.h>
 2 
 3 /* Convert Internet host address from numbers-and-dots notation in CP
 4    into binary data and store the result in the structure INP.  */
 5 int inet_aton (const char *__cp, struct in_addr *__inp);
 6 
 7 /* Convert Internet host address from numbers-and-dots notation in CP
 8    into binary data in network byte order.  */
 9 in_addr_t inet_addr (const char *__cp);
10 
11 /* Convert Internet number in IN to ASCII representation.  The return value
12    is a pointer to an internal array containing the string.  */
13 char *inet_ntoa (struct in_addr __in);
```
以上三个函数在点分十进制数串（如“127.0.0.1"）和32位网络字节序二进制值之间转换IPv4地址。
inet_aton将__cp指向的字符串转成网络序的地址存在__inp指向的地址结构。成功返回1，否则返回0。(据书中所说，如果__inp指针为空，那么该函数仍然对输入字符串进行有效性检查但是不存储任何结果）
inet_addr功能和inet_aton类似，但是inet_addr出错时返回INADDR_NONE常值（通常是32位均为1的值），这就意味着至少有一个IPv4的地址（通常为广播地址255.255.255.255）不能由该函数处理。建议使用inet_aton代替inet_addr。
inet_ntoa将网络序二进制IPv4地址转换成点分十进制数串。该函数的返回值所指向的字符串驻留在静态内存中。这意味着该函数是不可重入的。同时我们也该注意到该函数以一个结构体为参数而不是常见的以一个结构体指针作为参数。
对IPv4地址和IPv6地址同时都适用的函数
```
1 #include <arpa/inet.h>
 2 
 3 /* Convert from presentation format of an Internet number in buffer
 4    starting at CP to the binary network format and store result for
 5    interface type AF in buffer starting at BUF.  */
 6 int inet_pton (int __af, const char *__restrict __cp,
 7               void *__restrict __buf);
 8 
 9 /* Convert a Internet address in binary network format for interface
10    type AF in buffer starting at CP to presentation form and place
11    result in buffer of length LEN astarting at BUF.  */
12 const char *inet_ntop (int __af, const void *__restrict __cp,
13                   char *__restrict __buf, socklen_t __len);
```
这两个函数的__af参数既可以是AF_INET也可以是AF_INET6。如果以不支持的地址族作为参数就会返回一个错误，并将errno置为EAFNOSUPPORT。
inet_pton将字串转成对应的网络序二进制值，inet_ntop做相反的事情，其中__len参数指定目标缓冲区的大小，在<netinet/in.h>头文件中定义了
```
1 #define INET_ADDRSTRLEN 16    /* for IPv4 dotted-decimal */
2 #define INET6_ADDRSTRLEN 46     /* for IPv6 hex string */
```
如果给的__len太小那么会返回一个空指针，并置errno为ENOSPC。
