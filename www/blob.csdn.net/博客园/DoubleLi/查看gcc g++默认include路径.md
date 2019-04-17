# 查看gcc/g++默认include路径 - DoubleLi - 博客园







转自：[http://gcc.gnu.org/ml/gcc-help/2007-09/msg00205.html](http://gcc.gnu.org/ml/gcc-help/2007-09/msg00205.html)



**`gcc -print-prog-name=cc1plus` -v**

**`g++ -print-prog-name=cc1plus` -v**



例如，CentOS 6.3 64bit 下，`g++ -print-prog-name=cc1plus` -v 的运行结果是：

> 
ignoring nonexistent directory "/usr/lib/gcc/x86_64-redhat-linux/4.4.6/include-fixed"

ignoring nonexistent directory "/usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../x86_64-redhat-linux/include"

#include "..." search starts here:

#include <...> search starts here:

 /usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../include/c++/4.4.6

 /usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../include/c++/4.4.6/x86_64-redhat-linux

 /usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../include/c++/4.4.6/backward

 /usr/local/include

 /usr/lib/gcc/x86_64-redhat-linux/4.4.6/include

 /usr/include

End of search list.


即，默认include路径是

> 
 /usr/include/c++/4.4.6

 /usr/include/c++/4.4.6/x86_64-redhat-linux

 /usr/include/c++/4.4.6/backward

 /usr/local/include

 /usr/lib/gcc/x86_64-redhat-linux/4.4.6/include

 /usr/include













