# linux中环境变量LD_PRELOAD是怎样工作的？ - xqhrs232的专栏 - CSDN博客
2016年03月23日 15:37:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：450
原文地址：[http://blog.csdn.net/zgl07/article/details/8998024](http://blog.csdn.net/zgl07/article/details/8998024)
相关文章
1、[linux
 ERROR: ld.so: object '/lib/libcwait.so' from /etc/ld.so.preload cannot be preloaded: ignored.](http://www.cnblogs.com/quanweiru/p/3764168.html)----[http://www.cnblogs.com/quanweiru/p/3764168.html](http://www.cnblogs.com/quanweiru/p/3764168.html)
2、linux平台LD_PRELOAD及其机制的一种技术----[http://www.tuicool.com/articles/aqERfi](http://www.tuicool.com/articles/aqERfi)
## [A Simple LD_PRELOAD Tutorial](http://www.catonmat.net/blog/simple-ld-preload-tutorial/)
有的时候为了研究需要，我们需要重载C的标准库函数，比如printf,fopen等等，这篇文章介绍如何利用LD_PRELOAD这个环境变量实现这个目标。
首先由一个简单的C程序开始。（prog.c）
**[cpp]**[view
 plain](http://blog.csdn.net/zgl07/article/details/8998024#)[copy](http://blog.csdn.net/zgl07/article/details/8998024#)
- #include <stdio.h>
- 
- int main(void) {  
-     printf("Calling the fopen() function...\n");  
- 
- FILE *fd = fopen("test.txt","r");  
- if (!fd) {  
-         printf("fopen() returned NULL\n");  
- return 1;  
-     }  
- 
-     printf("fopen() succeeded\n");  
- 
- return 0;  
- }  
这段程序简单地调用了fopen函数并检查返回值。
下面编译执行以下
**[cpp]**[view
 plain](http://blog.csdn.net/zgl07/article/details/8998024#)[copy](http://blog.csdn.net/zgl07/article/details/8998024#)
- $ ls  
- prog.c  test.txt  
- 
- $ gcc prog.c -o prog  
- 
- $ ls  
- prog  prog.c  test.txt  
- 
- $ ./prog  
- Calling the fopen() function...  
- fopen() succeeded  
可以看到调用标准库函数成功。
然后我们编写自己的fopen替代libc库函数中的fopen
**[cpp]**[view
 plain](http://blog.csdn.net/zgl07/article/details/8998024#)[copy](http://blog.csdn.net/zgl07/article/details/8998024#)
- #include <stdio.h>
- 
- FILE *fopen(constchar *path, constchar *mode) {  
-     printf("Always failing fopen\n");  
- return NULL;  
- }  
并且编译成共享库。
**[cpp]**[view
 plain](http://blog.csdn.net/zgl07/article/details/8998024#)[copy](http://blog.csdn.net/zgl07/article/details/8998024#)
- gcc -Wall -fPIC -shared -o myfopen.so myfopen.c  
- Now we can simply modify LD_PRELOAD:  
- 
- $ LD_PRELOAD=./myfopen.so ./prog  
- Calling the fopen() function...  
- Always failing fopen  
- fopen() returned NULL  
可以看到，调用失败，因为我们的共享库返回null。
## 总结
我们如果需要调试或者使用我们自己的函数来代替libc库或者其他库的函数时，这个方法是可行的。
