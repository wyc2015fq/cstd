# libevent参考手册第五章：辅助类型和函数 - u013366022的专栏 - CSDN博客
2016年11月30日 15:20:11[slitaz](https://me.csdn.net/u013366022)阅读数：135
<event2/util.h>定义了很多在实现可移植应用时有用的函数，libevent内部也使用这些类型和函数。
## 1 基本类型
## 1.1 evutil_socket_t
在除Windows之外的大多数地方，套接字是个整数，操作系统按照数值次序进行处理。然而，使用Windows套接字API时，socket具有类型SOCKET，它实际上是个类似指针的句柄，收到这个句柄的次序是未定义的。在Windows中，libevent定义evutil_socket_t类型为整型指针，可以处理socket()或者accept()的输出，而没有指针截断的风险。
### 定义
#ifdef WIN32
#define evutil_socket_t intptr_t
#**else**
#define evutil_socket_t **int**
#endif
这个类型在2.0.1-alpha版本中引入。
## 1.2 标准整数类型
落后于21世纪的C系统常常没有实现C99标准规定的stdint.h头文件。考虑到这种情况，libevent定义了来自于stdint.h的、位宽度确定（bit-width-specific）的整数类型：
|**Type**|**Width**|**Signed**|**Maximum**|**Minimum**|
|----|----|----|----|----|
|ev_uint64_t|64|No|EV_UINT64_MAX|0|
|ev_int64_t|64|Yes|EV_INT64_MAX|EV_INT64_MIN|
|ev_uint32_t|32|No|EV_UINT32_MAX|0|
|ev_int32_t|32|Yes|EV_INT32_MAX|EV_INT32_MIN|
|ev_uint16_t|16|No|EV_UINT16_MAX|0|
|ev_int16_t|16|Yes|EV_INT16_MAX|EV_INT16_MIN|
|ev_uint8_t|8|No|EV_UINT8_MAX|0|
|ev_int8_t|8|Yes|EV_INT8_MAX|EV_INT8_MIN|
跟C99标准一样，这些类型都有明确的位宽度。
这些类型由1.4.0-alpha版本引入。MAX/MIN常量首次出现在2.0.4-alpha版本。
## 1.3 各种兼容性类型
在有ssize_t（有符号的size_t）类型的平台上，ev_ssize_t定义为ssize_t；而在没有的平台上，则定义为某合理的默认类型。ev_ssize_t类型的最大可能值是EV_SSIZE_MAX；最小可能值是EV_SSIZE_MIN。（在平台没有定义SIZE_MAX的时候，size_t类型的最大可能值是EV_SIZE_MAX）
ev_off_t用于代表文件或者内存块中的偏移量。在有合理off_t类型定义的平台，它被定义为off_t；在Windows上则定义为ev_int64_t。
某些套接字API定义了socklen_t长度类型，有些则没有定义。在有这个类型定义的平台中，ev_socklen_t定义为socklen_t，在没有的平台上则定义为合理的默认类型。
ev_intptr_t是一个有符号整数类型，足够容纳指针类型而不会产生截断；而ev_uintptr_t则是相应的无符号类型。
ev_ssize_t类型由2.0.2-alpha版本加入。ev_socklen_t类型由2.0.3-alpha版本加入。ev_intptr_t与ev_uintptr_t类型，以及EV_SSIZE_MAX/MIN宏定义由2.0.4-alpha版本加入。ev_off_t类型首次出现在2.0.9-rc版本。
## 2 定时器可移植函数
不是每个平台都定义了标准timeval操作函数，所以libevent也提供了自己的实现。
### 接口
#define evutil_timeradd(tvp, uvp, vvp) /* ... */#define evutil_timersub(tvp, uvp, vvp) /* ... */
这些宏分别对前两个参数进行加或者减运算，将结果存放到第三个参数中。
### 接口
#define evutil_timerclear(tvp) /* ... */#define evutil_timerisset(tvp) /* ... */
清除timeval会将其值设置为0。evutil_timerisset宏检查timeval是否已经设置，如果已经设置为非零值，返回ture，否则返回false。
### 接口
#define evutil_timercmp(tvp, uvp, cmp)
evutil_timercmp宏比较两个timeval，如果其关系满足cmp关系运算符，返回true。比如说，evutil_timercmp(t1,t2,<=)的意思是“是否t1<=t2？”。注意：与某些操作系统版本不同的是，libevent的时间比较支持所有C关系运算符（也就是<、>、==、!=、<=和>=）。
### 接口
**int** evutil_gettimeofday(**struct** timeval *tv, **struct** timezone *tz);
evutil_gettimeofdy（）函数设置tv为当前时间，tz参数未使用。
### 示例
**struct** timeval tv1, tv2, tv3;*/* Set tv1 = 5.5 seconds */*tv1.tv_sec = 5; tv1.tv_usec = 500*1000;*/* Set tv2 = now */*evutil_gettimeofday(&tv2, NULL);*/* Set tv3 = 5.5 seconds in the future */*evutil_timeradd(&tv1, &tv2, &tv3);*/* all 3 should print true */***if** (evutil_timercmp(&tv1, &tv1, ==))  /* == "If tv1 == tv1" */   puts("5.5 sec == 5.5 sec");**if** (evutil_timercmp(&tv3, &tv2, >=))  /* == "If tv3 >= tv2" */   puts("The future is after the present.");**if** (evutil_timercmp(&tv1, &tv2, <))   /* == "If tv1 < tv2" */   puts("It is no longer the past.");
除evutil_gettimeofday（）由2.0版本引入外，这些函数由1.4.0-beta版本引入。
注意：在1.4.4之前的版本中使用<=或者>=是不安全的。
## 3 套接字API兼容性
本节由于历史原因而存在：Windows从来没有以良好兼容的方式实现Berkeley套接字API。
### 接口
**int** evutil_closesocket(evutil_socket_t s);#define EVUTIL_CLOSESOCKET(s) evutil_closesocket(s)
这个接口用于关闭套接字。在Unix中，它是close()的别名；在Windows中，它调用closesocket()。（在Windows中不能将close()用于套接字，也没有其他系统定义了closesocket()）
evutil_closesocket()函数在2.0.5-alpha版本引入。在此之前，需要使用EVUTIL_CLOSESOCKET宏。
### 接口
#define EVUTIL_SOCKET_ERROR()#define EVUTIL_SET_SOCKET_ERROR(errcode)#define evutil_socket_geterror(sock)#define evutil_socket_error_to_string(errcode)
这些宏访问和操作套接字错误代码。EVUTIL_SOCKET_ERROR（）返回本线程最后一次套接字操作的全局错误号，evutil_socket_geterror（）则返回某特定套接字的错误号。（在类Unix系统中都是errno）EVUTIL_SET_SOCKET_ERROR()修改当前套接字错误号（与设置Unix中的errno类似），evutil_socket_error_to_string（）返回代表某给定套接字错误号的字符串（与Unix中的strerror()类似）。
（因为对于来自套接字函数的错误，Windows不使用errno，而是使用WSAGetLastError()，所以需要这些函数。）
**注意**：Windows套接字错误与从errno看到的标准C错误是不同的。
### 接口
**int** evutil_make_socket_nonblocking(evutil_socket_t sock);
用于对套接字进行非阻塞IO的调用也不能移植到Windows中。evutil_make_socket_nonblocking()函数要求一个套接字（来自socket()或者accept()）作为参数，将其设置为非阻塞的。（设置Unix中的O_NONBLOCK标志和Windows中的FIONBIO标志）
### 接口
**int** evutil_make_listen_socket_reuseable(evutil_socket_t sock);
这个函数确保关闭监听套接字后，它使用的地址可以立即被另一个套接字使用。（在Unix中它设置SO_REUSEADDR标志，在Windows中则不做任何操作。不能在Windows中使用SO_REUSEADDR标志：它有另外不同的含义（译者注：多个套接字绑定到相同地址））
### 接口
**int** evutil_make_socket_closeonexec(evutil_socket_t sock);
这个函数告诉操作系统，如果调用了exec()，应该关闭指定的套接字。在Unix中函数设置FD_CLOEXEC标志，在Windows上则没有操作。
### 接口
**int** evutil_socketpair(**int** family, **int** type, **int** protocol,        evutil_socket_t sv[2]);
这个函数的行为跟Unix的socketpair（）调用相同：创建两个相互连接起来的套接字，可对其使用普通套接字IO调用。函数将两个套接字存储在sv[0]和sv[1]中，成功时返回0，失败时返回-1。
在Windows中，这个函数仅能支持AF_INET协议族、SOCK_STREAM类型和0协议的套接字。注意：在防火墙软件明确阻止127.0.0.1，禁止主机与自身通话的情况下，函数可能失败。
除了evutil_make_socket_closeonexec（）由2.0.4-alpha版本引入外，这些函数都由1.4.0-alpha版本引入。
## 4 可移植的字符串操作函数
### 接口
ev_int64_t evutil_strtoll(**const****char** *s, **char** **endptr, **int** base);
这个函数与strtol行为相同，只是用于64位整数。在某些平台上，仅支持十进制。
### 接口
**int** evutil_snprintf(**char** *buf, size_t buflen, **const****char** *format, ...);**int** evutil_vsnprintf(**char** *buf, size_t buflen, **const****char** *format, va_list ap);
这些snprintf替代函数的行为与标准snprintf和vsnprintf接口相同。函数返回在缓冲区足够长的情况下将写入的字节数，不包括结尾的NULL字节。（这个行为遵循C99的snprintf()标准，但与Windows的_snprintf()相反：如果字符串无法放入缓冲区，_snprintf()会返回负数）
evutil_strtoll（）从1.4.2-rc版本就存在了，其他函数首次出现在1.4.5版本中。
## 5 区域无关的字符串操作函数
实现基于ASCII的协议时，可能想要根据字符类型的ASCII记号来操作字符串，而不管当前的区域设置。libevent为此提供了一些函数：
### 接口
**int** evutil_ascii_strcasecmp(**const****char** *str1, **const****char** *str2);**int** evutil_ascii_strncasecmp(**const****char** *str1, **const****char** *str2, size_t n);
这些函数与strcasecmp()和strncasecmp()的行为类似，只是它们总是使用ASCII字符集进行比较，而不管当前的区域设置。这两个函数首次在2.0.3-alpha版本出现。
## 6 IPv6辅助和兼容性函数
### 接口
**const****char** *evutil_inet_ntop(**int** af, **const****void** *src, **char** *dst, size_t len);**int** evutil_inet_pton(**int** af, **const****char** *src, **void** *dst);
这些函数根据RFC 3493的规定解析和格式化IPv4与IPv6地址，与标准inet_ntop()和inet_pton()函数行为相同。要格式化IPv4地址，调用evutil_inet_ntop()，设置af为AF_INET，src指向in_addr结构体，dst指向大小为len的字符缓冲区。对于IPv6地址，af应该是AF_INET6，src则指向in6_addr结构体。要解析IP地址，调用evutil_inet_pton()，设置af为AF_INET或者AF_INET6，src指向要解析的字符串，dst指向一个in_addr或者in_addr6结构体。
失败时evutil_inet_ntop()返回NULL，成功时返回到dst的指针。成功时evutil_inet_pton()返回0，失败时返回-1。
### 接口
**int** evutil_parse_sockaddr_port(**const****char** *str, **struct** sockaddr *out,**int** *outlen);
这个接口解析来自str的地址，将结果写入到out中。outlen参数应该指向一个表示out中可用字节数的整数；函数返回时这个整数将表示实际使用了的字节数。成功时函数返回0，失败时返回-1。函数识别下列地址格式：
l [ipv6]:端口号（如[ffff::]:80）
l ipv6（如ffff::）
l [ipv6]（如[ffff::]）
l ipv4:端口号（如1.2.3.4:80）
l ipv4（如1.2.3.4）
如果没有给出端口号，结果中的端口号将被设置为0。
### 接口
**int** evutil_sockaddr_cmp(**const****struct** sockaddr *sa1,**const****struct** sockaddr *sa2, **int** include_port);
evutil_sockaddr_cmp()函数比较两个地址，如果sa1在sa2前面，返回负数；如果二者相等，则返回0；如果sa2在sa1前面，则返回正数。函数可用于AF_INET和AF_INET6地址；对于其他地址，返回值未定义。函数确保考虑地址的完整次序，但是不同版本中的次序可能不同。
如果include_port参数为false，而两个地址只有端口号不同，则它们被认为是相等的。否则，具有不同端口号的地址被认为是不等的。
除evutil_sockaddr_cmp()在2.0.3-alpha版本引入外，这些函数在2.0.1-alpha版本中引入。
## 7 结构体可移植性函数
### 接口
#define evutil_offsetof(type, field) /* ... */
跟标准offsetof宏一样，这个宏返回从type类型开始处到field字段的字节数。
这个宏由2.0.1-alpha版本引入，但2.0.3-alpha版本之前是有bug的。
## 8 安全随机数发生器
很多应用（包括evdns）为了安全考虑需要很难预测的随机数。
### 接口
**void** evutil_secure_rng_get_bytes(**void** *buf, size_t n);
这个函数用随机数据填充buf处的n个字节。
如果所在平台提供了arc4random()，libevent会使用这个函数。否则，libevent会使用自己的arc4random()实现，种子则来自操作系统的熵池（entropy pool）（Windows中的CryptGenRandom，其他平台中的/dev/urandom）
### 接口
**int** evutil_secure_rng_init(**void**);**void** evutil_secure_rng_add_bytes(**const****char** *dat, size_t datlen);
不需要手动初始化安全随机数发生器，但是如果要确认已经成功初始化，可以调用evutil_secure_rng_init()。函数会播种RNG（如果没有播种过），并在成功时返回0。函数返回-1则表示libevent无法在操作系统中找到合适的熵源（source of entropy），如果不自己初始化RNG，就无法安全使用RNG了。
如果程序运行在可能会放弃权限的环境中（比如说，通过执行chroot()），在放弃权限前应该调用evutil_secure_rng_init()。
可以调用evutil_secure_rng_add_bytes()向熵池加入更多随机字节，但通常不需要这么做。
这些函数是2.0.4-alpha版本引入的。
原文：http://www.wangafu.net/~nickm/libevent-book/Ref5_evutil.html
