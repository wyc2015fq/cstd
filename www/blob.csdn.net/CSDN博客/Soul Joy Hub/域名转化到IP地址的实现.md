# 域名转化到IP地址的实现 - Soul Joy Hub - CSDN博客

2016年06月13日 10:01:04[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：953


http://blog.csdn.net/u011239443/article/details/51655354

在linux中，有一些函数可以实现主机名和地址的转化，最常见的有gethostbyname()、gethostbyaddr()等，它们都可以实现IPv4和IPv6的地址和主机名之间的转化。其中gethostbyname()是将主机名转化为IP地址，gethostbyaddr()则是逆操作，是将IP地址转化为主机名。

    函数原型：

```
1    #include <netdb.h>
2 
3         struct hostent* gethostbyname(const char* hostname);
4 
5         struct hostent* gethostbyaddr(const char* addr, size_t len, int family);
```

    结构体：

```
1 struct hostent
 2 
 3         {
 4 
 5             char *h_name;       /*正式主机名*/
 6 
 7             char **h_aliases;   /*主机别名*/
 8 
 9             int h_addrtype;     /*主机IP地址类型 IPv4为AF_INET*/
10 
11             int h_length;       /*主机IP地址字节长度，对于IPv4是4字节，即32位*/
12 
13             char **h_addr_list; /*主机的IP地址列表*/
14 
15         }
16 
17         #define  h_addr  h_addr_list[0] /*保存的是ip地址*/
```

   　　 函数gethostbyname()：用于将域名（www.baidu.com）或主机名转换为IP地址。参数hostname指向存放域名或主机名的字符串。

 　　   函数gethostbyaddr()：用于将IP地址转换为域名或主机名。参数addr是一个IP地址，此时这个ip地址不是普通的字符串，而是要通过函数inet_aton()转换。len为IP地址的长度，AF_INET为4。family可用AF_INET：Ipv4或AF_INET6：Ipv6。

　　Example：将百度的www.baidu.com 转换为ip地址

```
1 #include <netdb.h>
 2 
 3 #include <sys/socket.h>
 4 
 5 #include <stdio.h>
 6 
 7 int main(int argc, char **argv)
 8 
 9 {
10 
11 char *ptr, **pptr;
12 
13     struct hostent *hptr;
14 
15     char str[32] = {'\0'};
16 
17 /* 取得命令后第一个参数，即要解析的域名或主机名 */
18 
19 ptr = argv[1];  //如www.baidu.com
20 
21 /* 调用gethostbyname()。结果存在hptr结构中 */
22 
23     if((hptr = gethostbyname(ptr)) == NULL)
24 
25     {
26 
27         printf(" gethostbyname error for host:%s\n", ptr);
28 
29         return 0;
30 
31     }
32 
33 /* 将主机的规范名打出来 */
34 
35     printf("official hostname:%s\n",hptr->h_name);
36 
37 /* 主机可能有多个别名，将所有别名分别打出来 */
38 
39 for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
40 
41 printf(" alias:%s\n",*pptr);
42 
43               /* 根据地址类型，将地址打出来 */
44 
45 switch(hptr->h_addrtype)
46 
47     {
48 
49 case AF_INET,AF_INET6:
50 
51 pptr=hptr->h_addr_list;
52 
53                             /* 将刚才得到的所有地址都打出来。其中调用了inet_ntop()函数 */
54 
55             for(; *pptr!=NULL; pptr++)
56 
57                 printf(" address:%s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
58 
59             printf(" first address: %s\n", inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));
60 
61         break;
62 
63         default:
64 
65             printf("unknown address type\n");
66 
67         break;
68 
69     }
70 
71     return 0;
72 
73 }
```

编译运行

#gcc test.c

#./a.out www.baidu.com

official hostname:www.a.shifen.com

alias:www.baidu.com

address: 220.181.111.148

……

first address: 220.181.111.148

![](https://img-blog.csdn.net/20170508121021601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

