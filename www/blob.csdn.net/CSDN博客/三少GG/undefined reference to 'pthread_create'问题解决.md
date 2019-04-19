# undefined reference to 'pthread_create'问题解决 - 三少GG - CSDN博客
2011年10月20日 11:15:16[三少GG](https://me.csdn.net/scut1135)阅读数：2966标签：[reference																[linux																[程序开发																[编程																[thread																[ubuntu](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=程序开发&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)
个人分类：[Coding实战](https://blog.csdn.net/scut1135/article/category/909756)
### [undefined reference to 'pthread_create'问题解决](http://blog.csdn.net/llqkk/article/details/2854558)
分类： [Linux Ubuntu程序开发](http://blog.csdn.net/llqkk/article/category/401044)2008-08-31 09:51
12152人阅读[评论](http://blog.csdn.net/llqkk/article/details/2854558#comments)(9)收藏[举报](http://blog.csdn.net/llqkk/article/details/2854558#report)
由于是Linux新手，所以现在才开始接触线程编程，照着GUN/Linux编程指南中的一个例子输入编译，结果出现如下错误：
undefined reference to 'pthread_create'
undefined reference to 'pthread_join'
问题原因：
**pthread 库不是 Linux 系统默认的库，连接时需要使用静态库 libpthread.a，**所以在使用pthread_create()创建线程，以及调用 pthread_atfork()函数建立fork处理程序时，**需要链接该库。**
问题解决：
    在编译中要加 -lpthread参数（最好加在最后面，我到貌似就是这样才行）
    gcc thread.c -o thread -lpthread
    thread.c为你些的源文件，不要忘了加上头文件#include<pthread.h>

