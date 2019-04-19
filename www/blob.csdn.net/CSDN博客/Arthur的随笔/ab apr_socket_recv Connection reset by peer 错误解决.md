# ab "apr_socket_recv: Connection reset by peer" 错误解决 - Arthur的随笔 - CSDN博客
2012年03月09日 16:09:49[largetalk](https://me.csdn.net/largetalk)阅读数：22763
在ubuntu 11.04上的ab（apache-utils)有些问题， 在并发数稍微大点（大概3，400）的情况下就会报 apr_socket_recv: Connection reset by peer 错误，这肯定会影响我们使用， 所以需要自己编译一个。
从 [https://code.google.com/p/apachebench-standalone/downloads/detail?name=ab-standalone-0.1.tar.bz2&can=2&q=](https://code.google.com/p/apachebench-standalone/downloads/detail?name=ab-standalone-0.1.tar.bz2&can=2&q=) 下载ab
 源代码
这篇文章 [https://code.google.com/p/apachebench-standalone/wiki/HowToBuild](https://code.google.com/p/apachebench-standalone/wiki/HowToBuild) 基本上可以使用，里面有些url，如apr，
 apr-utils的下载地址不对， 到这里[http://apr.apache.org/download.cgi](http://apr.apache.org/download.cgi) 去下载下来就行了。
vi ab.c 到1392行左右，将代码改成如下：
```cpp
1390                 return;
1391             } else {
1392                 //apr_err("apr_socket_recv", status);
1393                 bad++;
1394                 close_connection(c);
1395                 return;
1396             }
```
然后编译之。
用自己编译的ab测试并发能达到1000，但超过1000之后会报 open too many files错误， 在网上找了一些解决办法，如ulimit -n 65535, echo "65535" >> /proc/sys/fs/file_max,
 都没有解决这个问题
